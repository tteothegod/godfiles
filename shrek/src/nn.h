#pragma once

#include <cmath>
#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <random>


#ifndef MATRIX_H
#define MATRIX_H

class Matrix_
{
public:
    int rows;
    int cols;
    float** matrix;

    Matrix_(int rows_, int cols_);
    void randomize();

    static Matrix_* add(Matrix_& a, Matrix_& b);
    void add(Matrix_& n);
    void add(float n);

    static Matrix_* subtract(Matrix_& a, Matrix_& b);
    void subtract(Matrix_& n);
    void subtract(float n);

    Matrix_* multiply(Matrix_& n);
    void hadamard(Matrix_& n);

    static Matrix_* multiply(Matrix_& a, Matrix_& b);
    void multiply(float n);
    static Matrix_* multiply(Matrix_& a, float n);

    Matrix_* transpose();
    static Matrix_* transpose(Matrix_& a);

    void print();

    Matrix_* mapDouble(std::function<double(double)> func);
    Matrix_* mapFloat(std::function<float(float)> func);
    void mapInPlace(std::function<double(double)> func);

    static Matrix_* fromArray(float* input, int arr_len);
    static float* toArray(Matrix_& input);
};

#endif


#ifndef TESTDATA_H
#define TESTDATA_H
class Test_Data
{
public:

    int amount_of_datapoints;
    float** training_array;
    float** labels;

    Test_Data( int amt_of_datapoints, int datapoint_size, int label_size);
    void loadImages(int img_widths, int img_heights, cv::Mat* training_imgs);
    void assignLabels();
};

#endif


#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

class NeuralNetwork
{ 
public:
    int num_inputs;
    int num_hidden;
    int num_outputs;

    float learning_rate = .1;

    Matrix_* weights_ih;
    Matrix_* weights_ho;
    Matrix_* bias_h;
    Matrix_* bias_o;

    NeuralNetwork(int num_i, int num_h, int num_o);

    Matrix_* feedForward(float* input_array);

    Matrix_* feedForward(Matrix_& input);

    void train(float* input_array, float* target_array);
};

#endif

void saveMatrixData(const std::string& filename, const Matrix_& input);
Matrix_* loadMatrixData(const std::string& filename);

