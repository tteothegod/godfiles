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

std::vector<v2> furthestEdge(std::vector<cv::Point> updatedContours, v2& avgPoint, vectorDerivationMethod method)
{
    std::vector<v2> outputs;

    if (method == LONGEST_DISTANCE) 
    {
        if (updatedContours.empty()) 
        {
            return outputs; // Return empty if no contours
        }

        // Initialize variables to track the farthest point
        float max_distance = -1;
        v2 farthest_point = {0, 0};

        // Iterate through all contour points
        for (const auto& contourPoint : updatedContours) {
            float dx = avgPoint.x - contourPoint.x;
            float dy = avgPoint.y - contourPoint.y;
            float distance = dx * dx + dy * dy; // Avoid sqrt for efficiency

            // Update farthest point if this point is farther
            if (distance > max_distance) {
                max_distance = distance;
                farthest_point = {static_cast<float>(contourPoint.x), static_cast<float>(contourPoint.y)};
            }
        }

        // If needed, calculate the actual distance (with sqrt)
        max_distance = std::sqrt(max_distance);

        // Optionally, calculate the average of the top N farthest points
        int average_amount = 20;
        std::vector<v2> top_points;
        float threshold_distance = max_distance - 10; // Example threshold

        for (const auto& contourPoint : updatedContours) {
            float dx = avgPoint.x - contourPoint.x;
            float dy = avgPoint.y - contourPoint.y;
            float distance = std::sqrt(dx * dx + dy * dy);

            if (distance >= threshold_distance) {
                top_points.push_back({static_cast<float>(contourPoint.x), static_cast<float>(contourPoint.y)});
                if (top_points.size() >= average_amount) {
                    break; // Stop after collecting enough points
                }
            }
        }

        // Calculate the average of the top points
        v2 avg_cone_end = {0, 0};
        for (const auto& point : top_points) {
            avg_cone_end.x += point.x;
            avg_cone_end.y += point.y;
        }
        avg_cone_end.x /= top_points.size();
        avg_cone_end.y /= top_points.size();

        // Add the farthest point and the average point to the outputs
        outputs.push_back(farthest_point);
        outputs.push_back(avg_cone_end);
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
    // model_mat.m[0][0] = v1_model.x / length(v1_model);
    // model_mat.m[0][1] = v1_model.y / length(v1_model);
    // model_mat.m[1][0] = v2_model.x / length(v2_model);
    // model_mat.m[1][1] = v2_model.y / length(v2_model);    
    
    // input_mat.m[0][0] = v1_input.x / length(v1_input);
    // input_mat.m[0][1] = v1_input.y / length(v1_input);
    // input_mat.m[1][0] = v2_input.x / length(v2_input);
    // input_mat.m[1][1] = v2_input.y / length(v2_input);
    
    model_mat.m[0][0] = v1_model.x;
    model_mat.m[0][1] = v1_model.y;
    model_mat.m[1][0] = v2_model.x;
    model_mat.m[1][1] = v2_model.y;

    input_mat.m[0][0] = v1_input.x;
    input_mat.m[0][1] = v1_input.y;
    input_mat.m[1][0] = v2_input.x;
    input_mat.m[1][1] = v2_input.y;

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

    initial_cone_top = furthestEdge(working_contour, initial_cone_center, mode);

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

    //TODO: remove and replace with contours already calculated
    //Convert the image from 3 channel to 1 channel
    cv::cvtColor(three_chann_model_cone, model_temp_img, cv::COLOR_BGR2GRAY);
    cv::threshold(model_temp_img, model_temp_img, 128, 255, cv::THRESH_BINARY);
    // Find the contours of the model image
    std::vector<std::vector<cv::Point>> modelContours;
    cv::findContours(model_temp_img, modelContours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

    ////////////////////////////////////////// *Processing Model Cone Into Vectors* //////////////////////////////////////////
    init_model_cone_center = getBinaryCenter(modelContours[0]);

    // Finds the initial vector for the top of the cone
    init_model_cone_top = furthestEdge(modelContours[0], init_model_cone_center, mode);

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

