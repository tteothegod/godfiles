#include <iostream>
#include <vector>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <chrono>
#include <thread>
#include <fcntl.h>
#include <string.h>
#include "maths.h"
#include <pthread.h>


extern std::vector<v2> normalized_outputs;

extern pthread_mutex_t mutex;

bool sendData(int client_socket, std::vector<v2> normalized_outputs);
int initServerSock();
int acceptConnect(int sock);
// int initWinSock();
void closeSocket(int server_socket);
void* comms(void*);


