#pragma once

#include <stdexcept>

using Iterator = std::istreambuf_iterator<char>;

class File
{
public:
    static std::string read_file(const std::string& filename) {
        std::ifstream file(filename, std::ios_base::binary | std::ios_base::in);
        if (!file.is_open())
            throw std::runtime_error("Failed to open " + filename);
        std::string content(Iterator{file}, Iterator{});
        if (!file)
            throw std::runtime_error("Failed to read " + filename);
        return content;
    }

    static float read_float(std::ifstream& filestream) {
        float value;
        if(!(filestream >> value)) {
            throw std::runtime_error("can't read file");
        }
        return value;
    }
};
