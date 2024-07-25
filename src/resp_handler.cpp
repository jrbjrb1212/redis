#include "resp_handler.hpp"


std::string getArgFromBulkString(std::string &bulkString){
    std::string &keyUnclean = bulkString;
    size_t keyCLRF = keyUnclean.find(CLRF);
    ssize_t keyLen = stoi(keyUnclean.substr(0, keyCLRF));
    std::string key = keyUnclean.substr(keyCLRF+2, keyLen);
    return key;
}

std::string RESPHandler::genError(std::string errormsg){
    return "-" + errormsg + CLRF;
}


std::string RESPHandler::handlePING(std::vector<std::string> &parsedReq){
    if (parsedReq.size() != 1)
        return genError("wrong number of arguments for command");
    
    return "+PONG\r\n";
}


std::string RESPHandler::handleECHO(std::vector<std::string> &parsedReq){
    if (parsedReq.size() != 2)
        return genError("wrong number of arguments for command");

    std::string response;
    std::string &echoMsg = parsedReq[1];

    size_t firstCLRF = echoMsg.find(CLRF);
    echoMsg = echoMsg.substr(firstCLRF+2);

    response = "+" + echoMsg;
    return response;
}


std::string RESPHandler::handleGET(std::vector<std::string> &parsedReq){
    std::string key = getArgFromBulkString(parsedReq[1]);

    storeKeyInfo valueSaved = dataStore.getStoreKey(key);
    if (!valueSaved.keyPresent)
        return "$-1\r\n";
    else if (valueSaved.listType)
        return genError("WRONGTYPE Operation against a key holding the wrong kind of value");
    else
        return "+\"" + valueSaved.value + "\"" + CLRF;
}


std::string RESPHandler::handleSET(std::vector<std::string> &parsedReq){
    std::string &keyUnclean = parsedReq[1];
    std::string &valUnclean = parsedReq[2];
    
    size_t keyCLRF = keyUnclean.find(CLRF);
    size_t valueCLRF = valUnclean.find(CLRF);

    ssize_t keyLen = stoi(keyUnclean.substr(0, keyCLRF));
    ssize_t valueLen = stoi(valUnclean.substr(0, valueCLRF));

    std::string key = keyUnclean.substr(keyCLRF+2, keyLen);
    std::string value = valUnclean.substr(valueCLRF+2, valueLen);

    std::cout << "key: " << key << std::endl;
    std::cout << "value: " << value << std::endl;

    
    dataStore.setStoreKey(key, value);
    storeKeyInfo dataSaved = dataStore.getStoreKey(key);
    std::cout << "Stored value: " << dataSaved.value << std::endl;

    return "+OK\r\n";
}


std::string RESPHandler::handleEXISTS(std::vector<std::string> &parsedReq){
    std::cout << "Not Currently Working" << std::endl;
}


std::string RESPHandler::handleDEL(std::vector<std::string> &parsedReq){
    std::cout << "Not Currently Working" << std::endl;
}


std::string RESPHandler::handleINCR(std::vector<std::string> &parsedReq){
    std::cout << "Not Currently Working" << std::endl;
}


std::string RESPHandler::handleDECR(std::vector<std::string> &parsedReq){
    std::cout << "Not Currently Working" << std::endl;
}


std::string RESPHandler::handleLPUSH(std::vector<std::string> &parsedReq){
    std::cout << "Not Currently Working" << std::endl;
}


std::string RESPHandler::handleRPUSH(std::vector<std::string> &parsedReq){
    std::cout << "Not Currently Working" << std::endl;
}


std::string RESPHandler::handleSAVE(std::vector<std::string> &parsedReq){
    std::cout << "Not Currently Working" << std::endl;
}

std::string RESPHandler::handleKEYS(std::vector<std::string> &parsedReq){
    // get size of redis dictionary
    std::string &keyUnclean = parsedReq[1];
    size_t keyCLRF = keyUnclean.find(CLRF);
    ssize_t keyLen = stoi(keyUnclean.substr(0, keyCLRF));
    std::string key = keyUnclean.substr(keyCLRF+2, keyLen);
    std::cout << "key: " << key << std::endl;
    std::string response;

    // Want all keys in the cache
    if (key == "*")
    {
        ssize_t storeSize = dataStore.redisCache.size();
        response = "*" + std::to_string(storeSize) + "\r\n";
        
        for (const auto &keyValPair : dataStore.redisCache) 
        {
            const std::string &key = keyValPair.first;
            response += "$" + std::to_string(key.size()) + "\r\n" + key + "\r\n"; 
        }

    }
    // Want a specific key
    else
    {   
        if (dataStore.redisCache.find(key) == dataStore.redisCache.end())
            return "*0\r\n";
        else
            return "*1\r\n$" + std::to_string(keyLen) + CLRF + key + CLRF;
    }

    return response;
}


// Routing function
// - takes a vector of parsed arguments from the redis-cli
// - the first argument is always  
std::string RESPHandler::handleMsg(std::vector<std::string> &parsedReq){
    std::string &command = parsedReq[0];
    std::string RESPResponse;
    size_t firstCLDF, secCLDF;
    firstCLDF = command.find(CLRF);
    secCLDF = command.find(CLRF, firstCLDF+1);
   
    std::string extractCommand = command.substr(firstCLDF+2, secCLDF - 2 - firstCLDF);
    std::transform(extractCommand.begin(), extractCommand.end(), extractCommand.begin(), ::toupper); 

    // TODO: refactor into a switch statement or some mapping function    
    if (extractCommand == "PING")
    {
        RESPResponse = handlePING(parsedReq);
    }
    else if (extractCommand == "ECHO")
    {
        RESPResponse = handleECHO(parsedReq);
    }
    else if (extractCommand == "GET")
    {
        RESPResponse = handleGET(parsedReq);
    }
    else if (extractCommand == "SET")
    {
        RESPResponse = handleSET(parsedReq);
    }
    else if (extractCommand == "EXISTS")
    {
        RESPResponse = handleEXISTS(parsedReq);
    }
    else if (extractCommand == "DEL")
    {
        RESPResponse = handleDEL(parsedReq);
    }
    else if (extractCommand == "INCR")
    {
        RESPResponse = handleINCR(parsedReq);
    }
    else if (extractCommand == "DECR")
    {
        RESPResponse = handleDECR(parsedReq);
    }
    else if (extractCommand == "LPUSH")
    {
        RESPResponse = handleLPUSH(parsedReq);
    }
    else if (extractCommand == "RPUSH")
    {
        RESPResponse = handleRPUSH(parsedReq);
    }
    else if (extractCommand == "SAVE")
    {
        RESPResponse = handleSAVE(parsedReq);
    }
    else if (extractCommand == "KEYS")
    {
        RESPResponse = handleKEYS(parsedReq);
    }

    return RESPResponse;
}

