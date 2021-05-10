// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 254.
//!+

// Chat is a server that lets clients chat with each other.
package main

import (
	"bufio"
	"flag"
	"fmt"
	"log"
	"net"
	"os"
	"strings"
	"time"
)

//!+broadcaster
type userInfo struct {
	usernameN string
	ip        string
	cha       chan string
	strtConn  string
	connect   net.Conn
}

type client chan<- string // an outgoing message channel

var (
	entering = make(chan client)
	leaving  = make(chan client)
	messages = make(chan string) // all incoming client messages
	adm      = ""
	cnt      = 0
	date     = make(map[string]string)
	connx    = make(map[string]net.Conn)
	chann    = make(map[string]chan string)
	clients  = make(map[client]bool)
	clientL  = make(map[string]bool)
	info     [20]userInfo
)

func broadcaster() {
	for {
		select {
		case msg := <-messages:
			// Broadcast incoming message to all
			// clients' outgoing message channels.
			for cli := range clients {
				cli <- msg
			}

		case cli := <-entering:
			clients[cli] = true

		case cli := <-leaving:
			delete(clients, cli)
			close(cli)
		}
	}
}

//!-broadcaster

func checkU(conn net.Conn) {
	ch := make(chan string)
	go clientWriter(conn, ch)
	server := "irc-server > "
	name := ""
	if val, ok := clientL[name]; val && ok {
		ch <- server + "User already on the server."
	}
}

