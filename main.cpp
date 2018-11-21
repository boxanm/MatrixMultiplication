#include <iostream>
#include "matrix.hpp"

using namespace std;

int main() {
    Matrix a(2);
    Matrix b(2);
    Matrix c(2);
    Matrix d(2);

    a.print();
    b.print();
    c.print();
    d.print();


    Matrix e(a,b,c,d);
    e.print();


    std::vector<Matrix> matrices;
//    matrices.reserve(4);
    matrices = e.divideMatrixIntoQuarters(matrices);

    cout<<matrices.size()<<endl<<endl;
    matrices[0].print();
    matrices[1].print();
    matrices[2].print();
    matrices[3].print();

//    mergeMatrices(matrix1,matrix2);

//    Matrix c(matrix1,matrix2);
//    Matrix matrix3 = (matrix1+matrix2);
//    matrix3.print();

//    matrix1+=matrix2;
//    matrix1.print();

//    Matrix c = matrix1*matrix2;
//    c.print();


}