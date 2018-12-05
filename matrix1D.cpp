//
// Created by matt on 11/13/18.
//
#define INTERVAL_RANDOM_NUMBER 10;

#include <random>
#include "matrix.hpp"
#include <thread>
#include <future>

//copy constructor
Matrix::Matrix(const Matrix &matrix){
    m_rows = matrix.m_rows;
    m_cols = matrix.m_cols;

    m_data = new int*[m_rows*m_cols];
//    std::cout<<"copy contructor"<<std::endl;

    for (int i = 0; i < m_rows; ++i){
        for (int j = 0; j < m_cols; ++j) {
            m_data[i*m_rows + j] = matrix.m_data[i*m_rows + j];
        }
    }
}

Matrix::Matrix(const unsigned int rows, const unsigned int cols){
    m_rows = rows;
    m_cols = cols;

    m_data = new MATRIX_VALUE_TYPE*[rows];


    for (int i = 0; i < rows; ++i){
        m_data[i] = new MATRIX_VALUE_TYPE[cols];
    }
}

Matrix::Matrix(unsigned int size){
    m_rows = size;
    m_cols = size;

    m_data = new MATRIX_VALUE_TYPE*[size];


    for (int i = 0; i < size; ++i){
        m_data[i] = new MATRIX_VALUE_TYPE[size];
    }
}

Matrix::Matrix(unsigned int size, MATRIX_VALUE_TYPE array []){
    m_rows = size;
    m_cols = size;

    m_data = new MATRIX_VALUE_TYPE*[size];

    for (int i = 0; i < size; ++i){
        m_data[i] = new MATRIX_VALUE_TYPE[size];
        for (int j = 0; j < size; ++j) {
            m_data[i][j] = array[j + i*size];
        }
    }
}

Matrix::Matrix(const Matrix &a,const Matrix &b,const Matrix &c,const Matrix &d) {
    if(a.m_rows == a.m_cols && a.m_cols == b.m_cols && a.m_cols == c.m_cols && a.m_cols == d.m_cols){
        unsigned int size = a.m_rows * 2;
        m_rows = size;
        m_cols = size;

        m_data = new int*[size];

        for (int i = 0; i < size; ++i){
            m_data[i] = new int[size];
            for (int j = 0; j < size; ++j) {
                if(i<m_rows/2 && j<m_cols/2)
                    m_data[i][j]=a.m_data[i][j];
                else if(i<m_rows/2 && j>=m_cols/2)
                    m_data[i][j]=b.m_data[i][j-m_cols/2];
                else if(i>=m_rows/2 && j<m_cols/2)
                    m_data[i][j]=c.m_data[i-m_rows/2][j];
                else if(i>=m_rows/2 && j>=m_cols/2)
                    m_data[i][j]=c.m_data[i-m_rows/2][j-m_cols/2];
            }
        }
    }
}

Matrix::~Matrix(){
    for (int i = 0; i < m_rows; ++i) {
        if(m_data[i]!= nullptr)
            delete[] m_data[i];
    }
    if(m_data!= nullptr)
        delete[] m_data;
}

//assignment operator
Matrix& Matrix::operator= (const Matrix &matrix){
    if(this != &matrix){
        unsigned int rows = matrix.m_rows;
        unsigned int cols = matrix.m_cols;

        MATRIX_VALUE_TYPE** data = new MATRIX_VALUE_TYPE*[rows];
        for (int i = 0; i < rows; ++i){
            data[i] = new MATRIX_VALUE_TYPE[cols];
            std::copy(matrix.m_data[i],matrix.m_data[i]+matrix.m_cols,data[i]);
        }

        for (int i = 0; i < m_rows; ++i) {
            if(m_data[i]!= nullptr)
                delete[] m_data[i];
        }
        if(m_data!= nullptr)
            delete[] m_data;

        m_rows = rows;
        m_cols = cols;
        m_data = data;

    }
    return *this;
}

bool Matrix::operator==(const Matrix &matrix){
    if(m_rows!=matrix.m_rows or m_cols!=matrix.m_cols)
        return false;
    for (int i = 0; i < m_rows; ++i) {
        for (int j = 0; j < m_cols; ++j) {
            if(this->m_data[i][j] != matrix.m_data[i][j])
                return false;
        }
    }
    return true;
}

Matrix Matrix::operator+(const Matrix &matrix) {
    if (m_rows!=matrix.m_rows or m_cols!=matrix.m_cols)
    {
        std::cout<<"Can't add matrices with different width and height"<<std::endl;
        return *this;
    }

    Matrix matrix1(m_rows);

    for (int i = 0; i < m_rows; ++i) {
        for (int j = 0; j < m_cols; ++j) {
            matrix1.m_data[i][j] = m_data[i][j] + matrix.m_data[i][j];
        }
    }
    return matrix1;
}

