package main

import (
	"database/sql"
	"encoding/binary"
	"fmt"
	"os"
	"reflect"
	"strings"
	"time"

	shell "github.com/ipfs/go-ipfs-api"
	_ "github.com/mattn/go-sqlite3"
)

//
//ipfs daemon --enable-pubsub-experiment

//https://github.com/mattn/go-sqlite3
//https://www.codeproject.com/Articles/5261771/Golang-SQLite-Simple-Example

// /https://www.codeproject.com/Articles/5261771/Golang-SQLite-Simple-Example

var DB *sql.DB

func main() {
	fmt.Println("Hello world")

	//pubsubpub()

	err := os.Remove("sqlite-database.db")

	if err != nil {
		fmt.Println(err)
		//	return
	}

	DB, err = sql.Open("sqlite3", "./sqlite-database.db")

	if err != nil {
		fmt.Println(err)
		return
	}

	//defer db.Close()
	pubsubpub()
}

func pubsubpub() {

	sh := shell.NewShell("localhost:5001")

	cid, err := sh.Add(strings.NewReader("Hello World??!!"))

	if err != nil {
		fmt.Fprintf(os.Stderr, "error: %s", err)
		os.Exit(1)
	}

	fmt.Println("Added: " + cid)

	pubsub, err := sh.PubSubSubscribe("Hello World")

	if err != nil {
		fmt.Fprintf(os.Stderr, "error: %s", err)
		os.Exit(1)
	}

	go waitForMessage(pubsub)

	//PubSubPublishWithDetails
	//^ IPNS
	//err = sh.PubSubPublish("Hello World", "This is just a test")

	//if err != nil {
	//	fmt.Fprintf(os.Stderr, "error: %s", err)
	//	os.Exit(1)
	//}

	time.Sleep(time.Hour * 1)
}

func waitForMessage(pubsub *shell.PubSubSubscription) {
	fmt.Println("Waiting for message")

	for {
		msg, err := pubsub.Next()

		if err != nil {
			fmt.Fprintf(os.Stderr, "error: %s", err)
			os.Exit(1)
		}

		strData := string(msg.Data)

		fmt.Println("Got new message: ")
		fmt.Println("From: " + msg.From)
		fmt.Println("Data: " + strData)

		fmt.Print("Seqno: ")
		data := binary.BigEndian.Uint64(msg.Seqno)
		fmt.Println(data)

		//1615210180678445407
		//1615210180678445408
		fmt.Print("Topic IDs: ")
		fmt.Println(msg.TopicIDs)

		fmt.Println("Executing Query")
		if DB == nil {
			fmt.Println("Error: db == nil")
			return
		}

		res, err := DB.Query(strData)
		if err != nil {
			fmt.Println(err)
			return
		}

		cols, err := res.Columns()
		if err != nil {
			fmt.Println(err)
			return
		}
		fmt.Println("Columns")
		for _, v := range cols {
			fmt.Println(v)
		}
		fmt.Println("End columns")

		defer res.Close()
		for res.Next() {
			colTypes, err := res.ColumnTypes()
			if err != nil {
				fmt.Println(err)
				return
			}

			for _, colType := range colTypes {
				fmt.Println("Name: " + colType.Name())
				fmt.Print("Length: ")
				fmt.Println(colType.Length())
				fmt.Println("DBTypeName: " + colType.DatabaseTypeName())
				//colType.ScanType().
				fmt.Println("ScanType: " + colType.ScanType().Name())
				isInt := colType.ScanType().AssignableTo(reflect.TypeOf(int64(0)))
				isStr := colType.ScanType().AssignableTo(reflect.TypeOf(""))

				fmt.Println("isInt: %B isStr: %B", isInt, isStr)

			}

		}

		/*
			stmt, err := DB.Prepare(strData)

			if err != nil {
				fmt.Println(err)
				return
			}

			res, err := stmt.Exec()
			if err != nil {
				fmt.Println(err)
				return
			}
		*/

		fmt.Println("Query Executed")

	}
}

/*
	stmt, err := db.Prepare("CREATE TABLE `new_table1` (`id` INT NOT NULL,`name` VARCHAR(45) NULL);")
	stmt.Exec()
	if err != nil {
		fmt.Println(err)
		return
	}

	stmt.Exec()
	for i := 0; i < 10; i++ {
		stmt, err = db.Prepare("INSERT INTO `new_table1` (`id`, `name`) VALUES ('2', 'dsaf');")
		if err != nil {
			fmt.Println(err)
			return
		}
		stmt.Exec()
	}

	row, err := db.Query("SELECT * FROM `new_table1`;")
	if err != nil {
		fmt.Println(err)
		return
	}

	defer row.Close()

	for row.Next() {
		var id int
		var name string
		row.Scan(&id, &name)
		fmt.Println(fmt.Sprintf("ID: %d Name: %s", id, name))
	}
*/
