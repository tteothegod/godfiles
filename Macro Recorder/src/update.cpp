#include "update.h"
#include "RecordSystem.h"
#include "PlaybackSystem.h"
#include "PlaybackManual.h"
#include "PlaybackRecorded.h"
#include "ProcessRecordedData.h"
#include "MouseMovementParametricizer.h"
#include <vector>
#include <future>




void resetTimer(Properties* prop)
{
    prop->timer.reset();
    std::cout << "Timer has been reset." << std::endl;
}

int initApplication(Properties* prop)
{
    std::cout << "Enter 1 for recording mode or 0 for play mode: ";
    std::cin >> prop->userInput;

    if (prop->userInput == 0)
    {
        prop->playable_macro_index = 0;
        prop->loaded_macro_data = loadFromFile("./recordedMacroData.bin");
        prop->loaded_mouse_drag_data = loadMouseDataFromFile("./processedMouseMovementData.bin");

        std::cout << "recordedMacroData size = " << prop->loaded_macro_data.size() << std::endl;
        std::cout << "processedMouseMovementData size = " << prop->loaded_mouse_drag_data.size() << std::endl;

        float time_between_poses = 0;

        std::vector<float> times_between_pose_updates;
        float prev_time = 0;
        for(int i = 0; i < prop->loaded_mouse_drag_data.size(); i++)
        {
            for(int l = 0; l < prop->loaded_mouse_drag_data[i].durations.size(); l++)
            {
                if(prop->loaded_mouse_drag_data[i].positions[l].x != prop->loaded_mouse_drag_data[i].positions[l + 1].x)
                {
                    times_between_pose_updates.push_back(time_between_poses);
                    time_between_poses = 0;
                }
                else time_between_poses += prop->timer.elapsedSeconds() - prev_time;

                prev_time = prop->timer.elapsedSeconds();
                printf("Time = %f || Pose = (%f, %f)\n", prop->loaded_mouse_drag_data[i].durations[l], prop->loaded_mouse_drag_data[i].positions[l].x, prop->loaded_mouse_drag_data[i].positions[l].y);
            }
        }

        int total_amt = 0;
        float sum = 0;
        for(int i = 0; i < times_between_pose_updates.size(); i++)
        {
            total_amt++;
            sum += times_between_pose_updates[i];
        }

        float avg = sum / total_amt;

        printf("avg time between updated = %f\n", avg);

        // Reset the timer after loading data
        resetTimer(prop);
        pushRecordedExecutableEvents(prop);
    }

    else if (prop->userInput == 1)
    {
        prop->event_data.start_time = 0;
        prop->event_data.stop_time = 0;
        prop->event_data.state = RECORD_NONE;
        v2 current_mouse_pose;
        ScreenRecorder::getMousePosition(current_mouse_pose.x, current_mouse_pose.y);
        prop->prev_mouse_pose = current_mouse_pose;

        // Start monitoring mouse position in a separate thread
        prop->mouseClickThread = std::thread([prop]() {
            ScreenRecorder::monitorMouseAndClicks(prop);
        });
        // // Use a lambda to create a thread
        // prop->positionCallbackThread = std::thread([prop]() {
        //     isMousePositionObtained(prop);
        // });
        // // Start monitoring mouse position in a separate thread
        // prop->mouseClickThread = std::thread([prop]() {
        //     ScreenRecorder::monitorMouseClicks(prop);
        // });


        prop->timer.reset();
        prop->frames_without_callback = 0;
    }
    else if (prop->userInput == 2)
    {
        prop->playable_macro_index = 0;
        prop->loaded_macro_data = loadFromFile("./recordedMacroData.bin");
        std::cout << "Vector size = " << prop->loaded_macro_data.size() << std::endl;
        processRecordedData(prop);
    }
    else if (prop->userInput == 3)
    {
        // Sample data
        using namespace std;
        vector<double> time = {0.0, 1.0, 2.0, 3.0, 4.0};
        vector<double> x_values = {1.0, 2.1, 3.2, 4.3, 5.4};
        vector<double> y_values = {2.0, 3.1, 4.2, 5.3, 6.4};
        int degree = 2;

        // Perform polynomial regression
        polynomialRegression(time, x_values, y_values, degree);

        return 0;
    }

    // Initialize GLFW
    if (1 != 1) {
        return -1;
    }
    else return 1;
}




