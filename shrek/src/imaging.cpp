#include "imaging.h"
#include <cmath>

float yellowDetector(cv::Vec3b pixelColor)
{
    float yellow_r = 0.92f * 2.0f - 1.0f;
    float yellow_g = 0.69f * 2.0f - 1.0f;
    float yellow_b = 0.027f * 2.0f - 1.0f;

    float yellow_ness = 
        (2.0f * pixelColor[2] / 255.0f - 1.0f) * yellow_r +
        (2.0f * pixelColor[1] / 255.0f - 1.0f) * yellow_g + 
        (2.0f * pixelColor[0] / 255.0f - 1.0f) * yellow_b;

    return (yellow_ness + 3.0f) / 6;
}

bool compareDv2ByDistMax(const dv2& a, const dv2& b) 
{
    return a.dist > b.dist;
}

bool compareDv2ByDistMin(const dv2& a, const dv2& b) 
{
    return a.dist < b.dist;
}

bool compareMinY(const v2& a, const v2& b) 
{
    return a.y < b.y;
}

bool compareMaxY(const v2& a, const v2& b) 
{
    return a.y > b.y;
}

bool compareMinX(const v2& a, const v2& b) 
{
    return a.x < b.x;
}

bool compareMaxX(const v2& a, const v2& b) 
{
    return a.x > b.x;
}

v2 getBinaryCenter(std::vector<cv::Point> updatedContours)
{
    float accumX = 0;
    float accumY = 0;
    float posPixels = 0;
    for(int i = 1; i < updatedContours.size(); i++)
    {
        accumX += updatedContours[i].x;
        accumY += updatedContours[i].y;
        posPixels++;
    }
    int roundedAvgX = round(accumX / posPixels);
    int roundedAvgY = round(accumY / posPixels);
    v2 output = {static_cast<float>(roundedAvgX), static_cast<float>(roundedAvgY)};

    return output;
}

