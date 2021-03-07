//
// Created by Lai on 2020/12/1.
//

#ifndef OPENGLDEMO_IMAGEINFO_H
#define OPENGLDEMO_IMAGEINFO_H

#include <cstdlib>


class ImageInfo {
public:
    int width, height;
    unsigned char *pixels;
    //std::string brushId;

    ImageInfo() : width(0), height(0), pixels(nullptr)/*,brushId(nullptr)*/ {
    }

    ImageInfo(int width, int height, unsigned char *pixels) : width(width), height(height),
                                                              pixels(pixels)/*,brushId(brushId) */{}

    ~ImageInfo() {
        if (pixels != nullptr) {
            free(pixels);
            pixels = nullptr;
            //LOGE("11111", "free(pixels)")
        }
    };
};

class BrushInfo {

public:
    typedef enum {
        DRAW = 0,
        ERASER = 1
    } OutType;

    BrushInfo() : brushSize(15.0f), outType(DRAW), imageInfo(nullptr),
                  R(0.0f), G(0.0f), B(0.0f), A(1.0f), isTextureRotate(true) {}

    ~BrushInfo() {
        if (imageInfo != nullptr) {
            delete imageInfo;
            imageInfo = nullptr;
        }
    }

    float brushSize;
    OutType outType;
    ImageInfo *imageInfo;
    float R;
    float G;
    float B;
    float A;
    bool isTextureRotate;
};

#endif //OPENGLDEMO_IMAGEINFO_H
