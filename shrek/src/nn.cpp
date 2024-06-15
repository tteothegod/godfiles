#include "maths.h"
#include "nn.h"
#include <fstream>

std::default_random_engine generator(std::time(0));

Matrix_::Matrix_(int rows_, int cols_)
{
    rows = rows_;
    cols = cols_;
    matrix = new float*[rows];

    for (int i = 0; i < rows; i++)
    {
        matrix[i] = new float[cols];
        for (int j = 0; j < cols; j++)
        {
            matrix[i][j] = 0;
        }
    }
}

void Matrix_::randomize()
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            std::uniform_real_distribution<double> distribution(-1, 1);
            matrix[i][j] = static_cast<float>(distribution(generator));
        }
    }
}


Matrix_* Matrix_::add(Matrix_& a, Matrix_& b)
{

    Matrix_* result = new Matrix_(a.rows, a.cols);

    if (a.rows != b.rows || a.cols != b.cols) 
    {
        printf("A's Rows and Columns must be the same size as B's Rows and Columns");
        return nullptr;
    }

    for (int i = 0; i < a.rows; i++) {
        for (int j = 0; j < a.cols; j++) {
            result->matrix[i][j] = a.matrix[i][j] + b.matrix[i][j];
        }
    }
    return result;
}

void Matrix_::add(Matrix_& n)
{
    if (rows != n.rows || cols != n.cols) 
    {
        printf("A's Rows and Columns must be the same size as B's Rows and Columns");
        return;
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] += n.matrix[i][j];
        }
    }
}

void Matrix_::add(float n)
{
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                matrix[i][j] += n;
            }
        }
    } 
}

Matrix_* Matrix_::subtract(Matrix_& a, Matrix_& b)
{

    Matrix_* result = new Matrix_(a.rows, a.cols);

    if (a.rows != b.rows || a.cols != b.cols) 
    {
        printf("A's Rows and Columns must be the same size as B's Rows and Columns");
        return nullptr;

    }

    for (int i = 0; i < a.rows; i++) {
        for (int j = 0; j < a.cols; j++) {
            result->matrix[i][j] = a.matrix[i][j] - b.matrix[i][j];
        }
    }

    return result;
}

void Matrix_::subtract(Matrix_& n)
{
    if (rows != n.rows || cols != n.cols) 
    {
        printf("A's Rows and Columns must be the same size as B's Rows and Columns");
        return;
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] -= n.matrix[i][j];
        }
    }
}

void Matrix_::subtract(float n)
{
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                matrix[i][j] -= n;
            }
        }
    } 
}

Matrix_* Matrix_::multiply(Matrix_& n)
{
    if(cols != n.rows)
    {
        printf("\nColumns of A must match Rows of B. \n\n");
        return nullptr;
    }

    Matrix_* resultant_matrix = new Matrix_(rows, n.cols);

    for (int i = 0; i < resultant_matrix->rows; i++)
    {
        for (int j = 0; j < resultant_matrix->cols; j++)
        {
            float sum = 0;
            for (int k = 0; k < cols; k++)
            {
                sum += matrix[i][k] * n.matrix[k][j];
            }
            resultant_matrix->matrix[i][j] = sum;
        }
    }
    return resultant_matrix;
}

void Matrix_::hadamard(Matrix_& n)
{
    if(cols == n.cols && rows == n.rows)
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                matrix[i][j] *= n.matrix[i][j];
            }
        }
    }
    else 
    {
        printf(" Hadamard: Columns and rows of A and B must be equal \n");
        return;
    }
}

Matrix_* Matrix_::multiply(Matrix_& a, Matrix_& b)
{
    Matrix_* resultant_matrix = new Matrix_(a.rows, b.cols);

    if((a.cols != b.rows))
    {
        printf("\nColumns of A must match Rows of B. \n\n");
        return nullptr;
    }

    for (int i = 0; i < resultant_matrix->rows; i++)
    {
        for (int j = 0; j < resultant_matrix->cols; j++)
        {
            float sum = 0;
            for (int k = 0; k < a.cols; k++)
            {
                sum += a.matrix[i][k] * b.matrix[k][j];
            }
            resultant_matrix->matrix[i][j] = sum;
        }
    }
    return resultant_matrix;
}

void Matrix_::multiply(float n)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            matrix[i][j] *= n;
        }
    }
}

