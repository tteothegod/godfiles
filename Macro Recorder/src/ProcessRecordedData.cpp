#include "update.h"
#include "ProcessRecordedData.h"
#include "MouseMovementParametricizer.h"


#include <fstream>
#include <string>

void processRecordedData(Properties* props)
{
    std::vector<Event> analyzed_events;
    Event alterable_event;
    bool state_change = true;
    MouseProcessData mouse_data;

    std::vector<SavedProcessData> mouse_drag_event_data;
    SavedProcessData pushable_drag_data;
    float mousedrag_start_time = 0;

    int mouse_drag_task_index = 0;

    deleteFunctionContentAndReplace("./src/PlaybackRecorded.cpp");

    for(int i = 0; i < props->loaded_macro_data.size(); i++)
    {
        if(props->loaded_macro_data[i].recorded_state != props->loaded_macro_data[i + 1].recorded_state)
        {
            state_change = true;
        }

        if(state_change)
        {

            if(props->loaded_macro_data[i + 1].recorded_state == RECORD_MOUSE_DRAG)
            {
                mousedrag_start_time = props->loaded_macro_data[i].elapsed_time;
            }
            
            switch(props->loaded_macro_data[i].recorded_state)
            {
                case RECORD_NONE:
                {

                } break;
                case RECORD_WAIT_TIMER:
                {

                } break;
                case RECORD_MOUSE_DRAG:
                {
                    if(props->loaded_macro_data[i + 1].recorded_state != RECORD_MOUSE_DRAG)
                    {
                        std::vector<double> time(mouse_data.times.begin(), mouse_data.times.end());
                        std::vector<double> x_values(mouse_data.x.begin(), mouse_data.x.end());
                        std::vector<double> y_values(mouse_data.y.begin(), mouse_data.y.end());
                        int degree = 100;

                        // printf("duration = %f\n", props->loaded_macro_data[i].elapsed_time - alterable_event.start_time);
                        // Perform polynomial regression
                        // alterable_event.mouse_path_equations = polynomialRegression(time, x_values, y_values, degree);

                        mouse_drag_event_data.push_back(pushable_drag_data);

                        mouse_data.x.clear();
                        mouse_data.y.clear();
                        mouse_data.times.clear();

                        pushable_drag_data.positions.clear();
                        pushable_drag_data.durations.clear();
                    }

                } break;
                case RECORD_LEFT_CLICK_DOWN:
                case RECORD_RIGHT_CLICK_DOWN:
                case RECORD_LEFT_CLICK_UP:
                case RECORD_RIGHT_CLICK_UP:
                {
                    alterable_event.mouse_click.position = props->loaded_macro_data[i].click_data.position;
                } break;    

                case RECORD_RIGHT_CLICK:
                {

                } break;           
            };

            alterable_event.duration = props->loaded_macro_data[i].elapsed_time - alterable_event.start_time;
            alterable_event.type = static_cast<PlaybackEvent>(props->loaded_macro_data[i].recorded_state);
            analyzed_events.push_back(alterable_event);
            alterable_event.start_time = props->loaded_macro_data[i].elapsed_time;
            state_change = false;

        }

        switch(props->loaded_macro_data[i].recorded_state)
        {
            case RECORD_NONE:
            {

            } break;
            case RECORD_WAIT_TIMER:
            {

            } break;
            case RECORD_MOUSE_DRAG:
            {
                mouse_data.x.push_back(props->loaded_macro_data[i].drag_data.position.x);
                mouse_data.y.push_back(props->loaded_macro_data[i].drag_data.position.y);
                mouse_data.times.push_back(props->loaded_macro_data[i].elapsed_time);

                float duration = props->loaded_macro_data[i].elapsed_time - mousedrag_start_time;
                pushable_drag_data.positions.push_back(props->loaded_macro_data[i].drag_data.position);
                pushable_drag_data.durations.push_back(duration);

            } break;
            case RECORD_LEFT_CLICK_DOWN:
            case RECORD_RIGHT_CLICK_DOWN:
            case RECORD_LEFT_CLICK_UP:
            case RECORD_RIGHT_CLICK_UP:
            {
                alterable_event.mouse_click.position = props->loaded_macro_data[i].click_data.position;
            } break;    
            case RECORD_RIGHT_CLICK:
            {

            } break;           
        };


    }

    const std::string filename = "./src/PlaybackRecorded.cpp";

    for (int i = 0; i < analyzed_events.size(); i++)
    {
        std::string comment_to_find = "Recording Events Start " + std::to_string(i);
        std::string code_to_insert = R"(
    {
        Event event;
        event.type = %EVENT_TYPE%;
        %CHANGEABLE_INFORMATION%
        pushEvent(&props->mgr, event);
    }
    // Recording Events Start )";
        code_to_insert = code_to_insert + std::to_string(i + 1) + "\n";

        std::string event_type;
        std::string changeable_info;
        switch (analyzed_events[i].type)
        {
            case EVENT_NONE:
            {

            }
            break;
            case EVENT_WAIT_TIMER:
            {
                event_type = "EVENT_WAIT_TIMER";
                changeable_info =
                    R"(event.duration = %D%;)";
                std::string string_duration = std::to_string(analyzed_events[i].duration);
                size_t pos;
                while ((pos = changeable_info.find("%D%")) != std::string::npos)
                {
                    changeable_info.replace(pos, 3, string_duration);
                }
            }
            break;
            case EVENT_MOUSE_DRAG:
            {
                event_type = "EVENT_MOUSE_DRAG";
        //         changeable_info =
        //         R"(
        // event.polynomial_x = [](float t) -> float {
        // %CHANGEABLE_INFORMATION_1%
        // };
        
        // event.polynomial_y = [](float t) -> float {
        // %CHANGEABLE_INFORMATION_2%
        // };

        // // Generated Mouse Path Task
        // event.duration = %D%;
        // event.mouse_drag.mouse_drag_index = %I%;)";
                changeable_info =
                R"(
        event.polynomial_x = [](float t) -> float {
        return 0;
        };
        
        event.polynomial_y = [](float t) -> float {
        return 0;
        };

        // Generated Mouse Path Task
        event.duration = %D%;
        event.mouse_drag.mouse_drag_index = %I%;)";

                std::string string_duration = std::to_string(analyzed_events[i].duration);
                std::string string_mouse_drag_index = "";
                string_mouse_drag_index += std::to_string(mouse_drag_task_index);

                size_t pos;
                
                while ((pos = changeable_info.find("%D%")) != std::string::npos)
                {
                    changeable_info.replace(pos, 3, string_duration);
                }
                size_t pos_3;
                while ((pos_3 = changeable_info.find("%I%")) != std::string::npos)
                {
                    changeable_info.replace(pos_3, 3, string_mouse_drag_index);
                }
                // size_t pos_1;
                // while ((pos_1 = changeable_info.find("%CHANGEABLE_INFORMATION_1%")) != std::string::npos)
                // {
                //     changeable_info.replace(pos_1, 26, analyzed_events[i].mouse_path_equations.first);
                // }

                // size_t pos_2;
                // while ((pos_2 = changeable_info.find("%CHANGEABLE_INFORMATION_2%")) != std::string::npos)
                // {
                //     changeable_info.replace(pos_2, 26, analyzed_events[i].mouse_path_equations.second);
                // }
                mouse_drag_task_index++;
            } break;

            case EVENT_LEFT_CLICK_DOWN:
            case EVENT_RIGHT_CLICK_DOWN:
            case EVENT_LEFT_CLICK_UP:
            case EVENT_RIGHT_CLICK_UP:
            {

                switch (analyzed_events[i].type) {
                    case EVENT_LEFT_CLICK_DOWN:
                    {
                        event_type = "EVENT_LEFT_CLICK_DOWN";
                    } break;
                    case EVENT_RIGHT_CLICK_DOWN:
                    {
                        event_type = "EVENT_RIGHT_CLICK_DOWN";
                    } break;
                    case EVENT_LEFT_CLICK_UP:
                    {
                        event_type = "EVENT_LEFT_CLICK_UP";
                    } break;
                    case EVENT_RIGHT_CLICK_UP:
                    {
                        event_type = "EVENT_RIGHT_CLICK_UP";
                    } break;

                    default: break;
                };

                changeable_info =
                    R"(event.mouse_click.position = {%X%, %Y%};)";
                std::string string_x = std::to_string(analyzed_events[i].mouse_click.position.x);
                std::string string_y = std::to_string(analyzed_events[i].mouse_click.position.y);
                size_t pos;
                while ((pos = changeable_info.find("%X%")) != std::string::npos)
                {
                    changeable_info.replace(pos, 3, string_x);
                }
                while ((pos = changeable_info.find("%Y%")) != std::string::npos)
                {
                    changeable_info.replace(pos, 3, string_y);
                }

            }
            break;
            case EVENT_RIGHT_CLICK:
            {

            }
            break;
        };

        // Replace placeholders with actual values
        std::string final_code = code_to_insert;
        size_t pos;

        while ((pos = final_code.find("%EVENT_TYPE%")) != std::string::npos)
        {
            final_code.replace(pos, 12, event_type);
        }

        while ((pos = final_code.find("%CHANGEABLE_INFORMATION%")) != std::string::npos)
        {
            final_code.replace(pos, 24, changeable_info);
        }

        std::ifstream input_file(filename);
        if (!input_file.is_open())
        {
            std::cerr << "Failed to open file: " << filename << std::endl;
        }

        std::ofstream output_file(filename + "_temp");
        if (!output_file.is_open())
        {
            std::cerr << "Failed to create temporary file." << std::endl;
        }

        std::string line;
        bool found_comment = false;

        while (std::getline(input_file, line))
        {
            if (!found_comment && line.find(comment_to_find) != std::string::npos)
            {
                // Found the comment, insert the code
                output_file << line << '\n';
                output_file << final_code;
                found_comment = true;
            }
            else
            {
                output_file << line << '\n';
            }
        }

        input_file.close();
        output_file.close();

        // Replace the original file with the modified version
        std::remove(filename.c_str());
        std::rename((filename + "_temp").c_str(), filename.c_str());

        std::cout << "Code inserted successfully." << std::endl;
    }

    savePositionDataToFile(mouse_drag_event_data, "./processedMouseMovementData.bin");
}


