#include <fstream>
#include "FileOperations.h"
#include "Header.h"
using namespace std;


/* Parameter 1: Binary input file stream of .tga file
 * Reads first 18 bytes of the .tga file located at filePath
 * into the returned header struct */
Header GetFileHeader(ifstream& inFile) {
    Header header;
    inFile.read(&header.idLength, sizeof(header.idLength));
    inFile.read(&header.colorMapType, sizeof(header.colorMapType));
    inFile.read(&header.dataTypeCode, sizeof(header.dataTypeCode));
    inFile.read(reinterpret_cast<char*>(&header.colorMapOrigin), sizeof(header.colorMapOrigin));
    inFile.read(reinterpret_cast<char*>(&header.colorMapLength), sizeof(header.colorMapLength));
    inFile.read(&header.colorMapDepth, sizeof(header.colorMapDepth));
    inFile.read(reinterpret_cast<char*>(&header.xOrigin), sizeof(header.xOrigin));
    inFile.read(reinterpret_cast<char*>(&header.yOrigin), sizeof(header.yOrigin));
    inFile.read(reinterpret_cast<char*>(&header.imageWidth), sizeof(header.imageWidth));
    inFile.read(reinterpret_cast<char*>(&header.imageHeight), sizeof(header.imageHeight));
    inFile.read(&header.bitsPerPixel, sizeof(header.bitsPerPixel));
    inFile.read(&header.imageDescriptor, sizeof(header.imageDescriptor));
    return header;
}


/* Parameter 1: Binary input file stream of .tga file
 * Parameter 2: (Output parameter) Stores all .tga file pixels
 * Parameter 3: Header struct containing .tga image Width and Height
 * Read all file data (pixels) after header into pixel objects
 * stored in Parameter 2 */
void GetFilePixels(ifstream& inFile, const Header& header, vector<Pixel>& output) {
    for (int i = 0; i < header.imageWidth * header.imageHeight; i++) {
        Pixel pixel;
        inFile.read(reinterpret_cast<char*>(&pixel.B), sizeof(pixel.B));
        inFile.read(reinterpret_cast<char*>(&pixel.G), sizeof(pixel.G));
        inFile.read(reinterpret_cast<char*>(&pixel.R), sizeof(pixel.R));
        output.push_back(pixel);
    }
}


/* Parameter 1: Binary output file stream - new .tga file
 * Parameter 2: File header information stored in Header struct
 * Writes 18-byte header information into Parameter 2 .tga file stream,
 * returning true if successful, false otherwise */
bool WriteHeader(ofstream& outFile, Header& header) {
    if (outFile.is_open()) {
        try {
            outFile.write(&header.idLength, sizeof(header.idLength));
            outFile.write(&header.colorMapType, sizeof(header.colorMapType));
            outFile.write(&header.dataTypeCode, sizeof(header.dataTypeCode));
            outFile.write(reinterpret_cast<char*>(&header.colorMapOrigin), sizeof(header.colorMapOrigin));
            outFile.write(reinterpret_cast<char*>(&header.colorMapLength), sizeof(header.colorMapLength));
            outFile.write(&header.colorMapDepth, sizeof(header.colorMapDepth));
            outFile.write(reinterpret_cast<char*>(&header.xOrigin), sizeof(header.xOrigin));
            outFile.write(reinterpret_cast<char*>(&header.yOrigin), sizeof(header.yOrigin));
            outFile.write(reinterpret_cast<char*>(&header.imageWidth), sizeof(header.imageWidth));
            outFile.write(reinterpret_cast<char*>(&header.imageHeight), sizeof(header.imageHeight));
            outFile.write(&header.bitsPerPixel, sizeof(header.bitsPerPixel));
            outFile.write(&header.imageDescriptor, sizeof(header.imageDescriptor));
            return true;
        } catch (exception e) { return false; }
    }
    return false;
}


/* Parameter 1: Binary output file stream - .tga file with header already written
 * Parameter 2: Size = header.imageHeight * header.imageWidth
 * Writes pixels to .tga file that matches header height/width specifications */
bool WritePixels(ofstream& outFile, vector<Pixel>& pixels) {
    if (outFile.is_open()) {
        try {
            for (int i = 0; i < pixels.size(); i++) {
                outFile.write(reinterpret_cast<char*>(&pixels[i].B), sizeof(pixels[i].B));
                outFile.write(reinterpret_cast<char*>(&pixels[i].G), sizeof(pixels[i].G));
                outFile.write(reinterpret_cast<char*>(&pixels[i].R), sizeof(pixels[i].R));
            }
            return true;
        } catch (exception e) { return false; }
    }
    return false;
}