#include "../include/Studio.h"
#include <iostream>

using namespace std;

extern Studio* backup = nullptr;

int main(int argc, char** argv){

    std::cout << "i am here!@#!#@!#@!#!@#!@#" << std::endl;
    if(argc!=2){
        std::cout << "usage: studio <config_path>" << std::endl;
        return 0;
    }
    string configurationFile = argv[1];
    Studio studio(configurationFile);
    studio.start();
    if(backup!=nullptr){
    	delete backup;
    	backup = nullptr;
    }
    return 0;
}