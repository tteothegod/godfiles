#pragma once
#include <iostream>
#include <chrono>
#include <vector>
#include "maths.h"

struct Properties;

enum Events
{
    RECORD_NONE = 0,
    RECORD_RIGHT_CLICK,
    RECORD_LEFT_CLICK_DOWN,
    RECORD_MOUSE_DRAG,
    RECORD_WAIT_TIMER,
    
    RECORD_LEFT_CLICK_UP,
    RECORD_RIGHT_CLICK_DOWN,
    RECORD_RIGHT_CLICK_UP,

};

struct EventData_Wait_Timer
{
    float duration;
};

struct EventData_Mouse_Drag
{
    v2 position;
    float time;
};
struct EventData_Mouse_Click
{
    v2 position;
    float time;
};

struct WrittenRecordingData
{
    float elapsed_time = 0;;
    Events recorded_state = RECORD_NONE;
    float start_time = 0;
    float stop_time = 0;
    EventData_Wait_Timer timer_data = {0};
    EventData_Mouse_Drag drag_data = {{0,0}, 0};
    EventData_Mouse_Click click_data = {{0,0}, 0};
};

struct RecordingEventData
{
    std::vector<WrittenRecordingData> writing_data;
    float event_timer;
    float curr_time, prev_time;
    float start_time, stop_time;
    std::vector<Events> event_log;
    bool event_started = false;
    Events prev_state;
    Events state;
    union
    {
        EventData_Wait_Timer timer;
        float idk;
    };
};




class Timer {
private:
    std::chrono::steady_clock::time_point startTime;
public:
    Timer();
    float elapsedSeconds() const;
    float elapsedMilliseconds() const;
    void reset();
};

void recordEvents(Properties* props);

void saveToFile(const std::vector<WrittenRecordingData>& data, const std::string& filename);

std::vector<WrittenRecordingData> loadFromFile(const std::string& filename);




