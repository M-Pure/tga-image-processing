# TGA Image Processing 

Reads binary TGA image file data, applies blending techniques to transform image pixels, and writes new images in the same form to output file.

## Features
**"Image"** refers to a binary TGA image file
**RGB channel values normalized to remain within 8-bit 0-255 limit upon blending**

Modify Channel: Adjust an image's RGB color channel by a constant value or using the RGB color channel of another image.
Modify Channels: Set all image RGB channels to a constant value. 
Multiply: Multiply RGB channel values of the product image by those of the factor image.
Subtract: Subtract RGB channel values of the minuend image by those of the subtrahend image.
Screen: Lightens images. Inverts base and top layers, Rmultiplies, and inverts the product.
Overlay: Increase contrast. Combine multiply and screen effects to brighten majority grey (screen) and darken minority grey (multiply).
Flip: 180 degree image rotation.
Combine4: Combine 4 images of equal n x n width and height dimensions into a larger 2n x 2n image.

## Implementation Details
**Header class** Contains preliminary TGA image file data, namely image width/height necessary for file reads.
**Pixel class** The most basic unit of an image file. Holds 8-bit R, G, and B unsigned char (values 0-255) channels.
**Image class** Has a header and pixel data. Easy serialize and deserialize of TGA images to/from desired files.  
**FileOperations** Collection of binary read/write methods for header and pixel data.

## System Requirements
* C++11 compliant compiler (i.e. GCC 4.8.1+)
* Build tool: GNU Make
* CL Environment

## Build Commands
