#include <stdexcept>

#include "ResourceObject.hpp"

ResourceObject::ResourceObject(const std::string& fileName, const std::string& resName,
    uint16_t typeId) : fileName(fileName), resName(resName), typeId(typeId)
{
}

void ResourceObject::write(File& out) {
    readData();

    out.write16(typeId);

    if (resName.length() > UINT16_MAX)
        throw std::runtime_error("Resource name is too long (" + std::to_string(UINT16_MAX) +
            "): " + resName);
    out.write16(resName.length());
    out.write(std::vector<uint8_t>(resName.data(), resName.data() + resName.length()));

    if (data.size() > UINT32_MAX)
        throw std::runtime_error("Resource data is too long (" + std::to_string(UINT32_MAX) +
            "): " + fileName);
    out.write32(data.size());
    out.write(data);
}
