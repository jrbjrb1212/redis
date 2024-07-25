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
    if (parsedReq.size() != 3)
        return genError("syntax error");
    
    std::string key = getArgFromBulkString(parsedReq[1]);
    std::string value = getArgFromBulkString(parsedReq[2]);

    dataStore.setStoreKey(key, value);

    return "+OK\r\n";
}


std::string RESPHandler::handleEXISTS(std::vector<std::string> &parsedReq){
    if (parsedReq.size() != 2)
        return genError("syntax error");

    
    std::string key = getArgFromBulkString(parsedReq[1]);
    bool isContained = dataStore.checkStoreKey(key);
    std::string response = ":" + std::to_string(isContained) + "\r\n"; 
    return response;
}


// TODO: handle deletion of string arguments 1 through N
// - would need to parse n arguments in a loop
std::string RESPHandler::handleDEL(std::vector<std::string> &parsedReq){
    if (parsedReq.size() != 2)
        return genError("syntax error");

    
    std::string key = getArgFromBulkString(parsedReq[1]);
    bool isDeleted = dataStore.delStoreKey(key);
    std::string response = ":" + std::to_string(isDeleted) + "\r\n"; 
    return response;
}


std::string RESPHandler::handleINCRorDECR(std::vector<std::string> &parsedReq){
    if (parsedReq.size() != 2)
        return genError("wrong number of arguments for command");
     
    std::string key = getArgFromBulkString(parsedReq[1]);
    storeKeyInfo value = dataStore.updateStoreValue(key, 1);

    if (value.listType)
        return genError("Operation against a key holding the wrong kind of value");
    
    std::string response = ":" + value.value + CLRF; 
    return response;
}



std::string RESPHandler::handleLPUSH(std::vector<std::string> &parsedReq){
    if (parsedReq.size() != 3)
        return genError("wrong number of arguments for 'LPUSH' command");
    
    std::string key = getArgFromBulkString(parsedReq[1]);
    std::string value = getArgFromBulkString(parsedReq[2]);

    bool insertAtFront = true;
    ssize_t numElements = dataStore.insertIntoStoreRange(key, value, insertAtFront);
    
    std::string response = ":" + std::to_string(numElements) + CLRF;
    return response;
}


std::string RESPHandler::handleRPUSH(std::vector<std::string> &parsedReq){
    if (parsedReq.size() != 3)
        return genError("wrong number of arguments for 'RPUSH' command");
    
    std::string key = getArgFromBulkString(parsedReq[1]);
    std::string value = getArgFromBulkString(parsedReq[2]);

    bool insertAtFront = false;
    ssize_t numElements = dataStore.insertIntoStoreRange(key, value, insertAtFront);
    
    std::string response = ":" + std::to_string(numElements) + CLRF;
    return response;
}


std::string RESPHandler::handleLRange(std::vector<std::string> &parsedReq){
    if (parsedReq.size() != 4)
        return genError("wrong number of arguments for 'range' command");
    
    std::string key = getArgFromBulkString(parsedReq[1]);
    std::string startStr = getArgFromBulkString(parsedReq[2]);
    std::string endStr = getArgFromBulkString(parsedReq[3]);

    std::vector<std::string> &valueListRef = dataStore.getListRef(key);

    ssize_t start = stoi(startStr);
    ssize_t end = stoi(endStr);
    if (start < 0)
        start = valueListRef.size() + start;
    
    if (end < 0)
        end = valueListRef.size() + end;
    
    start = std::max(start, ssize_t(0));
    end = std::min(end, ssize_t(valueListRef.size()-1));

    ssize_t numElements = end - start + 1;

    if (numElements <= 0)
        return "*0\r\n";
    
    std::string response = "*" + std::to_string(numElements) + CLRF;
    for (int i=start; i<=end; i++){ 
        response += "$" + std::to_string(valueListRef[i].size()) + CLRF + valueListRef[i] + CLRF;
    }

    return response;
}


std::string RESPHandler::handleSAVE(std::vector<std::string> &parsedReq){
    std::cout << "Not Currently Working" << std::endl;
    return genError("Not Implemented");
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


std::string RESPHandler::handleFLUSHALL(std::vector<std::string> &parsedReq){
    if (parsedReq.size() != 1)
        return genError("syntax error");

    dataStore.redisCache.clear();
    return "+OK\r\n";
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
    else if (extractCommand == "INCR" || extractCommand == "DECR")
    {
        RESPResponse = handleINCRorDECR(parsedReq);
    }
    else if (extractCommand == "LPUSH")
    {
        RESPResponse = handleLPUSH(parsedReq);
    }
    else if (extractCommand == "LRANGE")
    {
        RESPResponse = handleLRange(parsedReq);
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
    else if (extractCommand == "FLUSHALL")
    {
        RESPResponse = handleFLUSHALL(parsedReq);
    }
    else
    {
        std::string message = "unknown command \'" + extractCommand + "\'";
        RESPResponse = genError(message);
    }

    return RESPResponse;
}

