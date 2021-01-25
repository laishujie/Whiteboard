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

    ImageInfo() {
        width = 0;
        height = 0;
        pixels = nullptr;
    }

    ImageInfo(int width, int height, unsigned char *pixels) : width(width), height(height),
                                                              pixels(pixels) {}
    ~ImageInfo() {
       /* if (pixels != nullptr) {
            free(pixels);
            pixels= nullptr;
            LOGE("11111","free(pixels)")
        }*/
    };

};

#endif //OPENGLDEMO_IMAGEINFO_H
