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
        size_t saveTimer = 300; // default create snap shot every 5 minutes 
        size_t cacheHits = 0;
        size_t cacheMisses = 0;
        
        bool checkCacheIsOnDisk();
        std::unordered_map<std::string, std::string> loadCacheFromDisk();
        bool writeCacheToDisk();

    public:        
        RedisStore(ssize_t maxReqBytes){
            maxBytes = maxReqBytes;
        };

        // TODO: add a check to load the cache from disk or create a new one
        RedisStore(){

        };

        std::unordered_map<std::string, std::vector<std::string>> redisCache;
        bool setStoreKey(std::string &key, std::string &value);
        storeKeyInfo getStoreKey(std::string &key);
        bool checkStoreKey(std::string key);
        bool delStoreKey(std::string key);
        storeKeyInfo updateStoreValue(std::string key, size_t valUpdate); 
        bool saveStore();
        std::unordered_map<std::string, ssize_t> getStoreStats(); 
        void ping(); 
        ssize_t size();        
};