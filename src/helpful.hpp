#pragma once

#include <iostream>
#include <vector>
#include <string>

// TODO: Delete helpful functions later
void printVector(std::vector<std::string> toPrint){
    for (int i = 0; i<toPrint.size(); i++)
        std::cout << "- ["<< i+1 << "] element: " << toPrint[i] << std::endl; 
}


void printVector(std::string message, std::vector<std::string> toPrint){
    std::cout << "Message: " << message << std::endl;
    for (int i = 0; i<toPrint.size(); i++)
        std::cout << "- ["<< i+1 << "] element: " << toPrint[i] << std::endl; 
}