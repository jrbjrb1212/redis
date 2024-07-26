#pragma once

#define CLRF "\r\n"
#define dataSaveName "redis.json"

#include <vector>
#include <string>
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <unordered_map>
#include <filesystem>
#include <thread>
#include <fstream>
#include <nlohmann/json.hpp>
#include <atomic>

using json = nlohmann::json;