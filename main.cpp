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
bool isFilenameValid(const string &filename){
    return filename.find(".txt") != std::string::npos;
}

int findArgumentUsage(const std::string &c){
    if(c == "-h" || c=="-H" || c=="--help"){
        cout<<"vypisuji help... "<<endl;
        return 0;
    }
    else if(c == "-f" || c == "-F"|| c=="--file"){
        cout<<"Loading matrices from file ";
        return 1;
    }
    else if(c == "-r" || c == "-R" || c=="--random"){
        cout<<"Generating random matrices"<<endl;
        return 2;
    }
    else if(c == "-p" || c == "-P" || c == "--parallel"){
        cout<<"Performance of multiple threads"<<endl;
        return 3;
    }
    else if(c == "-s" || c == "-S" || c == "--single"){
        cout<<"Performance of single thread"<<endl;
        return 4;
    }
    else if(c == "-c" || c == "-C" || c == "--compare"){
        cout<<"Compare speed of single thread and parallel"<<endl;
        return 5;
    }
    else{
        cout<<"Unknown parameter, for help run with argument -h or -H or --help "<<endl;
        return -1;
    }

}

double runPerformanceTest(Matrix &matrix1, Matrix &matrix2, unsigned int strategy){
    auto start = std::chrono::high_resolution_clock::now();
    matrix1.setStrategy(strategy);
    Matrix g = matrix1*matrix2;
    auto end = std::chrono::high_resolution_clock::now();
    switch (strategy){
        case 0:
            cout<<"Naive algorithm needed: " << to_ms(end-start).count() <<" ms to finish"<<endl;
            break;
        case 1:
            cout<<"Single thread Divide & conquer algorithm needed: " << to_ms(end-start).count() <<" ms to finish"<<endl;
            break;
        case 2:
            cout<<"Parallel divide & conquer needed: " << to_ms(end-start).count() <<" ms to finish"<<endl;
            break;
        default:
            break;
    }
    return to_ms(end-start).count();
}

void runComparison(Matrix matrix1, Matrix matrix2){
    runPerformanceTest(matrix1,matrix2,1);
    runPerformanceTest(matrix1,matrix2,2);
}

