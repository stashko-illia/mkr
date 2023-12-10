#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>
#include <unistd.h>

#include <chrono>
#include <cstring>
#include <iostream>
#include <matrix.hpp>
#include <thread>
#include <vector>

using namespace std;
class Server {
   private:
    int server_socket;
    vector<thread> threads;

   public:
    Server(int port = 5555) {
        struct sockaddr_in server_adress;
        server_adress.sin_family = AF_INET;
        server_adress.sin_port = htons(port);
        server_adress.sin_addr.s_addr = INADDR_ANY;

        // Створення сокета
        server_socket = socket(AF_INET, SOCK_STREAM, 0);
        // Прив'язування сокета до адреси
        if (bind(server_socket, (struct sockaddr*)&server_adress,
                 sizeof(server_adress)) == -1) {
            std::cerr << "Error binding socket" << '\n';
            close(server_socket);
            return;
        }

        listen(server_socket, 5);
    }
    int kill() {
        if (server_socket != -1) {
            close(server_socket);
            server_socket = -1;
        }
        return 0;
    }
    void run() {
        while (true) {
            std::cout << "Очікую підключення..." << std::endl;
            sockaddr_in newSockAddr;
            socklen_t newSockAddrSize = sizeof(newSockAddr);
            int clientSocket = accept(server_socket, (sockaddr*)&newSockAddr,
                                      &newSockAddrSize);
            std::cout << clientSocket << std::endl;
            if (clientSocket == -1)
                std::cout << "Сталась помилка." << std::endl;
            else
                std::cout << "Нове підключення." << std::endl;

            threads.push_back(thread(&Server::handleClient, this, clientSocket,
                                     threads.size() + 1));
        }
    }

   private:
    vector<string> split(char* str, char delimiter) {
        vector<string> tokens;

        char* token = strtok(str, &delimiter);
        while (token != nullptr) {
            tokens.push_back(token);
            token = std::strtok(nullptr, &delimiter);
        }
        return tokens;
    }
    Matrix processString(char* str) {
        vector<string> arr = split(str, '|');
        Matrix myMatrix = Matrix(atoi(arr[0].c_str()), atoi(arr[1].c_str()));
        myMatrix.stringToMatrix(arr[2]);
        return myMatrix;
    }
    std::string intToString(int number, int width) {
        std::string result = std::to_string(number);
        result = std::string(width - result.length(), '0') + result;
        return result;
    }

    void handleClient(int clientSocket, int index) {
        char length[6];
        // Розміри рядків першої матриці
        if (recv(clientSocket, length, 6, 0) <= 0) {
            close(clientSocket);
            return;
        }
        const int lengthInt1 = atoi(length);
        // Матриця
        char* matrixStr1 = new char[lengthInt1];
        if (recv(clientSocket, matrixStr1, lengthInt1, 0) != lengthInt1) {
            delete[] matrixStr1;
            close(clientSocket);
            return;
        }
        Matrix m1 = processString(matrixStr1);
        delete[] matrixStr1;

        memset(length, 0, sizeof(length));
        // Розміри рядків другої матриці
        if (recv(clientSocket, length, 6, 0) <= 0) {
            close(clientSocket);
            return;
        }
        const int lengthInt2 = atoi(length);
        // Матриця
        char* matrixStr2 = new char[lengthInt2];
        if (recv(clientSocket, matrixStr2, lengthInt2, 0) != lengthInt2) {
            delete[] matrixStr1;
            close(clientSocket);
            return;
        };
        Matrix m2 = processString(matrixStr2);
        delete[] matrixStr2;

        if (m1.getCols() != m2.getRows()) {
            close(clientSocket);
            return;
        }
        const Matrix resultMatrix = m1.multiply(m2);
        const string resultStr = resultMatrix.matrixToString();
        const int resultSize = resultStr.size();
        const string resultSizeString = intToString(resultStr.size(), 6);
        std::cout << "result size: " << resultSize << '\n';

        send(clientSocket, resultSizeString.c_str(), 6, 0);
        send(clientSocket, resultStr.c_str(), resultSize, 0);
        close(clientSocket);
    }
};

#endif