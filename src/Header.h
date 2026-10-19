#pragma once

struct Header {
    char idLength;
    char colorMapType;
    char dataTypeCode;
    short colorMapOrigin;
    short colorMapLength;
    char colorMapDepth;
    short xOrigin;
    short yOrigin;
    short imageWidth;
    short imageHeight;
    char bitsPerPixel;
    char imageDescriptor;

    Header();
};

