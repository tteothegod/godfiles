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
#include <sys/ioctl.h>


void sendData(std::vector<v2> normalized_outputs, int sock);
int initSocket();

void updateShrek(int socket);


