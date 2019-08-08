#include <cstdio>
#include <memory>
#include <stdexcept>

#include "pack.hpp"
#include "File.hpp"

#include "PngResource.hpp"

static uint16_t FileFormatVersion = 1;

static std::string getFileBaseName(const std::string& fileName) {
    auto pos = fileName.find_last_of("\\/");
    if (pos == std::string::npos)
        return fileName;

    if (pos == fileName.length() - 1)
        throw std::runtime_error("Invalid file name: " + fileName);

    return fileName.substr(pos + 1);
}

static void parseFileName(const std::string& fileName, std::string& resName,
    std::string& fileExtension)
{
    auto baseName = getFileBaseName(fileName);

    auto dotPos = baseName.find_last_of('.');
    if (dotPos == std::string::npos || dotPos == baseName.length() - 1)
        throw std::runtime_error("File has no extension: " + fileName);

    if (dotPos == 0)
        throw std::runtime_error("Invalid file name: " + fileName);

    resName = baseName.substr(0, dotPos);
    fileExtension = baseName.substr(dotPos + 1);
}

static void strToLowerCase(std::string& str) {
    for (auto& i : str) {
        i = tolower(i);
    }
}

static std::unique_ptr<ResourceObject> selectResObject(const std::string& fileName) {
    std::string resName;
    std::string fileExtension;
    parseFileName(fileName, resName, fileExtension);

    strToLowerCase(fileExtension);
    if (fileExtension == "png") return std::make_unique<PngResource>(fileName, resName);

    throw std::runtime_error("Unsupported resource: " + fileName);
}

void pack(const std::vector<std::string>& inputFiles, const std::string& outputFile) {
    auto out = File(outputFile, true);

    out.write16(FileFormatVersion);
    out.write32(0); // Number of resource objects. Fill later

    uint32_t numResObjects = 0;

    for (auto& i : inputFiles) {
        printf("Packing %s\n", i.c_str());
        auto resObject = selectResObject(i);
        if (numResObjects == UINT32_MAX)
            throw std::runtime_error("Maximum number of resource objects reached (" +
                std::to_string(UINT32_MAX) + ")");
        resObject->write(out);
        numResObjects++;
    }

    // Write the number of resource objects
    out.seek(0x2);
    out.write32(numResObjects);

    printf("\nResource pack file %s successfully created\n", outputFile.c_str());
}
