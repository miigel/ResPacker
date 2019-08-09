#include <stdexcept>

#include "File.hpp"

File::File(const std::string& fileName, bool isOutputFile) :
    fileName(fileName), isOutputFile(isOutputFile)
{
    if (isOutputFile) {
        fs.open(fileName, std::fstream::out | std::fstream::binary | std::fstream::trunc);
        if (!fs.is_open())
            throw std::runtime_error("Unable to write to location: " + fileName);
    } else {
        fs.open(fileName, std::fstream::in | std::fstream::binary);
        if (!fs.is_open())
            throw std::runtime_error("Unable to open file: " + fileName);
    }
}

void File::write(const std::vector<uint8_t>& v) {
    if (!isOutputFile)
        throw std::runtime_error("Write operation on input file: " + fileName);

    fs.write(reinterpret_cast<const char*>(v.data()), v.size());
    if (!fs.good())
        throw std::runtime_error("Write error: " + fileName);
}

void File::write8(uint8_t d) {
    write({d});
}

void File::write16(uint16_t d) {
    std::vector<uint8_t> v;
    auto p = reinterpret_cast<uint8_t*>(&d);
    v.assign(p, p + sizeof(d));
    write(v);
}

void File::write32(uint32_t d) {
    std::vector<uint8_t> v;
    auto p = reinterpret_cast<uint8_t*>(&d);
    v.assign(p, p + sizeof(d));
    write(v);
}

void File::write64(uint64_t d) {
    std::vector<uint8_t> v;
    auto p = reinterpret_cast<uint8_t*>(&d);
    v.assign(p, p + sizeof(d));
    write(v);
}

std::vector<uint8_t> File::read(int len) {
    if (isOutputFile)
        throw std::runtime_error("Read operation on output file: " + fileName);

    std::vector<uint8_t> v(len, 0);
    fs.read(reinterpret_cast<char*>(v.data()), len);
    if (fs.eof())
        throw std::runtime_error("Unexpected end of file: " + fileName);
    if (!fs.good())
        throw std::runtime_error("Read error: " + fileName);
    return v;
}

uint8_t File::read8() {
    return read(1).at(0);
}

uint16_t File::read16() {
    auto data = read(sizeof(uint16_t));
    return *reinterpret_cast<const uint16_t*>(data.data());
}

uint32_t File::read32() {
    auto data = read(sizeof(uint32_t));
    return *reinterpret_cast<const uint32_t*>(data.data());
}

uint64_t File::read64() {
    auto data = read(sizeof(uint64_t));
    return *reinterpret_cast<const uint64_t*>(data.data());
}

void File::seek(uint64_t pos) {
    if (isOutputFile)
        fs.seekp(pos);
    else
        fs.seekg(pos);
}