std::vector<v2> furthestEdge(std::vector<cv::Point> updatedContours, v2 avgPoint, vectorDerivationMethod method)
{
    std::vector<v2> contourMaxMin;
    //Places the contours of the largest contour in a vector
    if(!updatedContours.empty())
    {
        for(int i = 0; i < updatedContours.size(); i++)
        {
            contourMaxMin.push_back({(float) updatedContours[i].x, (float) updatedContours[i].y});
        }
    }

    std::vector<cv::Point> contour = updatedContours;

    // Declares output vector
    std::vector<v2> outputs;
    v2 avg_cone_end;

    if(method == RANGE_AVERAGE)
    {
        sort(contourMaxMin.begin(), contourMaxMin.end(), compareMinY);
        int min_y = contourMaxMin[0].y;

        // for (int i = 0; i < contourMaxMin.size(); i++)
        // {
        //     std::cout << contourMaxMin[i].y << "\n";
        // }

        std::cout << contourMaxMin.size() << "  " << "\n";

        sort(contourMaxMin.begin(), contourMaxMin.end(), compareMaxY);
        int max_y = contourMaxMin[0].y;

        sort(contourMaxMin.begin(), contourMaxMin.end(), compareMinX);
        int min_x = contourMaxMin[0].x;

        sort(contourMaxMin.begin(), contourMaxMin.end(), compareMaxX);
        int max_x = contourMaxMin[0].x;

        float bottom_y = avgPoint.y - min_y;
        float top_y = max_y - avgPoint.y;
        float bottom_x = avgPoint.x - min_x;
        float top_x = max_x - avgPoint.x;

        //std::cout << bottom_y << "  "<< top_y << "  " << bottom_x << "  "<< top_x << "  " << std::endl;
        std::cout << max_y << "  "<< avgPoint.y << "  " << top_y << std::endl;

        int accum_y = 0;
        int pos_y = 0;
        int accum_x = 0;
        int pos_x = 0;
        float avg_y = 0;
        float avg_x = 0;
        if(top_y > bottom_y)
        {
            for (int i = 0; i < contourMaxMin.size(); i++)
            {
                if (contourMaxMin[i].y > avgPoint.y)
                {
                    pos_y++;
                    accum_y = accum_y + contourMaxMin[i].y;

                }
            }
            avg_y = accum_y/pos_y;
        }

        else
        {
            for (int i = 0; i < contourMaxMin.size(); i++)
            {
                if (contourMaxMin[i].y < avgPoint.y)
                {
                    pos_y++;
                    accum_y = accum_y + contourMaxMin[i].y;

                }
            }

            avg_y = accum_y/pos_y;
        }

        for (int i = 0; i < contourMaxMin.size(); i++)
        {
            if (contourMaxMin[i].y == avg_y)
            {
                pos_x++;
            }
        }

        for (int i = 0; i < contourMaxMin.size(); i++)
        {
            if (contourMaxMin[i].y == avg_y)
            {
                accum_x = accum_x + contourMaxMin[i].x;
            }
        }
        avg_x = accum_x / pos_x;

        avg_cone_end = {round(avg_x), round(avg_y)};
        std::cout << "AVG POINT:" << avg_cone_end.x << "," << avg_cone_end.y << "\n";


        outputs.push_back(avg_cone_end);
        outputs.push_back({0, (float) max_y});
        outputs.push_back({0, (float) avgPoint.y});

    }
    else if(method == LONGEST_DISTANCE)
    {
        //Declares Vectors to be used
        std::vector<dv2> dist_and_point;

        // Goes through every contour point and finds the distance to the point
        for (size_t i = 0; i < contour.size(); i++)
        {
            // Iterate through all the points in the contour
            int x = contour[i].x;
            int y = contour[i].y;

            // Saves the [i] distance and point to the vector
            float distance = sqrtf(((avgPoint.x - x) * (avgPoint.x - x)) + ((avgPoint.y - y) *(avgPoint.y - y)));
            dist_and_point.push_back({{static_cast<float>(x), static_cast<float>(y)}, distance});
        }

        // Sorts the values in the vector to set cone_end to the longesst point
        sort(dist_and_point.begin(), dist_and_point.end(), compareDv2ByDistMax);
        v2 cone_end = {dist_and_point[0].point.x, dist_and_point[0].point.y};

        // Find the of the longest 20 of the distance vectors
        int average_amount = 20;
        v2 accum_point = {0,0};

        // Iterates through first 20 vectors to add them to a accumulated vector
        for(int i = 0; i < average_amount; i++)
        {
            accum_point = {accum_point.x + dist_and_point[i].point.x, accum_point.y + dist_and_point[i].point.y};
        }

        // Divides the accumulated vector by the amount of things averaged
        avg_cone_end = {accum_point.x / average_amount, accum_point.y / average_amount};

        // if the difference between the average and the longest point is greater than 10 it filteres out the false average.
        if(dist_and_point[0].dist - sqrtf(((avg_cone_end.x - avgPoint.x) * (avg_cone_end.x - avgPoint.x)) + ((avg_cone_end.y - avgPoint.y) *(avg_cone_end.y - avgPoint.y))) > 10)
        {
            avg_cone_end = dist_and_point[0].point;
        }
        outputs.push_back(avg_cone_end);
        contour.clear();
        updatedContours.clear();
        contourMaxMin.clear();
        dist_and_point.clear();

    }

    return outputs;
}

std::vector<v2> leftPerpendicularEdge(std::vector<cv::Point> updatedContours, cv::Mat frame, v2 vector, v2 avgPoint)
{

    //Places the contours of the largest contour in a vector
    std::vector<cv::Point> contour = updatedContours;

    // Declares the vector of the point that will be outputted
    std::vector<v2> outputs;

    // Declares the vector that stores all of the distances for 
    std::vector<dv2> left_point_and_dist;
    
    // Finds the left perpendicular vector to the vector of the avg point to the cones end
    v2 left_cone_vec = leftPerpendicular(vector - avgPoint);
    left_cone_vec = left_cone_vec + avgPoint;

    // Iterate between 

    v2 left_cone_step_vec = normalize(left_cone_vec);

    while (true)
    {
        left_cone_vec = left_cone_vec + left_cone_step_vec;

        if(left_cone_vec.x < frame.cols && left_cone_vec.x > 0 && left_cone_vec.y < frame.rows && left_cone_vec.y > 0)
        {
            if(frame.at<char>(left_cone_vec.y, left_cone_vec.x) == 0)
            {
                break;
            }
            else
            {
                left_cone_vec = left_cone_vec + left_cone_step_vec;
            }
        }
        else 
        {
            break;
        }
    }


    // for (size_t i = 0; i < contour.size(); i++)
    // {
    //     float x = static_cast<float>(contour[i].x);
    //     float y = static_cast<float>(contour[i].y);

    //     v2 left_diff_vec = {left_cone_vec.x + avgPoint.x - x, left_cone_vec.y + avgPoint.y - y};
    //     float left_vec_dist = length(left_diff_vec);
    //     left_point_and_dist.push_back({{x, y}, left_vec_dist});
    // }

    // sort(left_point_and_dist.begin(), left_point_and_dist.end(), compareDv2ByDistMin);
    
    outputs.push_back( { round(left_cone_vec.x), round(left_cone_vec.y) } );
    outputs.push_back(left_cone_vec);

    left_point_and_dist.clear();


    return outputs;
}


