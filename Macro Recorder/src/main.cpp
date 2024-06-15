#include <iostream>
#include <iomanip>
#include <future>


#include <OpenGL/gl.h>
#include <GLFW/glfw3.h>
#include"../include/config.h"
#include "update.h"
#include "ProcessRecordedData.h"


using namespace std;



int frame_index = 0;
Properties props;

int main()
{
    props.running = true;
    // Assuming frame_index and props are declared somewhere

    if (frame_index == 0)
    {
        int init_return = initApplication(&props);
        if (init_return == -1) return -1;
    }
    auto inputFuture = std::async(std::launch::async, []() { checkInput(&props); });


    int return_val;

    while (props.running)
    {
        return_val = update(&props);
        if (return_val != 1)
        {
            // Break the loop if update returns a value other than 1
            break;
        }
    }
    // Stop and join the threads
    props.stopAllThreads();
    if(props.userInput == 1)
    {
        // saveToFile(props.event_data.writing_data, "./recordedMacroData.bin");
        printf("Data Saved!\n");
        props.loaded_macro_data = props.event_data.writing_data;
        processRecordedData(&props);
    }
    else if(props.userInput == 1)
    {
        printf("Completed Playback\n");
    }
    props.joinAllThreads();

    return return_val;
}


