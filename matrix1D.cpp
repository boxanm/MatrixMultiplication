//
// Created by matt on 11/13/18.
//
#define INTERVAL_RANDOM_NUMBER 10;

#include <random>
#include <thread>
#include <future>
#include "matrix1D.hpp"

//copy constructor
Matrix1D::Matrix1D(const Matrix1D &matrix){
    m_rows = matrix.m_rows;
    m_cols = matrix.m_cols;

    m_data = new int[m_rows*m_cols];

    for (int i = 0; i < m_rows; ++i){
        for (int j = 0; j < m_cols; ++j) {
            m_data[i*m_rows + j] = matrix.m_data[i*m_rows + j];
        }
    }
}

Matrix1D::Matrix1D(const unsigned int rows, const unsigned int cols){
    m_rows = rows;
    m_cols = cols;

    m_data = new MATRIX_VALUE_TYPE[rows*cols];
}

Matrix1D::Matrix1D(unsigned int size){
    m_rows = size;
    m_cols = size;

    m_data = new MATRIX_VALUE_TYPE[size*size];
}

Matrix1D::Matrix1D(unsigned int size, MATRIX_VALUE_TYPE array []){
    m_rows = size;
    m_cols = size;

    m_data = new MATRIX_VALUE_TYPE[size*size];

    for (int i = 0; i < size; ++i){
        for (int j = 0; j < size; ++j) {
            m_data[i*m_rows + j] = array[i*size + j];
        }
    }
}

Matrix1D::Matrix1D(const Matrix1D &a,const Matrix1D &b,const Matrix1D &c,const Matrix1D &d) {
    if(a.m_rows == a.m_cols && a.m_cols == b.m_cols && a.m_cols == c.m_cols && a.m_cols == d.m_cols){
        unsigned int size = a.m_rows * 2;
        m_rows = size;
        m_cols = size;

        m_data = new int[size*size];

        for (int i = 0; i < size; ++i){
            for (int j = 0; j < size; ++j) {
                if(i<m_rows/2 && j<m_cols/2)
                    m_data[i*m_rows + j]=a.m_data[i*m_rows/2 + j];
                else if(i<m_rows/2 && j>=m_cols/2)
                    m_data[i*m_rows + j]=b.m_data[i*m_rows/2 + j-m_cols/2];
                else if(i>=m_rows/2 && j<m_cols/2)
                    m_data[i*m_rows + j]=c.m_data[(i-m_rows/2)*m_rows/2 + j];
                else if(i>=m_rows/2 && j>=m_cols/2)
                    m_data[i*m_rows + j]=c.m_data[(i-m_rows/2)*m_rows/2 + j-m_cols/2];
            }
        }
    }
}

Matrix1D::~Matrix1D(){
    if(m_data!= nullptr)
        delete[] m_data;
}

//assignment operator
Matrix1D& Matrix1D::operator= (const Matrix1D &Matrix1D){
    if(this != &Matrix1D){
        unsigned int rows = Matrix1D.m_rows;
        unsigned int cols = Matrix1D.m_cols;

        MATRIX_VALUE_TYPE* data = new MATRIX_VALUE_TYPE[rows * rows];
        for (int i = 0; i < rows; ++i){
            std::copy(Matrix1D.m_data,Matrix1D.m_data+Matrix1D.m_cols*m_rows,data);
        }
        if(m_data!= nullptr)
            delete[] m_data;

        m_rows = rows;
        m_cols = cols;
        m_data = data;

    }
    return *this;
}

bool Matrix1D::operator==(const Matrix1D &Matrix1D){
    if(m_rows!=Matrix1D.m_rows or m_cols!=Matrix1D.m_cols)
        return false;
    for (int i = 0; i < m_rows; ++i) {
        for (int j = 0; j < m_cols; ++j) {
            if(this->m_data[i*m_rows+j] != Matrix1D.m_data[i*m_rows+j])
                return false;
        }
    }
    return true;
}