int update(Properties* props)
{
    if(props->userInput == 0)
    {
        updatePlayback(props);
    }
    else if(props->userInput == 1)
    {
        WrittenRecordingData current_data;
        if (props->event_data.writing_data.size() > 0)
        {
            current_data = props->event_data.writing_data[props->event_data.writing_data.size() - 1];
        }
        switch(props->event_data.state)
        {
            case RECORD_WAIT_TIMER:
            {
            } break;

            case RECORD_MOUSE_DRAG:
            {
                // printf("Written Data: Position = (%f, %f) --------- Time = %f\n", current_data.drag_data.position.x, current_data.drag_data.position.y, current_data.drag_data.time);
            } break;
        }


        if(!props->mouse_moving && !props->mouse_click)
        {
            props->event_data.state = RECORD_WAIT_TIMER;
            // printf("waiting = %d\n",(!props->mouse_moving && !props->mouse_click) );


        }
        else if(props->mouse_moving)
        {
            props->event_data.state = RECORD_MOUSE_DRAG;
            // printf("Mouse Mouse Moving = %d\n", props->mouse_moving);

        }
        else if(props->mouse_click)
        {
            // printf("Mouse Down = %d\n", props->mouse_click);

            switch (props->last_click_type)
            {
                case LEFT_CLICK_DOWN:
                {
                    props->event_data.state = RECORD_LEFT_CLICK_DOWN;
                } break;
                case RIGHT_CLICK_DOWN:
                {
                    props->event_data.state = RECORD_RIGHT_CLICK_DOWN;
                } break;
                case LEFT_CLICK_UP:
                {
                    props->event_data.state = RECORD_LEFT_CLICK_UP;
                } break;
                case RIGHT_CLICK_UP:
                {
                    props->event_data.state = RECORD_RIGHT_CLICK_UP;
                } break;

                default: break;

            };
        }
        else
        {
            printf("None");
            props->event_data.state = RECORD_NONE;
        }


        if (props->mouse_move_event)
        {
            printf("Mouse Move event Occured\n");
            ScreenRecorder::getMousePosition(props->current_mouse_pose.x, props->current_mouse_pose.y);
            props->frames_without_callback = 0;



            // printf("Current Mouse Pose = (%f, %f) \n", props->current_mouse_pose.x, props->current_mouse_pose.y);

            // Calculate the difference between current and previous mouse positions
            v2 diff_between_poses = props->current_mouse_pose - props->prev_mouse_pose;

            float norm_diff = length(diff_between_poses);

            // If the difference is non-zero and it's the first mouse movement
            if (norm_diff != 0 && props->first_mouse_move)
            {
                props->mouse_moving = true;
                props->first_mouse_move = false;
                // printf("Mouse Start Moving\n");
                props->event_data.event_log.push_back(RECORD_MOUSE_DRAG);

            }

            props->prev_mouse_pose = props->current_mouse_pose;
        }
        else
        {
            props->frames_without_callback++;
            // printf("frames = %d\n", props->frames_without_callback);
        }

        // printf("Mouse Moving = %d\n", props->mouse_moving);

        if(props->frames_without_callback > 500000) props->mouse_moving = false;
        // if(props->frames_without_callback > 500000) props->mouse_moving = false;

        if(props->frames_without_callback > 500000)
        // if(props->frames_without_callback > 500000)
        {
            props->first_mouse_move = true;
        }

        float curr_time = props->timer.elapsedSeconds();
        float delay = curr_time - props->delay_start;

        if(delay > 0.018371)
        {
            props->delay_start = props->timer.elapsedSeconds();
            // printf("Current Recording State = %d\n", props->event_data.state);
            recordEvents(props);
        }

        // Update Recording System
        props->mouse_move_event = false;

    }
    else if(props->userInput == 2)
    {


    }
    else return 0;

    // Check for Escape key press
    if (isEscapeKeyPressed())
    {
        return 0;
    }

    return 1;
}

bool isEscapeKeyPressed() 
{
#ifdef __APPLE__
    // Get the current state of the keyboard
    CGEventRef event = CGEventCreate(NULL);
    bool isEscapePressed = (CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode) == 36);
    CFRelease(event);
    if(isEscapePressed) printf("pressed\n");
    return isEscapePressed;
#else
    // Implement similar logic for other platforms if needed
    return false; // Placeholder return for non-macOS platforms
#endif
}

void checkInput(Properties* props) {
    while (props->running) {
        std::string input;
        std::cin >> input;

        if (input == "exit" || input == "quit" || input == "q") {
            props->running = false;
            break;
        }
    }
}
// Define a callback function with the correct signature
CGEventRef mousePositionCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void* userInfo) 
{
#ifdef __APPLE__
    Properties* props = static_cast<Properties*>(userInfo);

    CGPoint cursor = CGEventGetLocation(event);
    // Modify properties here
    printf("Mouse Move event Occured\n");

    props->mouse_move_event = true;
    return event;
#endif
}

void isMousePositionObtained(Properties* props) 
{
#ifdef __APPLE__

    if(props->stopThreads) return;

    // Pass the address of mousePositionObtained as the userInfo
    CGEventMask eventMask = (1 << kCGEventMouseMoved);
    CFMachPortRef eventTap = CGEventTapCreate(kCGSessionEventTap, kCGHeadInsertEventTap, kCGEventTapOptionDefault, eventMask, mousePositionCallback, props);
    if (!eventTap) {
        std::cerr << "Failed to create event tap!" << std::endl;
        return;
    }

    printf("running mouse move loop\n");

    CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
    CGEventTapEnable(eventTap, true);

    // Run the run loop
    CFRunLoopRun();
#endif
}







