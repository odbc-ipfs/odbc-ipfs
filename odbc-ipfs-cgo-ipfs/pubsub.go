package main

import "C"
import (
	"fmt"
	"os"

	shell "github.com/ipfs/go-ipfs-api"
)

//import "fmt"

//go build -buildmode=c-archive -o pubsub.o pubsub.go

//https://jmeubank.github.io/tdm-gcc/download/
//C:\TDM-GCC-64

var i int

var sh *shell.Shell

func init() {
	//fmt.Printf("Hello worlf from GO\n")
}

func main() {
	//fmt.Println("Hello world")
}

//export CreateShell
func CreateShell() {
	sh = shell.NewShell("localhost:5001")
	fmt.Printf("Hello world from GO\n")
	i = 2
}

//export Publish
func Publish(topicC, messageC *C.char) { //topic, data []byte

	topic := C.GoString(topicC)
	message := C.GoString(messageC)

	err := sh.PubSubPublish(topic, message)
	i = 1
	if err != nil {
		fmt.Fprintf(os.Stderr, "error: %s", err)
		os.Exit(1)
	}
	fmt.Println("Published!")
}
