# Reliable_UDP

# Purpose

This program implements and uses Reliable UDP to send a message from the client to the server through a proxy server. The purpose of each is as follows.



* Server 
1. Receive packet from proxy server
2. Checks the validity of the received packet and outputs the data to the console.
3. Send an ACK packet for the packet it received to the proxy server.
* Client
1. Read input from user and send message to proxy server
2. When it receives an acknowledgement packet for the message from the server through the proxy server, go to 1 and repeat the process
3. If it does not receive the acknowledgement packet within 2 seconds, send the packet again to the proxy server.
* Proxy Server
1. Receives a packet from the client and sends it to the server
2. Receives an acknowledgment from the server and sends it to the client
3. Drop some packets with a certain probability to check the reliability of the protocol. In other words, it does not forward the received packet to its destination sometimes.


# Intended Users

This guide is intended for users who have basic experience with the POSIX system.



* Users who can run programs using a terminal on a POSIX system
* Users who know what the command line arguments and flags are
* Users who know how to use cmake.


# Software Requirements



* Clang-tidy
* Cmake
* Dot (This is used by Doxygen)
* Doxygen (Although this is not necessary to run the program, it is intended to be used when building this program, so if you want to build without creating a Doxygen file, edit CMakeLists.txt)
* Standard C17 compiler (e.g. GCC, Clang)


# Platform



* macOS
* Fedora 36

I tested programs on macOS and Fedora Linux (Fedora 36).


# Installing


## Obtaining

Clone this repo with the following command.


```
git clone https://github.com/hurshik7/reliable_udp
```


Then, you can see the `source` directory.


## Building and Installing

First, go into the `source/server/` directory and build the server with the following commands.


```
cd source/server
cmake -S . -B build
cmake --build build
```


Second, go back to the `source/` directory and go into the `client/` directory to build the client.


```
cd ..
cd client
cmake -S . -B build
cmake --build build 
```


Finally, go back to the `source/` directory again and go into the `proxy_server/` directory to build the proxy server.

Note: The compiler can be specified by passing one of the following to cmake when configure the project:



* `-DCMAKE_C_COMPILER="gcc"`
* `-DCMAKE_C_COMPILER="clang"`

