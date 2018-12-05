//
// Created by matt on 11/13/18.
//

#ifndef PJCSEMESTRALWORK_MATRIX_H
#define PJCSEMESTRALWORK_MATRIX_H

#include <random>
#include <thread>
#include <future>
#include <iostream>
#include <vector>

typedef int MATRIX_VALUE_TYPE;

class Matrix{
private:
    unsigned int m_rows;
    unsigned int m_cols;
    MATRIX_VALUE_TYPE** m_data;
    unsigned int m_strategy = 0;
public:
    Matrix(unsigned int width, unsigned int height);
    explicit Matrix(unsigned int size);
    Matrix(const Matrix &matrix);
    Matrix(const Matrix &a,const Matrix &b,const Matrix &c,const Matrix &d);
    Matrix(unsigned int size, MATRIX_VALUE_TYPE array []);
    ~Matrix();
    Matrix& operator= (const Matrix &matrix);
    bool operator==(const Matrix &matrix);
    Matrix operator+(const Matrix &matrix);
    Matrix operator+=(const Matrix &matrix);
    Matrix operator*(const Matrix &matrix);
    void setStrategy(const unsigned int &strategy);
    void fillWithRandomData();

    friend Matrix divideAndConquer(const Matrix &matrix1, const Matrix &matrix2);
    friend Matrix divideAndConquerThreads(const Matrix &matrix1, const Matrix &matrix2, const bool &parallel);
    friend Matrix naiveMultiplication(const Matrix &matrix1, const Matrix &matrix2);
    friend void setData(Matrix *matrix, int row, int col, MATRIX_VALUE_TYPE value);
    void divideMatrixIntoQuarters(std::vector<Matrix>& vector) const;


//    Matrix(Matrix&& rhs);
//    Matrix& operator=(Matrix&& rhs);


    void print();

    int getRandomInt();
    unsigned int getRows() const;
    unsigned int getCols() const;
    MATRIX_VALUE_TYPE getData(int row, int col) const;
};



#endif //PJCSEMESTRALWORK_MATRIX_H
