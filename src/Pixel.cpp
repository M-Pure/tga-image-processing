#include "Pixel.h"

Pixel::Pixel() :
    B(0), // Black
    G(0),
    R(0) {}

Pixel::Pixel(unsigned char b, unsigned char g, unsigned char r)
: B(b), G(g), R(r) {}