int j = 0;
bool colorPicker(std::vector<cv::Rect> currentRects, cv::Mat frame)
{
    j++;
    cv::Mat pickedSpot;

    std::cout << "Picking Color \n";
    for(int i = 0; i < currentRects.size(); i++)
    {
        std::cout << "Rect " << i << " x = " << currentRects[i].x + (currentRects[i].width / 2) << "\n" << "Rect " << i << " y = " << currentRects[i].y + (currentRects[i].height / 2) << "\n";
    }

    int x, y;
    std::cin >> x >> y;

    pickedSpot = frame.clone();
    cv::circle(pickedSpot, cv::Point(x,y), 2, cv::Scalar(0,0,255), -1);
    //cv::imshow("Dot Frame", pickedSpot);
    cv::waitKey(1);

    std::string checked;
    std::cin >> checked;

    cv::destroyWindow("Dot Frame");

    if(checked == "yes")
    {
        if (x >= 0 && x < frame.cols && y >= 0 && y < frame.rows) 
        {
            cv::Vec3b colorAt = frame.at<cv::Vec3b>(cv::Point(x,y));
            std::cout << "#" << j << " Color = " << (int)colorAt[0] << "  " << (int)(colorAt[1]) << "  " << (int)(colorAt[2]) << "\n";
            std::ofstream calibration ("/Users/StarG/Documents/Gogoo/Cones/src/calibration.txt");
            if (calibration.is_open())
            {
                calibration << "#" << j << ": ";
                for(int i = 0; i < 3; i++)
                {
                    calibration << static_cast<int>(colorAt[i]) << " ";
                }
                calibration << "\n";
                calibration.close();
            }
        } 
        else
        {
        std::cout << "Invalid pixel location\n";
        }
        return true;
    }
    else 
    {
        return false;
        std::cout << "Hola";
    }
}


Matrix operator*(const Matrix& m1, const Matrix& m2) {
    Matrix result;
    result.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0];
    result.m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1];
    result.m[1][0] = m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0];
    result.m[1][1] = m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1];
    return result;
}

// Compute the inverse of a 2x2 matrix
Matrix matrix_inverse(Matrix mat) 
{
    double det = mat.m[0][0] * mat.m[1][1] - mat.m[0][1] * mat.m[1][0];
    Matrix inv_mat;
    inv_mat.m[0][0] = mat.m[1][1] / det;
    inv_mat.m[0][1] = -mat.m[0][1] / det;
    inv_mat.m[1][0] = -mat.m[1][0] / det;
    inv_mat.m[1][1] = mat.m[0][0] / det;
    return inv_mat;
}

// Compute the new pixel position given the input parameters
void transform_pixel(v2 v1_model, v2 v2_model,
                     v2 v1_input, v2 v2_input,
                     v2 pixel_pos, v2 center_pos, 
                     v2& new_pixel_pos) 
{
    // Compute the transformation matrices
    Matrix model_mat, input_mat, transform_mat;
    model_mat.m[0][0] = v1_model.x / length(v1_model);
    model_mat.m[0][1] = v1_model.y / length(v1_model);
    model_mat.m[1][0] = v2_model.x / length(v2_model);
    model_mat.m[1][1] = v2_model.y / length(v2_model);

    input_mat.m[0][0] = v1_input.x / length(v1_input);
    input_mat.m[0][1] = v1_input.y / length(v1_input);
    input_mat.m[1][0] = v2_input.x / length(v2_input);
    input_mat.m[1][1] = v2_input.y / length(v2_input);

    transform_mat = matrix_inverse(model_mat) * input_mat;

    // Compute the new pixel position
    v2 pixel_pos_rel = { pixel_pos.x - center_pos.x, pixel_pos.y - center_pos.y };
    new_pixel_pos.x = transform_mat.m[0][0] * pixel_pos_rel.x + transform_mat.m[0][1] * pixel_pos_rel.y + center_pos.x;
    new_pixel_pos.y = transform_mat.m[1][0] * pixel_pos_rel.x + transform_mat.m[1][1] * pixel_pos_rel.y + center_pos.y;
}