Matrix1D Matrix1D::operator+(const Matrix1D &matrix) {
    if (m_rows!=matrix.m_rows or m_cols!=matrix.m_cols)
    {
        std::cout<<"Can't add matrices with different width and height"<<std::endl;
        return *this;
    }

    Matrix1D returnMatrix(m_rows);

    for (int i = 0; i < m_rows; ++i) {
        for (int j = 0; j < m_cols; ++j) {
            returnMatrix.m_data[i*m_rows+j] = m_data[i*m_rows+j] + matrix.m_data[i*m_rows+j];
        }
    }
    return returnMatrix;
}

Matrix1D Matrix1D::operator+=(const Matrix1D &Matrix1D) {
    if (m_rows!=Matrix1D.m_rows or m_cols!=Matrix1D.m_cols)
    {
        std::cout<<"Can't add matrices with different width and height"<<std::endl;
        return *this;
    }
    for (int i = 0; i < m_rows; ++i) {
        for (int j = 0; j < m_cols; ++j) {
            m_data[i*m_rows+j] += Matrix1D.m_data[i*m_rows+j];
        }
    }
    return *this;
}

Matrix1D Matrix1D::operator*(const Matrix1D &Matrix1D){
    switch (m_strategy){
        case 0://naive
            return naiveMultiplication(*this,Matrix1D);
        case 1://divideAndConquer
            return divideAndConquer(*this,Matrix1D);
        case 2://divideAndConquerThreads
            return divideAndConquerThreads(*this,Matrix1D,true);
        default:
            return naiveMultiplication(*this,Matrix1D);
    }
}

void Matrix1D::fillWithRandomData(){
    for (int i = 0; i < m_rows; ++i){
        for (int j = 0; j < m_cols; ++j) {
            m_data[i*m_rows+j] = getRandomInt();
        }
    }
}

void Matrix1D::divideMatrixIntoQuarters(std::vector<Matrix1D>& matrices) const{
    if(m_cols == m_rows && m_cols%2 == 0){
        Matrix1D matrix1(m_rows/2);
        Matrix1D matrix2(m_rows/2);
        Matrix1D matrix3(m_rows/2);
        Matrix1D matrix4(m_rows/2);

        for (int i = 0; i < m_rows; ++i) {
            for (int j = 0; j < m_cols; ++j) {
                if(i<m_rows/2 && j<m_cols/2){
                    matrix1.m_data[i*m_rows/2+j] = m_data[i*m_rows+j];
                }
                else if(i<m_rows/2 && j>=m_cols/2){
                    matrix2.m_data[i*m_rows/2 + j-m_cols/2] = m_data[i*m_rows+j];
                }
                else if(i>=m_rows/2 && j<m_cols/2){
                    matrix3.m_data[(i-m_rows/2)*m_rows/2 + j] = m_data[i*m_rows+j];

                }
                else if(i>=m_rows/2 && j>=m_cols/2){
                    matrix4.m_data[(i-m_rows/2)*m_rows/2 + j-m_cols/2] = m_data[i*m_rows+j];

                }
            }
        }
        matrices.emplace_back(matrix1);
        matrices.emplace_back(matrix2);
        matrices.emplace_back(matrix3);
        matrices.emplace_back(matrix4);
    }
    std::vector<Matrix1D> empty;
}

