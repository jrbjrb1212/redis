#include "resp_handler.hpp"


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
    std::cout << "Not Currently Working" << std::endl;
}


std::string RESPHandler::handleSET(std::vector<std::string> &parsedReq){
    std::cout << "Not Currently Working" << std::endl;
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


// Routing function
// - takes a vector of parsed arguments from the redis-cli
// - the first argument is always  
std::string RESPHandler::handleMsg(std::vector<std::string> &parsedReq){
    std::string &command = parsedReq[0];
    std::string RESPResponse;
    size_t firstCLDF, secCLDF;
    firstCLDF = command.find(CLRF);
    secCLDF = command.find(CLRF, firstCLDF+1);

    // std::cout << "left pos: " <<  firstCLDF+2 << ", right pos: " <<  secCLDF << std::endl;
    // std::cout << command.substr(firstCLDF+2, secCLDF - 2 - firstCLDF) << std::endl;
    
    std::string extractCommand = command.substr(firstCLDF+2, secCLDF - 2 - firstCLDF);
    std::transform(extractCommand.begin(), extractCommand.end(), extractCommand.begin(), ::toupper); 
    
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

    return RESPResponse;
}

