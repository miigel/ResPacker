#include "pack.hpp"
#include "File.hpp"

static uint16_t FileFormatVersion = 1;

void pack(const std::vector<std::string>& inputFiles, const std::string& outputFile) {
    auto out = File(outputFile, true);

    out.write16(FileFormatVersion);
    out.write16(0); // Number of resource objects. Fill later

    for (auto& i : inputFiles) {
        auto in = File(i, false);
    }

    // Write the number of resource objects
    out.seek(0x2);
    out.write16(0);
}
