//
// Created by matt on 12/5/18.
//
#include "menu.hpp"
using namespace std;

template <typename TimePoint>
chrono::milliseconds to_ms(TimePoint tp) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(tp);
}
bool isFilenameValid(const string &filename){
    return filename.find(".txt") != std::string::npos;
}

int findArgumentUsage(const std::string &c){
    if(c == "-h" || c=="-H" || c=="--help"){
        cout<<"Usage: [options]{implementation}\nAn option summary appears below.  Please see the documentation for details.\n\n"
              "List of options:\n-----------\n";
        printf("-h -H --help\t\t\t\t\t\t\t\t\t\tShow help\n");
        printf("-t -T --test [n][lowBound][upperBound][filename]\tRun testing algorithm for n rounds starting from lowBound to upperBound.\n");
        printf("\t\t\t\t\t\t\t\t\t\t\t\t\tBoth lowBound and upperBound must be a power of 2\n");
        printf("\t\t\t\t\t\t\t\t\t\t\t\t\tResults are saved to file\n");
        printf("-f -F --file [filename]{implementation}\t\t\t\tLoad matrices from file and run defined implementation help\n");
        printf("\t\t\t\t\t\t\t\t\t\t\t\t\tSee types if implementations below.\n");
        printf("-r -R --random [n]{implementation}\t\t\t\t\tRun defined implementation on random matrices of size n\n");
        printf("\t\t\t\t\t\t\t\t\t\t\t\t\tMatrix size must be a power of 2\n");
        printf("\t\t\t\t\t\t\t\t\t\t\t\t\tSee types if implementations below\n");
        printf("List of implementations:\n-----------\n");
        printf("-p -P --parallel\t\t\t\t\t\t\t\t\tPerformance of multiple threads\n");
        printf("-s -S --single\t\t\t\t\t\t\t\t\t\tPerformance of single thread\n");
        printf("-c -C --compare\t\t\t\t\t\t\t\t\t\tCompare speed of  single thread and parallel implementation\n");
        return 0;
    }
    else if(c == "-f" || c == "-F"|| c=="--file"){
        cout<<"Loading matrices from file: ";
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
    else if(c == "-t" || c == "-T" || c == "--test"){
        cout<<"Running time tests of single thread and parallel algorithms"<<endl;
        return 6;
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
    return to_ms(end-start).count();
}

double runPerformanceTest(Matrix1D &matrix1, Matrix1D &matrix2, unsigned int strategy){
    auto start = std::chrono::high_resolution_clock::now();
    matrix1.setStrategy(strategy);
    matrix1*matrix2;
    auto end = std::chrono::high_resolution_clock::now();
    return to_ms(end-start).count();
}

void printPerformance(double time,unsigned int strategy){
    switch (strategy){
        case 0:
            cout<<"Naive algorithm needed: " << time <<" ms to finish"<<endl;
            break;
        case 1:
            cout<<"Single thread Divide&conquer algorithm needed: " << time <<" ms to finish"<<endl;
            break;
        case 2:
            cout<<"Parallel      Divide&conquer algorithm needed: " << time <<" ms to finish"<<endl;
            break;
        default:
            break;
    }
}

void runComparison(Matrix matrix1, Matrix matrix2){
    double time;
    time =  runPerformanceTest(matrix1,matrix2,1);
    printPerformance(time, 1);
    time = runPerformanceTest(matrix1,matrix2,2);
    printPerformance(time, 2);
}

void runComparison(Matrix1D matrix1, Matrix1D matrix2){
    double time;
    time =  runPerformanceTest(matrix1,matrix2,1);
    printPerformance(time, 1);
    time = runPerformanceTest(matrix1,matrix2,2);
    printPerformance(time, 2);
}

void saveMatrices(const Matrix &matrix1, const Matrix &matrix2, const std::string &filename){
    ofstream file(filename);
    if (file.is_open()) {
        unsigned  int size = matrix1.getRows();
        file << size<< "\n";
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                file << matrix1.getData(i,j)<< "  ";
            }
            file << "\n";
        }
        file << "\n";
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                file << matrix2.getData(i,j)<< "  ";
            }
            file << "\n";
        }
    }
    else{
        std::cout<<"file not opened"<<endl;
    }
    file.close();

}

void runTests(const unsigned int &rounds, const unsigned int &lowPow, const unsigned int &upperPow, const string &filename, bool oneDimension){
    ofstream outfile(filename);
    outfile << "Rounds: " << rounds << "\n";
    unsigned int power = lowPow;
    double meanSingleThread = 0;
    double meanParallel = 0;
    double counterSingleThread = 0;
    double counterParallel = 0;
    while(power <= upperPow){
        for (int i = 0; i < rounds; ++i) {
            Matrix1D e(power);
            Matrix1D f(power);
            e.fillWithRandomData();
            f.fillWithRandomData();
            counterSingleThread += runPerformanceTest(e,f,1);
            counterParallel += runPerformanceTest(e,f,2);
        }
        meanSingleThread = counterSingleThread/rounds;
        meanParallel = counterParallel/rounds;

        cout<<"Mean: ";
        printPerformance(meanSingleThread,1);
        cout<<"Mean: ";
        printPerformance(meanParallel,2);

        outfile<<power<<"   "<<meanSingleThread<<"   "<<meanParallel<<"\n";

        counterSingleThread = 0;
        counterParallel = 0;
        power = power*2;
    }

    outfile.close();
}

