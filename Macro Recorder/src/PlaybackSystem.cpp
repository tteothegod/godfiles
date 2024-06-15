#include "PlaybackSystem.h"
#include "ScreenInteractor.h"
#include "update.h"


static void eventStep(Event* event, Properties* props);

void updateManager(PlaybackMgr* mgr, Properties* props);

static void eventStart(Event* event, Properties* props)
{
	event->start_time = props->timer.elapsedSeconds();
	event->mouse_drag.working_index = 0;
	switch (event->type) {
		case EVENT_MOUSE_DRAG: 
		{
			for(int i = 0; i < props->loaded_mouse_drag_data[event->mouse_drag.mouse_drag_index].durations.size(); i++)
			{
				if(props->loaded_mouse_drag_data[event->mouse_drag.mouse_drag_index].durations[i] < 0.1) 
				{
					event->mouse_drag.working_index = i;
					break;
				}

			}
			
		} break;

		default: break;
	}
}

static void eventStep(Event* event, Properties* props)
{
    switch(event->type)
    {
        case EVENT_LIST: 
        {
            updateManager(event->list, props);
        } break;

        case EVENT_WAIT_TIMER: 
        {
            // printf("Currently Waiting: %f\n", props->timer.elapsedSeconds() - event->start_time);
        } break;

        case EVENT_LEFT_CLICK_DOWN: 
        {
			event->manual_event_complete = true;
            ScreenInteractor::click(event->mouse_click.position.x, event->mouse_click.position.y, LEFT_CLICK_DOWN);
        } break;


        case EVENT_LEFT_CLICK_UP: 
        {
			event->manual_event_complete = true;
            ScreenInteractor::click(event->mouse_click.position.x, event->mouse_click.position.y, LEFT_CLICK_UP);
        } break;

		case EVENT_RIGHT_CLICK_DOWN: 
        {
			event->manual_event_complete = true;
            ScreenInteractor::click(event->mouse_click.position.x, event->mouse_click.position.y, RIGHT_CLICK_DOWN);
        } break;


        case EVENT_RIGHT_CLICK_UP: 
        {
			event->manual_event_complete = true;
            ScreenInteractor::click(event->mouse_click.position.x, event->mouse_click.position.y, RIGHT_CLICK_UP);
        } break;

		case EVENT_MOUSE_DRAG: 
        {

			// printf("Mouse Task index =%d\n", event->mouse_drag.mouse_drag_index);
			// printf("Mouse Drag Working Index =%d\n", event->mouse_drag.working_index);
			float curr_duration = props->timer.elapsedSeconds() - event->start_time;
			v2 target_pose;

			for(int i = event->mouse_drag.working_index; i < props->loaded_mouse_drag_data[event->mouse_drag.mouse_drag_index].durations.size(); i++)
			{
				// printf("Index =%d\n", i);
				// printf("Working Index =%d\n", event->mouse_drag.working_index);

				float index_duration = props->loaded_mouse_drag_data[event->mouse_drag.mouse_drag_index].durations[i];
				// printf("Index Duration =%f\n", index_duration);
				// printf("Curr Duration =%f\n", curr_duration);

				if(curr_duration < index_duration)
				{

					target_pose = props->loaded_mouse_drag_data[event->mouse_drag.mouse_drag_index].positions[i];
					// printf("Loaded Pose =(%f, %f)\n", target_pose.x, target_pose.y);
					event->mouse_drag.working_index = i;
					break;
				}
			}



			v2 current_pose;
			ScreenRecorder::getMousePosition(current_pose.x, current_pose.y);
			// printf("dragging to pose (%f, %f)\n", target_pose.x, target_pose.y);
			ScreenInteractor::drag(current_pose.x, current_pose.y, target_pose.x, target_pose.y);
        } break;
    }
}


static void doEvent(PlaybackMgr* mgr, Event* event, Properties* props) {
	// Do the event
	if (!event->started) {
		event->started = true;
		eventStart(event, props);
	}

    eventStep(event, props);
    bool complete = (props->timer.elapsedSeconds() > event->start_time + event->duration) || event->manual_event_complete;
	if (complete) {
		printf("Event Complete %d\n", event->type);
		if (event->type == EVENT_LIST) free(event->list);
		*event = { };
		mgr->read_head = (mgr->read_head + 1) % PLAYBACKMGR_MAX_EVENTS;
	}
}

void updateManager(PlaybackMgr* mgr, Properties* props) 
{

	if (mgr->is_parallel) {
		for (uint64_t i=0; i<mgr->write_head; i++) {
			doEvent(mgr, &mgr->playback_buffer[i], props);
		}
	}
	else 
	{
		if (mgr->read_head != mgr->write_head)
		{
			doEvent(mgr, &mgr->playback_buffer[mgr->read_head], props);
		}
	}
}



bool pushEvent(PlaybackMgr* mgr, Event event) 
{
	if (((mgr->write_head + 1) % PLAYBACKMGR_MAX_EVENTS) == mgr->read_head) {
		printf("Playback mgr; Failed to add event, queue is full!\n");
		return false;
	}

	printf("Event Pushed %d\n", event.type);

	mgr->playback_buffer[mgr->write_head] = event;
	mgr->write_head = (mgr->write_head + 1) % PLAYBACKMGR_MAX_EVENTS;
	return true;
}

std::vector<SavedProcessData> loadMouseDataFromFile(const std::string& filename)
{
    std::vector<SavedProcessData> loadedData;
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

    // Read each SavedProcessData object from the file
    for (auto& recording : loadedData)
    {
        // Read positions vector size
        size_t positionsSize;
        inputFile.read(reinterpret_cast<char*>(&positionsSize), sizeof(size_t));
        recording.positions.resize(positionsSize);
        // Read positions vector data
        for (auto& pos : recording.positions) {
            inputFile.read(reinterpret_cast<char*>(&pos), sizeof(v2));
        }

        // Read durations vector size
        size_t durationsSize;
        inputFile.read(reinterpret_cast<char*>(&durationsSize), sizeof(size_t));
        recording.durations.resize(durationsSize);
        // Read durations vector data
        for (auto& dur : recording.durations) {
            inputFile.read(reinterpret_cast<char*>(&dur), sizeof(float));
        }
    }

    inputFile.close();

    return loadedData;
}