v2 tranformPixelModelSpace(v2 image_v1, v2 image_v2, v2 model_v1, v2 model_v2, v2 pixel_pos)
{
    mat2x2 identity_to_image = {
        image_v1.x, image_v2.x,
        image_v1.y, image_v2.y,
    };

    v2 identity_space = transform(inverse(identity_to_image), pixel_pos);

    mat2x2 identity_to_model = {
        model_v1.x, model_v2.x,
        model_v1.y, model_v2.y
    };

    v2 model_space = transform(identity_to_model, identity_space); //model_v1 * identity_space.x + model_v2 * identity_space.y;

    return model_space;
}

cv::Mat fillCotourArea(std::vector<cv::Point> inputted_contour, cv::Mat inputted_image)
{
    for(int i = 0; i < inputted_contour.size(); i++)
    {
        v2 pixel_test = {(float) inputted_contour[i].x, (float)inputted_contour[i].y};
        v2 stop_position = v2 {0,0};

        for (int k = pixel_test.x; k < inputted_image.cols; k++)
        {
            if(inputted_image.at<char>(pixel_test.y, k) == -1)
            {
                stop_position.x = k;
            }
        }

        for (int k = 0; k < pixel_test.x; k++)
        {
            if(inputted_image.at<char>(pixel_test.y, k) == -1)
            {
                stop_position.x = k;
            }
        }

        for (int k = pixel_test.y; k < inputted_image.rows; k++)
        {
            if(inputted_image.at<char>(k, pixel_test.x) == -1)
            {
                stop_position.y = k;
            }
        }

        for (int k = 0; k < pixel_test.y; k++)
        {
            if(inputted_image.at<char>(k, pixel_test.x) == -1)
            {
                stop_position.y = k;
            }
        }

        if (stop_position.x == 0)
        {
            continue;
        }
        
        else if (stop_position.x > pixel_test.x)
        {
            for (int k = pixel_test.x; k < stop_position.x; k++)
            {
                inputted_image.at<char>(pixel_test.y, k) = 255;
            }
        }

        else if (stop_position.x < pixel_test.x)
        {
            for (int k = stop_position.x; k < pixel_test.x; k++)
            {
                inputted_image.at<char>(pixel_test.y, k) = 255;
            }
        }

        if (stop_position.y == 0)
        {
            continue;
        }
        
        else if(stop_position.y > pixel_test.y)
        {
            for (int k = pixel_test.y; k < stop_position.y ; k++)
            {
                inputted_image.at<char>(k, pixel_test.x) = 255;
            }
        }

        else if(stop_position.y < pixel_test.y)
        {
            for (int k = stop_position.y; k < pixel_test.y; k++)
            {
                inputted_image.at<char>(k, pixel_test.x) = 255;
            }
        }
    }
    return inputted_image;
}

bool compareContourAreas ( std::vector<cv::Point> contour1, std::vector<cv::Point> contour2 ) {
    double i = fabs( contourArea(cv::Mat(contour1)) );
    double j = fabs( contourArea(cv::Mat(contour2)) );
    return ( i > j );
}

float avgYellowness(cv::Mat frame)
{
    float accum_y = 0;
                for(int i = 0; i < frame.cols; i++)
                {
                    for(int l = 0; l < frame.rows; l++)
                    {
                        cv::Vec3b pixelColor = frame.at<cv::Vec3b>(l,i);
                        float y = yellowDetector(pixelColor);
                        accum_y += y;
                    }
                }
                accum_y /= frame.cols * frame.rows;
                return accum_y;
}

float frameStdDev(cv::Mat frame, float accum_y)
{
    float var_y = 0;
                for(int i = 0; i < frame.cols; i++)
                {
                    for(int l = 0; l < frame.rows; l++)
                    {
                        cv::Vec3b pixelColor = frame.at<cv::Vec3b>(l,i);
                        float y = yellowDetector(pixelColor);

                        var_y += (y - accum_y) * (y - accum_y);
                    }
                }

                var_y /= frame.cols * frame.rows;
                float yellow_dev = sqrtf(var_y);
                return yellow_dev;
}

