#include <iostream>
#include <string>
#include <sstream>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

int main() {
    // ChatGPT API endpoint URL and API key
    string apiEndpoint = "https://api.openai.com/v1/engines/davinci/";
    string apiKey = "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";
    string data = "{\"prompt\": \"Hello, world!\", \"max_tokens\": 50, \"temperature\": 0.5}";

    // HTTP GET request message with the Authorization header
    stringstream ss;
ss << "GET /v1/engines/davinci/completions HTTP/1.1\r\n";
ss << "Host: api.openai.com\r\n";
ss << "Authorization: Bearer " << apiKey << "\r\n";
ss << "Content-Type: application/json\r\n";
ss << "Content-Length: " << data.length() << "\r\n";
ss << "Connection: close\r\n";
ss << "\r\n";
ss << data;
string requestMsg = ss.str();
cout << requestMsg;

    // Resolve the API endpoint IP address
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int status = getaddrinfo(apiEndpoint.c_str(), "http", &hints, &res);
    if (status != 0) {
        cerr << "getaddrinfo() failed: " << gai_strerror(status) << endl;
        return 1;
    }

    // Create a socket and establish a connection to the API server
    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        cerr << "socket() failed: " << strerror(errno) << endl;
        freeaddrinfo(res);
        return 1;
    }

    if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        cerr << "connect() failed: " << strerror(errno) << endl;
        close(sockfd);
        freeaddrinfo(res);
        return 1;
    }

    // Send the HTTP request message to the API server
    if (send(sockfd, requestMsg.c_str(), requestMsg.size(), 0) == -1) {
        cerr << "send() failed: " << strerror(errno) << endl;
        close(sockfd);
        freeaddrinfo(res);
        return 1;
    }

    // Receive the API response from the server
    char buffer[1024];
    string responseMsg = "";
    int numBytesReceived;
    while ((numBytesReceived = recv(sockfd, buffer, sizeof(buffer), 0)) > 0) {
        responseMsg.append(buffer, numBytesReceived);
    }

    // Print the API response message to the console
    cout << responseMsg << endl;

    // Close the socket connection
    close(sockfd);
    freeaddrinfo(res);

    return 0;
}