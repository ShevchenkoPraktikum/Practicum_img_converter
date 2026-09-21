#include <img_lib.h>
#include <jpeg_image.h>
#include <ppm_image.h>
#include <bmp_image.h>

#include <filesystem>
#include <string_view>
#include <iostream>

using namespace std;
using namespace std::filesystem;

namespace img_lib {
    enum class Format {
        UNKNOWN,
        PPM,
        JPEG,
        BMP
    };
}

namespace FormatInterfaces {
    class ImageFormatInterface {
    public:
        virtual ~ImageFormatInterface() = default;
        virtual bool SaveImage(const img_lib::Path& file, const img_lib::Image& image) const = 0;
        virtual img_lib::Image LoadImage(const img_lib::Path& file) const = 0;
    };

    class PPMFormat : public ImageFormatInterface {
    public:
        bool SaveImage(const img_lib::Path& file, const img_lib::Image& image) const override {
            return img_lib::SavePPM(file, image);
        }

        img_lib::Image LoadImage(const img_lib::Path& file) const override {
            return img_lib::LoadPPM(file);
        }
    };

    class JPEGFormat : public ImageFormatInterface {
        public:
        bool SaveImage(const img_lib::Path& file, const img_lib::Image& image) const override {
            return img_lib::SaveJPEG(file, image);
        }

        img_lib::Image LoadImage(const img_lib::Path& file) const override {
            return img_lib::LoadJPEG(file);
        }
    };

    class BMPFormat : public ImageFormatInterface {
        public:
        bool SaveImage(const img_lib::Path& file, const img_lib::Image& image) const override {
            return img_lib::SaveBMP(file, image);
        }

        img_lib::Image LoadImage(const img_lib::Path& file) const override {
            return img_lib::LoadBMP(file);
        }
    };
} // namespace FormatInterfaces

img_lib::Format GetFormatByExtension(const img_lib::Path& input_file) {
    const string ext = input_file.extension().string();
    if (ext == ".jpg"sv || ext == ".jpeg"sv) {
        return img_lib::Format::JPEG;
    }

    if (ext == ".ppm"sv) {
        return img_lib::Format::PPM;
    }

    if (ext == ".bmp"sv) {
        return img_lib::Format::BMP;
    }

    return img_lib::Format::UNKNOWN;
}

const FormatInterfaces::ImageFormatInterface* GetFormatInterface(const img_lib::Path& path) {
    static const FormatInterfaces::PPMFormat ppmInterface;
    static const FormatInterfaces::JPEGFormat jpegInterface;
    static const FormatInterfaces::BMPFormat bmpInterface;

    switch (GetFormatByExtension(path)) {
        case img_lib::Format::PPM:
            return &ppmInterface;
        case img_lib::Format::JPEG:
            return &jpegInterface;
        case img_lib::Format::BMP:
            return &bmpInterface;
        default:
            return nullptr;
    }
}

int main(int argc, const char** argv) {
    if (argc != 3) {
        cerr << "Usage: "sv << argv[0] << " <in_file> <out_file>"sv << endl;
        return 1;
    }

    img_lib::Path in_path = argv[1];
    img_lib::Path out_path = argv[2];

    const auto* inInterface = GetFormatInterface(in_path);
    if (!inInterface) {
        cerr << "Unknown format of the input file"sv << endl;
        return 2;
    }

    const auto* outInterface = GetFormatInterface(out_path);
    if (!outInterface) {
        cerr << "Unknown format of the output file"sv << endl; 
        return 3;
    }

    img_lib::Image image = inInterface->LoadImage(in_path);
    if(!image) {
        cerr << "Loading failed"sv << endl;
        return 4;
    }

    if (!outInterface->SaveImage(out_path, image)) {
        cerr << "Saving failed"sv << endl;
        return 5;
    }

    cout << "Successfully converted"sv << endl;
    return 0;
}