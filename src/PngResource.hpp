#pragma once

#include "ResourceObject.hpp"

class PngResource : public ResourceObject {
public:
    PngResource(const std::string& fileName, const std::string& resName);

private:
    void readData() override;
};
