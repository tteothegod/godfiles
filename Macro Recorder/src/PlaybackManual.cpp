#include "update.h"
#include "PlaybackSystem.h"


void pushManuallyExecutableEvents(Properties* props)
{
    // Recording Start
    {
        Event event;
        event.type = EVENT_LEFT_CLICK_DOWN;
        event.mouse_click.position = {234, 863};
        pushEvent(&props->mgr, event);
    }
    {
        Event event;
        event.type = EVENT_WAIT_TIMER;
        event.duration = 1;
        pushEvent(&props->mgr, event);
    }

    for(int i = 0; i < 500; i++)
    {
        {
            Event event;
            event.type = EVENT_LEFT_CLICK_DOWN;
            event.mouse_click.position = {230, 421};
            pushEvent(&props->mgr, event);
        }
        {
            Event event;
            event.type = EVENT_WAIT_TIMER;
            event.duration = 0.05;
            pushEvent(&props->mgr, event);
        }
    }
}

void updatePlayback(Properties* props)
{
    // // Write Code Here
    // if(props->timer.elapsedSeconds() > props->loaded_macro_data[props->loaded_macro_data.size() - 1].elapsed_time) 
    // {
    //     props->running = false;
    //     // resetTimer(props);
    //     // props->playable_macro_index = 0;
    // }

    // WrittenRecordingData nearest_loaded_data_timestamp;
    // for (int i = props->playable_macro_index; i < props->loaded_macro_data.size();) 
    // {
    //         i++;
    //         if (props->loaded_macro_data[i].elapsed_time > props->timer.elapsedSeconds() || props->loaded_macro_data[i].recorded_state == RECORD_LEFT_CLICK_DOWN) 
    //         {
    //             nearest_loaded_data_timestamp = props->loaded_macro_data[i];
    //             // printf("index diff = %d\n", i - props->playable_macro_index);
    //             props->playable_macro_index = i;
    //             break;
    //         }
    // }

    // switch(nearest_loaded_data_timestamp.recorded_state)
    // {
    //     case RECORD_WAIT_TIMER:
    //     {
    //         // printf("timer %f\n", nearest_loaded_data_timestamp.elapsed_time);
    //     } break;

    //     case RECORD_MOUSE_DRAG:
    //     {
    //         v2 current_pose;
    //         ScreenRecorder::getMousePosition(current_pose.x, current_pose.y);
    //         ScreenInteractor::drag(current_pose.x, current_pose.y, nearest_loaded_data_timestamp.drag_data.position.x, nearest_loaded_data_timestamp.drag_data.position.y);
    //         // printf("Loaded Data: Position = (%f, %f) --------- Time = %f\n", nearest_loaded_data_timestamp.drag_data.position.x, nearest_loaded_data_timestamp.drag_data.position.y, nearest_loaded_data_timestamp.drag_data.time);
    //         // printf("drag\n");
    //     } break;

    //     case RECORD_LEFT_CLICK_DOWN:
    //     {
    //         ScreenInteractor::click(nearest_loaded_data_timestamp.click_data.position.x, nearest_loaded_data_timestamp.click_data.position.y, SINGLE_LEFT_CLICK);
    //         // printf("click\n");
    //     } break;
    // }

    updateManager(&props->mgr, props);
}