Matrix_* Matrix_::multiply(Matrix_& a, float n)
{
    Matrix_* result = new Matrix_(a.rows, a.cols);

    for (int i = 0; i < a.rows; i++)
    {
        for (int j = 0; j < a.cols; j++)
        {
            result->matrix[i][j] = a.matrix[i][j] * n;
        }
    }
    return result;
}

Matrix_* Matrix_::transpose()
{
    Matrix_* result = new Matrix_(cols, rows);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            result->matrix[j][i] = matrix[i][j];
        }
    }
    return result;
}

Matrix_* Matrix_::transpose(Matrix_& a)
{
    Matrix_* result = new Matrix_(a.cols, a.rows);

    for (int i = 0; i < a.rows; i++)
    {
        for (int j = 0; j < a.cols; j++)
        {
            result->matrix[j][i] = a.matrix[i][j];
        }
    }
    return result;
}

void Matrix_::print()
{
    printf("\nMatrix:\n");
    for(int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("%f  ", matrix[i][j]);
        }
        printf("\n");
    }
}

Matrix_* Matrix_::mapDouble(std::function<double(double)> func)
{
    Matrix_* result = new Matrix_(rows, cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            result->matrix[i][j] = static_cast<float>(func(matrix[i][j]));
        }
    }
    return result;
}

Matrix_* Matrix_::mapFloat(std::function<float(float)> func)
{
    Matrix_* result = new Matrix_(rows, cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            result->matrix[i][j] = func(matrix[i][j]);
        }
    }
    return result;
}

void Matrix_::mapInPlace(std::function<double(double)> func)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            matrix[i][j] = static_cast<float>(func(matrix[i][j]));
        }
    }
}

Matrix_* Matrix_::fromArray(float* input, int arr_len)
{
    Matrix_* result = new Matrix_(arr_len, 1);

    for (int i = 0; i < arr_len; i++)
    {
        result->matrix[i][0] = input[i];
    }
    return result;
}

float* Matrix_::toArray(Matrix_& input)
{
    float* result = new float[input.rows * input.cols];
    int curr = 0;
    for (int i = 0; i < input.rows; i++)
    {
        for (int j = 0; j < input.cols; j++)
        {
            result[curr] = input.matrix[i][j];
            curr++;
        }
    }
    return result;
}


Test_Data::Test_Data( int amt_of_datapoints, int datapoint_size, int label_size)
{
    amount_of_datapoints = amt_of_datapoints;
    training_array = new float*[amt_of_datapoints];
    for (int i = 0; i < amt_of_datapoints; i++)
    {
        training_array[i] = new float[datapoint_size];
    }

    labels = new float*[amt_of_datapoints];
    for (int i = 0; i < amt_of_datapoints; i++)
    {
        labels[i] = new float[label_size];
    }
}

void Test_Data::loadImages(int img_widths, int img_heights, cv::Mat* training_imgs)
{
    for (int i = 0; i < amount_of_datapoints; i++) 
    {
        int count = 0;
        for (int j = 0; j < img_widths; j++) 
        {
            for (int l = 0; l < img_heights; l++) 
            {
                if ((float)training_imgs[i].at<uchar>(l, j) > 0) training_array[i][count] = 1;
                else training_array[i][count] = 0;
                count++; // Increment the count here
            }
        }
    }
}

void Test_Data::assignLabels()
{
    for (int i = 0; i < amount_of_datapoints; i++) {
        if (i < 1000) {
            labels[i][0] = 1.0;
        }
        else {
            labels[i][0] = 0.0;
        }
    }
}


NeuralNetwork::NeuralNetwork(int num_i, int num_h, int num_o)
{
    num_inputs = num_i;
    num_hidden = num_h;
    num_outputs = num_o;

    weights_ih = new Matrix_(num_hidden, num_inputs);      
    weights_ho = new Matrix_(num_outputs, num_hidden);
    weights_ih->randomize();
    weights_ho->randomize();

    bias_h = new Matrix_(num_hidden, 1);
    bias_o = new Matrix_(num_outputs, 1);
    bias_h->randomize();
    bias_o->randomize();
}