Matrix Matrix::operator+=(const Matrix &matrix) {
    if (m_rows!=matrix.m_rows or m_cols!=matrix.m_cols)
    {
        std::cout<<"Can't add matrices with different width and height"<<std::endl;
        return *this;
    }
    for (int i = 0; i < m_rows; ++i) {
        for (int j = 0; j < m_cols; ++j) {
            m_data[i][j] += matrix.m_data[i][j];
        }
    }
    return *this;
}

Matrix Matrix::operator*(const Matrix &matrix){
    switch (m_strategy){
        case 0://naive
            return naiveMultiplication(*this,matrix);
        case 1://divideAndConquer
            return divideAndConquer(*this,matrix);
        case 2://divideAndConquerThreads
            return divideAndConquerThreads(*this,matrix,true);
        default:
            return naiveMultiplication(*this,matrix);
    }
}

void Matrix::fillWithRandomData(){
    for (int i = 0; i < m_rows; ++i){
        for (int j = 0; j < m_cols; ++j) {
            m_data[i][j] = getRandomInt();
        }
    }
}

void Matrix::divideMatrixIntoQuarters(std::vector<Matrix>& matrices) const{
    if(m_cols == m_rows && m_cols%2 == 0){
        Matrix matrix1(m_rows/2);
        Matrix matrix2(m_rows/2);
        Matrix matrix3(m_rows/2);
        Matrix matrix4(m_rows/2);

        for (int i = 0; i < m_rows; ++i) {
            for (int j = 0; j < m_cols; ++j) {
                if(i<m_rows/2 && j<m_cols/2){
                    matrix1.m_data[i][j] = m_data[i][j];
                }
                else if(i<m_rows/2 && j>=m_cols/2){
                    matrix2.m_data[i][j-m_cols/2] = m_data[i][j];
                }
                else if(i>=m_rows/2 && j<m_cols/2){
                    matrix3.m_data[i-m_rows/2][j] = m_data[i][j];

                }
                else if(i>=m_rows/2 && j>=m_cols/2){
                    matrix4.m_data[i-m_rows/2][j-m_cols/2] = m_data[i][j];

                }
            }
        }
        matrices.emplace_back(matrix1);
        matrices.emplace_back(matrix2);
        matrices.emplace_back(matrix3);
        matrices.emplace_back(matrix4);
    }
    std::vector<Matrix> empty;
}

Matrix divideAndConquer(const Matrix &matrix1, const Matrix &matrix2){
    if(matrix1.m_rows==2){
        Matrix C(matrix1.m_rows);
        C.m_data[0][0]=matrix1.m_data[0][0]*matrix2.m_data[0][0] + matrix1.m_data[0][1]*matrix2.m_data[1][0];
        C.m_data[0][1]=matrix1.m_data[0][0]*matrix2.m_data[0][1] + matrix1.m_data[0][1]*matrix2.m_data[1][1];
        C.m_data[1][0]=matrix1.m_data[1][0]*matrix2.m_data[0][0] + matrix1.m_data[1][1]*matrix2.m_data[1][0];
        C.m_data[1][1]=matrix1.m_data[1][0]*matrix2.m_data[0][1] + matrix1.m_data[1][1]*matrix2.m_data[1][1];
        return C;
    }
    else{
        std::vector<Matrix> matricesA;
        matrix1.divideMatrixIntoQuarters(matricesA);
        std::vector<Matrix> matricesB;
        matrix2.divideMatrixIntoQuarters(matricesB);

        Matrix c11 = divideAndConquer(matricesA[0],matricesB[0])+divideAndConquer(matricesA[1],matricesB[2]);
        Matrix c12 = divideAndConquer(matricesA[0],matricesB[1])+divideAndConquer(matricesA[1],matricesB[3]);
        Matrix c21 = divideAndConquer(matricesA[2],matricesB[0])+divideAndConquer(matricesA[3],matricesB[2]);
        Matrix c22 = divideAndConquer(matricesA[2],matricesB[1])+divideAndConquer(matricesA[3],matricesB[3]);


        Matrix C(c11,c12,c21,c22);
        return C;
    }
}

