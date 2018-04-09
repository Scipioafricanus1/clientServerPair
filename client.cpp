// Brent Harris
// A Client program
// It compiles without warnings with gcc
#include <unistd.h>
#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include <limits>
//#include <winsock.h>                 //(((((((((((((((((((((((((REMOVE LATER)))))))))))))))))


using namespace std;

void error(char *msg)
{
    perror(msg);
    exit(0);
}
int main(int argc, char *argv[])
{
    int sockfd, portno, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];

    string hostName, message;

    cout<< "Please declare the host name for the server: ";
    cin >> hostName;

    cout << "Please declare the port number for the server: ";
    cin >> portno;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error(const_cast<char *>("ERROR opening socket"));
    const char *charHostName = hostName.c_str();
    server = gethostbyname(charHostName);


    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        error(const_cast<char *>("ERROR connecting"));
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    printf("Please enter the message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    fflush (stdin);
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0)
        error("ERROR writing to socket");
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0)
        error(const_cast<char *>("ERROR reading from socket"));
    printf("%s\n",buffer);
    return 0;
}
