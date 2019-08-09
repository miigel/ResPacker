#include <stdexcept>

#include <png.h>

#include "PngResource.hpp"

struct ErrorStruct {
    std::string fileName;
};

class LoadContext {
public:
    LoadContext(const std::string& fileName) {
        errorStruct.fileName = fileName;
    }

    ~LoadContext() {
        png_destroy_read_struct(&pngRead, &pngInfo, nullptr);

        if (pixelRows) {
            for (int y = 0; y < height; y++) {
                if (!pixelRows[y])
                    continue;
                delete[] pixelRows[y];
            }
            delete[] pixelRows;
        }
    }

    void allocatePixelData(int height) {
        pixelRows = new png_bytep[sizeof(png_bytep) * height];
        for (int y = 0; y < height; y++) {
            pixelRows[y] = new png_byte[png_get_rowbytes(pngRead, pngInfo)];
        }
        this->height = height;
    }

    ErrorStruct errorStruct;
    png_structp pngRead = nullptr;
    png_infop pngInfo = nullptr;
    png_bytep* pixelRows = nullptr;
    int height = 0;
};

static void readFn(png_struct* pngRead, uint8_t* data, png_size_t len) {
    auto file = reinterpret_cast<File*>(png_get_io_ptr(pngRead));
    auto data_ = file->read(len);
    memcpy(data, data_.data(), len);
}

// Libpng error handling is absurd. Safest way to handle errors is to just exit here
static void errorFn(png_struct* pngRead, const char*) {
    auto errorStruct = reinterpret_cast<ErrorStruct*>(png_get_error_ptr(pngRead));
    fprintf(stderr, "Error: Failed to load PNG image: %s\n", errorStruct->fileName.c_str());
    exit(1);
}

PngResource::PngResource(const std::string& fileName, const std::string& resName) :
    ResourceObject(fileName, resName, 0)
{
}

void PngResource::readData() {
    auto file = File(fileName, false);

    auto signature = file.read(8);
    if (!png_check_sig(signature.data(), signature.size()))
        throw std::runtime_error("Invalid PNG signature: " + fileName);

    LoadContext loadContext(fileName);

    loadContext.pngRead = png_create_read_struct(PNG_LIBPNG_VER_STRING, &loadContext.errorStruct,
        errorFn, nullptr);
    if (!loadContext.pngRead)
        throw std::runtime_error("Failed to create PNG read struct: " + fileName);

    loadContext.pngInfo = png_create_info_struct(loadContext.pngRead);
    if (!loadContext.pngInfo)
        throw std::runtime_error("Failed to create PNG info struct: " + fileName);

    png_set_read_fn(loadContext.pngRead, &file, readFn);
    png_set_sig_bytes(loadContext.pngRead, signature.size());
    png_read_info(loadContext.pngRead, loadContext.pngInfo);

    uint32_t width = png_get_image_width(loadContext.pngRead, loadContext.pngInfo);
    uint32_t height = png_get_image_height(loadContext.pngRead, loadContext.pngInfo);

    if (png_get_color_type(loadContext.pngRead, loadContext.pngInfo) != PNG_COLOR_TYPE_RGBA ||
        png_get_bit_depth(loadContext.pngRead, loadContext.pngInfo) != 8)
    {
        throw std::runtime_error("Invalid PNG format. Only 32bit RGBA images allowed: " + fileName);
    }

    png_read_update_info(loadContext.pngRead, loadContext.pngInfo);

    loadContext.allocatePixelData(height);

    png_read_image(loadContext.pngRead, loadContext.pixelRows);

    data.insert(data.end(), reinterpret_cast<uint8_t*>(&width),
        reinterpret_cast<uint8_t*>(&width) + sizeof(width));
    data.insert(data.end(), reinterpret_cast<uint8_t*>(&height),
        reinterpret_cast<uint8_t*>(&height) + sizeof(height));

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            const auto pixel = &loadContext.pixelRows[y][x * 4];
            data.push_back(pixel[3]);
            data.push_back(pixel[2]);
            data.push_back(pixel[1]);
            data.push_back(pixel[0]);
        }
    }
}