Matrix1D divideAndConquer(const Matrix1D &matrix1, const Matrix1D &matrix2){
    if(matrix1.m_rows==2){
        Matrix1D C(matrix1.m_rows);
        C.m_data[0]=matrix1.m_data[0]*matrix2.m_data[0] + matrix1.m_data[1]*matrix2.m_data[2];
        C.m_data[1]=matrix1.m_data[0]*matrix2.m_data[1] + matrix1.m_data[1]*matrix2.m_data[3];
        C.m_data[2]=matrix1.m_data[2]*matrix2.m_data[0] + matrix1.m_data[3]*matrix2.m_data[2];
        C.m_data[3]=matrix1.m_data[2]*matrix2.m_data[1] + matrix1.m_data[3]*matrix2.m_data[3];
        return C;
    }
    else{
        std::vector<Matrix1D> matricesA;
        matrix1.divideMatrixIntoQuarters(matricesA);
        std::vector<Matrix1D> matricesB;
        matrix2.divideMatrixIntoQuarters(matricesB);

        Matrix1D c11 = divideAndConquer(matricesA[0],matricesB[0])+divideAndConquer(matricesA[1],matricesB[2]);
        Matrix1D c12 = divideAndConquer(matricesA[0],matricesB[1])+divideAndConquer(matricesA[1],matricesB[3]);
        Matrix1D c21 = divideAndConquer(matricesA[2],matricesB[0])+divideAndConquer(matricesA[3],matricesB[2]);
        Matrix1D c22 = divideAndConquer(matricesA[2],matricesB[1])+divideAndConquer(matricesA[3],matricesB[3]);


        Matrix1D C(c11,c12,c21,c22);
        return C;
    }
}

Matrix1D divideAndConquerThreads(const Matrix1D &matrix1, const Matrix1D &matrix2, const bool &parallel){
    if(matrix1.m_rows==2){
        Matrix1D C(matrix1.m_rows);
        C.m_data[0]=matrix1.m_data[0]*matrix2.m_data[0] + matrix1.m_data[1]*matrix2.m_data[2];
        C.m_data[1]=matrix1.m_data[0]*matrix2.m_data[1] + matrix1.m_data[1]*matrix2.m_data[3];
        C.m_data[2]=matrix1.m_data[2]*matrix2.m_data[0] + matrix1.m_data[3]*matrix2.m_data[2];
        C.m_data[3]=matrix1.m_data[2]*matrix2.m_data[1] + matrix1.m_data[3]*matrix2.m_data[3];
        return C;
    }
    else{
        if(parallel && std::thread::hardware_concurrency()!=1){
            std::vector<Matrix1D> matricesA;
            matrix1.divideMatrixIntoQuarters(matricesA);
            std::vector<Matrix1D> matricesB;
            matrix2.divideMatrixIntoQuarters(matricesB);
            switch (std::thread::hardware_concurrency()){
                case 2:{
                    auto t1 = std::async(std::launch::async, divideAndConquer,matricesA[0],matricesB[0]);
                    auto t2 = std::async(std::launch::async, divideAndConquer,matricesA[1],matricesB[2]);
                    Matrix1D c11 = t1.get() + t2.get();
                    t1 = std::async(std::launch::async, divideAndConquer,matricesA[0],matricesB[1]);
                    t2 = std::async(std::launch::async, divideAndConquer,matricesA[1],matricesB[3]);
                    Matrix1D c12 = t1.get() + t2.get();
                    t1 = std::async(std::launch::async, divideAndConquer,matricesA[2],matricesB[0]);
                    t2 = std::async(std::launch::async, divideAndConquer,matricesA[3],matricesB[2]);
                    Matrix1D c21 = t1.get() + t2.get();
                    t1 = std::async(std::launch::async, divideAndConquer,matricesA[2],matricesB[1]);
                    t2 = std::async(std::launch::async, divideAndConquer,matricesA[3],matricesB[3]);
                    Matrix1D c22 = t1.get() + t2.get();

                    Matrix1D C(c11,c12,c21,c22);
                    return C;

                }
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:{
                    auto t1 = std::async(std::launch::async, divideAndConquer,matricesA[0],matricesB[0]);
                    auto t2 = std::async(std::launch::async, divideAndConquer,matricesA[1],matricesB[2]);
                    auto t3 = std::async(std::launch::async, divideAndConquer,matricesA[0],matricesB[1]);
                    auto t4 = std::async(std::launch::async, divideAndConquer,matricesA[1],matricesB[3]);
                    Matrix1D c11 = t1.get() + t2.get();
                    Matrix1D c12 = t3.get() + t4.get();
                    t1 = std::async(std::launch::async, divideAndConquer,matricesA[2],matricesB[0]);
                    t2 = std::async(std::launch::async, divideAndConquer,matricesA[3],matricesB[2]);
                    t3 = std::async(std::launch::async, divideAndConquer,matricesA[2],matricesB[1]);
                    t4 = std::async(std::launch::async, divideAndConquer,matricesA[3],matricesB[3]);
                    Matrix1D c21 = t1.get() + t2.get();
                    Matrix1D c22 = t3.get() + t4.get();

                    Matrix1D C(c11,c12,c21,c22);
                    return C;

                }
                default:{
                    auto t1 = std::async(std::launch::async, divideAndConquer,matricesA[0],matricesB[0]);
                    auto t2 = std::async(std::launch::async, divideAndConquer,matricesA[1],matricesB[2]);
                    auto t3 = std::async(std::launch::async, divideAndConquer,matricesA[0],matricesB[1]);
                    auto t4 = std::async(std::launch::async, divideAndConquer,matricesA[1],matricesB[3]);
                    auto t5 = std::async(std::launch::async, divideAndConquer,matricesA[2],matricesB[0]);
                    auto t6 = std::async(std::launch::async, divideAndConquer,matricesA[3],matricesB[2]);
                    auto t7 = std::async(std::launch::async, divideAndConquer,matricesA[2],matricesB[1]);
                    auto t8 = std::async(std::launch::async, divideAndConquer,matricesA[3],matricesB[3]);
                    Matrix1D c11 = t1.get() + t2.get();
                    Matrix1D c12 = t3.get() + t4.get();
                    Matrix1D c21 = t5.get() + t6.get();
                    Matrix1D c22 = t7.get() + t8.get();

                    Matrix1D C(c11,c12,c21,c22);
                    return C;
                }
            }
        }
        else{
            return divideAndConquer(matrix1,matrix2);
        }
    }
}

