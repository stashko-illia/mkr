#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <matrix.hpp>
#include <string>

using namespace std;

std::string intToString(int number, int width) {

    std::string result = std::to_string(number);
    result = std::string(width - result.length(), '0') + result;
    return result;
}

int main() {
    Matrix m1 = Matrix(15, 15);
    Matrix m2 = Matrix(15, 15);
    m1.fillRandom();
    m2.fillRandom();

    
    std::cout<<"first: \n";
    m1.print();
    std::cout<<"second: \n";
    m2.print();

    // Створюємо сокет
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;                      // TCP
    server_address.sin_port = htons(5555);                    // PORT
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");  // IP

    // Створюємо сокет клієнта
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Помилка при створені сокета.");
        return 1;
    }
    // підключаємось до сервера
    int connectRes = connect(client_socket, (struct sockaddr*)&server_address,
                             sizeof(server_address));
    if (connectRes == 0)
        std::cout << "Успішне підключення до сервера.\n";
    else {
        perror("Помилка при підключенні");
        close(client_socket);
    }
    
    string matrixStr1 = 
        to_string(m1.getRows()) + '|' +
        to_string(m1.getCols()) + '|' + '\n' + 
        m1.matrixToString();

    string matrixStr2 = 
        to_string(m2.getRows()) + '|' +
        to_string(m2.getCols()) + '|' + '\n' + 
        m2.matrixToString();

    string matrixSize1 = intToString(matrixStr1.size(), 6);
    string matrixSize2 = intToString(matrixStr2.size(), 6);

    ssize_t bytes_sent;
    //Відправлення довжини
    bytes_sent = send(client_socket, matrixSize1.c_str(), matrixSize1.length(), 0);
    if (bytes_sent == -1) perror("Помилка під час відправлення даних");
    else std::cout << "Відправлено довжину рядка першої матриці.\n";
    // //Відправлення першої матриці
    bytes_sent = send(client_socket, matrixStr1.c_str(), matrixStr1.length(), 0);
    if (bytes_sent == -1) perror("Помилка під час відправлення даних");
    else std::cout << "Перша матриця успішно відправлена.\n";
    //Відправлення довжини
    bytes_sent = send(client_socket, matrixSize2.c_str(), matrixSize2.length(), 0);
    if (bytes_sent == -1) perror("Помилка під час відправлення даних");
    else std::cout << "Відправлено довжину рядка другої матриці.\n";
    // //Відправлення другої матриці
    bytes_sent = send(client_socket, matrixStr2.c_str(), matrixStr2.length(), 0);
    if (bytes_sent == -1) perror("Помилка під час відправлення даних");
    else std::cout << "Друга матриця успішно відправлена.\n";

    char length[6];
    recv(client_socket, length, 6,0);
    std::cout<<length<<'\n';

    int lengthInt = atoi(length);
    char* resultMatrixStr = new char[lengthInt];

    recv(client_socket, resultMatrixStr, lengthInt,0);
    std::cout<<resultMatrixStr<<'\n';

    Matrix resultMatrix = Matrix(m1.getCols(), m2.getRows());
    resultMatrix.stringToMatrix(resultMatrixStr);
    cout<<"result: \n";
    resultMatrix.print();
    close(client_socket);
    return 0;
}