#include "redis_store.hpp"


bool RedisStore::setStoreKey(std::string &key, std::string &value){
    redisCache[key] = {value};
    return true;
}


// TODO: A better solution not using a bandaid struct exists
storeKeyInfo RedisStore::getStoreKey(std::string &key){
    storeKeyInfo value;
    
    if (redisCache.find(key) == redisCache.end())
    {
        value.keyPresent = false;
        return value;
    }

    if (redisCache[key].size() > 1)
    {
        value.keyPresent = true;
        value.listType = true;
    }
    else
    {
        value.keyPresent = true;
        value.listType = false;
        value.value = redisCache[key][0];
    }
    
    return value;
}


// True -> key is in redis db
// False -> key is not in redis db
bool RedisStore::checkStoreKey(std::string key){
    if (redisCache.find(key) != redisCache.end())
        return true;
    
    return false;
}


// will work on a key that points to a single value or list of values
bool RedisStore::delStoreKey(std::string key){
    // cannot delete a key that is not in the redis db
    if (redisCache.find(key) == redisCache.end())
        return false;
    
    // deallocate both the key and the vector of values
    redisCache[key].clear();
    redisCache.erase(key);
    return true;
}


storeKeyInfo RedisStore::updateStoreValue(std::string key, size_t valUpdate){
    storeKeyInfo keyInfo = getStoreKey(key);

    if (keyInfo.listType == true)
        return keyInfo;

    ssize_t newVal;
    if (!keyInfo.keyPresent)
        newVal = valUpdate;
    else
        newVal = stoi(keyInfo.value) + valUpdate;

    std::string newValStr = std::to_string(newVal);
    setStoreKey(key, newValStr);
    keyInfo.value = newValStr;

    return keyInfo;
} 


std::vector<std::string>& RedisStore::getListRef(std::string &key){
    if (checkStoreKey(key))
        return redisCache[key];
    
    return emptyVec;
}


ssize_t RedisStore::insertIntoStoreRange(std::string &key, std::string &value, bool leftInsert){
    storeKeyInfo keyInfo = getStoreKey(key);
    
    // if the key does not exist, create a new one 
    if (!keyInfo.keyPresent)
    {
        setStoreKey(key, value);
        return 1;
    }

    std::vector<std::string> &valueListRef = getListRef(key);

    // if the key does exist, insert at the front or back of vector
    if (leftInsert)
        valueListRef.insert(valueListRef.begin(), value);
    else
        valueListRef.push_back(value);

    return valueListRef.size();
}


// Redis database dumps are called dump.rbd
// TODO: Can add onto this by saving to a user defined location
bool RedisStore::checkCacheIsOnDisk(){
    std::filesystem::path currentPath = std::filesystem::current_path();
    
    std::filesystem::path filePath = currentPath / "dump.rbd";
    
    return std::filesystem::exists(filePath);
}

// std::unordered_map<std::string, std::string> RedisStore::loadCacheFromDisk();
// bool RedisStore::writeCacheToDisk();
// bool RedisStore::saveStore();


// Return stats about the current size of the cache and 
// - interactions with the cache
std::unordered_map<std::string, ssize_t> RedisStore::getStoreStats(){
    std::unordered_map<std::string, ssize_t> storeStats;
    storeStats["Cache Hits"] = cacheHits;
    storeStats["Cache Misses"] = cacheMisses;
    storeStats["Total Keys"] = redisCache.size();
    storeStats["Current Bytes"] = currBytes;
    
    return storeStats;
}


// TODO: delete later
void RedisStore::ping(){
    std::cout << "Store PING" << std::endl;
}

ssize_t RedisStore::size(){
    return redisCache.size();
} 