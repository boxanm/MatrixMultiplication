//
// Created by matt on 12/5/18.
//

#ifndef PJCSEMESTRALWORK_MENU_H
#define PJCSEMESTRALWORK_MENU_H

#include "matrix.hpp"
#include "matrix1D.hpp"
#include <iostream>
#include <chrono>
#include <fstream>

template <typename TimePoint>
std::chrono::milliseconds to_ms(TimePoint tp);
bool isFilenameValid(const std::string &filename);
int findArgumentUsage(const std::string &c);
double runPerformanceTest(Matrix &matrix1, Matrix &matrix2, unsigned int strategy);
double runPerformanceTest(Matrix1D &matrix1, Matrix1D &matrix2, unsigned int strategy);
void printPerformance(double time,unsigned int strategy);
void runComparison(Matrix matrix1, Matrix matrix2);
void runComparison(Matrix1D matrix1, Matrix1D matrix2);
void saveMatrices(const Matrix &matrix1, const Matrix &matrix2, const std::string &filename);
void runTests(const unsigned int &rounds, const unsigned int &lowPow, const unsigned int &upperPow, const std::string &filename, bool oneDimension);
int menu(int argc, char **argv);
int menuFile(char **argv, const int &impementation);
int menuRandom(char **argv,  const int &impementation);
int menuTest(char **argv);

#endif //PJCSEMESTRALWORK_MENU_H
