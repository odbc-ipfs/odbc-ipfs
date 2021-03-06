package odbc_ipfs_go_node

import (
	"database/sql"
	"encoding/json"
	"fmt"
	"os"
	"sync"
	"time"

	shell "github.com/ipfs/go-ipfs-api"
	_ "github.com/mattn/go-sqlite3"
	. "github.com/odbc-ipfs/odbc-ipfs/odbc_ipfs_go_shared"
)

var lock = sync.RWMutex{} //lock for 'Nodes'
var Nodes map[string]int  //Fetch counter

//Commands
//QUERY
//ERROR
//FETCH
//FETCHEND
//RESULTEND
//DATA

var createTableQuery = MessageData{"QUERY", "nil", []string{"CREATE TABLE IF NOT EXISTS `new_table1` (`id` INT NOT NULL,`name` VARCHAR(45) NULL);"}, nil, -1}
var insertQuery = MessageData{"QUERY", "nil", []string{"INSERT INTO `new_table1` (`id`, `name`) VALUES ('555', 'Hello ECE49595');"}, nil, -1}
var selectQuery = MessageData{"QUERY", "nil", []string{"SELECT * FROM new_table1;"}, nil, -1}
var fetchQuery = MessageData{"FETCH", "nil", []string{"int", "string"}, nil, -1}

var DB *sql.DB
var sh *shell.Shell
var pubsub *shell.PubSubSubscription

func StartGoNode() {

	Nodes = make(map[string]int)

	setupDB()
	sh, pubsub = SetupPubSub("ipfs:5001") //ipfs daemon --enable-pubsub-experiment

	go server()

	fmt.Println("Listening...")

	/*
		SendMsg(sh, createTableQuery)
		SendMsg(sh, insertQuery)
		SendMsg(sh, insertQuery)
		SendMsg(sh, selectQuery)
		SendMsg(sh, fetchQuery)
		SendMsg(sh, fetchQuery)
		SendMsg(sh, fetchQuery)
		fmt.Printf("\n\n\n\n\n")
		SendMsg(sh, selectQuery)
		SendMsg(sh, fetchQuery)
		SendMsg(sh, fetchQuery)
		SendMsg(sh, fetchQuery)
		//SendMsg(sh,fetchQuery)
	*/
	time.Sleep(time.Hour * 48)
}

// server is the main routine that parses incoming queries
func server() {
	for {
		msg, err := pubsub.Next()
		if err != nil {
			fmt.Println(err)
			continue
		}

		md := MessageData{"nil", "nil", nil, nil, 0}
		err = json.Unmarshal(msg.Data, &md)
		if err != nil {
			fmt.Println(err)
			//send error back
			sendError(msg.From.String(), err.Error())
			continue
		}
		fmt.Println(md)

		switch md.Command {
		case "QUERY":
			queryCMD(msg, md)
			break
		case "FETCH":
			fetchCMD(msg)
			break
		case "FETCHEND": //Ignore

			break
		case "DATA": //Ignore
			break
		case "RESULTEND": //Ignore
			break
		case "ERROR": //Ignore
			break
		default:
			fmt.Println("Command: " + md.Command + " not found")
			break
		}

	}
}

// readNode sets an item in the Nodes map (thread safe)
func readNode(key string) int {
	lock.RLock()
	defer lock.RUnlock()
	return Nodes[key]
}

// writeNode updates the Nodes map (thread safe)
func writeNode(key string, val int) {
	lock.Lock()
	defer lock.Unlock()
	Nodes[key] = val
}

// fetchCMD increments the fetch counter (thread safe)
func fetchCMD(msg *shell.Message) {
	fmt.Println("Fetch")
	writeNode(msg.From.String(), readNode(msg.From.String())+1)
}

// queryCMD creates a new goroutine of queryDB from the given message data and message
func queryCMD(msg *shell.Message, md MessageData) {
	fmt.Println("Query")

	go queryDB(md, msg.From.String())
}

var queryActiveLock = sync.RWMutex{} //lock to make sure only one query occurs at a time

// queryDB is the main query logic such as handling returning data
func queryDB(md MessageData, sender string) error {
	queryActiveLock.Lock()
	defer queryActiveLock.Unlock()
	//fmt.Println(md.StringData[0])
	if readNode(sender) > 0 {
		fmt.Println("Sender executed new query before ending fetch")
		writeNode(sender, -1)
		time.Sleep(time.Millisecond * 100)
	}

	writeNode(sender, 0)

	res, err := DB.Query(md.StringData[0])
	if err != nil {
		return err
	}

	pinDB()

	defer res.Close()
	count := 0
	for res.Next() {
		//wait for fetch

		for readNode(sender) == 0 {
		}

		if readNode(sender) == -1 {
			//FetchEndCMD
			break
		}
		writeNode(sender, readNode(sender)-1)

		interfaceData, err := getData(fetchQuery, res)
		if err != nil {
			sendError(sender, err.Error())
			return nil
		}

		sendData(sender, interfaceData, md.StringData)

		fmt.Println(interfaceData)
		count++
	}

	if count != 0 {
		//Wait one more time
		for readNode(sender) == 0 {
		}

		sendFetchEnd(sender, md.StringData[0])
	}

	return nil
}

// getData gets all the data and brings it into one interface array
// It returns an interface array of all the values
func getData(md MessageData, res *sql.Rows) ([]interface{}, error) {
	cols := make([]interface{}, len(md.StringData))
	colsAddr := make([]interface{}, len(md.StringData))
	for i := 0; i < len(md.StringData); i++ {
		//cols[i] = reflect.Zero((reflect.TypeOf(1))).Addr().Interface()
		colsAddr[i] = &cols[i]
	}
	err := res.Scan(colsAddr...)
	if err != nil {
		fmt.Println(err)
		return nil, err
	}

	return cols, err
}

// sendData sends data over pubsub with the id of the requester
func sendData(to string, cols []interface{}, origCMD []string) {
	dataQuery := MessageData{"DATA", to, origCMD, cols, -1}
	SendMsg(sh, dataQuery)
}

// sendError sends error messages over pubsub with the id of the requester
func sendError(to string, err string) {
	errorQuery := MessageData{"ERROR", to, []string{err}, nil, -1}
	SendMsg(sh, errorQuery)
}

// sendError sends when all rows have been read over pubsub with the id of the requester
func sendFetchEnd(to string, cmd string) {
	resultFetchQuery := MessageData{"FETCHEND", to, []string{cmd}, nil, -1}
	SendMsg(sh, resultFetchQuery)
}

// setupDB sets up the basic sqlite databse
func setupDB() {
	err := os.Remove("sqlite-database.db")

	DB, err = sql.Open("sqlite3", "./sqlite-database.db")

	if err != nil {
		fmt.Println(err)
		return
	}
	fmt.Println("Setup DB")
}

// pinDB pins the current state of the database, note: this does not remove previous pins
func pinDB() {
	reader, err := os.Open("sqlite-database.db")
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}
	defer reader.Close()
	str, err := sh.Add(reader)
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}
	fmt.Println("Pinned: " + str)
}

/*

 */