int menu(int argc, char *argv[]){

    cout<<"arguments counter: " <<argc<<endl;

    if(argc<2){
        cout<<"No argument parsed, exiting"<<endl;
        return 0;
    }

    else if(argc == 2) {
        std::string argv1 = argv[1];
        switch (findArgumentUsage(argv1)) {
            case -1:
            case 0:
                return 0;
            case 1:
                cout << "You have to specify filename" << endl;
                return 0;
            case 2:
                cout << "You have to parse another argument defining matrix size" << endl;
                return 0;
            case 3:
            case 4:
            case 5:
                cout<< "You have to specify input matrices first, run with -h for help"<<endl;
                return 0;
            default:
                return 0;
        }
    }
    else if(argc == 3){
        std::string argv1 = argv[1];
        switch (findArgumentUsage(argv1)) {
            case -1:
                return 0;
            case 1: {
                string filename = argv[2];
                if(! isFilenameValid(filename)){
                    cout << "Invalid file name, file must be in .txt format" <<endl;
                    return 0;
                }
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
                        cout<<"Implementation not specified, running time comparison of single thread and parallel algorithm"<<endl;
                        runComparison(matrix1,matrix2);
                    }
                    catch (exception &e) {
                        cout << "Exception: " << e.what() << '\n';
                    }
                }
                else{
                    cout << "File not found" <<endl;
                }
                file.close();

                break;
            }
            case 2: {
                try {
                    std::string argv2 = argv[2];//TODO zaporne vstupy
                    auto size = (unsigned int) stoi(argv2);
                    cout<<"Matrix size: "<<size<<endl;
                    Matrix matrix1(size);
                    Matrix matrix2(size);
                    matrix1.fillWithRandomData();
                    matrix2.fillWithRandomData();

                    cout<<"Implementation not specified, running time comparison of single thread and parallel algorithm"<<endl;
                    runComparison(matrix1,matrix2);
                }
                catch (exception &e) {
                    cout <<"Wrong argument, insert matrix size as number! Exception: " << e.what() << '\n';
                }
                break;
            }
            default:
                return 0;
        }

    }
    else if(argc==4){
        std::string argv1 = argv[1];
        switch (findArgumentUsage(argv1)) {
            case -1:
                return 0;
            case 1: {
                string filename = argv[2];
                if(! isFilenameValid(filename)){
                    cout << "Invalid file name, file must be in .txt format" <<endl;
                    return 0;
                }
                string argv3 = argv[3];
                int implementation = findArgumentUsage(argv3);
                if(implementation > 5 || implementation < 3){
                    cout << "Invalid strategy" <<endl;
                    return 0;
                }
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
                        switch (implementation){
                            case 3:
                            case 4:
                                runPerformanceTest(matrix1,matrix2,(unsigned int) implementation-2);
                                break;
                            case 5:
                                runComparison(matrix1,matrix2);
                                break;
                            default:
                                return 0;
                        }
                    }
                    catch (exception &e) {
                        cout << "Exception: " << e.what() << '\n';
                    }
                }
                else{
                    cout << "File not found" <<endl;
                }
                file.close();

                break;
            }
            case 2: {
                try {
                    std::string argv2 = argv[2]; //TODO if not a number
                    auto size = (unsigned int) stoi(argv2);
                    cout<<"Matrix size: "<<size<<endl;
                    string argv3 = argv[3];
                    int implementation = findArgumentUsage(argv3);
                    if(implementation > 5 || implementation < 3){
                        cout << "Invalid strategy" <<endl;
                        return 0;
                    }
                    Matrix matrix1(size);
                    Matrix matrix2(size);
                    matrix1.fillWithRandomData();
                    matrix2.fillWithRandomData();

                    switch (implementation){
                        case 3:
                        case 4:
                            runPerformanceTest(matrix1,matrix2,(unsigned int) implementation-2);
                            break;
                        case 5:
                            runComparison(matrix1,matrix2);
                            break;
                        default:
                            return 0;
                    }
                }
                catch (exception &e) {
                    cout <<"Wrong argument, insert matrix size as number! Exception: " << e.what() << '\n';
                }
                break;
            }
            default:
                return 0;
        }

    }
    return 0;

}

void saveMatrices(const Matrix &matrix1, const Matrix &matrix2, const string &filename){
    ofstream file(filename);
    file.open(filename,std::ios_base::app);

    string line;
    if (file.is_open()) {
        unsigned  int size = matrix1.getRows();
        file << size;
        file << "\n";
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                file << matrix1.getData(i,j);
                file << "  ";
            }
            file << "\n";
        }
        file << "\n";
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                file << matrix2.getData(i,j);
                file << "  ";
            }
            file << "\n";
        }
    }

}

void runTests(const unsigned int rounds, const unsigned int lowPow, const unsigned int upperPow, const string &filename){
    ofstream outfile(filename);
    outfile << "Rounds: " << rounds << "\n";
    unsigned int power = lowPow;
    double meanSingleThread = 0;
    double meanParallel = 0;
    double counterSingleThread = 0;
    double counterParallel = 0;
    while(power <= upperPow){
        for (int i = 0; i < rounds; ++i) {
            Matrix e(power);
            Matrix f(power);
            e.fillWithRandomData();
            f.fillWithRandomData();
            counterSingleThread += runPerformanceTest(e,f,1);
            counterParallel += runPerformanceTest(e,f,2);
        }
        meanSingleThread = counterSingleThread/rounds;
        meanParallel = counterParallel/rounds;

        outfile<<power<<"   "<<meanSingleThread<<"   "<<meanParallel<<"\n";

        counterSingleThread = 0;
        counterParallel = 0;
        power = power*2;
    }

    outfile.close();
}

int main(int argc, char *argv[]){
    runTests(5,32,512,"test_2dArray.txt");
//    menu(argc,argv);
//    cout<< "arguments"<<endl;
//    for (int i = 0; i < argc; ++i) {
//        cout<< "   argv[" << i << "]   "<<argv[i]<< "\n";
//    }



    return 0;
}