Matrix_* NeuralNetwork::feedForward(float* input_array)
{
    Matrix_* input = Matrix_::fromArray(input_array, num_inputs);
    Matrix_* hidden;
    Matrix_* output;

    // Generating the hidden Outputs
    hidden = Matrix_::multiply(*weights_ih, *input);
    hidden->add(*bias_h);
    hidden->mapInPlace([](float x) -> float { return sigmoid(x); });

    // Generating the output Outputs
    output = Matrix_::multiply(*weights_ho, *hidden);
    output->add(*bias_o);
    output->mapInPlace([](float x) -> float { return sigmoid(x); });

    return output;
}

// Input matrix
Matrix_* NeuralNetwork::feedForward(Matrix_& input)
{
    // Generating the hidden Outputs
    Matrix_* hidden;
    Matrix_* output;

    hidden = Matrix_::multiply(*weights_ih, input);
    hidden->add(*bias_h);
    hidden->mapInPlace([](float x) -> float { return sigmoid(x); });

    // Generating the output Outputs
    output = Matrix_::multiply(*weights_ho, *hidden);
    output->add(*bias_o);
    output->mapInPlace([](float x) -> float { return sigmoid(x); });

    return output;
}

void NeuralNetwork::train(float* input_array, float* target_array)
{
    Matrix_* input = Matrix_::fromArray(input_array, num_inputs);
    Matrix_* target = Matrix_::fromArray(target_array, num_outputs);


    // Generating the hidden Outputs
    Matrix_* ff_hidden;
    Matrix_* ff_output;
    ff_hidden = Matrix_::multiply(*weights_ih, *input);
    ff_hidden->add(*bias_h);
    ff_hidden->mapInPlace([](float x) -> float { return sigmoid(x); });

    // Generating the output Outputs
    ff_output = Matrix_::multiply(*weights_ho, *ff_hidden);
    ff_output->add(*bias_o);
    ff_output->mapInPlace([](float x) -> float { return sigmoid(x); });

    Matrix_* output_errors = Matrix_::subtract(*target, *ff_output);
    output_errors->print();

    //Calculate output gradient
    Matrix_* output_gradient = ff_output->mapFloat([](float x) -> float { return dsigmoid(x); });
    output_gradient->hadamard(*output_errors);
    output_gradient->multiply(learning_rate);

    //Calculate hidden weight deltas
    Matrix_* hidden_t = Matrix_::transpose(*ff_hidden);
    Matrix_* weights_ho_deltas = Matrix_::multiply(*output_gradient, *hidden_t);

    // Add hidden deltas - Could be problem 
    weights_ho->add(*weights_ho_deltas);
    bias_o->add(*output_gradient);

    //Hidden layer errors
    Matrix_* who_t = Matrix_::transpose(*weights_ho);
    Matrix_* hidden_errors = Matrix_::multiply(*who_t, *output_errors);

    //Calculate hidden gradient
    Matrix_* hidden_gradient = ff_hidden->mapFloat([](float x) -> float { return dsigmoid(x); });
    hidden_gradient->hadamard(*hidden_errors);
    hidden_gradient->multiply(learning_rate);

    // Calculate input weight deltas
    Matrix_* input_t = Matrix_::transpose(*input);
    Matrix_* weights_ih_deltas = Matrix_::multiply(*hidden_gradient, *input_t);

    // Could be problem
    weights_ih->add(*weights_ih_deltas);
    bias_h->add(*hidden_gradient);
    weights_ho_deltas->print();
}


void saveMatrixData(const std::string& filename, const Matrix_& input) {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error: Unable to open the file for writing." << std::endl;
        return;
    }

    // Write matrix dimensions (number of rows and columns)
    outFile << input.rows << " " << input.cols << "\n";

    // Write matrix data
    for (int i = 0; i < input.rows; ++i) {
        for (int j = 0; j < input.cols; ++j) {
            outFile << input.matrix[i][j];
            if (j < input.cols - 1) {
                outFile << " ";
            }
        }
        outFile << "\n";
    }
}

Matrix_* loadMatrixData(const std::string& filename) {
    std::ifstream inFile(filename);

    // Read matrix dimensions
    int rows, cols;
    inFile >> rows >> cols;
    Matrix_* result = new Matrix_(rows, cols);

    // Read matrix data
    for (int i = 0; i < result->rows; ++i) {
        for (int j = 0; j < result->cols; ++j) {
            inFile >> result->matrix[i][j];
        }
    }
    return result;
}