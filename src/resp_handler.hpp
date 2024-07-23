#pragma once
#include "common.hpp"

class RESPHandler {
    private:
        std::string genError(std::string errorMsg);
        std::string handlePING(std::vector<std::string> &parsedReq);
        std::string handleECHO(std::vector<std::string> &parsedReq);
        std::string handleGET(std::vector<std::string> &parsedReq);
        std::string handleSET(std::vector<std::string> &parsedReq);
        std::string handleEXISTS(std::vector<std::string> &parsedReq);
        std::string handleDEL(std::vector<std::string> &parsedReq);
        std::string handleINCR(std::vector<std::string> &parsedReq);
        std::string handleDECR(std::vector<std::string> &parsedReq);
        std::string handleLPUSH(std::vector<std::string> &parsedReq);
        std::string handleRPUSH(std::vector<std::string> &parsedReq);
        std::string handleSAVE(std::vector<std::string> &parsedReq);

    public:        
        RESPHandler(){

        };
      
        std::string handleMsg(std::vector<std::string> &parsedReq);      
};