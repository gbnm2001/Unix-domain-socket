package main

import (
	"log"
	"net"
	"time"
	"io/ioutil"
	"fmt"
)

const (
	protocol = "unix"
	sockAddr = "./.socket"
)

func main() {
	for i := 0; i < 5; i++ {
		time.Sleep(1 * time.Second)

		conn, err := net.Dial(protocol, sockAddr)
		if err != nil {
			log.Fatal(err)
		}

		_, err = conn.Write([]byte("Hi"))
		if err != nil {
			log.Fatal(err)
		}

		
		err = conn.(*net.UnixConn).CloseWrite()
		if err != nil {
			log.Fatal(err)
		}

		b, err := ioutil.ReadAll(conn)
		if err != nil {
			log.Fatal(err)
		}

		fmt.Println("received ",string(b))
	}
}
