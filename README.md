Create by TuanTNT19

* Overview
Program to base on some knowledges such as thread, IPC/socket to create a connection and communication betwen many devices. User communicates with system by some command. Each command has difference feature


* Directories Structure:\\
  bin : Directory for the exe file\\
  obj : Directory for the object files\\
  inc : Directory for the header files\\
  src : Directory for the sources files\\
  lib : Directory for the static and Share lib\\


* Explain command\\
1. help: Display information about the available user interface options or command manual.\\
2. myip: Display the IP address of this process.\\
Note: The IP should not be your “Local” address (127.0.0.1). It should be the actual IP of the computer.\\
3. myport: Display the port on which this process is listening for incoming connections.\\
4. connect <destination> <port no> : This command establishes a new TCP connection to the specified
<destination> at the specified < port no>. The <destination> is the IP address of the computer. Any attempt
to connect to an invalid IP should be rejected and suitable error message should be displayed. Success or
failure in connections between two peers should be indicated by both the peers using suitable messages.
Self-connections and duplicate connections should be flagged with suitable error messages.\\
5. list: Display a numbered list of all the connections this process is part of. This numbered list will include connections initiated by this process and connections initiated by other processes. The output should display the IP address and the listening port of all the peers the process is connected to.\\
E.g., id: IP address Port No.\\

1: 192.168.21.20 4545\\

2: 192.168.21.21 5454\\

3: 192.168.21.23 5000\\

4: 192.168.21.24 5000\\

6. terminate <connection id.> This command will terminate the connection listed under the specified
number when LIST is used to display all connections. E.g., terminate 2. In this example, the connection
with 192.168.21.21 should end. An error message is displayed if a valid connection does not exist as
number 2. If a remote machine terminates one of your connections, you should also display a message.\\
7. send <connection id.> <message> (For example, send 3 Oh! This project is a piece of cake). This will
send the message to the host on the connection that is designated by the number 3 when command “list” is
used. The message to be sent can be up-to 100 characters long, including blank spaces. On successfully
executing the command, the sender should display “Message sent to <connection id>” on the screen. On
receiving any message from the peer, the receiver should display the received message along with the
sender information.\\
(Eg. If a process on 192.168.21.20 sends a message to a process on 192.168.21.21 then the output on
192.168.21.21 when receiving a message should display as shown:\\

Message received from 192.168.21.20\\

Sender’s Port: <The port no. of the sender>\\

Message: “<received message>”\\
8. exit Close all connections and terminate this process. The other peers should also update their connection
list by removing the peer that exits


*Usage:\\
make mk_obj: build object file\\
make all: build runfile\\
make run1: listening at port 6000\\
make run2: listening at port 5000\\
make run3: listening at port 4000\\
make clean: clean file in obj, bin\\
