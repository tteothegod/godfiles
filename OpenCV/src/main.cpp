#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include "visual.h"

// function to detect faces in a given image
std::vector<cv::Rect> detectFaces(const cv::Mat& image) {
    // load Haar Cascade classifier for face detection
    cv::CascadeClassifier faceCascade;
    faceCascade.load("/Users/StarG/Documents/Gogoo/OpenCV/src/haarcascade_frontalface_default.xml");

    // detect faces in image
    std::vector<cv::Rect> faces;
    faceCascade.detectMultiScale(image, faces, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));

    return faces;
}

int main() {

    string video_path = "/Users/StarG/Documents/GitHub/godfiles/OpenCV/resources/video.mp4";
    cv::VideoCapture cap(video_path);

    // check if video opened successfully
    if (!cap.isOpened()) {
        std::cout << "Error opening video file" << std::endl;
        return -1;
    }

    while (true) {
        cv::Mat frame;
        cap >> frame;

        // check if video ended
        if (frame.empty()) {
            std::cout << "Video ended" << std::endl;
            break;
        }

        // convert frame to grayscale
        cv::Mat grayFrame;
        cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);

        // equalize histogram of grayscale frame
        cv::equalizeHist(grayFrame, grayFrame);

        // detect faces in current frame
        std::vector<cv::Rect> faces = detectFaces(grayFrame);
        std::vector<cv::Rect> filteredFaces;

        for (const auto& face : faces) {
            double aspect_ratio = (double)face.width / (double)face.height;
            if (aspect_ratio < 0.8 || aspect_ratio > 1.2) {
                // Discard detected rectangles that are not approximately square
                continue;
            }
            
            if (face.width < 80 || face.height < 80) {
                // Discard detected rectangles that are too small
                continue;
            }
            
            if (face.x < 0 || face.y < 0 ||
                face.x + face.width > frame.cols ||
                face.y + face.height > frame.rows) {
                // Discard detected rectangles that are out of bounds
                continue;
            }
            
            filteredFaces.push_back(face);
        }

        // draw rectangle around each detected face
        for (size_t i = 0; i < filteredFaces.size(); i++) {

            
            cv::rectangle(frame, filteredFaces[i], cv::Scalar(0, 255, 0), 2);
        }

        // Estimate the position of the face relative to the camera
        for (size_t i = 0; i < filteredFaces.size(); i++) {
            cv::Point3d facePosition = estimateFacePosition(grayFrame, grayFrame.size(), filteredFaces[0]);
            std::cout << "Face " << i << " position: " << facePosition << std::endl;
        }

        // display the video feed with detected faces
        cv::imshow("Video Feed", frame);

        // check if user pressed 'q' to quit
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}