Matrix divideAndConquerThreads(const Matrix &matrix1, const Matrix &matrix2, const bool &parallel){
    if(matrix1.m_rows==2){
        Matrix C(matrix1.m_rows);
        C.m_data[0][0]=matrix1.m_data[0][0]*matrix2.m_data[0][0] + matrix1.m_data[0][1]*matrix2.m_data[1][0];
        C.m_data[0][1]=matrix1.m_data[0][0]*matrix2.m_data[0][1] + matrix1.m_data[0][1]*matrix2.m_data[1][1];
        C.m_data[1][0]=matrix1.m_data[1][0]*matrix2.m_data[0][0] + matrix1.m_data[1][1]*matrix2.m_data[1][0];
        C.m_data[1][1]=matrix1.m_data[1][0]*matrix2.m_data[0][1] + matrix1.m_data[1][1]*matrix2.m_data[1][1];
        return C;
    }
    else{
        if(parallel){
            std::vector<Matrix> matricesA;
            matrix1.divideMatrixIntoQuarters(matricesA);
            std::vector<Matrix> matricesB;
            matrix2.divideMatrixIntoQuarters(matricesB);

            auto t1 = std::async(std::launch::async, divideAndConquer,matricesA[0],matricesB[0]);
            auto t2 = std::async(std::launch::async, divideAndConquer,matricesA[1],matricesB[2]);
            auto t3 = std::async(std::launch::async, divideAndConquer,matricesA[0],matricesB[1]);
            auto t4 = std::async(std::launch::async, divideAndConquer,matricesA[1],matricesB[3]);
            auto t5 = std::async(std::launch::async, divideAndConquer,matricesA[2],matricesB[0]);
            auto t6 = std::async(std::launch::async, divideAndConquer,matricesA[3],matricesB[2]);
            auto t7 = std::async(std::launch::async, divideAndConquer,matricesA[2],matricesB[1]);
            auto t8 = std::async(std::launch::async, divideAndConquer,matricesA[3],matricesB[3]);

            Matrix c11 = t1.get() + t2.get();
            Matrix c12 = t3.get() + t4.get();
            Matrix c21 = t5.get() + t6.get();
            Matrix c22 = t7.get() + t8.get();

            Matrix C(c11,c12,c21,c22);
            return C;

        }

        else{
            std::vector<Matrix> matricesA;
            matrix1.divideMatrixIntoQuarters(matricesA);
            std::vector<Matrix> matricesB;
            matrix2.divideMatrixIntoQuarters(matricesB);

            Matrix c11 = divideAndConquer(matricesA[0],matricesB[0])+divideAndConquer(matricesA[1],matricesB[2]);
            Matrix c12 = divideAndConquer(matricesA[0],matricesB[1])+divideAndConquer(matricesA[1],matricesB[3]);
            Matrix c21 = divideAndConquer(matricesA[2],matricesB[0])+divideAndConquer(matricesA[3],matricesB[2]);
            Matrix c22 = divideAndConquer(matricesA[2],matricesB[1])+divideAndConquer(matricesA[3],matricesB[3]);

            Matrix C(c11,c12,c21,c22);
            return C;

        }
    }
}

Matrix naiveMultiplication(const Matrix &matrix1, const Matrix &matrix2){
//    if (matrix1.m_cols=matrix2.m_rows or matrix1.m_cols!=matrix2.m_cols)//TODO podminky pro ruzne velikosti matic
//        //exception
//    {}
    Matrix newMatrix(matrix1.m_cols,matrix2.m_rows);

    for (int i = 0; i < matrix1.m_rows; ++i) {
        for (int j = 0; j < matrix2.m_cols; ++j) {
            int sum = 0;
            for (int k = 0; k < matrix1.m_rows; ++k) {
                sum += matrix1.m_data[i][k] * matrix2.m_data[k][j];
            }
            newMatrix.m_data[i][j] = sum;
        }
    }
    return newMatrix;
}

void setData(Matrix *matrix, int row, int col, MATRIX_VALUE_TYPE value){
    matrix->m_data[row][col] = value;
}

void Matrix::print() {
    printf("\n");
    for (int i = 0; i < m_rows; ++i) {
        for (int j = 0; j < m_cols; ++j) {
            printf("%d\t",m_data[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void Matrix::setStrategy(const unsigned int &strategy){
    if(strategy>2) m_strategy = 0;
    else    m_strategy = strategy;
}

unsigned int Matrix::getRows() const{
    return m_rows;
}
unsigned int Matrix::getCols() const{
    return m_cols;
}
MATRIX_VALUE_TYPE Matrix::getData(int row, int col) const{
    return m_data[row][col];

}

int Matrix::getRandomInt() {
    double interval = INTERVAL_RANDOM_NUMBER;
    static std::mt19937 mt{ std::random_device{}() };
    static std::uniform_real_distribution<> dist(-interval, interval);
    return (int)dist(mt);
}