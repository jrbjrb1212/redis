#pragma once
#include "common.hpp"
#include "redis_store.hpp"


class RESPHandler {
    private:
        RedisStore &dataStore;
        std::string genError(std::string errorMsg);
        std::string handlePING(std::vector<std::string> &parsedReq);
        std::string handleECHO(std::vector<std::string> &parsedReq);
        std::string handleGET(std::vector<std::string> &parsedReq);
        std::string handleSET(std::vector<std::string> &parsedReq);
        std::string handleEXISTS(std::vector<std::string> &parsedReq);
        std::string handleDEL(std::vector<std::string> &parsedReq);
        std::string handleINCRorDECR(std::vector<std::string> &parsedReq);
        std::string handleLPUSH(std::vector<std::string> &parsedReq);
        std::string handleRPUSH(std::vector<std::string> &parsedReq);
        std::string handleLRange(std::vector<std::string> &parsedReq);
        std::string handleSAVE(std::vector<std::string> &parsedReq);
        std::string handleKEYS(std::vector<std::string> &parsedReq);
        std::string handleFLUSHALL(std::vector<std::string> &parsedReq);

    public:        
        RESPHandler(RedisStore &store) : dataStore(store) {};
      
        std::string handleMsg(std::vector<std::string> &parsedReq);    
};