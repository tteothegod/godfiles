#ifndef FACE_POSITION_H
#define FACE_POSITION_H

#include <opencv2/core.hpp>
#include <vector>

/**
 * Estimates the position of the face relative to the camera based on the most likely face.
 * @param grayFrame The grayscale frame in which the face was detected.
 * @param imageSize The size of the image captured by the camera.
 * @param faceRect The rectangle bounding the detected face.
 * @param fx The focal length in the x-direction of the camera (in pixels).
 * @param fy The focal length in the y-direction of the camera (in pixels).
 * @param cx The x-coordinate of the principal point of the camera (in pixels).
 * @param cy The y-coordinate of the principal point of the camera (in pixels).
 * @return The position of the face relative to the camera (in millimeters).
 */
cv::Point3d estimateFacePosition(const cv::Mat& grayFrame, const cv::Size& imageSize, const cv::Rect& faceRect,
                                 double fx = 500, double fy = 500, double cx = 320, double cy = 240);

#endif // FACE_POSITION_H
