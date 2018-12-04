#include <iostream>
#include <chrono>
#include <fstream>
#include <exception>
#include <stdexcept>
#include <thread>
#include "matrix.hpp"

using namespace std;

template <typename TimePoint>
std::chrono::milliseconds to_ms(TimePoint tp) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(tp);
}


int findArgumentUsage(std::string c){
    if(c == "-h" || c=="-H" || c=="--help"){
        cout<<"vypisuji help... "<<endl;
        return 0;

    }
    else if(c == "-f" || c == "-F"|| c=="--file"){
        cout<<"nahravam matice ze souboru ";
        return 1;
    }
    else if(c == "-r" || c == "-R" || c=="--random"){
        cout<<"generuji nahodne matice... "<<endl;
        return 2;
    }

    else{
        cout<<"unknown parametr, for help run with argument h or H or help "<<endl;
        return -1;
    }

}


int main(int argc, char *argv[]){

    cout<<"arguments counter: " <<argc<<endl;

    if(argc<2){
        cout<<"No argument parsed, exiting"<<endl;
        return 0;
    }

    if(argc == 2) {
        std::string argv1 = argv[1];
        switch (findArgumentUsage(argv1)) {
            case -1:
            case 0:
                return 0;
            case 1:
                cout << "you have to specify filename" << endl;
                return 0;
            case 2:
                cout << "you have to parse another argument defining matrix size" << endl;
                return 0;
            default:
                cout << "unknown parametr, for help run with argument h or H or help " << endl;
                return 0;
        }
    }

    if(argc == 3){
        std::string argv1 = argv[1];
        switch (findArgumentUsage(argv1)) {
            case -1:
                return 0;
            case 1: {
                string filename = argv[2];
                cout<<filename<<endl;
                ifstream file;
                file.open(filename);
                string line;
                if (file.is_open()) {
                    try {
                        unsigned int size;
                        file>>size;
                        cout<<"width: "<<size<<endl;
                        int array[size * size];
                        for (int i = 0; i < size * size; ++i) {
                            file >> array[i];
                        }
                        Matrix matrix1(size, array);
                        for (int i = 0; i < size * size; ++i) {
                            file >> array[i];
                        }
                        Matrix matrix2(size, array);
                        Matrix martix3 = matrix1*matrix2;
                        martix3.print();
                    }
                    catch (exception &e) {
                        cout << "Exception: " << e.what() << '\n';
                    }
                }
                file.close();
                break;
            }
            case 2: {
                try {
                    std::string argv2 = argv[2];
                    int size = stoi(argv2);
                    cout<<"Matrix size: "<<size<<endl;
                    Matrix matrix1(size);
                    Matrix matrix2(size);
                    matrix1.print();
                    matrix2.print();

                    Matrix martix3 = matrix1*matrix2;
                    martix3.print();
                }
                catch (exception &e) {
                    cout <<"Wrong argument, insert matrix size as number! Exceptiom: " << e.what() << '\n';
                }
                break;
            }
            default:
                cout << "unknown parametr, for help run with argument h or H or help " << endl;
                return 0;
        }

    }
    cout<< "arguments"<<endl;
    for (int i = 0; i < argc; ++i) {
        cout<< "   argv[" << i << "]   "<<argv[i]<< "\n";
    }

//    Matrix a(2);
//    Matrix b(2);
//    Matrix c(2);
//    Matrix d(2);

//    a.print();
//    b.print();
//    c.print();
//    d.print();

//    Matrix e(size);
//    Matrix f(size);
////    e.print();
////    f.print();
//
//
////    std::vector<Matrix> matrices;
////    e.divideMatrixIntoQuarters(matrices);
////
////    cout<<matrices.size()<<endl<<endl;
////    matrices[0].print();
////    matrices[1].print();
////    matrices[2].print();
////    matrices[3].print();
//
//    auto start = std::chrono::high_resolution_clock::now();
//    Matrix naive = e*f;
//    auto end = std::chrono::high_resolution_clock::now();
//    cout<<"Naive needed: " << to_ms(end-start).count() <<" ms to finish"<<endl;
////    naive.print();
//
//    start = std::chrono::high_resolution_clock::now();
//    Matrix g = divideAndConquer(e,f);
//    end = std::chrono::high_resolution_clock::now();
//    cout<<"Divide and conquer needed: " << to_ms(end-start).count() <<" ms to finish"<<endl;
////    g.print();
//
//    start = std::chrono::high_resolution_clock::now();
//    Matrix thread = divideAndConquerThreads(e,f,0);
//    end = std::chrono::high_resolution_clock::now();
//    cout<<"Divide and conquer threads needed: " << to_ms(end-start).count() <<" ms to finish"<<endl;
//    thread.print();

    return 0;
}


