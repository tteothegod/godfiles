#pragma once
#include <iostream>
#include "RecordSystem.h"
#include <functional>
#include <fstream>
#include "ProcessRecordedData.h"




#define PLAYBACKMGR_MAX_EVENTS (1024000)

struct PlaybackMgr;

enum PlaybackEvent
{
    EVENT_NONE = 0,
    EVENT_RIGHT_CLICK,
    EVENT_LEFT_CLICK_DOWN,
    EVENT_MOUSE_DRAG,
    EVENT_WAIT_TIMER,

    EVENT_LEFT_CLICK_UP,
    EVENT_RIGHT_CLICK_DOWN,
    EVENT_RIGHT_CLICK_UP,

    // Write events above this
    EVENT_LIST
};

struct PlaybackData_Wait_Timer
{
    float start_time;
    float duration;
};

struct PlaybackData_Mouse_Drag
{
    int mouse_drag_index;
    float start_time;
    float duration;
    v2 position;   
    int working_index;
};
struct PlaybackData_Mouse_Click
{
    v2 position;
    float time;
};


struct Event
{
    
    bool manual_event_complete = false;
    bool started = false;
    float event_timer;
    float start_time = 0;
    float stop_time = 0;
    float duration = 0;
    PlaybackEvent type = (PlaybackEvent) EVENT_NONE;

    std::pair<std::string, std::string> mouse_path_equations;
    std::function<float(float)> polynomial_x;
    std::function<float(float)> polynomial_y;
    union
    {
        PlaybackMgr* list;
        PlaybackData_Wait_Timer wait_timer;
        PlaybackData_Mouse_Drag mouse_drag;
        PlaybackData_Mouse_Click mouse_click;
    };
};

struct PlaybackMgr
{
	uint64_t write_head = 0;
	uint64_t read_head = 0;

	// this is a ring buffer, keep that in mind
	Event playback_buffer[PLAYBACKMGR_MAX_EVENTS] = {};

	bool is_parallel = false;
};

bool pushEvent(PlaybackMgr* mgr, Event event);

void updateManager(PlaybackMgr* mgr, Properties* props);

std::vector<SavedProcessData> loadMouseDataFromFile(const std::string& filename);


