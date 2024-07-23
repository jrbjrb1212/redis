#include "resp_parser.hpp"



// Assume each RESP protocol request is an array of bulk strings
// "Clients send commands to a Redis Server as a RESP Array of Bulk Strings"
// TODO: Version 1 only includes RESP2 protocol data types
std::vector<std::string> RESPParser::deserialize(std::string message){
    std::vector<std::string> out;
    
    // TODO: generate an error message
    if (message.size() == 0)
        return out;

    // if first byte is not a * (array), return error
    std::string firstByte (1, message[0]);

    // TODO: generate an error message as we assume client request is array of bulk strings
    if (firstByte != "*")
        return out;
    
    size_t firstCLRF = message.find(CLRF);
    size_t arraySize = stoi(message.substr(1, firstCLRF-1));
    size_t dollarSign = message.find("$");

    for (int i=0; i<arraySize; i++)
    {
        size_t leftPos = dollarSign+1;
        size_t rightPos = message.find("$", leftPos);
        
        // Last entry in the array
        if (rightPos == std::string::npos)
            rightPos = message.size();
        size_t leftOffset = rightPos - leftPos;

        std::string encodedMsg = message.substr(leftPos, leftOffset);
        out.push_back(encodedMsg);
        dollarSign = rightPos;
    }

    return out;
}

// Wrote the wrong deserialization method
/* 
std::vector<std::string> RESPParser::deserialize(std::string message){
    std::vector<std::string> out;
    
    // TODO: generate an error message
    if (message.size() == 0)
        return out;

    // first byte determines the data type
    // - if the first byte is a simple type, just need to search for CLRF
    // - if the first byte is an aggregate type, need to split by mutliple CLRF
    std::string firstByte (1, message[0]);
    size_t messageEnd = message.find(CLRF);

    // if CLRF is not at the end of the string, it must be an aggregate type
    if (messageEnd != message.size() - 2)
    {
        // Bulk string type
        if (firstByte == "$")
        {
            // $5\r\nHello\r\n
            // len: 10
            // want: [4, 8]
            size_t leftPos = messageEnd + 2;
            size_t leftOffset = (message.size()-2) - (leftPos);
            std::string encodedMsg = message.substr(messageEnd+2, leftOffset);
            out = {firstByte, encodedMsg};
        }

        // Arrays
        // - Currently supports arrays of simple data types
        // - TODO: Support nested aggregate types such as a array in an array
        else if (firstByte == "*")
        {   
            size_t numItems = stoi(message.substr(1, messageEnd-1));
            for (int i=0; i<numItems; i++)
            {
                size_t nextCLRF = message.find(CLRF, messageEnd+2);
                size_t leftOffset = (nextCLRF) - (messageEnd+2);
                std::string encodedMsg = message.substr(messageEnd+2, leftOffset);
                
                // std::cout << "message size: " << encodedMsg.size() << ", parsed message: " << encodedMsg << std::endl;
                out.push_back(encodedMsg);
                messageEnd = nextCLRF;
            }
            // std::cout << "Arrays not implemented yet" << std::endl;
        }
    }
    else
    {
        std::string encodedMsg = message.substr(1, messageEnd);
        out = {firstByte, encodedMsg};
    }


    return out;
}
*/

// Assume each RESP protocol response is a string
// - May send a simple string like +OK\r\n.
// - May send a large string like $<length>\r\n<data>\r\n
// - May send a array like *<number-of-elements>\r\n<element-1>...<element-n>
//      - Ex: *2\r\n$5\r\nhello\r\n$5\r\nworld\r\n
std::string RESPParser::serialize(){
    std::string out;
    std::cout << "Serializer is not implemented!!!"<< std::endl;

    return out;
}


// int main(){
//     RESPParser parser;
//     std::vector<std::string> out;
//     /* Error message */
//     // std::string message = "-Error message\r\n";

//     /* Simple String */
//     // std::string message = "+hello world\r\n";

//     /* Integer */
//     // std::string message = ":619\r\n";

//     /* Bulk string*/
//     // std::string message = "$5\r\nHello\r\n";

//     /* Arrays */
//     // std::string message = "*2\r\n$5\r\nhello\r\n$5\r\nworld\r\n";
//     // std::string message = "*3\r\n:1\r\n:2\r\n:3\r\n";
//     // std::string message = "*5\r\n:1\r\n:2\r\n:3\r\n:4\r\n$5\r\nhello\r\n";
    
//     /* Input Redis Command: */
//     // std::string message = "*3\r\n$3\r\nSET\r\n$3\r\nkey\r\n$5\r\nvalue\r\n";
//     // std::string message = "*3\r\n$5\r\nLPUSH\r\n$6\r\nmylist\r\n$5\r\nitem1\r\n";
//     std::string message = "*4\r\n$6\r\nLRANGE\r\n$6\r\nmylist\r\n$1\r\n0\r\n$2\r\n-1\r\n";

//     out = parser.deserialize(message);
//     if (!out.empty())
//         printVector(message, out);
// }