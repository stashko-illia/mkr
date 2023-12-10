#ifndef MATRIX_H
#define MATRIX_H

#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>
#include <vector>

class Matrix {
   private:
    int n;                               // rows
    int m;                               // cols
    std::vector<std::vector<int>> M;  // matrix
   public:
    std::vector<int>& operator[](int n) { return M[n]; }

    Matrix(int n, int m) : n(n), m(m) {
        if (n < 1 || m < 1) {
            throw std::invalid_argument(
                "Matrix dimensions must be greater than 0");
        }
        M = std::vector<std::vector<int>>(n, std::vector<int>(m, 0.0));
    }
    void fillRandom() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dist(1000.0, 10000.0);

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) M[i][j] = dist(gen);
        }
    }
    void print() const {
        std::cout << std::fixed << std::setprecision(5);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) std::cout << M[i][j] << " ";
            std::cout << '\n';
        }
    }
    std::string matrixToString() const {
        std::ostringstream oss;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) oss << M[i][j] << " ";
            oss << '\n';
        }
        return oss.str();
    }

    void stringToMatrix(const std::string& str) {
        std::istringstream iss(str);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                if (!(iss >> M[i][j]))
                    throw std::invalid_argument(
                        "Error converting string to matrix");
            }
        }
    }
    int getRows() const { return n; }
    int getCols() const { return m; }
    double getItem(int i, int j) const {
        if (i < 0 || i < 0) {
            throw std::invalid_argument(
                "Matrix indeces must be greater or equal to 0");
        }
        return M[i][j];
    }
    Matrix multiply(const Matrix& other) const {
        if (m != other.n) {
            throw std::invalid_argument(
                "Matrix dimensions mismatch for multiplication");
        }

        Matrix result(n, other.m);

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < other.m; ++j) {
                for (int k = 0; k < m; ++k) {
                    result[i][j] += M[i][k] * other.M[k][j];
                }
            }
        }

        return result;
    }
};

#endif