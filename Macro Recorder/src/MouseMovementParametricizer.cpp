#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include <cfloat>

using namespace std;
using namespace Eigen;

float polynomial_x(float t) {
    return (-3.55772e-52) * pow(t, 100) + (2.11398e-51) * pow(t, 99) + (-7.58378e-51) * pow(t, 98) + (2.66176e-50) * pow(t, 97) + (4.83554e-50) * pow(t, 96) + (-4.24141e-49) * pow(t, 95) + (5.73606e-49) * pow(t, 94) + (3.36686e-48) * pow(t, 93) + (-1.54991e-47) * pow(t, 92) + (1.21011e-46) * pow(t, 91) + (2.4383e-46) * pow(t, 90) + (-2.82688e-45) * pow(t, 89) + (-4.73167e-45) * pow(t, 88) + (-1.797e-44) * pow(t, 87) + (1.35448e-43) * pow(t, 86) + (-3.65015e-43) * pow(t, 85) + (1.03419e-42) * pow(t, 84) + (-2.74764e-42) * pow(t, 83) + (6.45167e-42) * pow(t, 82) + (1.00352e-40) * pow(t, 81) + (-1.1864e-40) * pow(t, 80) + (-5.08353e-40) * pow(t, 79) + (-2.07074e-39) * pow(t, 78) + (1.12917e-38) * pow(t, 77) + (-1.04603e-37) * pow(t, 76) + (1.1171e-37) * pow(t, 75) + (6.25661e-37) * pow(t, 74) + (6.55585e-36) * pow(t, 73) + (-3.78022e-36) * pow(t, 72) + (1.79534e-35) * pow(t, 71) + (2.90325e-34) * pow(t, 70) + (1.68102e-33) * pow(t, 69) + (-8.21266e-35) * pow(t, 68) + (-3.80582e-32) * pow(t, 67) + (-7.66453e-32) * pow(t, 66) + (-5.85623e-31) * pow(t, 65) + (-3.70104e-31) * pow(t, 64) + (5.34141e-30) * pow(t, 63) + (9.52797e-30) * pow(t, 62) + (-2.75532e-29) * pow(t, 61) + (5.86752e-30) * pow(t, 60) + (-5.07228e-28) * pow(t, 59) + (-5.97834e-27) * pow(t, 58) + (2.45501e-26) * pow(t, 57) + (4.50552e-26) * pow(t, 56) + (2.30971e-26) * pow(t, 55) + (1.52777e-24) * pow(t, 54) + (-2.6876e-24) * pow(t, 53) + (8.46052e-24) * pow(t, 52) + (-6.28965e-24) * pow(t, 51) + (2.19838e-22) * pow(t, 50) + (-2.79467e-22) * pow(t, 49) + (6.21135e-22) * pow(t, 48) + (-1.07432e-20) * pow(t, 47) + (3.01164e-20) * pow(t, 46) + (-2.16875e-19) * pow(t, 45) + (3.04144e-19) * pow(t, 44) + (-7.46506e-18) * pow(t, 43) + (6.59737e-19) * pow(t, 42) + (4.10864e-17) * pow(t, 41) + (-3.28527e-16) * pow(t, 40) + (-3.11211e-16) * pow(t, 39) + (6.58041e-16) * pow(t, 38) + (2.45659e-14) * pow(t, 37) + (4.66941e-14) * pow(t, 36) + (3.38863e-13) * pow(t, 35) + (-2.4918e-13) * pow(t, 34) + (-3.92946e-12) * pow(t, 33) + (-1.91837e-12) * pow(t, 32) + (-2.27398e-12) * pow(t, 31) + (5.15055e-11) * pow(t, 30) + (-1.071e-09) * pow(t, 29) + (7.14182e-10) * pow(t, 28) + (1.57984e-09) * pow(t, 27) + (-9.25889e-10) * pow(t, 26) + (-1.37317e-07) * pow(t, 25) + (8.74135e-07) * pow(t, 24) + (2.21212e-06) * pow(t, 23) + (-1.23229e-05) * pow(t, 22) + (-1.09293e-05) * pow(t, 21) + (0.000185862) * pow(t, 20) + (0.000230431) * pow(t, 19) + (-0.00465284) * pow(t, 18) + (0.00344715) * pow(t, 17) + (0.0348129) * pow(t, 16) + (-0.090663) * pow(t, 15) + (-0.0313915) * pow(t, 14) + (1.98697) * pow(t, 13) + (-5.68902) * pow(t, 12) + (-7.85979) * pow(t, 11) + (38.4936) * pow(t, 10) + (-110.982) * pow(t, 9) + (271.408) * pow(t, 8) + (2980.01) * pow(t, 7) + (-17849.2) * pow(t, 6) + (43679.7) * pow(t, 5) + (-123908) * pow(t, 4) + (423959) * pow(t, 3) + (-873379) * pow(t, 2) + (883815) * pow(t, 1) + (-346800);
}
float polynomial_y(float t) {
    return (-6.41619e-53) * pow(t, 100) + (4.2685e-52) * pow(t, 99) + (-6.75248e-52) * pow(t, 98) + (4.80492e-51) * pow(t, 97) + (-3.31325e-51) * pow(t, 96) + (-3.4635e-50) * pow(t, 95) + (1.21457e-49) * pow(t, 94) + (-1.8673e-48) * pow(t, 93) + (3.86565e-49) * pow(t, 92) + (-2.12008e-47) * pow(t, 91) + (8.46801e-47) * pow(t, 90) + (8.50488e-47) * pow(t, 89) + (-8.94238e-47) * pow(t, 88) + (9.42986e-46) * pow(t, 87) + (1.09757e-44) * pow(t, 86) + (-5.82762e-44) * pow(t, 85) + (4.97503e-44) * pow(t, 84) + (-1.05928e-43) * pow(t, 83) + (1.83816e-42) * pow(t, 82) + (2.92369e-41) * pow(t, 81) + (6.27671e-41) * pow(t, 80) + (-5.03852e-40) * pow(t, 79) + (4.91821e-40) * pow(t, 78) + (-5.61583e-39) * pow(t, 77) + (-4.29985e-39) * pow(t, 76) + (7.06384e-38) * pow(t, 75) + (-1.8347e-38) * pow(t, 74) + (4.00612e-37) * pow(t, 73) + (-6.14557e-36) * pow(t, 72) + (-1.55921e-35) * pow(t, 71) + (-2.0197e-35) * pow(t, 70) + (4.06844e-34) * pow(t, 69) + (-4.19198e-34) * pow(t, 68) + (2.39651e-33) * pow(t, 67) + (-1.24453e-32) * pow(t, 66) + (-2.3678e-32) * pow(t, 65) + (1.78074e-31) * pow(t, 64) + (7.37351e-31) * pow(t, 63) + (-1.0031e-30) * pow(t, 62) + (-4.15152e-30) * pow(t, 61) + (-4.03582e-29) * pow(t, 60) + (8.76092e-29) * pow(t, 59) + (-8.46241e-28) * pow(t, 58) + (3.77036e-27) * pow(t, 57) + (1.49518e-26) * pow(t, 56) + (3.70564e-26) * pow(t, 55) + (4.00634e-25) * pow(t, 54) + (-9.24603e-25) * pow(t, 53) + (-1.47533e-24) * pow(t, 52) + (-1.85079e-23) * pow(t, 51) + (-6.9951e-24) * pow(t, 50) + (7.34719e-23) * pow(t, 49) + (-1.03461e-21) * pow(t, 48) + (9.1377e-22) * pow(t, 47) + (2.70482e-21) * pow(t, 46) + (-5.02643e-20) * pow(t, 45) + (1.98404e-20) * pow(t, 44) + (9.87322e-20) * pow(t, 43) + (4.38355e-19) * pow(t, 42) + (5.89415e-18) * pow(t, 41) + (-3.12233e-17) * pow(t, 40) + (3.85992e-17) * pow(t, 39) + (6.08753e-17) * pow(t, 38) + (2.38179e-15) * pow(t, 37) + (8.45343e-15) * pow(t, 36) + (8.10264e-14) * pow(t, 35) + (-2.09085e-13) * pow(t, 34) + (-8.77633e-13) * pow(t, 33) + (2.03394e-12) * pow(t, 32) + (-9.50621e-12) * pow(t, 31) + (4.41507e-11) * pow(t, 30) + (-1.26432e-10) * pow(t, 29) + (-6.44294e-10) * pow(t, 28) + (-1.07906e-09) * pow(t, 27) + (-2.33013e-09) * pow(t, 26) + (2.10276e-08) * pow(t, 25) + (1.21012e-08) * pow(t, 24) + (3.65394e-07) * pow(t, 23) + (7.92577e-07) * pow(t, 22) + (4.61438e-06) * pow(t, 21) + (-9.0859e-06) * pow(t, 20) + (0.00010912) * pow(t, 19) + (-0.000463349) * pow(t, 18) + (-0.00104372) * pow(t, 17) + (0.00300394) * pow(t, 16) + (-0.045726) * pow(t, 15) + (0.0020156) * pow(t, 14) + (0.654704) * pow(t, 13) + (1.12893) * pow(t, 12) + (-1.19549) * pow(t, 11) + (-28.314) * pow(t, 10) + (-12.1858) * pow(t, 9) + (280.723) * pow(t, 8) + (319.392) * pow(t, 7) + (-5428.82) * pow(t, 6) + (17719.5) * pow(t, 5) + (-29603.9) * pow(t, 4) + (30294.4) * pow(t, 3) + (-29933.9) * pow(t, 2) + (32624) * pow(t, 1) + (-16661);
}


