//
// Created by matt on 11/13/18.
//

#ifndef PJCSEMESTRALWORK_MATRIX_H
#define PJCSEMESTRALWORK_MATRIX_H

#include <iostream>
#include <vector>

class Matrix{
private:
    unsigned int m_rows;
    unsigned int m_cols;
    int** m_data;
    unsigned int m_strategy;
    const unsigned int m_maxThreadDepth = 5;
public:
    Matrix(unsigned int width, unsigned int height);
    Matrix(unsigned int size);
    Matrix(const Matrix &a,const Matrix &b,const Matrix &c,const Matrix &d);
    Matrix(unsigned int size, int array []);
//    ~Matrix();
    bool operator==(const Matrix &matrix);
    Matrix operator+(const Matrix matrix);
    Matrix operator+=(const Matrix &matrix);
    Matrix operator*(const Matrix &matrix);
    void setStrategy(const int strategy);

    friend Matrix divideAndConquer(const Matrix &matrix1, const Matrix &matrix2);
    friend Matrix divideAndConquerThreads(const Matrix &matrix1, const Matrix &matrix2, const int depth);
//    std::vector<Matrix> divideMatrixIntoQuarters(std::vector<Matrix> vector) const;
    void divideMatrixIntoQuarters(std::vector<Matrix>& vector) const;


//    Matrix(Matrix&& rhs);
//    Matrix& operator=(Matrix&& rhs);

    int getRandomInt();

    void print(void);

    unsigned int getRows() const;
    unsigned int getCols() const;
    unsigned int getMaxThreadDepth() const;
};



#endif //PJCSEMESTRALWORK_MATRIX_H