(For example, `cmake -S . -B build -DCMAKE_C_COMPILER="gcc")`


## Running



* Server

 	You are able to configure the ip address and port number of the server. The default port number is `5050` if you do not input the argument. If you have a firewall turned on, I recommend turning it off. These are the examples of commands to run the server. Let’s assume the current directory is `source/server/` when running the server.

	`./build/udp_server`


```
	./build/udp_server -i <ip address>
	./build/udp_server -p <port number>
	./build/udp_server -i <ip address> -p <port number>

```



* Client

You are able to configure the IP address and port of the **proxy server**. Let's assume the current directory is `source/client/` when running the client. If you do not input the port number with the -p flag, the default port number for the proxy server is `5050`. (The ip address of the proxy server must be provided with -o flag)

	`./build/udp_client -o 192.168.0.1`


```
	./build/udp_client -o 192.168.0.1 -p 5000

```



* Proxy server

You are able to configure the ip address and port number of this proxy server and also server’s ip address and port number. You must provide the ip address of the server with the -o flag. For the port number, use -p flag for the proxy server’s port, and -P for the server’s port.

 	You also can configure the % chance of dropping a message from the client, and an acknowledgement from the server. Use -r flag for the % chance of dropping a message from the client and -R for the acknowledgement. The default % chance of dropping them is **50%**. Let’s see examples. (also, let’s assume that we are at the `source/proxy_server` directory)


```
    ./build/udp_proxy -i 192.168.0.1 -o 192.168.0.2
    ./build/udp_proxy -i 192.168.0.1 -p 5050 -o 192.168.0.2
    ./build/udp_proxy -i 192.168.0.1 -o 192.168.0.2 -P 8080
    ./build/udp_proxy -i 192.168.0.1 -p 5050 -o 192.168.0.2 -P 8080
    ./build/udp_proxy -i 192.168.0.1 -p 5050 -o 192.168.0.2  -P 8080 -r 20 -R 80
```



    //  it means that this proxy server will drop a message with 20% chance and an acknowledgement with 80% of chance.


## Environment Variable

If you are using a **Mac** and the following message appears when you run the program either Server or Client, set the following environment variable. 


```
malloc: nano zone abandoned due to inability to preallocate reserved vm space.


<table>
  <tr>
   <td>```

<strong>Variable</strong>
   </td>
   <td><strong>Value</strong>
   </td>
  </tr>
  <tr>
   <td>MallocNanoZone
   </td>
   <td>0
   </td>
  </tr>
</table>


Then, it does not show the message any more. The above message does not cause any problems running these programs, it only appears on macOS among POSIX systems. For more details and solutions, please refer to the following. 

[https://stackoverflow.com/questions/64126942/malloc-nano-zone-abandoned-due-to-inability-to-preallocate-reserved-vm-space](https://stackoverflow.com/questions/64126942/malloc-nano-zone-abandoned-due-to-inability-to-preallocate-reserved-vm-space)


# Features


## Server



* All socket communication uses UDP.
* When you run the server you can see a message with the port number. The default port number is `5050`.

    ```
    Listening on port 5050
    ```


* You can configure the server’s ip address and port number by passing flags (-i for the ip address and -p for the port number) when you run the server. For example,

    ```
    ./udp_server -i 192.168.0.4 -p 8080
    ```


* If you want to exit the server, press command + c on macOS, ctrl + c or ctrl + d on Fedora linux.
* When the server receives a packet from a proxy server it shows the message in the packet on the console (stdout).


* The server continues to receive a packet until the client sends a FIN packet.
* When the server receives a packet, it checks whether the packet is valid, and if it is corrupted, it sends a NAK packet, and if it is valid, it sends an ACK packet.
* Data is validated through checksums.
* If the server receives the same packet as the previous packet, it does not show the message in that packet again and sends only an ACK again.
* If the server receives a FIN packet, it sends an ACK packet for that FIN and prints the message that the client's message transmission is complete. After that, the server does not shut down and continues to wait for packets to be received. (You can also receive transmissions from other clients.)


## Client


* All socket communication uses UDP.
* When you run the client you can configure the ip address and port number of the proxy server. The default port number for the proxy server is `5050`.

    ```
    ./udp_client -i 192.168.0.5 -p 5050

    ```


* Not only can you type messages through the keyboard and send them to the proxy server, you can also use STDIN redirection. For example,

	`./udp_client -o 192.168.0.5 -p 5050 &lt; msg.txt`



* When sending a message in a packet, the checksum value is calculated and sent together.
* When a client sends a message (messages are separated by a new line ‘\n’), it waits for an ACK for that message. If it receives the ACK, it will read the next message. However, if it does not receive the ACK within 2 seconds, it resends the previous message again.
* When a user inputs EOF (ctrl + d), it sends a FIN packet and waits for the ACK for that FIN packet. If it receives the ACK, the program is terminated, otherwise it continues to send FIN packets.


## Proxy Server



* All socket communication uses UDP.
* When you run the proxy server you can configure the ip address and port number of the server and also this proxy server’s ip address and port number. The default port number for the proxy server and the server is `5050`. 


```
./udp_proxy -i 192.168.0.5 -p 5050 -o 192.168.0.4 -P 8080
```


	(-p is for this proxy server’s port number and -P is for the server’s)



* The proxy server will randomly choose to drop a message from client or send it to the server
* The proxy server will randomly choose to drop the acknowledgement from the server or send it to the client.
* When you run the proxy server, you can configure the % chance of dropping a packet by passing -r -R flags. For example,

	`./udp_proxy -i 192.168.0.5 -o 192.168.0.4 -r 50 -R 50`

`-r`: the % chance of dropping client’s packet

	`-R`: the % chance of dropping server’s ACK



* The proxy server uses multithreading to run both tasks (client packet to server, server's ACK to client) simultaneously. (pthreads are used for this)