void regressionDebugging(const vector<double>& time, const vector<double>& x_values, const vector<double>& y_values, int degree, VectorXd a, VectorXd b)
{
    int n = time.size();
       // Find the index of time closest to 3 seconds
    int index_closest_to_3 = -1;
    double min_diff = DBL_MAX;
    for (int i = 0; i < n; ++i) {
        double diff = abs(time[i] - 3.0);
        if (diff < min_diff) {
            min_diff = diff;
            index_closest_to_3 = i;
        }
    }
    printf("index closest to 3 %d\n", index_closest_to_3);

    if (index_closest_to_3 != -1) {
        double actual_x_at_3 = x_values[index_closest_to_3];
        double actual_y_at_3 = y_values[index_closest_to_3];
        double predicted_x_at_3 = 0.0;
        double predicted_y_at_3 = 0.0;
        for (int i = degree; i >= 0; --i) {
            predicted_x_at_3 += a(i) * pow(3.0, i);
            predicted_y_at_3 += b(i) * pow(3.0, i);
        }
        cout << "\nActual and Predicted Values at t = 3 seconds:" << endl;
        cout << "Actual X = " << actual_x_at_3 << ", Predicted X = " << predicted_x_at_3 << endl;
        cout << "Actual Y = " << actual_y_at_3 << ", Predicted Y = " << predicted_y_at_3 << endl;
    } else {
        cout << "Error: No data points found." << endl;
    }
        // for (int i = 0; i < time.size() / 2; i++)
    {
        printf("p(%f) = (%f, %f)\n", 3.0, polynomial_x(3.0), polynomial_y(3.0));
    }
}

