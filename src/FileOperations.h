#pragma once

#include <fstream>
#include <vector>
#include "Header.h"
#include "Pixel.h"
using namespace std;


/* Parameter 1: Binary input file stream of .tga file
 * Reads first 18 bytes of the passed in .tga file stream
 * into the returned header struct */
Header GetFileHeader(ifstream& inFile);


/* Parameter 1: Binary input file stream of .tga file
 * Parameter 2: (Output parameter) Stores all .tga file pixels
 * Parameter 3: Header struct containing .tga image Width and Height
 * Read all file data (pixels) - after header - into pixel objects
 * stored in Parameter 2 */
void GetFilePixels(ifstream& inFile, const Header& header, vector<Pixel>& output);


/* Parameter 1: Binary output file stream - new .tga file
 * Parameter 2: File header information stored in Header struct
 * Writes 18-byte header information into passed in .tga file stream,
 * returning true if successful, false otherwise */
bool WriteHeader(ofstream& outFile, Header& header);


/* Parameter 1: Binary output file stream - .tga file with header already written
 * Parameter 2: Size = header.imageHeight * header.imageWidth
 * Writes pixels to .tga file that matches header height/width specifications
 * returning true if successful, false otherwise */
bool WritePixels(ofstream& outFile, vector<Pixel>& pixels);