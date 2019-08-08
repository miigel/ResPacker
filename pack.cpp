#include "pack.hpp"

void pack(const std::vector<std::string>& inputFiles, const std::string& outputFile) {
    printf("Input files:\n");
    for (auto& i : inputFiles) {
        printf("%s\n", i.c_str());
    }
    printf("Output file: %s\n", outputFile.c_str());
}
