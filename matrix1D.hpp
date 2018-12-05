//
// Created by matt on 11/13/18.
//

#ifndef PJCSEMESTRALWORK_MATRIX1D_H
#define PJCSEMESTRALWORK_MATRIX1D_H

#include <iostream>
#include <vector>

typedef int MATRIX_VALUE_TYPE;

class Matrix1D{
private:
    unsigned int m_rows;
    unsigned int m_cols;
    MATRIX_VALUE_TYPE* m_data;
    unsigned int m_strategy = 0;
public:
    Matrix1D(unsigned int width, unsigned int height);
    explicit Matrix1D(unsigned int size);
    Matrix1D(const Matrix1D &Matrix1D);
    Matrix1D(const Matrix1D &a,const Matrix1D &b,const Matrix1D &c,const Matrix1D &d);
    Matrix1D(unsigned int size, MATRIX_VALUE_TYPE array []);
    ~Matrix1D();
    Matrix1D& operator= (const Matrix1D &Matrix1D);
    bool operator==(const Matrix1D &Matrix1D);
    Matrix1D operator+(const Matrix1D &Matrix1D);
    Matrix1D operator+=(const Matrix1D &Matrix1D);
    Matrix1D operator*(const Matrix1D &Matrix1D);
    void setStrategy(const unsigned int &strategy);
    void fillWithRandomData();

    friend Matrix1D divideAndConquer(const Matrix1D &Matrix1D1, const Matrix1D &Matrix1D2);
    friend Matrix1D divideAndConquerThreads(const Matrix1D &Matrix1D1, const Matrix1D &Matrix1D2, const bool &parallel);
    friend Matrix1D naiveMultiplication(const Matrix1D &Matrix1D1, const Matrix1D &Matrix1D2);
    friend void setData(Matrix1D *Matrix1D, int row, int col, MATRIX_VALUE_TYPE value);
    void divideMatrixIntoQuarters(std::vector<Matrix1D>& vector) const;


//    Matrix1D(Matrix1D&& rhs);
//    Matrix1D& operator=(Matrix1D&& rhs);


    void print();

    int getRandomInt();
    unsigned int getRows() const;
    unsigned int getCols() const;
    MATRIX_VALUE_TYPE getData(int row, int col) const;
};



#endif //PJCSEMESTRALWORK_MATRIX1D_H
