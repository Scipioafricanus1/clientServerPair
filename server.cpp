//
// Brent Harris

// A simple server in the internet domain using TCP
// The port nu1mber is passed as an argument



#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
//#include <winsock.h>
#include "algorithm"

using namespace std;

template <typename Iterator, typename Delimiter> //variadic functions made by cdhowie on stackOverflow.
bool split_into(Iterator begin, Iterator end, Delimiter, std::string & first) //Splits by delimiter.
{
    first.assign(begin, end);
    return true;
}

template <typename Iterator, typename Delimiter, typename... Strings>
bool split_into(Iterator begin, Iterator end, Delimiter d, std::string & first, Strings & ... strings) //Splits by delimiter.
{
    auto i = std::find(begin, end, d);

    first.assign(begin, i);

    if (i != end) {
        return split_into(++i, end, d, strings...);
    }

    return sizeof...(Strings) == 0;
}
class Salary //Class for containing the major, and two salaries.
{
    string major;
    int lowSalary;
    int highSalary;
public:
    Salary(string _major, int _lowSalary, int _highSalary)
    {
        major = _major;
        lowSalary = _lowSalary;
        highSalary = _highSalary;
    }
    string getMajor() const { return major; }
    int getLowSalary() const { return lowSalary; }
    int getHighSalary() const { return highSalary;}
};

vector<Salary> table;
string delSpaces(string &str) //function from ddacot of stackoverflow. Removes spaces from string.
{
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    return str;
}


void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    ifstream input("salaries.txt");
    string str;
    while( getline(input, str))
    {
        //str = delSpaces(str);
        string major;
        string strLowSalary;
        string strHighSalary;
        split_into(str.begin(), str.end(), '\t', major, strLowSalary, strHighSalary);
        int lowSalary = stoi(strLowSalary);
        int highSalary = stoi(strHighSalary);
        cout << "Major: " << major << endl;
        cout << "Low Salary: " << lowSalary << endl;
        cout << "High Salary: " << highSalary << endl;
        table.push_back(Salary(major, lowSalary, highSalary));
    }
    int sockfd, newsockfd, portno;
    unsigned int clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error(const_cast<char *>("ERROR opening socket"));
    bzero((char *) &serv_addr, sizeof(serv_addr));
    cout << "Please declare the port number for the server: ";
    cin >> portno;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
        error(const_cast<char *>("ERROR on binding"));
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0)
        error(const_cast<char *>("ERROR on accept"));
    bzero(buffer,256);
    n = read(newsockfd,buffer,255);
    if (n < 0) error(const_cast<char *>("ERROR reading from socket"));
    printf("Here is the message: %s\n",buffer);
    n = write(newsockfd,"I got your message",18);
    if (n < 0) error(const_cast<char *>("ERROR writing to socket"));
    return 0;
}
