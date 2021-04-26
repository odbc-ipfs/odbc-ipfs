package main

import (
	"database/sql"
	"encoding/json"
	"fmt"
	"os"
	"sync"
	"time"

	shell "github.com/ipfs/go-ipfs-api"
	_ "github.com/mattn/go-sqlite3"
)

type Message struct {
	sender      string
	messageData MessageData
}

type MessageData struct {
	Command    string
	To         string
	StringData []string
	Data       []interface{}
	Nonce      int
}

type Node struct {
	Id    string
	Fetch int
	Nonce int
}

var lock = sync.RWMutex{} //lock for 'Nodes'
var Nodes map[string]int  //Fetch counter

//Commands
//QUERY
//ERROR
//FETCH
//FETCHEND
//RESULTEND
//DATA

type FakeMsg struct {
	From string
	Data []byte
}

var messages []FakeMsg

var createTableQuery = MessageData{"QUERY", "nil", []string{"CREATE TABLE IF NOT EXISTS `new_table1` (`id` INT NOT NULL,`name` VARCHAR(45) NULL);"}, nil, -1}
var insertQuery = MessageData{"QUERY", "nil", []string{"INSERT INTO `new_table1` (`id`, `name`) VALUES ('555', 'Hello ECE49595');"}, nil, -1}
var selectQuery = MessageData{"QUERY", "nil", []string{"SELECT * FROM new_table1;"}, nil, -1}
var fetchQuery = MessageData{"FETCH", "nil", []string{"int", "string"}, nil, -1}

var DB *sql.DB
var sh *shell.Shell
var pubsub *shell.PubSubSubscription

func setupPubSub() {
	sh = shell.NewShell("localhost:5001") //ipfs
	var err error
	pubsub, err = sh.PubSubSubscribe("Hello World")

	if err != nil {
		fmt.Println("Error setting up pubsub")
		fmt.Println(err)
		os.Exit(1)
	}
}

func main() {

	Nodes = make(map[string]int)

	setupDB()
	setupPubSub() //ipfs daemon --enable-pubsub-experiment

	go server()

	time.Sleep(1 * time.Second)

	sendMsg(createTableQuery)
	sendMsg(insertQuery)
	sendMsg(insertQuery)
	sendMsg(selectQuery)
	sendMsg(fetchQuery)

	sendMsg(fetchQuery)

	//sendMsg(fetchQuery)
	time.Sleep(1 * time.Hour)

}

func sendMsg(messageData MessageData) {
	bts, err := json.Marshal(messageData)
	if err != nil {
		fmt.Println(err)
		return
	}
	sh.PubSubPublish("Hello World", string(bts))
}

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
		case "FETCHEND":
			fetchEndCMD(msg)
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

func readNode(key string) int {
	lock.RLock()
	defer lock.RUnlock()
	return Nodes[key]
}

func writeNode(key string, val int) {
	lock.Lock()
	defer lock.Unlock()
	Nodes[key] = val
}

func fetchEndCMD(msg *shell.Message) {
	writeNode(msg.From.String(), -1)
}

func fetchCMD(msg *shell.Message) {
	writeNode(msg.From.String(), readNode(msg.From.String())+1)
}

func queryCMD(msg *shell.Message, md MessageData) {
	fmt.Println("Query")

	go queryDB(md, msg.From.String())
}

var queryActiveLock = sync.RWMutex{}

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
	defer res.Close()

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
		sendData(sender, interfaceData)

		fmt.Println(interfaceData)
	}

	sendResultEnd(sender, md.StringData[0])

	return nil
}

func waitForFetch(res *sql.Rows) {
	getData(fetchQuery, res)

}

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

func sendData(to string, cols []interface{}) {
	dataQuery := MessageData{"DATA", to, nil, cols, -1}
	sendMsg(dataQuery)
}

func sendError(to string, err string) {
	errorQuery := MessageData{"ERROR", to, []string{err}, nil, -1}
	sendMsg(errorQuery)
}

func sendResultEnd(to string, cmd string) {
	resultEndQuery := MessageData{"RESULTEND", to, []string{cmd}, nil, -1}
	sendMsg(resultEndQuery)
}

func setupDB() {
	err := os.Remove("sqlite-database.db")

	DB, err = sql.Open("sqlite3", "./sqlite-database.db")

	if err != nil {
		fmt.Println(err)
		return
	}
	fmt.Println("Setup DB")
}

/*

 */
