package main

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
import "C"
import (
	"encoding/json"
	"fmt"
	"unsafe"

	shell "github.com/ipfs/go-ipfs-api"
	. "github.com/odbc-ipfs/odbc-ipfs/shared"
)

//import "fmt"

//go build -buildmode=c-shared -o cgo-connector.dll

//https://jmeubank.github.io/tdm-gcc/download/
//C:\TDM-GCC-64

var sh *shell.Shell
var pubsub *shell.PubSubSubscription

func main() {

}

//export CreateShell
func CreateShell() {
	sh, pubsub = SetupPubSub("localhost:5001")
	fmt.Printf("Created IPFS shell and pubsub in GO\n")
}

//export CloseShell
func CloseShell() {
	err := pubsub.Cancel()
	if err != nil {
		fmt.Println("Error closing pubsub")
		return
	}

}

//export Query
func Query(queryCString *C.char) {
	query := C.GoString(queryCString)

	msg := MessageData{"QUERY", "nil", []string{query}, nil, -1}
	fmt.Println("Query: " + query)
	SendMsg(sh, msg)
}

const (
	ERROR    = int32(0)
	QUERY    = int32(1)
	FETCHEND = int32(2)
)

//export Fetch
func Fetch(dataAddressList []unsafe.Pointer, types []int32, len int32) C.int {

	msg := MessageData{"FETCH", "nil", nil, nil, -1}
	var typeStringArray []string

	msg.StringData = typeStringArray
	//fmt.Print("Other Main: ")
	//fmt.Println(msg)

	for i := int32(0); i < len; i++ {

		if types[i] == 0 { // integer

			typeStringArray = append(typeStringArray, "int")
		} else if types[i] == 1 { //string

			typeStringArray = append(typeStringArray, "string")
		} else {
			fmt.Println("Error: type not recognized")
		}

	}
	msg.StringData = typeStringArray

	SendMsg(sh, msg)

	retMd := FetchData()

	if retMd.Command == "FETCHEND" {
		return C.int(FETCHEND)
	} else if retMd.Command == "ERROR" {
		return C.int(ERROR)
	}

	for i := int32(0); i < len; i++ {

		if types[i] == 0 { // integer
			ata := (*int32)(dataAddressList[i])
			*ata = int32(retMd.Data[i].(float64))

		} else if types[i] == 1 { //string
			ats := (*C.char)(dataAddressList[i])
			//gostring := C.GoString(ats)
			//fmt.Printf("Passed string: %s\n", gostring)

			newCstr := C.CString(retMd.Data[i].(string))
			C.strcpy(ats, newCstr)
			C.free(unsafe.Pointer(newCstr))

		}

	}
	return C.int(QUERY)
}

func FetchData() MessageData {
	md := MessageData{"nil", "nil", nil, nil, 0}
	for {
		msg, err := pubsub.Next()
		if err != nil {
			fmt.Println(err)
			return md
		}
		err = json.Unmarshal(msg.Data, &md)
		if err != nil {
			fmt.Println(err)
			return md
		}
		info, _ := sh.ID()
		if md.To != info.ID {
			continue
		}
		return md
	}

}

/*
//export Publish
func Publish(topicC, messageC *C.char) { //topic, data []byte

	topic := C.GoString(topicC)
	message := C.GoString(messageC)
	topicC = C.CString("")

	err := sh.PubSubPublish(topic, message)

	if err != nil {
		fmt.Fprintf(os.Stderr, "error: %s", err)
		os.Exit(1)
	}
	fmt.Println("Published!")
}

//export Recieve
func Recieve(topicC *C.char) *C.char {
	topic := C.GoString(topicC)

	sub, err := sh.PubSubSubscribe(topic)
	if err != nil {
		fmt.Fprintf(os.Stderr, "error: %s", err)
		os.Exit(1)
	}

	msg, err := sub.Next()
	if err != nil {
		fmt.Fprintf(os.Stderr, "error: %s", err)
		os.Exit(1)
	}

	msgString := string(msg.Data)

	cStr := C.CString(msgString)

	fmt.Println("Recieved!")

	return cStr
}
*/
