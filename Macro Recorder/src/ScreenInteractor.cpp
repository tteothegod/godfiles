// ScreenInteractor.cpp
#include "ScreenInteractor.h"
#include "update.h"
#include <iostream>
#include <atomic>

#ifdef __APPLE__
#include <ApplicationServices/ApplicationServices.h>
#endif

void ScreenInteractor::click(int x, int y, ClickType click_type) 
{
#ifdef __APPLE__

    switch (click_type)
    {
        case SINGLE_LEFT_CLICK:
        {
            CGEventRef clickEventDown = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseDown, CGPointMake(x, y), kCGMouseButtonLeft);
            CGEventPost(kCGHIDEventTap, clickEventDown);
            CFRelease(clickEventDown);

            CGEventRef clickEventUp = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseUp, CGPointMake(x, y), kCGMouseButtonLeft);
            CGEventPost(kCGHIDEventTap, clickEventUp);
            CFRelease(clickEventUp);

        } break;

        case SINGLE_RIGHT_CLICK:
        {
            CGEventRef clickEventDown = CGEventCreateMouseEvent(NULL, kCGEventRightMouseDown, CGPointMake(x, y), kCGMouseButtonRight);
            CGEventPost(kCGHIDEventTap, clickEventDown);
            CFRelease(clickEventDown);

            CGEventRef clickEventUp = CGEventCreateMouseEvent(NULL, kCGEventRightMouseUp, CGPointMake(x, y), kCGMouseButtonRight);
            CGEventPost(kCGHIDEventTap, clickEventUp);
            CFRelease(clickEventUp);

        } break;

        case LEFT_CLICK_DOWN:
        {
            CGEventRef clickEventDown = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseDown, CGPointMake(x, y), kCGMouseButtonLeft);
            CGEventPost(kCGHIDEventTap, clickEventDown);
            CFRelease(clickEventDown);

        } break;

        case LEFT_CLICK_UP:
        {
            CGEventRef clickEventUp = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseUp, CGPointMake(x, y), kCGMouseButtonLeft);
            CGEventPost(kCGHIDEventTap, clickEventUp);
            CFRelease(clickEventUp);

        } break;

        case RIGHT_CLICK_DOWN:
        {
            CGEventRef clickEventDown = CGEventCreateMouseEvent(NULL, kCGEventRightMouseDown, CGPointMake(x, y), kCGMouseButtonRight);
            CGEventPost(kCGHIDEventTap, clickEventDown);
            CFRelease(clickEventDown);

        } break;

        case RIGHT_CLICK_UP:
        {
            CGEventRef clickEventUp = CGEventCreateMouseEvent(NULL, kCGEventRightMouseUp, CGPointMake(x, y), kCGMouseButtonRight);
            CGEventPost(kCGHIDEventTap, clickEventUp);
            CFRelease(clickEventUp);

        } break;
    }


#endif

    std::cout << "Simulated click at: (" << x << ", " << y << ")" << std::endl;
}

void ScreenInteractor::drag(int startX, int startY, int endX, int endY) 
{
#ifdef __APPLE__

    // Create mouse drag event from starting position to ending position
    CGEventRef mouseDragEvent = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseDragged, CGPointMake(endX, endY), kCGMouseButtonLeft);
    CGEventPost(kCGHIDEventTap, mouseDragEvent);
    CFRelease(mouseDragEvent);

#endif

    // std::cout << "Simulated drag from (" << startX << ", " << startY << ") to (" << endX << ", " << endY << ")" << std::endl;
}

void ScreenInteractor::clickDrag(int startX, int startY, int endX, int endY) 
{
#ifdef __APPLE__
    // Create mouse down event at starting position
    CGEventRef mouseDownEvent = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseDown, CGPointMake(startX, startY), kCGMouseButtonLeft);
    CGEventPost(kCGHIDEventTap, mouseDownEvent);

    // Create mouse drag event from starting position to ending position
    CGEventRef mouseDragEvent = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseDragged, CGPointMake(endX, endY), kCGMouseButtonLeft);
    CGEventPost(kCGHIDEventTap, mouseDragEvent);

    // Create mouse up event at ending position
    CGEventRef mouseUpEvent = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseUp, CGPointMake(endX, endY), kCGMouseButtonLeft);
    CGEventPost(kCGHIDEventTap, mouseUpEvent);

    // Release events
    CFRelease(mouseDownEvent);
    CFRelease(mouseDragEvent);
    CFRelease(mouseUpEvent);
#endif

    std::cout << "Simulated drag from (" << startX << ", " << startY << ") to (" << endX << ", " << endY << ")" << std::endl;
}

void ScreenRecorder::getMousePosition(float& x, float& y) 
{

#ifdef __APPLE__

    // Get current mouse location
    CGEventRef event = CGEventCreate(NULL);
    CGPoint cursor = CGEventGetLocation(event);
    x = static_cast<float>(cursor.x);
    y = static_cast<float>(cursor.y);
    CFRelease(event);

#endif

}

CGEventRef mouseClickMonitoringCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *userInfo) {
    Properties* props = static_cast<Properties*>(userInfo);

        // Check the event type to determine which mouse button was clicked
        switch (type) {
        case kCGEventLeftMouseDown:
        {
            // printf("Mouse Down!\n");
            props->last_click_type = LEFT_CLICK_DOWN;
        } break;
        case kCGEventRightMouseDown:
        {
            props->last_click_type = RIGHT_CLICK_DOWN;
        } break;
        case kCGEventLeftMouseUp:
        {
            printf("Mouse up!\n");
            props->last_click_type = LEFT_CLICK_UP;
        } break;
        case kCGEventRightMouseUp:
        {
            props->last_click_type = RIGHT_CLICK_UP;
        } break;

        default: break;
        }

    CGPoint location = CGEventGetLocation(event);
    std::cout << "Mouse clicked at: (" << location.x << ", " << location.y << ")" << std::endl;
    props->mouse_click = true;
    return event;
}