void thresholdFrame(cv::Mat frame, cv::Mat& yellow_mask, float accum_y, float yellow_dev)
{
    for(int i = 0; i < frame.cols; i++)
    {
        for(int l = 0; l < frame.rows; l++)
        {
            cv::Vec3b pixelColor = frame.at<cv::Vec3b>(l, i);
            
            float y = yellowDetector(pixelColor); 
               
            if( y - accum_y > 1 * yellow_dev )
            {
                yellow_mask.at<char>(l,i) = 255 * y;
            }
            else 
            {
                yellow_mask.at<char>(l,i) = 0;
            }
        }
    }
    return;
}

void loadFrames(std::vector<cv::Mat>& three_chann_model_cone, cv::Mat& frame, cv::Mat& hsv, cv::Mat& yellow_mask, cv::VideoCapture cap, cv::Mat& display_frame){
    // Model paths
    std::string straight_low = "./models/straight_low.png";
    std::string straight_mid = "./models/straight_mid.png";
    std::string straight_high = "./models/straight_high.png";
    std::string side_low = "./models/side_low.png";
    std::string side_mid = "./models/side_mid.png";
    std::string side_high = "./models/side_high.png";

    // Define the lower and upper bounds of the yellow color
    cv::Scalar lower_yellow = cv::Scalar(20, 200, 200);
    cv::Scalar upper_yellow = cv::Scalar(30, 255, 255);

    // Capture a frame from the camera
    three_chann_model_cone.push_back(cv::imread(straight_low));
    three_chann_model_cone.push_back(cv::imread(straight_mid));
    three_chann_model_cone.push_back(cv::imread(straight_high));
    three_chann_model_cone.push_back(cv::imread(side_low));
    three_chann_model_cone.push_back(cv::imread(side_mid));
    three_chann_model_cone.push_back(cv::imread(side_high));

    cap >> frame;

    if(frame.empty())
    {
        std::cout << "Empty Frame!";
    }
    display_frame = frame; 
    // Convert the frame to the HSV color space
    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

    // Threshold the image to get only yellow pixels
    cv::inRange(hsv, lower_yellow, upper_yellow, yellow_mask);

    return;
}

void filterContours(std::vector<std::vector<cv::Point>> contours, cv::Mat& yellow_mask)
{
    for (const auto& contour : contours) 
    {
        // Ignore contours that are too small or too large
        double area = cv::contourArea(contour);
        if (area < 250) 
        {
            cv::drawContours(yellow_mask, std::vector<std::vector<cv::Point>>{contour}, 0, cv::Scalar(0, 0, 0), -1, cv::LINE_8);            
            continue;
        }

        // Ignore contours that are not roughly cone-shaped
        double perimeter = cv::arcLength(contour, true);
        double circularity = 4 * M_PI * area / (perimeter * perimeter);
        if (circularity < .2) 
        {
            cv::drawContours(yellow_mask, std::vector<std::vector<cv::Point>>{contour}, 0, cv::Scalar(0, 0, 0), -1, cv::LINE_8);            
            continue;
        }
    }
    return;
}

void processFrameVectors(v2& new_cone_center, std::vector<v2>& cone_top, std::vector<v2>& cone_perpendicular, v2& initial_cone_center, vectorDerivationMethod mode, std::vector<cv::Point> working_contour, cv::Mat yellow_mask)
{
    std::vector<v2> initial_cone_top ;   

    initial_cone_center = getBinaryCenter(working_contour);
    // Finds the initial vector for the top of the cone

    initial_cone_top = furthestEdge(working_contour, v2 {static_cast<float>(initial_cone_center.x), static_cast<float>(initial_cone_center.y)}, mode);

    // Add a small vector to the cones center
    new_cone_center = {static_cast<float>(initial_cone_center.x) - ((initial_cone_top[0].x - initial_cone_center.x) / 3), static_cast<float>(initial_cone_center.y) - ((initial_cone_top[0].y - initial_cone_center.y)/ 3)};
    cone_top = furthestEdge(working_contour, new_cone_center, mode);

    // Output the old center and the new center
    //std::cout << "Video Center X = " << new_cone_center.x << "\n" << "Video Center Y = " << new_cone_center.y << "\n";

    // Get the vector of points for the perpendicular vectors
    cone_perpendicular = leftPerpendicularEdge(working_contour, yellow_mask, cone_top[0], new_cone_center);
    return;
}

