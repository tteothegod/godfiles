#include "visual.h"

cv::Point3d estimateFacePosition(const cv::Mat& grayFrame, const cv::Size& imageSize, const cv::Rect& faceRect,
                                 double fx, double fy, double cx, double cy) {
    // calculate position of face relative to camera
    double x = (faceRect.x + faceRect.width / 2.0 - cx) / fx; // adjust for offset from camera center
    double y = (faceRect.y + faceRect.height / 2.0 - cy) / fy;
    double z = fx * 200.0 / faceRect.width; // estimate distance of face from camera
    
    x = x * 3.28084; // convert from meters to feet
    y = y * 3.28084;
    z = z * 3.28084;

    return cv::Point3d(x, y, z);
}
