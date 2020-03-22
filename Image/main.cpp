#include <chrono>
#include <iostream>
#include <boost/filesystem.hpp>
#include "util/image/ImageBMP.h"
#include "ui/Window.h"
#include "MainWindow.h"

#define W 500
#define H 500

using namespace std;
namespace fs = boost::filesystem;

string getCurrentPath(char* argv[]) {
    string argv_str(argv[0]);
    string base = argv_str.substr(0, argv_str.find_last_of("/"));
    cout << base << endl;
}

int main(int argc, char* argv[]) {
//    readImage("/home/dmitriy/University/ImageAnalyser/images/example.bmp");
//    for (int i = 0; i < argc; i++) {
//        cout << argv[i] << endl;
//    }

    MainWindow window(800, 600, "ImageAnalyser");
    window.start();


    return 0;
}