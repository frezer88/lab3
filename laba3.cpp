#include <iostream>

using namespace std;

const int lengthMatrix = 3; // Размерность системы

// Функция для вывода матрицы
void printMatrix(double** matrix, double* answerMatrix) {
    for (int i = 0; i < lengthMatrix; i++) {
        for (int j = 0; j < lengthMatrix; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << " = " << answerMatrix[i] << endl;
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

    double** matrix = createMatrixSecond(lengthMatrix);

    double* answerMatrix = createMatrixFirst(lengthMatrix);

    printMatrix(matrix, answerMatrix);

    return 0;
}
