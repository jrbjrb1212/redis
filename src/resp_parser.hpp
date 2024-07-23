#pragma once
#include "common.hpp"

#define ITEM_LEN_MAX 536870912 //512 MiB

class RESPParser {
    private:
        int readFD = -1;
        std::string _read_cache = "";
        
    public:
        std::string test = "Hello World";
        
        RESPParser(int fd){
            readFD = fd;
        }

        RESPParser(){

        };


        std::vector<std::string> deserialize(std::string message);      
        std::string serialize();      
};