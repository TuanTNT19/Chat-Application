Create by TuanTNT19


# Project Overview
This program utilizes concepts such as threads, IPC/socket to establish connections and facilitate communication between multiple devices. Users interact with the system using various commands, each with different features.

## Directories Structure
- `bin`: Directory for executable files
- `obj`: Directory for object files
- `inc`: Directory for header files
- `src`: Directory for source files
- `lib`: Directory for static and shared libraries

## Commands
1. **help**: Display information about available user interface options or command manual.
2. **myip**: Display the IP address of this process. Note: The IP should not be your "Local" address (127.0.0.1). It should be the actual IP of the computer.
3. **myport**: Display the port on which this process is listening for incoming connections.
4. **connect \<destination\> \<port no\>**: Establish a new TCP connection to the specified \<destination\> at the specified \<port no\>. The \<destination\> is the IP address of the computer. Invalid connection attempts should be rejected with a suitable error message. Success or failure in connections between two peers should be indicated by both peers using suitable messages. Self-connections and duplicate connections should be flagged with appropriate error messages.
5. **list**: Display a numbered list of all connections this process is part of, including connections initiated by this process and other processes. The output should display the IP address and listening port of all connected peers.
   - E.g., 
     ```
     1: 192.168.21.20 4545
     2: 192.168.21.21 5454
     3: 192.168.21.23 5000
     4: 192.168.21.24 5000
     ```
6. **terminate \<connection id.\>**: Terminate the connection listed under the specified number when the `list` command is used to display all connections. E.g., `terminate 2`. Display an error message if a valid connection does not exist as number 2. If a remote machine terminates one of your connections, display a message.
7. **send \<connection id.\> \<message\>**: Send the message to the host on the connection designated by the number 3 when the `list` command is used. The message can be up to 100 characters long, including blank spaces. On successfully executing the command, the sender should display "Message sent to \<connection id\>" on the screen. On receiving any message from the peer, the receiver should display the received message along with the sender information.
   - E.g., 
     ```
     Message received from 192.168.21.20
     Sender’s Port: <The port no. of the sender>
     Message: “<received message>”
     ```
8. **exit**: Close all connections and terminate this process. Other peers should update their connection list by removing the exiting peer.

## Usage
- `make mk_obj`: Build object files
- `make all`: Build runnable file
- `make run1`: Listening at port 6000
- `make run2`: Listening at port 5000
- `make run3`: Listening at port 4000
- `make clean`: Clean files in obj, bin