//!+handleConn
func handleConn(conn net.Conn) {
	ch := make(chan string) // outgoing client messages
	go clientWriter(conn, ch)
	server := "irc-server > "

	user := make([]byte, 128)
	_, err := conn.Read(user)
	name := ""
	if err != nil {
		conn.Close()
		log.Print(err)
	}

	for i := 0; i < len(user); i++ {
		if user[i] == 0 {
			break
		}
		name = name + string(user[i])
	}

	us := userInfo{
		usernameN: name,
		ip:        conn.RemoteAddr().String(),
		cha:       ch,
		strtConn:  time.Now().Format("2006-01-02 15:04:05"),
		connect:   conn,
	}

	if val, ok := clientL[name]; val && ok {
		//checkU(us.connect)
		ch <- server + "User already on the server."
		conn.Close()
	} else { // --------------------------- Initialize ----------------------------
		info[cnt] = us
		ch <- server + "Welcome to the Simple IRC Server"
		messages <- server + "New connected user [" + name + "]"
		ch <- server + "Your user [" + name + "] is succesfully logged"
		clientL[us.usernameN] = true
		chann[name] = ch
		connx[name] = conn
		date[name] = time.Now().Format("2006-01-02 15:04:05")
		if len(clientL) == 1 {
			ch <- server + "Congrats, you were the first user." // Greeting the first user.
		}
		if adm == "" { // Here we promote a user to admin mode, being it the first that entered or the next one in line after the preceding leaves the channel.
			//makeAdm(us.connect)
			adm = us.usernameN
			ch <- server + "You're the new IRC Server ADMIN"
			fmt.Println(server + "[" + name + "] was promoted as the channel ADMIN")
		}
		fmt.Println(server + "New connected user [" + name + "]") // Letting others know there's a new user.
		entering <- ch
		inwards := bufio.NewScanner(conn) // Getting the input of the users.
		for inwards.Scan() {              // Depending on the text it will be taken as a message to the chat or a command which would have different effects.
			action := strings.Split(inwards.Text(), " ")
			if action[0] == "/users" { // First action would be /users which will display info about all members of the chat.
				for i := range clientL {
					ch <- server + i + " - connected since: " + date[i]
				}
			} else if action[0] == "/msg" { // Second action would be a direct message, here we take care of that.
				if len(action) > 1 { // We validate that the input corresponds with what is needed for the command to work properly.
					if len(action) > 2 {
						if to, dest := clientL[action[1]]; to && dest {
							message := ""
							for i := 2; i < len(action); i++ {
								message = message + action[i] + " "
							}
							chann[action[1]] <- "[" + name + "] sent a message to you, it says: " + message
						} else {
							ch <- server + "The user is disconnected or does not exist."
						}
					} else {
						ch <- server + "Write something you want to say..." // If we said we wanted to send a DM to userX but we didn't write the message.
					}
				} else {
					ch <- server + "ERROR: Wrong Usage."
				}
			} else if action[0] == "/time" { // Third action will get us the time of the zone in which the user who ran the command is.
				if err != nil {
					log.Fatal("ERROR: Issue getting the Time Zone.")
				}
				//zip, _ := time.Now().Zone() // In case I wanted to know the exact Time Zone in which the user is in.
				ch <- server + "Local Time: America/Mexico_City " + time.Now().Format("15:04") //+ zip + " "
			} else if action[0] == "/user" { // Fourth action will get someone the information about an especific user.
				if len(action) > 1 { // We validate that the input corresponds with what is needed for the command to work properly... again.
					if username, ok := clientL[action[1]]; username && ok {
						ch <- server + "username: " + action[1] + ", IP: " + connx[action[1]].RemoteAddr().String() + " Connected since: " + date[action[1]]
					} else {
						ch <- server + "The user is disconnected or does not exist."
					}
				} else {
					ch <- server + "Which user you want to check info about?" // Did you say who you wanted to know about?
				}
			} else if action[0] == "/kick" { // Fifth and final action is kick, only available to the admin, someone's being rude? Kick 'em.
				if len(action) > 1 { // We validate that the input corresponds with what is needed for the command to work properly... yet again haha.
					if name == adm {
						if username, ok := clientL[action[1]]; username && ok {
							chann[action[1]] <- server + "You're kicked from this channel" // This lets the user kicked know what happened.
							chann[action[1]] <- server + "Bad language is not allowed on this channel"
							connx[action[1]].Close()
							messages <- server + "[" + action[1] + "] was kicked from channel for bad language policy violation" // This lets every other user know what happened.
							fmt.Println(server + "[" + action[1] + "] was kicked")                                               // This goes to the server.
						} else {
							ch <- server + "The user is disconnected or does not exist."
						}
					} else {
						ch <- server + "The command /kick is only available for admins." // You're not an admin, no rights here, sorry haha.
					}
				} else {
					ch <- server + "Who would you like to kick?" // You didn't say who you wanted to kick tho.
				}
			} else if string(action[0][0]) == "/" {
				ch <- server + "Non-existent command." // No command exists as you wrote it.
			} else {
				messages <- name + " > " + inwards.Text() // This is not a command, just a message to the public chat.
			}
		}
	}

	// NOTE: ignoring potential errors from inwards.Err()
	leaving <- ch
	messages <- server + "[" + name + "] left channel"
	fmt.Println(server + "[" + name + "] left")
	clientL[us.usernameN] = false
	if adm == name {
		for i := range clientL {
			if clientL[i] == true {
				adm = i
				chann[i] <- server + "You're the new IRC Server ADMIN"
				cnt++
				break
			}
		}
		if adm == name {
			adm = ""
		}
	}
	conn.Close()
}

func makeAdm(conn net.Conn) {
	ch := make(chan string)
	go clientWriter(conn, ch)
	name := ""
	user := make([]byte, 128)
	for i := 0; i < len(user); i++ {
		if user[i] == 0 {
			break
		}
		name = name + string(user[i])
	}
	server := "irc-server > "
	adm = name
	ch <- server + "You're the new IRC Server ADMIN"
	fmt.Println(server + "[" + name + "] was promoted as the channel ADMIN")
}

func clientWriter(conn net.Conn, ch <-chan string) {
	for msg := range ch {
		fmt.Fprintln(conn, msg) // NOTE: ignoring network errors
	}
}

//!-handleConn

//!+main
func main() {

	if len(os.Args) != 5 {
		log.Fatal("ERROR: Wrong Usage.")
	} else {
		host := flag.String("host", "", "name of host")
		port := flag.String("port", "", "port #")
		flag.Parse()
		listener, err := net.Listen("tcp", *host+":"+*port)
		if err != nil {
			log.Fatal(err)
		}
		fmt.Println("irc-server > Simple IRC Server started at " + os.Args[2] + ":" + os.Args[4])
		fmt.Println("irc-server > Ready for receiving new clients ")
		go broadcaster()
		for {
			conn, err := listener.Accept()
			if err != nil {
				log.Print(err)
				continue
			}
			go handleConn(conn)
		}
	}
}

//!-main
