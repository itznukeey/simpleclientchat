#include <iostream>
#include <sys/socket.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <zconf.h>
#include <cstring>

#define PORT 8888
#define SERVER_IP "127.0.0.1"
#define INTERNAL_ERROR 500
using namespace std;

int main() {

    auto clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress;

    if (clientSocket < 0) {
        cerr << "Error while creating client socket" << endl;
        exit(INTERNAL_ERROR);
    }

    bzero(&serverAddress, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);

    //localhost
    serverAddress.sin_addr.s_addr = inet_addr(SERVER_IP);

    auto connection = connect(clientSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress));

    if (connection < 0) {
        cerr << "Error while connecting to the server";
        exit(INTERNAL_ERROR);
    }

    char buffer[BUFSIZ];

    while (true) {
        bzero(buffer, BUFSIZ);
        cout << "Enter message: ";
        string input;
        getline(cin,input);
        strcpy(buffer,input.c_str());

        if (input == "exit" | input == "EXIT" | input == "") {
            write(clientSocket, buffer, sizeof(buffer));
            close(clientSocket);
            cout << "closed" << endl;
            break;
        }

        write(clientSocket, buffer, sizeof(buffer));
        bzero(buffer,sizeof(buffer));
        read(clientSocket,buffer,sizeof(buffer));


        cout << "Server: " << buffer << endl;
    }

}