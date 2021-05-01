package shared

import (
	"encoding/json"
	"fmt"
	"os"

	shell "github.com/ipfs/go-ipfs-api"
)

type MessageData struct {
	Command    string
	To         string
	StringData []string
	Data       []interface{}
	Nonce      int
}

// setupPubSub sets up the ipfs shell as well as subscribes to the channel
func SetupPubSub(url string) (*shell.Shell, *shell.PubSubSubscription) {
	sh := shell.NewShell(url) //ipfs
	var err error
	pubsub, err := sh.PubSubSubscribe("Hello World")
	if err != nil {
		fmt.Println("Error setting up pubsub")
		fmt.Println(err)
		os.Exit(1)
	}
	return sh, pubsub
}

// SendMsg sends a message over pubsub given the message data
func SendMsg(sh *shell.Shell, messageData MessageData) {
	bts, err := json.Marshal(messageData)
	if err != nil {
		fmt.Println(err)
		return
	}
	sh.PubSubPublish("Hello World", string(bts))
}
