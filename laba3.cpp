#include <iostream>
#include "/opt/homebrew/Cellar/libomp/17.0.6/include/omp.h"

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

void matrixTransformation(double** matrix, double* answerMatrix) {
    for (int k = 0; k < lengthMatrix - 1; k++) {
        #pragma omp parallel for shared(matrix, answerMatrix)//распараллелить цикл, используя общие данные matrix и answerMatrix, которые могут быть доступны для чтения и записи из всех потоков, участвующих в этой параллельной области
        for (int i = k + 1; i < lengthMatrix; i++) {
            double temp = matrix[i][k] / matrix[k][k];
            for (int j = k; j < lengthMatrix; j++) {
                matrix[i][j] -= temp * matrix[k][j];
            }
            answerMatrix[i] -= temp * answerMatrix[k];
        }
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

    double** matrix = createMatrixSecond(lengthMatrix);

    double* answerMatrix = createMatrixFirst(lengthMatrix);

    double* solution = new double [3];

    cout << "Исходная система уравнений:" << endl;
    printMatrix(matrix, answerMatrix);

    matrixTransformation(matrix, answerMatrix);

    cout << "\nМатрица после прямого хода:" << endl;
    printMatrix(matrix, answerMatrix);

    calculationSolution(matrix, answerMatrix, solution);

    cout << "\nРешение СЛАУ:" << endl;
    for (int i = 0; i < lengthMatrix; i++) {
        cout << "x" << i + 1 << " = " << solution[i] << endl;
    }

    return 0;
}