// // Perform polynomial regression for x and y components
// void polynomialRegression(const vector<double>& time, const vector<double>& x_values, const vector<double>& y_values, int degree)
// {
//     int n = time.size();

//     // Create matrix X for x-values
//     MatrixXd X(n, degree + 1);
//     for (int i = 0; i < n; ++i) {
//         for (int j = 0; j <= degree; ++j) {
//             X(i, j) = pow(time[i], j);
//         }
//     }

//     // Create vector y for x-values
//     VectorXd y_x(n);
//     for (int i = 0; i < n; ++i) {
//         y_x(i) = x_values[i];
//     }

//     // Create vector y for y-values
//     VectorXd y_y(n);
//     for (int i = 0; i < n; ++i) {
//         y_y(i) = y_values[i];
//     }

//     // Solve for coefficients for x-values
//     VectorXd a = (X.transpose() * X).ldlt().solve(X.transpose() * y_x);

//     // Solve for coefficients for y-values
//     VectorXd b = (X.transpose() * X).ldlt().solve(X.transpose() * y_y);

//     // Print the functions as C++ code
//     cout << "// Equations for polynomial regression:" << endl;
//     cout << "float polynomial_x(float t) {" << endl;
//     cout << "    return ";
//     for (int i = degree; i >= 0; --i) {
//         cout << "(" << a(i) << ")";
//         if (i > 0) {
//             cout << " * pow(t, " << i << ")";
//             cout << " + ";
//         }
//     }
//     cout << ";" << endl;
//     cout << "}" << endl;

