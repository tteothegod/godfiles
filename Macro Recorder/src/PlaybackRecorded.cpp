#include "update.h"
#include "PlaybackSystem.h"
#include <functional>

void pushRecordedExecutableEvents(Properties* props)
{
    // Recording Events Start 0

    {
        Event event;
        event.type = EVENT_WAIT_TIMER;
        event.duration = 0.018384;
        pushEvent(&props->mgr, event);
    }
    // Recording Events Start 1

    {
        Event event;
        event.type = EVENT_WAIT_TIMER;
        event.duration = 0.496007;
        pushEvent(&props->mgr, event);
    }
    // Recording Events Start 2

    {
        Event event;
        event.type = EVENT_MOUSE_DRAG;
        
        event.polynomial_x = [](float t) -> float {
        return 0;
        };
        
        event.polynomial_y = [](float t) -> float {
        return 0;
        };

        // Generated Mouse Path Task
        event.duration = 0.385794;
        event.mouse_drag.mouse_drag_index = 0;
        pushEvent(&props->mgr, event);
    }
    // Recording Events Start 3

    {
        Event event;
        event.type = EVENT_WAIT_TIMER;
        event.duration = 0.018371;
        pushEvent(&props->mgr, event);
    }
    // Recording Events Start 4

    {
        Event event;
        event.type = EVENT_MOUSE_DRAG;
        
        event.polynomial_x = [](float t) -> float {
        return 0;
        };
        
        event.polynomial_y = [](float t) -> float {
        return 0;
        };

        // Generated Mouse Path Task
        event.duration = 0.349052;
        event.mouse_drag.mouse_drag_index = 1;
        pushEvent(&props->mgr, event);
    }
    // Recording Events Start 5

    {
        Event event;
        event.type = EVENT_WAIT_TIMER;
        event.duration = 0.018371;
        pushEvent(&props->mgr, event);
    }
    // Recording Events Start 6

    {
        Event event;
        event.type = EVENT_MOUSE_DRAG;
        
        event.polynomial_x = [](float t) -> float {
        return 0;
        };
        
        event.polynomial_y = [](float t) -> float {
        return 0;
        };

        // Generated Mouse Path Task
        event.duration = 0.073485;
        event.mouse_drag.mouse_drag_index = 2;
        pushEvent(&props->mgr, event);
    }
    // Recording Events Start 7

    {
        Event event;
        event.type = EVENT_LEFT_CLICK_DOWN;
        event.mouse_click.position = {752.761719, 464.097656};
        pushEvent(&props->mgr, event);
    }
    // Recording Events Start 8

    {
        Event event;
        event.type = EVENT_WAIT_TIMER;
        event.duration = 0.073484;
        pushEvent(&props->mgr, event);
    }
    // Recording Events Start 9

    {
        Event event;
        event.type = EVENT_LEFT_CLICK_UP;
        event.mouse_click.position = {752.761719, 464.097656};
        pushEvent(&props->mgr, event);
    }
    // Recording Events Start 10

    {
        Event event;
        event.type = EVENT_WAIT_TIMER;
        event.duration = 0.202083;
        pushEvent(&props->mgr, event);
    }
    // Recording Events Start 11

    {
        Event event;
        event.type = EVENT_MOUSE_DRAG;
        
        event.polynomial_x = [](float t) -> float {
        return 0;
        };
        
        event.polynomial_y = [](float t) -> float {
        return 0;
        };

        // Generated Mouse Path Task
        event.duration = 0.385794;
        event.mouse_drag.mouse_drag_index = 3;
        pushEvent(&props->mgr, event);
    }
    // Recording Events Start 12

    {
        Event event;
        event.type = EVENT_WAIT_TIMER;
        event.duration = 0.275589;
        pushEvent(&props->mgr, event);
    }
    // Recording Events Start 13

    {
        Event event;
        event.type = EVENT_LEFT_CLICK_DOWN;
        event.mouse_click.position = {744.593750, 471.742188};
        pushEvent(&props->mgr, event);
    }
    // Recording Events Start 14

    {
        Event event;
        event.type = EVENT_WAIT_TIMER;
        event.duration = 3.086403;
        pushEvent(&props->mgr, event);
    }
    // Recording Events Start 15

    {
        Event event;
        event.type = EVENT_LEFT_CLICK_UP;
        event.mouse_click.position = {744.593750, 471.742188};
        pushEvent(&props->mgr, event);
    }
    // Recording Events Start 16

    {
        Event event;
        event.type = EVENT_WAIT_TIMER;
        event.duration = 1.469725;
        pushEvent(&props->mgr, event);
    }
    // Recording Events Start 17

    {
        Event event;
        event.type = EVENT_MOUSE_DRAG;
        
        event.polynomial_x = [](float t) -> float {
        return 0;
        };
        
        event.polynomial_y = [](float t) -> float {
        return 0;
        };

        // Generated Mouse Path Task
        event.duration = 0.587890;
        event.mouse_drag.mouse_drag_index = 4;
        pushEvent(&props->mgr, event);
    }
    // Recording Events Start 18

    {
        Event event;
        event.type = EVENT_LEFT_CLICK_DOWN;
        event.mouse_click.position = {464.097656, 738.332031};
        pushEvent(&props->mgr, event);
    }
    // Recording Events Start 19

    {
        Event event;
        event.type = EVENT_WAIT_TIMER;
        event.duration = 0.036753;
        pushEvent(&props->mgr, event);
    }
    // Recording Events Start 20

    {
        Event event;
        event.type = EVENT_LEFT_CLICK_UP;
        event.mouse_click.position = {464.097656, 738.332031};
        pushEvent(&props->mgr, event);
    }
    // Recording Events Start 21

    {
        Event event;
        event.type = EVENT_WAIT_TIMER;
        event.duration = 1.157409;
        pushEvent(&props->mgr, event);
    }
    // Recording Events Start 22

}
