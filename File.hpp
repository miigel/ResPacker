#pragma once

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

class File {
public:
    File(const std::string& fileName, bool isOutputFile);
    void write(const std::vector<uint8_t>&);
    void write8(uint8_t);
    void write16(uint16_t);
    void write32(uint32_t);
    void write64(uint64_t);
    std::vector<uint8_t> read(int len);
    uint8_t read8();
    uint16_t read16();
    uint32_t read32();
    uint64_t read64();
    void seek(uint64_t pos);

private:
    std::fstream fs;
    std::string fileName;
    bool isOutputFile = false;
};
