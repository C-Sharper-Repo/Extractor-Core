#pragma once
#include <string>

namespace ExCore::IO {
    
    std::string openFile(const char* filter, const size_t maxPathLen = 260);
    std::string openFolder(const char* title, const size_t maxPathLen = 260);

}