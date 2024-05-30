#Client Program (client.c)
This program is a client for remote directory listing services. It communicates with the server through a socket to get and print the contents of the specified directory.

Main functions:
Create a socket: Use socket(AF_INET, SOCK_STREAM, 0) to create a stream socket.
Set server address: Use inet_addr("127.0.0.1") and htons(PORTNUM) to set the server's IP address and port number.
Connect to server: Use the connect() function to connect to the server.
Send request: Send a request to the server through the write() function.
Receive response: Use the read() function to read the server's response and print it to standard output.

#Server Program (server.c)
This program is a remote directory listing server that accepts client connections through a socket, runs the ls command and returns the results to the client.

Main functions:
Create a socket: Use socket(PF_INET, SOCK_STREAM, 0) to create a stream socket.
Bind port: Bind the socket to the specified port 15000.
Listen for connections: Use listen(sock_id, 1) to start listening for connections.
Accept connections: Use the accept() function to accept the client's connection request.
Read requests: Read the directory request sent by the client.
Clean input: Use the sanitize() function to clean the input to prevent injection attacks.
Execute commands: Execute the ls command through the popen() function and write the results back to the client.
