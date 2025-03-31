#include "maths.h"
#include "imaging.h"
#include "comms.h"
#include "listener.h"
#include  "pthread.h"
#include "nn.h"
#include <pthread.h>

pthread_mutex_t mutex;

#if 1

v2 positive_cone;

int saved_imgs = 0;

int main() 
{
    int AMT_OF_MODEL_IMGS = 5;

    cv::VideoCapture cap(1);
    
    if (!cap.isOpened()) 
    {
        std::cerr << "Unable to open camera!" << std::endl;
        return 1;
    }

    // // net code
    // int server_socket = initServerSock();
    // std::cout << "starting server! \n";

    while (true) 
    {
        bool connected = false;

        sockaddr_in client_address;
        socklen_t client_address_size = sizeof(client_address);
        int client_socket;
        
        //std::cout << "Waiting for connections \n";
        // while ((client_socket = accept(server_socket, (sockaddr*)&client_address, &client_address_size)) < 0)
        // {
        //     std::cerr << "Failed to accept connection to target machine, retrying in 1 second..." << std::endl;
        //     std::this_thread::sleep_for(std::chrono::seconds(1));
        // }
        //std::cout << "Connection found! \n";
        connected = true;

        while (connected)
        {        
            cv::Mat frame;
            cv::Mat hsv;
            cv::Mat yellow_mask;
            cv::Mat display_frame;

            //Load model frames
            std::vector<cv::Mat> three_chann_model_cone;
            loadFrames(three_chann_model_cone, frame, hsv, yellow_mask, cap, display_frame);
            
            // Finds the average yellowness of each pixel
            float accum_y = avgYellowness(frame);

            // Calculates the standard deviation varience of the average yellowness
            float yellow_dev = frameStdDev(frame, accum_y);

            // Calculates the threshold of the yellowness based on the deviation and the average
            float new_threshold = accum_y + yellow_dev;

            // Sets pixels either to 0 or 255 depending on if they are above threshold or not
            thresholdFrame(frame, yellow_mask, accum_y, yellow_dev);

            // Find contours in the yellow mask
            std::vector<std::vector<cv::Point>> contours;
            cv::findContours(yellow_mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

            // filters out contours with less that 1000 area and low circularity
            filterContours(contours, yellow_mask);

            // Find the contours after filtering
            std::vector<std::vector<cv::Point>> updatedContours;
            cv::findContours(yellow_mask, updatedContours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

            sort(updatedContours.begin(), updatedContours.end(), compareContourAreas);

            const int amt_of_imgs = 1;
            const int img_widths = 20;
            const int img_heights = 20;
            
            // Create neural network
            NeuralNetwork* nn = new NeuralNetwork(img_widths * img_heights, 10, 1);
            nn->weights_ih = loadMatrixData("./src/nn_presets/w_ih_data.txt");
            nn->weights_ho = loadMatrixData("./src/nn_presets/w_ho_data.txt");
            nn->bias_h = loadMatrixData("./src/nn_presets/bias_ih.txt");
            nn->bias_o = loadMatrixData("./src/nn_presets/bias_ho.txt");

            // Find the center of the cone by averaging all of the x and y values
            if(!updatedContours.empty())
            {
                int amt_contours_checked = updatedContours.size();
                if(amt_contours_checked > 2) amt_contours_checked = 2;

                for (int i = 0; i < amt_contours_checked; i++)
                {
                    vectorDerivationMethod mode = LONGEST_DISTANCE;
                    bool is_cone = false;

                    ////////////////////////////////////////////// *Processing Cone Into Vectors* //////////////////////////////////////////////
                    v2 new_cone_center;
                    std::vector<v2> cone_top;
                    std::vector<v2> cone_perpendicular;
                    v2 initial_cone_center;

                    processFrameVectors(new_cone_center, cone_top, cone_perpendicular, initial_cone_center, mode, updatedContours[i], yellow_mask);

                    std::vector<v2> model_cone_top;
                    std::vector<v2> model_cone_perpendicular;
                    cv::Mat model_temp_img;

                    v2 new_model_cone_center = {0,0};

                    cv::Mat transformationBinary;

                    for (int l = 3; l < 5; l++ )
                    {
                        processModelVectors(three_chann_model_cone[l], new_model_cone_center, model_cone_perpendicular, model_cone_top, model_temp_img, mode, yellow_mask);
                        tranformFrame2Model(model_temp_img, updatedContours[i], cone_top, new_cone_center, cone_perpendicular, model_cone_top, new_model_cone_center, model_cone_perpendicular, transformationBinary);
                        

                        ////////////////////////////////////////////// *Neural Network Code* //////////////////////////////////////////////

                        // Load images
                        cv::Mat prediction_img[amt_of_imgs] = {transformationBinary};
                        cv::resize(prediction_img[0], prediction_img[0], cv::Size(img_widths, img_heights), 0, 0, cv::INTER_LINEAR );

                        // Load data into array
                        Test_Data* pred_data = new Test_Data(amt_of_imgs, img_widths * img_heights, 1);
                        pred_data->loadImages(img_widths, img_heights, prediction_img);


                        Matrix_* prediction = nn->feedForward(pred_data->training_array[0]);
                        float prob_cone = prediction->matrix[0][0];
                        if(prob_cone > 0.85)
                        {
                            prediction->print();
                            is_cone = true;
                            imshow("hola", transformationBinary); 

                        }
                        else
                        {
                            // if(saved_imgs <= 2000)
                            // {
                            //     std::string img_save_name = "./negative_imgs/img_" + std::to_string(saved_imgs) + ".png";
                            //     std::cout << "Number" << std::to_string(saved_imgs) << std::endl;
                            //     cv::imwrite(img_save_name, transformationBinary);
                            //     saved_imgs++;
                            // }
                        }
                    }

                    // Get the bounding rectangle of the contour
                    if(is_cone)
                    {
                        cv::Rect rect = cv::boundingRect(updatedContours[i]);
                        // Draw the rectangle on the original frame
                        cv::rectangle(frame, rect, cv::Scalar(0, 255, 0), 2);
                        cv::line(frame, {(int)(initial_cone_center.x - rect.width/2), (int) initial_cone_center.y}, {(int)(initial_cone_center.x + rect.width/2), (int) initial_cone_center.y}, cv::Scalar(50, 0, 230), 2);
                        cv::line(frame, {(int) initial_cone_center.x, (int)(initial_cone_center.y - rect.height/2)}, {(int) initial_cone_center.x, (int)(initial_cone_center.y + rect.height/2)}, cv::Scalar(50, 0, 230), 2);

                        normalized_outputs.push_back({initial_cone_center.x / frame.cols, initial_cone_center.y / frame.rows});
                        positive_cone = initial_cone_center;

                        // if(saved_imgs <= 1000)
                        // {
                        //     std::string img_save_name = "./positive_imgs/img_" + std::to_string(saved_imgs) + ".png";
                        //     std::cout << "Number" << std::to_string(saved_imgs) << std::endl;
                        //     cv::imwrite(img_save_name, transformationBinary);
                        //     saved_imgs++;
                        // }
                    }

                    // else if(contourArea(updatedContours[i]) > 80000)
                    // {
                    //     cv::Rect rect = cv::boundingRect(updatedContours[0]);
                    //     cv::line(frame, {(int)(initial_cone_center.x - rect.width/2), (int) initial_cone_center.y}, {(int)(initial_cone_center.x + rect.width/2), (int) initial_cone_center.y}, cv::Scalar(50, 0, 230), 2);
                    //     cv::line(frame, {(int) initial_cone_center.x, (int)(initial_cone_center.y - rect.height/2)}, {(int) initial_cone_center.x, (int)(initial_cone_center.y + rect.height/2)}, cv::Scalar(50, 0, 230), 2);
                    //     cv::rectangle(frame, rect, cv::Scalar(0, 255, 0), 2);
                    //     //normalized_outputs.push_back({initial_cone_center.x / frame.cols, initial_cone_center.y / frame.rows});
                    // }

                    else
                    {
                        float diff = length({ positive_cone.x - initial_cone_center.x, positive_cone.y - initial_cone_center.y });
                        //Secondary filtering
                        // if( diff < 50)
                        // {
                        //     cv::Rect rect = cv::boundingRect(updatedContours[i]);
                        //     // Draw the rectangle on the original frame
                        //     cv::rectangle(frame, rect, cv::Scalar(0, 255, 0), 2);
                        //     cv::line(frame, {(int)(initial_cone_center.x - rect.width/2), (int) initial_cone_center.y}, {(int)(initial_cone_center.x + rect.width/2), (int) initial_cone_center.y}, cv::Scalar(50, 0, 230), 2);
                        //     cv::line(frame, {(int) initial_cone_center.x, (int)(initial_cone_center.y - rect.height/2)}, {(int) initial_cone_center.x, (int)(initial_cone_center.y + rect.height/2)}, cv::Scalar(50, 0, 230), 2);
                        //     positive_cone = initial_cone_center;
                        // }
                        // else
                        // {
                            cv::Rect rect = cv::boundingRect(updatedContours[i]);
                            // Draw the rectangle on the original frame
                            cv::rectangle(frame, rect, cv::Scalar(0, 0, 255), 2);
                            // cv::line(frame, {(int)(initial_cone_center.x - rect.width/2), (int) initial_cone_center.y}, {(int)(initial_cone_center.x + rect.width/2), (int) initial_cone_center.y}, cv::Scalar(50, 0, 230), 2);
                            // cv::line(frame, {(int) initial_cone_center.x, (int)(initial_cone_center.y - rect.height/2)}, {(int) initial_cone_center.x, (int)(initial_cone_center.y + rect.height/2)}, cv::Scalar(50, 0, 230), 2);
                        // }


                    }     
                }
            }

            // cv::imshow("binary",  yellow_mask);

            cv::imshow("binary",  yellow_mask);
            // cv::imshow("original",  frame);
            cv::waitKey(1);
    
            // net code error checking etc
            //printf("Thread Size: %d\n", normalized_outputs.size());
            if(!normalized_outputs.empty())
            {
                //printf("Coords (%f, %f)\n", normalized_outputs[0].x, normalized_outputs[0].y);
            }

            if (!sendData(client_socket, normalized_outputs))
            {
                connected = false;
            }
        }
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}


#else

std::default_random_engine generator1(std::time(0));

int main()
{

    const int amt_of_imgs = 2000;
    const int img_widths = 20;
    const int img_heights = 20;

    // Load images
    cv::Mat training_imgs[amt_of_imgs];
    for (int i = 0; i < amt_of_imgs; i++) {
        if(i < 1000) training_imgs[i] = cv::imread("./positive_imgs/img_" + std::to_string(i) + ".png");
        else training_imgs[i] = cv::imread("./negative_imgs/img_" + std::to_string(i - 1000) + ".png");
    }

    // Greyscale and scale imgs
    for (int i = 0; i < amt_of_imgs ; i++) {
        cv::cvtColor(training_imgs[i], training_imgs[i], cv::COLOR_BGR2GRAY);
        cv::resize(training_imgs[i], training_imgs[i], cv::Size(img_widths, img_heights), 0, 0, cv::INTER_LINEAR );
    }

    // Load data into array
    Test_Data* training_data = new Test_Data(amt_of_imgs, img_widths * img_heights, 1);
    training_data->loadImages(img_widths, img_heights, training_imgs);
    training_data->assignLabels();

    // Create neural network
    NeuralNetwork* nn = new NeuralNetwork(img_widths * img_heights, 20, 1);

    nn->weights_ih = loadMatrixData("./src/nn_presets/w_ih_data.txt");
    nn->weights_ho = loadMatrixData("./src/nn_presets/w_ho_data.txt");
    nn->bias_h = loadMatrixData("./src/nn_presets/bias_ih.txt");
    nn->bias_o = loadMatrixData("./src/nn_presets/bias_ho.txt");

    Matrix_* pre_train_pos = nn->feedForward(training_data->training_array[0]);
    Matrix_* pre_train_neg = nn->feedForward(training_data->training_array[1999]);

    // Train neural network using data
    for(int j = 0; j < 20000; j++)
    {
        std::uniform_real_distribution<double> distribution(200, 1799);
        int rand = static_cast<int>(std::round(distribution(generator1)));
        nn->train(training_data->training_array[rand], training_data->labels[rand]);
        if(j == 20000 - 1) 
        {
            printf("Finished Training\n");
        }
    }

    printf("Pre Training Positive: %f, %f\n", pre_train_pos->matrix[0][0], training_data->labels[0]);
    printf("Pre Training Negative: %f, %f\n", pre_train_neg->matrix[0][0], training_data->labels[1999]);

    // Find avg positive output
    float avg_pos = 0;
    for(int i = 0; i < 200; i++)
    {
        Matrix_* result = nn->feedForward(training_data->training_array[i]);
        avg_pos += result->matrix[0][0];
    }
    avg_pos /= 200;
    printf("avg pos = %f \n", avg_pos);


    // Find avg negative output
    float avg_neg = 0;
    for(int i = 1800; i < 2000; i++)
    {
        Matrix_* result_2 = nn->feedForward(training_data->training_array[amt_of_imgs - 1]);
        avg_neg += result_2->matrix[0][0];

    }
    avg_neg /= 200;
    printf("avg neg = %f \n", avg_neg);


    // saveMatrixData("./src/nn_presets/w_ih_data.txt", *nn->weights_ih);
    // saveMatrixData("./src/nn_presets/bias_ih.txt", *nn->bias_h);
    // saveMatrixData("./src/nn_presets/w_ho_data.txt", *nn->weights_ho);
    // saveMatrixData("./src/nn_presets/bias_ho.txt", *nn->bias_o);
}

#endif