int menuFile(char **argv, const int &implementation) {
    string filename = argv[2];
    if(! isFilenameValid(filename)){
        cout << "Invalid file name, file must be in .txt format" <<endl;
        return 0;
    }
    cout<<filename<<endl;
    ifstream file(filename);
    if (file.is_open()) {
        try {
            unsigned int size;
            file >> size;
            int array[size * size];
            for (int i = 0; i < size * size; ++i) {
                file >> array[i];
            }
            Matrix1D matrix1(size, array);
            for (int i = 0; i < size * size; ++i) {
                file >> array[i];
            }
            Matrix1D matrix2(size, array);
            if (implementation == -1) {
                cout << "Implementation not specified, running time comparison of single thread and parallel algorithm"
                     << endl;
                runComparison(matrix1, matrix2);
            }
            else{
                switch (implementation){
                    case 3:
                    case 4:{
                        double time = runPerformanceTest(matrix1,matrix2,(unsigned int) implementation-2);
                        printPerformance(time, (unsigned int)implementation -2);
                        break;
                    }
                    case 5:{
                        runComparison(matrix1,matrix2);
                        break;
                    }
                    default:
                        return 0;
                }
            }
            file.close();
            return 1;
        }
        catch (exception &e) {
            cout << "Exception: " << e.what() << '\n';
            file.close();
            return 0;
        }
    }
    else{
        cout << "File not found" <<endl;
        file.close();
        return 0;
    }
}

int menuRandom(char **argv, const int &implementation) {
    try {
        std::string argv2 = argv[2];
        int size = stoi(argv2);
        if(size < 2){
            cout<< "Matrix size must be at least 2, use -h for help" <<endl;
            return 0;
        }
        Matrix1D matrix1((unsigned) size);
        Matrix1D matrix2((unsigned) size);
        matrix1.fillWithRandomData();
        matrix2.fillWithRandomData();
        if(implementation == -1){
            cout<<"Implementation not specified, running time comparison of single thread and parallel algorithm"<<endl;
            runComparison(matrix1,matrix2);}

        else{
            switch (implementation){
                case 3:
                case 4:{
                    double time = runPerformanceTest(matrix1,matrix2,(unsigned int) implementation-2);
                    printPerformance(time, (unsigned int)implementation -2);
                    break;
                }
                case 5:
                    runComparison(matrix1,matrix2);
                    break;
                default:
                    return 0;
            }
        }
        return 1;
    }
    catch (exception &e) {
        cout << "Wrong argument, insert matrix size as number! Exception: " << e.what() << '\n';
        return 0;
    }
}

int menuTest(char **argv){
    std::string argv2 = argv[2];
    int rounds = stoi(argv2);
    if(rounds < 1){
        cout<< "Number of rounds must be at least 1, use -h for help" <<endl;
        return 0;
    }
    std::string argv3 = argv[3];
    int lowPow = stoi(argv3);
    if(lowPow%2 !=0){
        cout<< "Lower bound must be a power of 2, use -h for help" <<endl;
        return 0;
    }
    std::string argv4 = argv[4];
    int upperPow = stoi(argv4);
    if(upperPow%2 !=0 || upperPow < lowPow){
        cout<< "Upper bound must be a power of 2 and must be larger than lower bound, use -h for help" <<endl;
        return 0;
    }
    std::string filename = argv[5];
    runTests((unsigned ) rounds,(unsigned ) lowPow,(unsigned ) upperPow,filename,true);
    return 1;
}

int menu(int argc, char **argv) {
    switch (argc) {
        case 0:
        case 1:
            cout << "No argument parsed, for help run with argument -h or -H or --help " << endl;
            return 0;
        case 2: {
            std::string argv1 = argv[1];
            switch (findArgumentUsage(argv1)) {
                case -1:
                case 0:
                    break;
                case 1:
                    cout << "You have to specify filename, run with -h for help" << endl;
                    break;
                case 2:
                    cout << "You have to parse another argument defining matrix size, run with -h for help" << endl;
                    break;
                case 3:
                case 4:
                case 5:
                    cout << "You have to specify input matrices first, run with -h for help" << endl;
                    break;
                default:
                    break;
            }
            return 0;
        }
        case 3: {
            std::string argv1 = argv[1];
            switch (findArgumentUsage(argv1)) {
                case 1:
                    return menuFile(argv, -1);
                case 2:
                    return menuRandom(argv, -1);
                default:
                    return 0;
            }
        }
        case 4: {
            std::string argv1 = argv[1];
            switch (findArgumentUsage(argv1)) {
                case -1:
                    return 0;
                case 1: {
                    string argv3 = argv[3];
                    int implementation = findArgumentUsage(argv3);
                    if (implementation > 5 || implementation < 3) {
                        cout << "Invalid strategy" << endl;
                        return 0;
                    }
                    return menuFile(argv, implementation);
                }
                case 2: {
                    string argv3 = argv[3];
                    int implementation = findArgumentUsage(argv3);
                    if (implementation > 5 || implementation < 3) {
                        cout << "Invalid strategy" << endl;
                        return 0;
                    }
                    return menuRandom(argv, implementation);
                }
                default:
                    return 0;
            }
        }
        case 6:{
            std::string argv1 = argv[1];
            switch (findArgumentUsage(argv1)) {
                case 6:
                    return menuTest(argv);
                default:
                    break;
            }
            return 0;
        }

        default:
            cout<<"Invalid input, run with -h for help"<<endl;
            return 0;
    }
}
