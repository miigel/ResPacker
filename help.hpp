#pragma once

#include <string>

#include "version.hpp"

const std::string HelpText =
    "Resource packer v" VersionString "\n"
    "\n"
    "Usage: respack INPUT_FILES... OUTPUT_FILE\n"
    "-h, --help: Print this help\n"
    "-v, --version: Print version\n"
    "\n"
    "Supported resource files:\n"
    "png    Must be in 32bit RGBA format\n"
;
