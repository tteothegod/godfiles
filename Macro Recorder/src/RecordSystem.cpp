#include "update.h"
#include "RecordSystem.h"
#include <fstream>


void eventRecordStart(Properties* props)
{
    props->event_data.event_timer = 0;
    props->event_data.prev_time = props->timer.elapsedSeconds();

    props->event_data.start_time = props->timer.elapsedSeconds();

    switch(props->event_data.state)
    {
        case RECORD_NONE:
        {
            printf("Starting Record Event None.. \n");


        } break;
        case RECORD_WAIT_TIMER:
        {
            printf("Starting Record Event Wait Timer.. \n");

        } break;

        case RECORD_MOUSE_DRAG:
        {
            printf("Starting Record Event Mouse Drag.. \n");
        
        } break;

        case RECORD_LEFT_CLICK_DOWN:
        {
            printf("Starting Record Event Left Click.. \n");
        
        } break;

        case RECORD_RIGHT_CLICK:
        {
            printf("Starting Record Event Right Click.. \n");
        
        } break;
    }

}

void eventRecordStop(Events state, Properties* props)
{

    props->event_data.stop_time = props->timer.elapsedSeconds();

    switch(state)
    {
        case RECORD_NONE:
        {
            // printf("Terminating Record Event None.. \n");

        } break;
        case RECORD_WAIT_TIMER:
        {
            // printf("Terminating Record Event Wait Timer.. \n");

        } break;

        case RECORD_MOUSE_DRAG:
        {
            // printf("Terminating Record Event Mouse Drag.. \n");
        
        } break;
    }

}

void recordStep(Properties* props)
{
    props->event_data.curr_time = props->timer.elapsedSeconds();

    float delta_time = props->event_data.curr_time - props->event_data.prev_time;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
    props->event_data.event_timer += delta_time;
    // printf("Event Timer %f\n", props->event_data.event_timer);
    props->event_data.prev_time = props->event_data.curr_time;

    WrittenRecordingData pushable_data = {
        props->timer.elapsedSeconds(),
        props->event_data.state,
        props->event_data.start_time,
        props->event_data.stop_time,
        { 0 },
        (EventData_Mouse_Drag) {{0,0}, 0},
        {{0, 0}, 0},
    };

    switch(props->event_data.state)
    {
        case RECORD_NONE:
        {

        } break;
        case RECORD_WAIT_TIMER:
        {
            pushable_data.timer_data.duration = props->event_data.event_timer;

        } break;

        case RECORD_MOUSE_DRAG:
        {
            pushable_data.drag_data = {props->current_mouse_pose, props->timer.elapsedSeconds() };
        } break;

        case RECORD_LEFT_CLICK_DOWN:
        {
            pushable_data.click_data = {props->current_mouse_pose, props->timer.elapsedSeconds() };
            props->mouse_click = false;

        } break;

        case RECORD_RIGHT_CLICK_DOWN:
        {
            pushable_data.click_data = {props->current_mouse_pose, props->timer.elapsedSeconds() };
            props->mouse_click = false;

        } break;

        case RECORD_LEFT_CLICK_UP:
        {
            pushable_data.click_data = {props->current_mouse_pose, props->timer.elapsedSeconds() };
            props->mouse_click = false;

        } break;

        case RECORD_RIGHT_CLICK_UP:
        {
            pushable_data.click_data = {props->current_mouse_pose, props->timer.elapsedSeconds() };
            props->mouse_click = false;

        } break;

    }
    props->event_data.writing_data.push_back(pushable_data);
}

void recordEvents(Properties* props)
{
    if(!props->event_data.event_started)
    {
        props->event_data.event_started = true;
        eventRecordStart(props);
    }

    recordStep(props);

    if(props->event_data.state != props->event_data.prev_state) 
    {
        props->event_data.event_started = false;
        eventRecordStop(props->event_data.prev_state, props);
    }
    props->event_data.prev_state = props->event_data.state;
}

void saveToFile(const std::vector<WrittenRecordingData>& data, const std::string& filename)
{
    std::ofstream outputFile(filename, std::ios::binary);
    if (!outputFile.is_open())
    {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return;
    }

    // Write the size of the vector first
    size_t dataSize = data.size();
    outputFile.write(reinterpret_cast<const char*>(&dataSize), sizeof(size_t));

    // Write each WrittenRecordingData object to the file
    for (const auto& recording : data)
    {
        outputFile.write(reinterpret_cast<const char*>(&recording), sizeof(WrittenRecordingData));
    }

    outputFile.close();
}

std::vector<WrittenRecordingData> loadFromFile(const std::string& filename)
{
    std::vector<WrittenRecordingData> loadedData;
    std::ifstream inputFile(filename, std::ios::binary);
    if (!inputFile.is_open())
    {
        std::cerr << "Error opening file for reading: " << filename << std::endl;
        return loadedData; // Return empty vector
    }

    // Read the size of the vector
    size_t dataSize;
    inputFile.read(reinterpret_cast<char*>(&dataSize), sizeof(size_t));

    // Resize the vector to hold the loaded data
    loadedData.resize(dataSize);

    // Read each WrittenRecordingData object from the file
    for (auto& recording : loadedData)
    {
        inputFile.read(reinterpret_cast<char*>(&recording), sizeof(WrittenRecordingData));
    }

    inputFile.close();

    return loadedData;
}


Timer::Timer() : startTime(std::chrono::steady_clock::now()) {}

float Timer::elapsedSeconds() const {
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> duration = currentTime - startTime;
    return duration.count();
}

float Timer::elapsedMilliseconds() const {
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float, std::milli> duration = currentTime - startTime;
    return duration.count();
}

void Timer::reset() {
    startTime = std::chrono::steady_clock::now();
}