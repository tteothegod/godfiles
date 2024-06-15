#include <math.h>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string.h>
#include <fstream>

bool customComparator(int a, int b) {
    // Swap the second and third elements while comparing
    if (a == 2) a = 3;
    else if (a == 3) a = 2;

    if (b == 2) b = 3;
    else if (b == 3) b = 2;

    return a < b;
}

int main()
{
    cv::VideoCapture video_feed(1);

    while (true)
    {
        if (!video_feed.isOpened()) 
        {
            std::cerr << "Unable to open camera!" << std::endl;
            return 1;
        }

        cv::Mat video_frame;

        video_feed >> video_frame;

        // Write calibration code later >> n - 1
        int corners[4] = {0, 9, 19, 29};

        int differences[4];
        for (int i = 0; i < 4; i++)
        {
            differences[i] = corners[i+1] - corners[i];
        }

        float led_window_size[4];
        for (int i = 0; i < 4; i++)
        {
            float side_length;
            if(i == 0 || i == 3)
            {
                side_length = video_frame.cols;
            }
            else side_length = video_frame.rows;

            led_window_size[i] = side_length / differences[i];
        }

        std::vector<std::vector<float>> led_red;
        std::vector<std::vector<float>> led_green;
        std::vector<std::vector<float>> led_blue;

        // Loops through every side
        for (int i = 0; i < 3; i = i + 2)
        {
            int y;
            if(i == 0) y = 0;
            else y = video_frame.rows;

            std::vector<float> current_leds_red;
            std::vector<float> current_leds_green;
            std::vector<float> current_leds_blue;
            // Loops through every Led light
            for (int l  = 0; l < differences[i]; l++)
            {
                float sum_r = 0;
                float sum_g = 0;
                float sum_b = 0;

                float amt_of_pixels = 0;
                // Loops through every pixel
                for(int j = static_cast<int>(led_window_size[i] * l); j < static_cast<int>(led_window_size[i] * (l+1)); j++)
                {
                    cv::Vec3b pixel_val = video_frame.at<cv::Vec3b>(y, j);
                    sum_r += pixel_val[2];
                    sum_g += pixel_val[1];
                    sum_b += pixel_val[0];
                    amt_of_pixels++;
                }
                current_leds_red.push_back(sum_r/amt_of_pixels);
                current_leds_green.push_back(sum_g/amt_of_pixels);
                current_leds_blue.push_back(sum_b/amt_of_pixels);
            }
            led_red.push_back(current_leds_red);
            led_green.push_back(current_leds_green);
            led_blue.push_back(current_leds_blue);

        }

        for (int i = 1; i < 4; i = i + 2)
        {
            int x;
            if(i == 1) x = video_frame.cols;
            else x = 0;

            std::vector<float> current_leds_red;
            std::vector<float> current_leds_green;
            std::vector<float> current_leds_blue;
            for (int l  = 0; l < differences[i]; l++)
            {
                float sum_r = 0;
                float sum_g = 0;
                float sum_b = 0;

                if(l == differences[i] - 1) // Final LED
                {
                    printf("\n\n\nl = 2\nStart Pixel = %d\nEnd Pixel = %d\n", static_cast<int>(led_window_size[i] * l), static_cast<int>(led_window_size[i] * (l+1)));
                }

                float amt_of_pixels = 0;
                for(int j = static_cast<int>(led_window_size[i] * l); j < static_cast<int>(led_window_size[i] * (l+1)); j++)
                {
                    cv::Vec3b pixel_val = video_frame.at<cv::Vec3b>(j, x);
                    sum_r += pixel_val[2];
                    sum_g += pixel_val[1];
                    sum_b += pixel_val[0];
                    amt_of_pixels++;
                }
                // printf("(r, g, b) = (%f, %f, %f) \n", sum_r/amt_of_pixels, sum_g/amt_of_pixels, sum_b/amt_of_pixels);

                current_leds_red.push_back(sum_r/amt_of_pixels);
                current_leds_green.push_back(sum_g/amt_of_pixels);
                current_leds_blue.push_back(sum_b/amt_of_pixels);
            }
            led_red.push_back(current_leds_red);
            led_green.push_back(current_leds_green);
            led_blue.push_back(current_leds_blue);
        }
        std::vector<std::vector<float>> organized_led_red;
        std::vector<std::vector<float>> organized_led_green;
        std::vector<std::vector<float>> organized_led_blue;

        organized_led_red.push_back(led_red[0]);
        organized_led_red.push_back(led_red[2]);
        organized_led_red.push_back(led_red[1]);
        organized_led_red.push_back(led_red[3]);

        organized_led_green.push_back(led_green[0]);
        organized_led_green.push_back(led_green[2]);
        organized_led_green.push_back(led_green[1]);
        organized_led_green.push_back(led_green[3]);

        organized_led_blue.push_back(led_blue[0]);
        organized_led_blue.push_back(led_blue[2]);
        organized_led_blue.push_back(led_blue[1]);
        organized_led_blue.push_back(led_blue[3]);

        for (int i = 0; i < 3; i = i + 2)
        {
            int y = 0;
            if(i == 0) y = 0;
            else y = video_frame.rows;

            for (int l = 0; l < differences[i] + 1; l++)
            {
                // printf("%f, %f, %f\n", organized_led_red[0][0], organized_led_green[0][0], organized_led_blue[0][0]);
                // printf(" frame cols = %d\n", video_frame.cols);
                // printf("(x, y) = (%d, %d) \n", static_cast<int>(round((video_frame.cols / differences[i]))), y);
                // printf("%d\n", organized_led_red[0].size());
                // printf("(r, g, b) = (%f, %f, %f) \n", organized_led_red[i][l], organized_led_green[i][l], organized_led_blue[i][l]);
                cv::circle(video_frame, cv::Point( static_cast<int>(round((video_frame.cols / differences[i]))) * (l+0), y), 50, cv::Scalar(organized_led_red[i][l], organized_led_green[i][l], organized_led_blue[i][l]), -1);
                // cv::circle(video_frame, cv::Point( static_cast<int>(round((video_frame.cols / differences[i]))) * (l+1), y), 50, cv::Scalar(0, 0, 0), -1);
            }
        }

        for (int i = 1; i < 4; i = i + 2)
        {

        }

        cv::imshow("Camera Feed", video_frame);
        if (cv::waitKey(1) == 27) {
        break;
    }
    }

}