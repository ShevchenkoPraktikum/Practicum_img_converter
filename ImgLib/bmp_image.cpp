#include "bmp_image.h"
#include "pack_defines.h"

#include <array>
#include <fstream>
#include <string_view>
#include <vector>
#include <cstdint>

using namespace std;
using namespace std::filesystem;

namespace img_lib {

PACKED_STRUCT_BEGIN BitmapFileHeader {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
}
PACKED_STRUCT_END

PACKED_STRUCT_BEGIN BitmapInfoHeader {
    uint32_t biSize;
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
}
PACKED_STRUCT_END

// функция вычисления отступа по ширине
static int GetBMPStride(int width) {
    constexpr int BYTES_PER_PIXEL = 3;
    constexpr int ALIGNMENT = 4;
    int row_size_bytes = width * BYTES_PER_PIXEL;
    int stride = ((row_size_bytes + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT;
    return stride;
}

// напишите эту функцию
bool SaveBMP(const Path& file, const Image& image) {
    if (!image) {
        return false;
    }

    const int width = image.GetWidth();
    const int height = image.GetHeight();

    if (width <= 0 || height <= 0) {
        return false;
    }

    const int stride = GetBMPStride(width);
    const uint32_t data_size = static_cast<uint32_t>(stride) * static_cast<uint32_t>(height);
    const uint32_t file_size = 54 + data_size;

    BitmapFileHeader file_header{};
    BitmapInfoHeader info_header{};

    file_header.bfType = 0x4D42;
    file_header.bfSize = file_size;
    file_header.bfReserved1 = 0;
    file_header.bfReserved2 = 0;
    file_header.bfOffBits = 54;

    info_header.biSize = 40;
    info_header.biWidth = width;
    info_header.biHeight = height;
    info_header.biPlanes = 1;
    info_header.biBitCount = 24;
    info_header.biCompression = 0;
    info_header.biSizeImage = data_size;
    info_header.biXPelsPerMeter = 11811;
    info_header.biYPelsPerMeter = 11811;
    info_header.biClrUsed = 0;
    info_header.biClrImportant = 0x1000000;

    ofstream out(file, ios::binary);
    if (!out.is_open()) {
        return false;
    }

    out.write(reinterpret_cast<const char*>(&file_header), sizeof(file_header));
    out.write(reinterpret_cast<const char*>(&info_header), sizeof(info_header));

    if (!out) {
        return false;
    }

    vector<char> row_buffer(stride, 0);

    for (int y = height - 1; y >= 0; --y) {
        const Color* line = image.GetLine(y);

        for (int x = 0; x < width; ++x) {
            const auto& pixel = line[x];
            int offset = x * 3;

            row_buffer[offset + 0] = static_cast<char>(to_integer<int>(pixel.b));
            row_buffer[offset + 1] = static_cast<char>(to_integer<int>(pixel.g));
            row_buffer[offset + 2] = static_cast<char>(to_integer<int>(pixel.r));

        }

        out.write(row_buffer.data(), stride);
        if (!out) {
            return false;
        }
    }

    return out.good();
}

Image LoadBMP(const Path& file) {
    ifstream in(file, ios::binary);
    if (!in.is_open()) {
        return {};
    }

    BitmapFileHeader file_header;
    BitmapInfoHeader info_header;

    in.read(reinterpret_cast<char*>(&file_header), sizeof(file_header));
    in.read(reinterpret_cast<char*>(&info_header), sizeof(info_header));

    if (!in) {
        return {};
    } 

    if (file_header.bfType != 0x4D42) {
        return {};
    }

    if (info_header.biBitCount != 24) {
        return {};
    }

    int width = info_header.biWidth;
    int height = info_header.biHeight;

    bool is_top_down = (height < 0);
    height = std::abs(height);

    if (width <= 0 || height <= 0) {
        return {};
    }

    int stride = GetBMPStride(width);
    vector<char> row_buffer(stride);

    Image result(width, height, Color::Black());

    for (int i = 0; i < height; ++i) {
        in.read(row_buffer.data(), stride);
        if(!in) {
            return {};
        }

        int y;
        if (is_top_down) {
            y = i;
        } else {
            y = height - 1 - i;
        }

        Color* line = result.GetLine(y);
        for (int x = 0; x < width; ++x) {
            int offset = x * 3;
            if (offset + 2 >= stride) break;

            line[x] = Color{
                byte{static_cast<unsigned char>(row_buffer[offset + 2])},
                byte{static_cast<unsigned char>(row_buffer[offset + 1])},
                byte{static_cast<unsigned char>(row_buffer[offset + 0])},
                byte{255}  
            };
        }
    }

    return result;
}

}  // namespace img_lib