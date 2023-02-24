#include <iostream>
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
    // Load the image
    Mat image = imread(argv[1], IMREAD_COLOR);

    // Initialize the Tesseract OCR engine
    tesseract::TessBaseAPI tess;
    tess.Init(NULL, "eng", tesseract::OEM_LSTM_ONLY);
    tess.SetPageSegMode(tesseract::PSM_AUTO);

    // Extract text from the image
    tess.SetImage(image.data, image.size().width, image.size().height, 3, image.step);
    char* text = tess.GetUTF8Text();
    cout << "Extracted Text: " << text << endl;

    // Find progress bars in the image
    Mat gray, edges;
    cvtColor(image, gray, COLOR_BGR2GRAY);
    Canny(gray, edges, 50, 200);
    vector<Vec4i> lines;
    HoughLinesP(edges, lines, 1, CV_PI / 180, 50, 50, 10);

    // Determine the fill percentage for each progress bar
    for (size_t i = 0; i < lines.size(); i++) {
        Vec4i l = lines[i];
        float length = sqrt(pow(l[2] - l[0], 2) + pow(l[3] - l[1], 2));
        float progress = length / image.size().width;
        cout << "Progress Bar " << i + 1 << " is " << progress * 100 << "% filled." << endl;
    }

    // Free memory
    delete[] text;

    return 0;
}
