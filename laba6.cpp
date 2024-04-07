#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include "tbb/tbb.h"

using namespace std;

const int lengthMatrix = 500; // Размерность системы

// Функция для вывода матрицы
void printMatrix(double** matrix, double* answerMatrix) {
    for (int i = 0; i < lengthMatrix; i++) {
        for (int j = 0; j < lengthMatrix; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << " = " << answerMatrix[i] << endl;
    }
}

void matrixTransformation(double** matrix, double* answerMatrix) {
    for (int k = 0; k < lengthMatrix - 1; k++) {
        tbb::parallel_for(tbb::blocked_range<int>(k + 1, lengthMatrix),
            [&matrix, &answerMatrix, k](const tbb::blocked_range<int>& r) {
                for (int i = r.begin(); i != r.end(); ++i) {
                    double temp = matrix[i][k] / matrix[k][k];
                    for (int j = k; j < lengthMatrix; j++) {
                        matrix[i][j] -= temp * matrix[k][j];
                    }
                    answerMatrix[i] -= temp * answerMatrix[k];
                }
        });
    }
}

void calculationSolution(double** matrix, double* answerMatrix, double* solution) {
    solution[lengthMatrix - 1] = answerMatrix[lengthMatrix - 1] / matrix[lengthMatrix - 1][lengthMatrix - 1];
    for (int i = lengthMatrix - 2; i >= 0; i--) {
        double sum = answerMatrix[i];
        for (int j = i + 1; j < lengthMatrix; j++) {
            sum -= matrix[i][j] * solution[j];
        }
        solution[i] = sum / matrix[i][i];
    }
}

double** createMatrixSecond(int length) {
    double** matrix = new double* [length];
    for (int i = 0; i < length; i++) {
        matrix[i] = new double[length];
        for (int j = 0; j < length; j++) {
            matrix[i][j] = rand() % 10;
        }
    }
    return matrix;
}

double* createMatrixFirst(int length) {
    double* matrix = new double [length];
    for (int i = 0; i < length; i++) {
        matrix[i] = rand() % 10;
    }
    return matrix;
}

int main() {

    // srand(time(NULL));
    
    double** matrix = createMatrixSecond(lengthMatrix);
    double* answerMatrix = createMatrixFirst(lengthMatrix);
    double* solution = new double [lengthMatrix];

    cout << "Исходная система уравнений:" << endl;
    printMatrix(matrix, answerMatrix);

    auto start = std::chrono::steady_clock::now();

    matrixTransformation(matrix, answerMatrix);

    cout << "\nМатрица после прямого хода:" << endl;
    printMatrix(matrix, answerMatrix);

    calculationSolution(matrix, answerMatrix, solution);

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> solving_time = end - start;
    cout << "Время выполения: " << solving_time.count() << endl;

    cout << "\nРешение СЛАУ:" << endl;
    for (int i = 0; i < lengthMatrix; i++) {
        cout << "x" << i + 1 << " = " << solution[i] << endl;
    }

    return 0;
}