void savePositionDataToFile(const std::vector<SavedProcessData>& data, const std::string& filename)
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

    // Write each SavedProcessData object to the file
    for (const auto& recording : data)
    {
        // Write positions vector size
        size_t positionsSize = recording.positions.size();
        outputFile.write(reinterpret_cast<const char*>(&positionsSize), sizeof(size_t));
        // Write positions vector data
        for (const auto& pos : recording.positions) {
            outputFile.write(reinterpret_cast<const char*>(&pos), sizeof(v2));
        }

        // Write durations vector size
        size_t durationsSize = recording.durations.size();
        outputFile.write(reinterpret_cast<const char*>(&durationsSize), sizeof(size_t));
        // Write durations vector data
        for (const auto& dur : recording.durations) {
            outputFile.write(reinterpret_cast<const char*>(&dur), sizeof(float));
        }
    }

    outputFile.close();
}



void deleteFunctionContentAndReplace(const std::string& filePath)
{
    // Read the replacement function from string
    std::string replacementContent = R"(#include "update.h"
#include "PlaybackSystem.h"
#include <functional>

void pushRecordedExecutableEvents(Properties* props)
{
    // Recording Events Start 0

}
)";


    // Write the replacement content to the file
    std::ofstream outputFile(filePath);
    if (!outputFile.is_open())
    {
        std::cerr << "Error opening file for writing: " << filePath << std::endl;
        return;
    }

    outputFile << replacementContent;

    outputFile.close();

}
