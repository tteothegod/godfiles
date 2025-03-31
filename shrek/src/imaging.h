#include <opencv2/opencv.hpp>
#include <fstream>
#include <vector>
#include <cmath>
#include "maths.h"

//IF CODE DOESTN WORK UNCOMMENT
struct Matrix {
    double m[2][2];
};

struct dv2
{
    v2 point;
    float dist;
};

struct Data
{
    v2 resolution;
    std::vector<v2> positions;
};

enum vectorDerivationMethod
{
    LONGEST_DISTANCE = 1, RANGE_AVERAGE = 2
};

float yellowDetector(cv::Vec3b pixelColor);

v2 getBinaryCenter(std::vector<cv::Point> updatedContours);

std::vector<v2> furthestEdge(std::vector<cv::Point> updatedContours, v2& avgPoint, vectorDerivationMethod method);

std::vector<v2> leftPerpendicularEdge(std::vector<cv::Point> updatedContours, cv::Mat frame, v2 vector, v2 avgPoint);

bool compareDv2ByDist(const dv2& a, const dv2& b);

bool colorPicker(std::vector<cv::Rect> currentRects, cv::Mat frame);

Matrix matrix_inverse(Matrix mat);

void transform_pixel(v2 v1_model, v2 v2_model,
                     v2 v1_input, v2 v2_input,
                     v2 pixel_pos, v2 center_pos, 
                     v2& new_pixel_pos);

v2 tranformPixelModelSpace(v2 image_v1, v2 image_v2, v2 model_v1, v2 model_v2, v2 pixel_pos);

cv::Mat fillCotourArea(std::vector<cv::Point> inputted_contour, cv::Mat inputted_image);

bool compareContourAreas ( std::vector<cv::Point> contour1, std::vector<cv::Point> contour2 );

void distort(const cv::Mat& src, cv::Mat& dst, const cv::Mat& cameraMatrix, const cv::Mat& distCoeffs);

float avgYellowness(cv::Mat frame);

float frameStdDev(cv::Mat frame, float accum_y);

void thresholdFrame(cv::Mat frame, cv::Mat& yellow_mask, float accum_y, float yellow_dev);

void loadFrames(std::vector<cv::Mat>& three_chann_model_cone, cv::Mat& frame, cv::Mat& hsv, cv::Mat& yellow_mask, cv::VideoCapture cap, cv::Mat& display_frame);

void filterContours(std::vector<std::vector<cv::Point>> contours, cv::Mat& yellow_mask);

void processFrameVectors(v2& new_cone_center, std::vector<v2>& cone_top, std::vector<v2>& cone_perpendicular, v2& initial_cone_center, vectorDerivationMethod mode, std::vector<cv::Point> working_contour, cv::Mat yellow_mask);

void processModelVectors(cv::Mat three_chann_model_cone, v2& new_model_cone_center, std::vector<v2>& model_cone_perpendicular, std::vector<v2>& model_cone_top, cv::Mat& model_temp_img, vectorDerivationMethod mode, cv::Mat yellow_mask);

void tranformFrame2Model(cv::Mat model_temp_img, std::vector<cv::Point> working_contour, std::vector<v2> cone_top, v2 new_cone_center, std::vector<v2> cone_perpendicular, std::vector<v2> model_cone_top, v2 new_model_cone_center, std::vector<v2> model_cone_perpendicular, cv::Mat& transformationBinary);