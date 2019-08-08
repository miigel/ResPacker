#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "File.hpp"

class ResourceObject {
public:
    void write(File& out);

protected:
    ResourceObject(const std::string& fileName, const std::string& resName, uint16_t typeId);
    const std::string fileName;
    std::vector<uint8_t> data;

private:
    virtual void readData() = 0;
    const std::string resName;
    const uint16_t typeId;
};
