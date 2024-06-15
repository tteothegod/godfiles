#ifndef SCREEN_INTERACTOR_H
#define SCREEN_INTERACTOR_H

struct Properties;

enum ClickType {
    SINGLE_LEFT_CLICK = 0,
    SINGLE_RIGHT_CLICK,
    LEFT_CLICK_DOWN,
    LEFT_CLICK_UP,
    RIGHT_CLICK_DOWN,
    RIGHT_CLICK_UP,
};

class ScreenInteractor {
public:
    static void click(int x, int y, ClickType click_type);
    static void drag(int startX, int startY, int endX, int endY);
    static void clickDrag(int startX, int startY, int endX, int endY);

    // Other functions for screen interevent
};

class ScreenRecorder {
public:
    static void monitorMouseAndClicks(Properties* props);
    static void getMousePosition(float& x, float& y);
    static void monitorMouseClicks(Properties* props);
};

#endif // SCREEN_INTERACTOR_H
