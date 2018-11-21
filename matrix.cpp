//
// Created by matt on 11/13/18.
//
#define INTERVAL_RANDOM_NUMBER 10;

#include <random>
#include "matrix.hpp"


Matrix::Matrix(const unsigned int rows, const unsigned int cols){
    m_rows = rows;
    m_cols = cols;

    m_data = new int*[rows];


    for (int i = 0; i < rows; ++i){
        m_data[i] = new int[cols];
        for (int j = 0; j < cols; ++j) {
            m_data[i][j] = getRandomInt();
        }
    }
}

Matrix::Matrix(unsigned int size){
    m_rows = size;
    m_cols = size;

    m_data = new int*[size];


    for (int i = 0; i < size; ++i){
        m_data[i] = new int[size];
        for (int j = 0; j < size; ++j) {
            m_data[i][j] = getRandomInt();
        }
    }
}

Matrix::Matrix(unsigned int size, int array []){
    m_rows = size;
    m_cols = size;

    m_data = new int*[size];

    for (int i = 0; i < size; ++i){
        m_data[i] = new int[size];
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
                    m_data[i][j] = a.m_data[i][j];
                else if(i<m_rows/2 && j>=m_cols/2)
                    m_data[i][j] = b.m_data[i][j-m_cols/2];
                else if(i>=m_rows/2 && j<m_cols/2)
                    m_data[i][j] = c.m_data[i-m_rows/2][j];
                else if(i>=m_rows/2 && j>=m_cols/2)
                    m_data[i][j] = d.m_data[i-m_rows/2][j-m_cols/2];
            }
        }
    }
}

Matrix::~Matrix(){
    for (int i = 0; i < m_rows; ++i) {
        delete[] m_data[i];
    }
    delete[] m_data;
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

Matrix Matrix::operator+(const Matrix matrix) {
    if (m_rows!=matrix.m_rows or m_cols!=matrix.m_cols)
        //exception
    {}

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
        //exception
    {}
    for (int i = 0; i < m_rows; ++i) {
        for (int j = 0; j < m_cols; ++j) {
            m_data[i][j] += matrix.m_data[i][j];
        }
    }
    return *this;
}

Matrix Matrix::operator*(const Matrix &matrix){//TODO vyber strategie, divide and conquer, vlakna
    if (m_rows!=matrix.m_rows or m_cols!=matrix.m_cols)
        //exception
    {}
    this->print();
    Matrix newMatrix(m_rows);

    for (int i = 0; i < m_rows; ++i) {
        for (int j = 0; j < m_cols; ++j) {
            int sum = 0;
            for (int k = 0; k < m_rows; ++k) {
                sum += m_data[i][k] * matrix.m_data[k][j];
            }
            newMatrix.m_data[i][j] = sum;
        }
    }
    return newMatrix;
}

std::vector<Matrix> Matrix::divideMatrixIntoQuarters(std::vector<Matrix> matrices) const{
    if(m_cols == m_rows && m_cols%2 == 0){
        int array [4][m_rows];

        for (int i = 0; i < m_rows; ++i) {
            for (int j = 0; j < m_cols; ++j) {
                if(i<m_rows/2 && j<m_cols/2){
//                    std::cout<<"a "<<i*m_rows/2 + j<<std::endl;
                    array[0][i*m_rows/2 + j] = m_data[i][j];
                }
                else if(i<m_rows/2 && j>=m_cols/2){
//                    std::cout<<"b "<<i*m_rows/2 + (j-m_cols/2)<<std::endl;
                    array[1][i*m_rows/2 + (j-m_cols/2)] = m_data[i][j];
                }
                else if(i>=m_rows/2 && j<m_cols/2){
//                    std::cout<<"c "<<(i-m_rows/2)*m_rows/2 + j<<std::endl;
                    array[2][(i-m_rows/2)*m_rows/2 + j] = m_data[i][j];

                }
                else if(i>=m_rows/2 && j>=m_cols/2){
//                    std::cout<<"d "<<(i-m_rows/2)*m_rows/2 + (j-m_cols/2)<<std::endl;
                    array[3][(i-m_rows/2)*m_rows/2 + (j-m_cols/2)] = m_data[i][j];

                }
            }
        }

        Matrix a(m_cols/2,array[0]);
        Matrix b(m_cols/2,array[1]);
        Matrix c(m_cols/2,array[2]);
        Matrix d(m_cols/2,array[3]);

//        std::vector<Matrix> matrices;
        matrices.reserve(4);
        matrices.push_back(a);
        matrices.push_back(b);
        matrices.push_back(c);
        matrices.push_back(d);
        return matrices;
    }
    std::vector<Matrix> empty;
    return empty;
}

Matrix Matrix::divideAndConquer(const Matrix &matrix1, const Matrix &matrix2){
    if(matrix1.m_rows==2){
        return 1;
    }
    else{
        std::vector<Matrix> matricesA;
//        matricesA.reserve(4);
        matricesA = matrix1.divideMatrixIntoQuarters(matricesA);
        std::vector<Matrix> matricesB;
//        matricesB.reserve(4);
        matricesB = matrix2.divideMatrixIntoQuarters(matricesB);

        Matrix c11 = divideAndConquer(matricesA[0],matricesB[0])+divideAndConquer(matricesA[1],matricesB[2]);
        Matrix c12 = divideAndConquer(matricesA[0],matricesB[1])+divideAndConquer(matricesA[1],matricesB[3]);
        Matrix c21 = divideAndConquer(matricesA[2],matricesB[0])+divideAndConquer(matricesA[3],matricesB[2]);
        Matrix c22 = divideAndConquer(matricesA[2],matricesB[1])+divideAndConquer(matricesA[3],matricesB[3]);

        Matrix C(c11,c12,c21,c22);

        return C;
    }
}


void Matrix::print() {
    for (int i = 0; i < m_rows; ++i) {
        for (int j = 0; j < m_cols; ++j) {
            std::cout<<m_data[i][j]<<"  ";
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
}


unsigned int Matrix::getRows() const{
    return m_rows;
}
unsigned int Matrix::getCols() const{
    return m_cols;
}

int Matrix::getRandomInt() {
    double interval = INTERVAL_RANDOM_NUMBER;
    static std::mt19937 mt{ std::random_device{}() };
    static std::uniform_real_distribution<> dist(-interval, interval);
    return (int)dist(mt);
}