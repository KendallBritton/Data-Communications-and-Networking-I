#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <fstream>
using namespace std; 

//Server
int main(int argc, char *argv[]){

if(argc != 2){ //Specifiy a port number
    cerr << "Usage: port" << endl;
    return -1;
}

int port = atoi(argv[1]); //Obtains the port number
char message[1500]; //Buffer that allows to send/receive messages

sockaddr_in servAddress; //Sets up the socket and the connections
bzero((char*)&servAddress, sizeof(servAddress));
servAddress.sin_family = AF_INET;
servAddress.sin_addr.s_addr = htonl(INADDR_ANY);
servAddress.sin_port = htons(port);

int serverFd = socket(AF_INET, SOCK_DGRAM, 0); //Checks socket file descriptor
if (serverFd < 0){
    cerr << "Sever socket establishment error" << endl;
    return -1;
}

//Bind the socket to its local address
int socketBind = bind(serverFd, (struct sockaddr*) &servAddress,sizeof(servAddress));

if(socketBind < 0){
    cerr << "Error in binding to local address" << endl;
    return -1;
}

cout << "Waiting for a client to connect..." << endl;

listen(serverFd, 5); //Listening to up to 5 requests


cout << "Connected with client." << endl;

//Allows us to keep track of data sent
int bytesRead = 0;
int bytesWritten = 0;

while(1){
    //Receive a message from the client (listen)
    cout << "Awaiting client response..." << endl;
    memset(&message, 0, sizeof(message)); //Clears the buffer

    bytesRead += recv(serverFd, (char*) &message, sizeof(message), 0);

    if(!strcmp(message, "exit")){
        cout << "The Client has left the session" << endl;
        break;
    }

    cout << "Client: " << message << endl;
    cout << ">";
    string data;
    getline(cin, data);
    memset(&message, 0, sizeof(message)); //Clears the buffer
    strcpy(message, data.c_str());
    if (data == "exit"){
        //Send to client that server connection closed
        send(serverFd, (char*) &message, strlen(message), 0);
        break;
    }

    //Send the message to client
    bytesWritten += send(serverFd, (char*) &message, strlen(message),0);
}

close(serverFd); //Closing the socket descriptors
cout << "**********Session Ended**********" << endl;
cout << "Connection closed." << endl;

return 0;
}