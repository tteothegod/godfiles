#pragma once
#include "maths.h"
#include <iostream>
#include "ScreenInteractor.h"
#include "RecordSystem.h"
#include "PlaybackSystem.h"
#include "ScreenInteractor.h"
#include <thread>
#include <atomic>

#ifdef __APPLE__
#include <ApplicationServices/ApplicationServices.h>
// #include <HIToolbox/Events.h> // Include the header file

#endif


struct Properties
{
    bool running;
    int userInput;

    int playable_macro_index;

    bool first_mouse_move = true;
    bool mouse_moving = false;
    bool mouse_move_event = false;
    bool mouse_click = false;

    float delay_start = 0;

    v2 current_mouse_pose;
    v2 prev_mouse_pose;
    std::thread mouseClickThread;
    std::thread positionCallbackThread;

    RecordingEventData event_data;
    PlaybackMgr mgr;

    ClickType last_click_type;

    std::vector<WrittenRecordingData> loaded_macro_data;
    std::vector<SavedProcessData> loaded_mouse_drag_data;


    int frames_without_callback;

    std::atomic<bool> stopThreads{false};  // Atomic flag to stop the threads


    // Default constructor
    Properties() {}

    Timer timer;

    // Method to stop all threads
    void stopAllThreads() {
        stopThreads.store(true);  // Set the flag to stop the threads
    }

    // Method to join all threads
    void joinAllThreads() {
        if (mouseClickThread.joinable())
            mouseClickThread.join();

        if (positionCallbackThread.joinable())
            positionCallbackThread.join();
    }
};



//Julian Iso Isoxo

int update(Properties* props);
int initApplication(Properties* prop);
bool isEscapeKeyPressed();

void isMousePositionObtained(Properties* props);

void checkInput(Properties* props);

// bool isMousePositionObtained();


CGEventRef mousePositionCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void* userInfo);
