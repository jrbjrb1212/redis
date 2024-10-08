#pragma once
#include "common.hpp"

struct storeKeyInfo {
    bool keyPresent;
    bool listType;
    std::string value;
};

class RedisStore {
    private:
        // TODO: probably need to make value a doubly linked list 
        ssize_t currBytes = 0;
        ssize_t maxBytes = -1;
        size_t cacheHits = 0;
        size_t cacheMisses = 0;
        std::vector<std::string> emptyVec;
        
        bool checkCacheIsOnDisk();
        void loadCacheFromDisk();

    public:        
        RedisStore(ssize_t maxReqBytes){
            maxBytes = maxReqBytes;
        };

        // TODO: add a check to load the cache from disk or create a new one
        RedisStore(){
            if (checkCacheIsOnDisk()){
                loadCacheFromDisk();
            }
        };

        // TODO: make all of these references to avoid string copying
        std::unordered_map<std::string, std::vector<std::string>> redisCache;
        size_t saveTimer = 5; // default create snap shot every 5 seconds 
        
        bool setStoreKey(std::string &key, std::string &value);
        storeKeyInfo getStoreKey(std::string &key);
        bool checkStoreKey(std::string key);
        bool delStoreKey(std::string key);
        storeKeyInfo updateStoreValue(std::string key, size_t valUpdate); 
        ssize_t insertIntoStoreRange(std::string &key, std::string &value, bool leftOrRight);
        std::vector<std::string>& getListRef(std::string &key);
        bool writeCacheToDisk();
        std::unordered_map<std::string, ssize_t> getStoreStats(); 
        void ping(); 
        ssize_t size();        
};