Matrix1D naiveMultiplication(const Matrix1D &matrix1, const Matrix1D &matrix2){
    Matrix1D newMatrix1D(matrix1.m_cols,matrix2.m_rows);

    for (int i = 0; i < matrix1.m_rows; ++i) {
        for (int j = 0; j < matrix2.m_cols; ++j) {
            int sum = 0;
            for (int k = 0; k < matrix1.m_rows; ++k) {
                sum += matrix1.m_data[i*matrix1.m_rows + k] * matrix2.m_data[k * matrix2.m_rows + j];
            }
            newMatrix1D.m_data[i*matrix1.m_rows + j] = sum;
        }
    }
    return newMatrix1D;
}

void setData(Matrix1D *Matrix1D, int row, int col, MATRIX_VALUE_TYPE value){
    Matrix1D->m_data[row*Matrix1D->m_rows + col] = value;
}

void Matrix1D::print() {
    printf("\n");
    for (int i = 0; i < m_rows; ++i) {
        for (int j = 0; j < m_cols; ++j) {
            printf("%d\t",m_data[i*m_rows + j]);
        }
        printf("\n");
    }
    printf("\n");
}

void Matrix1D::setStrategy(const unsigned int &strategy){
    if(strategy>2) m_strategy = 0;
    else    m_strategy = strategy;
}

unsigned int Matrix1D::getRows() const{
    return m_rows;
}
unsigned int Matrix1D::getCols() const{
    return m_cols;
}
MATRIX_VALUE_TYPE Matrix1D::getData(int row, int col) const{
    return m_data[row*m_rows + col];

}

int Matrix1D::getRandomInt() {
    double interval = INTERVAL_RANDOM_NUMBER;
    static std::mt19937 mt{ std::random_device{}() };
    static std::uniform_real_distribution<> dist(-interval, interval);
    return (int)dist(mt);
}