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


//Client
int main(int argc, char *argv[]){

//Checks to see if the IP address and port number is correct
if(argc != 3){
    cerr << "Usage: ip_address port" << endl;
    return -1;
}

char *serverIp = argv[1]; //Grabs the IP address
int port = atoi(argv[2]); //Grabs the port number

char message[1500]; //Creates a buffer for the message

struct hostent* host = gethostbyname(serverIp); //Sets up the socket and connections
sockaddr_in sendSockAddress;
bzero((char*) &sendSockAddress, sizeof(sendSockAddress));
sendSockAddress.sin_family = AF_INET;
sendSockAddress.sin_addr.s_addr = 
    inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
sendSockAddress.sin_port = htons(port);
int clientFd = socket(AF_INET, SOCK_STREAM, 0);

//Checks to see if connection is properly established
int status = connect(clientFd, (sockaddr*) &sendSockAddress, sizeof(sendSockAddress));
if(status < 0){
    cout << "Socket connection error" << endl;
    return -1;
}

cout << "Connected to the server" << endl;

//Allows us to keep track of data sent
int bytesRead = 0;
int bytesWritten = 0;

while(1){
    cout << ">";
    string data;
    getline(cin,data);
    memset(&message, 0, sizeof(message)); //Clears the buffer
    strcpy(message, data.c_str());
    if(data == "exit"){
        send(clientFd, (char*) &message, strlen(message), 0);
        break;
    }
    bytesWritten += send(clientFd, (char*) &message, strlen(message),0);
    
    memset(&message, 0, sizeof(message)); //Clears the buffer
    cout << "Awaiting server response.." << endl;
    bytesRead += recv(clientFd, (char*) &message, sizeof(message), 0);

    if(!strcmp(message, "exit")){
        cout << "Server connection has ended." << endl;
        break;
    }

    cout << "Server: " << message << endl;
}

close(clientFd);
cout << "**********Session Ended**********" << endl;
cout << "Connection closed." << endl;

return 0;
}