//     // cout << "float polynomial_y(float t) {" << endl;
//     cout << "       return ";
//     for (int i = degree; i >= 0; --i) {
//         cout << "(" << b(i) << ")";
//         if (i > 0) {
//             cout << " * pow(t, " << i << ")";
//             cout << " + ";
//         }
//     }
//     cout << ";" << endl;
//     // cout << "}" << endl;

//     // regressionDebugging(time, x_values, y_values, degree, a, b);

// }


pair<string, string> polynomialRegression(const vector<double>& time, const vector<double>& x_values, const vector<double>& y_values, int degree)
{
    int n = time.size();

    // Create matrix X for x-values
    MatrixXd X(n, degree + 1);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j <= degree; ++j) {
            X(i, j) = pow(time[i], j);
        }
    }

    // Create vector y for x-values
    VectorXd y_x(n);
    for (int i = 0; i < n; ++i) {
        y_x(i) = x_values[i];
    }

    // Create vector y for y-values
    VectorXd y_y(n);
    for (int i = 0; i < n; ++i) {
        y_y(i) = y_values[i];
    }

    // Solve for coefficients for x-values
    VectorXd a = (X.transpose() * X).ldlt().solve(X.transpose() * y_x);

    // Solve for coefficients for y-values
    VectorXd b = (X.transpose() * X).ldlt().solve(X.transpose() * y_y);

    // Create strings to store the polynomial equations
    string polynomial_x = "";
    polynomial_x += "return ";
    for (int i = degree; i >= 0; --i) {
        polynomial_x += "(" + to_string(a(i)) + ")";
        if (i > 0) {
            polynomial_x += " * pow(t, " + to_string(i) + ")";
            polynomial_x += " + ";
        }
    }
    polynomial_x += ";";
    // polynomial_x += "}\n";

    string polynomial_y = "";
    polynomial_y += "return ";
    for (int i = degree; i >= 0; --i) {
        polynomial_y += "(" + to_string(b(i)) + ")";
        if (i > 0) {
            polynomial_y += " * pow(t, " + to_string(i) + ")";
            polynomial_y += " + ";
        }
    }
    polynomial_y += ";";

    // // Print the functions as C++ code
    // cout << "// Equations for polynomial regression:" << endl;
    // cout << "float polynomial_x(float t) {" << endl;
    // cout << "    return ";
    // for (int i = degree; i >= 0; --i) {
    //     cout << "(" << a(i) << ")";
    //     if (i > 0) {
    //         cout << " * pow(t, " << i << ")";
    //         cout << " + ";
    //     }
    // }
    // cout << ";" << endl;
    // cout << "}" << endl;

    // // cout << "float polynomial_y(float t) {" << endl;
    // cout << "       return ";
    // for (int i = degree; i >= 0; --i) {
    //     cout << "(" << b(i) << ")";
    //     if (i > 0) {
    //         cout << " * pow(t, " << i << ")";
    //         cout << " + ";
    //     }
    // }
    // cout << ";" << endl;
    // polynomial_y += "}\n";

    // regressionDebugging(time, x_values, y_values, degree, a, b);

    // std::cout << polynomial_x << std::endl << polynomial_y << std::endl;
    // return make_pair(polynomial_x, polynomial_y);
    return make_pair(";", ";");
}












