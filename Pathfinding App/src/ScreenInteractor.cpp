// ScreenInteractor.cpp
#include "ScreenInteractor.h"
#include <iostream>

#ifdef __APPLE__
#include <ApplicationServices/ApplicationServices.h>
#endif

void ScreenInteractor::click(int x, int y) {
#ifdef __APPLE__
    CGEventRef clickEvent = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseDown, CGPointMake(x, y), kCGMouseButtonLeft);
    CGEventPost(kCGHIDEventTap, clickEvent);
    CFRelease(clickEvent);
#endif
    std::cout << "Simulated click at: (" << x << ", " << y << ")" << std::endl;
}
