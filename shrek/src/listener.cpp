#include "listener.h"

#include <string.h>

#include <signal.h>

// int initWinSock() {
//     WSADATA wsa_data;
//     int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
//     if (result != 0) {
//         std::cerr << "WSAStartup failed with error: " << result << std::endl;
//         return 1;
//     }
//     return 0;
// }

std::vector<v2> normalized_outputs;

int initServerSock() 
{
    {
		struct sigaction act = {};
		act.sa_handler = SIG_IGN;
		sigaction(SIGPIPE, &act, NULL);
	}

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        exit(1);
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(5801); 
    address.sin_addr.s_addr = INADDR_ANY;

    const int enable = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
        perror("setsockopt(SO_REUSEADDR) failed");

    if (bind(sock, (sockaddr*)&address, sizeof(address)) != 0) {
        std::cerr << "Failed to bind socket to local address and port" << std::endl;
        exit(1);
    }
    else
    {
        std::cout << "binded \n";
    }

    if (listen(sock, 1) < 0) {
        std::cerr << "Failed to listen for incoming connections" << std::endl;
        exit(1);
    }

    return sock;
}

bool sendData(int client_socket, std::vector<v2> normalized_outputs)
{
    uint32_t num_outputs = normalized_outputs.size();

    //printf("Number of outputs: %d\n", num_outputs);
    if(send(client_socket, &num_outputs, sizeof(uint32_t), 0) < 0)
    {
        //perror("Failed to send size data");
        return false;
    }


    if (num_outputs)
    {
        printf("Sending position (%f, %f)", normalized_outputs[0].x, normalized_outputs[0].y);
        if(send(client_socket, normalized_outputs.data(), sizeof(v2) * normalized_outputs.size(), 0) < 0)
        {
            printf("Failed to send address data\n");
            return false;
        }
    }

    // for (size_t i = 0; i < num_outputs; i++) 
    // {
    //     v2 output = normalized_outputs[i];

    //     send()

    //     if(num_outputs != 1)
    //     {
    //         cout << num_outputs << std::endl;    
    //     }

    return true;
}


void closeSocket(int server_socket)
{
    close(server_socket);
}

// void* comms(void*)
// {

    
//     int server_socket = initServerSock();

//     std::cout << "comms thread running \n";

//     while (true)
//     {
//         bool connected = false;

//         sockaddr_in client_address;
//         socklen_t client_address_size = sizeof(client_address);
//         int client_socket;
        
//         std::cout << "Waiting for connections \n";
//         while ((client_socket = accept(server_socket, (sockaddr*)&client_address, &client_address_size)) < 0)
//         {
//             std::cerr << "Failed to accept connection to target machine, retrying in 1 second..." << std::endl;
//             std::this_thread::sleep_for(std::chrono::seconds(1));
//         }
        
//         std::cout << "Connection found! \n";
//         connected = true;
    

//         while (connected)
//         {
//             printf("Dirty flag is: %d\n", dirty_flag);

//             pthread_mutex_lock(&mutex);
            
//             if(dirty_flag)
//             {
//                 dirty_flag = false;

//                 printf("Thread Size: %d\n", normalized_outputs.size());
//                 if(!normalized_outputs.empty())
//                 {
//                     printf("Coords (%f, %f)\n", normalized_outputs[0].x, normalized_outputs[0].y);
                
//                 }
                
//                 if (!sendData(client_socket, normalized_outputs))
//                 {
//                     connected = false;
//                 }
//             }

//             pthread_mutex_unlock(&mutex);
//         }
//     }


//     return NULL;
// }
