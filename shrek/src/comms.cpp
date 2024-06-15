#include "comms.h"
#include <unistd.h>



FILE* test;



int initSocket()
{
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) 
    {
        perror("Failed to create socket");
        return -1;
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(5801);
    address.sin_addr.s_addr = inet_addr("10.44.19.23");

    printf("checkpoint\n");

    // while (connect(sock, (sockaddr*)&address, sizeof(address)) < 0) 
    // {
    //     printf("Failed to connect retrying in 1 second...\n");
    //     sleep(1);
    // }

    printf("Connected!\n");

    test = fopen("../test.csv", "wb");
    fprintf(test, "x,y\n");

    return sock;
}



std::vector<v2> cone_positions;

void updateShrek(int socket)
{
    uint32_t num_cones = 0;


    bool error_flag = false;

    {
        if (!error_flag)
        {

            int bytes = recv(socket, &num_cones, sizeof(num_cones), 0);

            if (bytes < 0)
            {
                printf("Failed to recieve number of cones, setting error flag...\n");
                error_flag = true;
            }
        }

        // printf("Debug: Number of cones is %d\n", num_cones);

        cone_positions.resize(num_cones);

        if (num_cones > 0)
        {
            if (!error_flag)
            {
                if (recv(socket, cone_positions.data(), sizeof(v2) * num_cones, 0) < 0)
                {
                    printf("Failed to recieve cone positions, setting error flag...\n");
                    error_flag = true;
                }
            }
        }

        if (error_flag)
        {
            close(socket);
            printf("Reconnecting to Shrek\n");
            initSocket();

            cone_positions.clear();
        }
    }

    if (cone_positions.size() != 0)
    {
        printf("what is going on??? %f, %f\n", cone_positions[0].x, cone_positions[0].y);
        fprintf(test, "%f,%f\n", cone_positions[0].x, cone_positions[0].y);
    }
}

// int overallInit()
// {
//     int server_socket = initSocket();
// }



