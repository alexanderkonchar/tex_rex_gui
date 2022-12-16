//
// Created by Alex on 12/7/22.
//

#ifndef BUILD_TEXT_EXTRACTOR_H
#define BUILD_TEXT_EXTRACTOR_H


#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

class text_extractor {
public:
    void extract_text_from_image(const std::string &image_path, char* &outText);
};


#endif //BUILD_TEXT_EXTRACTOR_H
