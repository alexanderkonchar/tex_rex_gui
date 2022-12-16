//
// Created by Alex on 12/7/22.
//

#include "text_extractor.h"

void text_extractor::extract_text_from_image(const std::string &image_path, char* &outText) {
    auto *api = new tesseract::TessBaseAPI();
    // Initialize tesseract-ocr with English, without specifying tessdata path
    if (api->Init(nullptr, "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }

    // Open input image with leptonica library
    Pix *image = pixRead(image_path.c_str());
    api->SetImage(image);
    // Get OCR result
    outText = api->GetUTF8Text();
    // Destroy used object and release memory
    api->End();
    delete api;
    pixDestroy(&image);
}