void ScreenRecorder::monitorMouseClicks(Properties* props) 
{
#ifdef __APPLE__
    if(props->stopThreads) return;

    // Define the event masks
    CGEventMask leftDownClickEventMask = (1 << kCGEventLeftMouseDown);
    CGEventMask rightDownClickEventMask = (1 << kCGEventRightMouseDown);
    CGEventMask leftUpClickEventMask = (1 << kCGEventLeftMouseUp);
    CGEventMask rightUpClickEventMask = (1 << kCGEventRightMouseUp);

    // Create event taps
    CFMachPortRef leftDownClickEventTap = CGEventTapCreate(
        kCGSessionEventTap,
        kCGHeadInsertEventTap,
        kCGEventTapOptionDefault,
        leftDownClickEventMask,
        mouseClickMonitoringCallback,
        props
    );

    CFMachPortRef rightDownClickEventTap = CGEventTapCreate(
        kCGSessionEventTap,
        kCGHeadInsertEventTap,
        kCGEventTapOptionDefault,
        rightDownClickEventMask,
        mouseClickMonitoringCallback,
        props
    );

    CFMachPortRef leftUpClickEventTap = CGEventTapCreate(
        kCGSessionEventTap,
        kCGHeadInsertEventTap,
        kCGEventTapOptionDefault,
        leftUpClickEventMask,
        mouseClickMonitoringCallback,
        props
    );

    CFMachPortRef rightUpClickEventTap = CGEventTapCreate(
        kCGSessionEventTap,
        kCGHeadInsertEventTap,
        kCGEventTapOptionDefault,
        rightUpClickEventMask,
        mouseClickMonitoringCallback,
        props
    );

    // Create an array of event taps and event masks
    CFMachPortRef eventTaps[] = {
        leftDownClickEventTap,
        rightDownClickEventTap,
        leftUpClickEventTap,
        rightUpClickEventTap
    };

    CGEventMask eventMasks[] = {
        leftDownClickEventMask,
        rightDownClickEventMask,
        leftUpClickEventMask,
        rightUpClickEventMask
    };

    // Create run loop sources for each event tap
    CFRunLoopSourceRef runLoopSources[4];
    for (int i = 0; i < 4; ++i) {
        runLoopSources[i] = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTaps[i], 0);
        CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSources[i], kCFRunLoopCommonModes);
        CGEventTapEnable(eventTaps[i], true);
    }

    // Run the run loop until shouldStop is set to true
    while (!props->stopThreads) {
        CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0.1, false);
    }

    // Clean up
    for (int i = 0; i < 4; ++i) {
        CGEventTapEnable(eventTaps[i], false);
        CFRunLoopRemoveSource(CFRunLoopGetCurrent(), runLoopSources[i], kCFRunLoopCommonModes);
        CFRelease(runLoopSources[i]);
        CFRelease(eventTaps[i]);
    }
#endif
}


// // Your callback function
// void mouseClickMonitoringCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *userInfo) {
//     ScreenRecorder::Properties* props = static_cast<ScreenRecorder::Properties*>(userInfo);
    

    
//     // Your existing callback logic here
//     // This function will be called for all mouse click events
// }

CGEventRef mouseEventCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void* userInfo) {
    Properties* props = static_cast<Properties*>(userInfo);

#ifdef __APPLE__
    // Check the event type to determine which event occurred
    switch (type) {
        case kCGEventLeftMouseDown:
            props->last_click_type = LEFT_CLICK_DOWN;
            break;
        case kCGEventRightMouseDown:
            props->last_click_type = RIGHT_CLICK_DOWN;
            break;
        case kCGEventLeftMouseUp:
            props->last_click_type = LEFT_CLICK_UP;
            break;
        case kCGEventRightMouseUp:
            props->last_click_type = RIGHT_CLICK_UP;
            break;
        case kCGEventMouseMoved:
            // Mouse move event
            printf("Mouse Move event Occurred\n");
            props->mouse_move_event = true;
            break;
        default:
            break;
    }

    if(type == kCGEventLeftMouseDown || type == kCGEventRightMouseDown || type == kCGEventLeftMouseUp || type == kCGEventRightMouseUp)
    {
        CGPoint location = CGEventGetLocation(event);
        std::cout << "Mouse clicked at: (" << location.x << ", " << location.y << ")" << std::endl;
        props->mouse_click = true;
    }


#endif

    return event;
}



void ScreenRecorder::monitorMouseAndClicks(Properties* props) {
#ifdef __APPLE__
    if (props->stopThreads) return;

    // Define the event masks
    CGEventMask eventMask = (1 << kCGEventLeftMouseDown) |
                            (1 << kCGEventRightMouseDown) |
                            (1 << kCGEventLeftMouseUp) |
                            (1 << kCGEventRightMouseUp) |
                            (1 << kCGEventMouseMoved);

    // Create event tap
    CFMachPortRef eventTap = CGEventTapCreate(
        kCGSessionEventTap,
        kCGHeadInsertEventTap,
        kCGEventTapOptionDefault,
        eventMask,
        mouseEventCallback,
        props
    );

    if (!eventTap) {
        std::cerr << "Failed to create event tap!" << std::endl;
        return;
    }

    // Create run loop source for the event tap
    CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
    CGEventTapEnable(eventTap, true);

    // Run the run loop until shouldStop is set to true
    while (!props->stopThreads) {
        CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0.1, false);
    }

    // Clean up
    CGEventTapEnable(eventTap, false);
    CFRunLoopRemoveSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
    CFRelease(runLoopSource);
    CFRelease(eventTap);
#endif
}