void processModelVectors(cv::Mat three_chann_model_cone, v2& new_model_cone_center, std::vector<v2>& model_cone_perpendicular, std::vector<v2>& model_cone_top, cv::Mat& model_temp_img, vectorDerivationMethod mode, cv::Mat yellow_mask)
{
    v2 init_model_cone_center;
    std::vector<v2> init_model_cone_top;

    //Convert the image from 3 channel to 1 channel
    cv::cvtColor(three_chann_model_cone, model_temp_img, cv::COLOR_BGR2GRAY);
    cv::threshold(model_temp_img, model_temp_img, 128, 255, cv::THRESH_BINARY);

    // Find the contours of the model image
    std::vector<std::vector<cv::Point>> modelContours;
    cv::findContours(model_temp_img, modelContours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

    ////////////////////////////////////////// *Processing Model Cone Into Vectors* //////////////////////////////////////////
    init_model_cone_center = getBinaryCenter(modelContours[0]);

    // Finds the initial vector for the top of the cone
    init_model_cone_top = furthestEdge(modelContours[0], v2 {static_cast<float>(init_model_cone_center.x), static_cast<float>(init_model_cone_center.y)}, mode);

    // Add a small vector to the cones center
    new_model_cone_center = {static_cast<float>(init_model_cone_center.x) - ((init_model_cone_top[0].x - init_model_cone_center.x) / 3), static_cast<float>(init_model_cone_center.y) - ((init_model_cone_top[0].y - init_model_cone_center.y)/ 3)};
    model_cone_top = furthestEdge(modelContours[0], new_model_cone_center, mode);

    // Output the old center and the new center
    //std::cout << "Model Center X = " << new_model_cone_center.x << "\n" << "Model Center Y = " << new_model_cone_center.y << "\n";

    // Get the vector of points for the perpendicular vectors
    model_cone_perpendicular = leftPerpendicularEdge(modelContours[0], yellow_mask, model_cone_top[0], new_model_cone_center);
    return;

}

void tranformFrame2Model(cv::Mat model_temp_img, std::vector<cv::Point> working_contour, std::vector<v2> cone_top, v2 new_cone_center, std::vector<v2> cone_perpendicular, std::vector<v2> model_cone_top, v2 new_model_cone_center, std::vector<v2> model_cone_perpendicular, cv::Mat& transformationBinary)
{
    transformationBinary = cv::Mat::zeros(model_temp_img.rows, model_temp_img.cols, CV_8UC3);
    cv::cvtColor(transformationBinary, transformationBinary, cv::COLOR_BGR2GRAY);

    std::vector<cv::Point> cone_contour = working_contour;

    std::vector<cv::Point> new_transform_contour;

    // Transforms cone contour into model image plane
    for (int i = 0; i < cone_contour.size(); i++)
    {
        v2 pixel_pos = {static_cast<float>(cone_contour[i].x), static_cast<float>(cone_contour[i].y)};

        v2 new_pixel_pos = new_model_cone_center + tranformPixelModelSpace(
            cone_top[0] - new_cone_center, cone_perpendicular[0] - new_cone_center,
            model_cone_top[0] - new_model_cone_center, model_cone_perpendicular[0] - new_model_cone_center,
            pixel_pos - new_cone_center);

        new_pixel_pos = {round(new_pixel_pos.x), round(new_pixel_pos.y)};
        if(new_pixel_pos.x < transformationBinary.cols && new_pixel_pos.x > 0 && new_pixel_pos.y < transformationBinary.rows && new_pixel_pos.y > 0)
        {
            transformationBinary.at<char>(new_pixel_pos.y, new_pixel_pos.x) = 255;
            new_transform_contour.push_back({(int) new_pixel_pos.x, (int) new_pixel_pos.y});
        }

    }

    fillCotourArea(new_transform_contour, transformationBinary);
    
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3)); // adjust kernel size as needed
    cv::dilate(transformationBinary, transformationBinary, kernel);
    cv::morphologyEx(transformationBinary, transformationBinary, cv::MORPH_CLOSE, kernel);

    std::vector<std::vector<cv::Point>> updatedModelContours;
    cv::findContours(transformationBinary, updatedModelContours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
    cv::drawContours(transformationBinary, updatedModelContours, 0, cv::Scalar(255,255,255), -1, cv::LINE_8);

    sort(updatedModelContours.begin(), updatedModelContours.end(), compareContourAreas);
    fillCotourArea(updatedModelContours[0], transformationBinary);
}

