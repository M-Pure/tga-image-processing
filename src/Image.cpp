#include <fstream>
#include <iostream>
#include <vector>
#include "FileOperations.h"
#include "Header.h"
#include "Image.h"
#include "Pixel.h"


Image::Image() {
    _hasHeader = false;
}

Image::Image(Header imageHeader) {
    _imageHeader = imageHeader;
    _hasHeader = true;
}

Image::Image(Header& imageHeader, vector<Pixel>& pixels) {
    _imageHeader = imageHeader;
    _pixels = pixels;
    _hasHeader = true;
}

Header Image::GetHeader() const {
    return _imageHeader;
}

vector<Pixel> Image::GetPixels() const {
    return _pixels;
}

// Get all values of "R" or "G" or "B" and store in output parameter
void Image::GetChannel(const string& channel, vector<unsigned char>& output) {
    // Edge case: No data to retrieve
    if (_pixels.size() == 0) {
        throw runtime_error("GetChannel ERROR: Cannot retrieve channel without pixels");
    }

    if (channel == "B") {
        for (int i = 0; i < _pixels.size(); i++) {
            output.push_back(_pixels[i].B);
        }
    }
    else if (channel == "G") {
        for (int i = 0; i < _pixels.size(); i++) {
            output.push_back(_pixels[i].G);
        }
    }
    else if (channel == "R") {
        for (int i = 0; i < _pixels.size(); i++) {
            output.push_back(_pixels[i].R);
        }
    }
    else {
        throw runtime_error("GetChannel ERROR: Unknown channel type");
    }
}

/* Set all values of "R" or "G" or "B" based on input channel
* if no pixels exist, create pixels using the input channel and
* with other channels each set to 0 */
void Image::SetChannel(const string& channel, vector<unsigned char>& input) {
    // Edge case: No data to modify - create pixels using input channel with other channels set to 0
    if (_pixels.size() == 0) {
        if (channel == "B") {
            for (int i = 0; i < input.size(); i++) {
                Pixel pixel;
                pixel.B = input[i];
                pixel.G = 0;
                pixel.R = 0;
                _pixels.push_back(pixel);
            }
            return;
        }
        if (channel == "G") {
            for (int i = 0; i < input.size(); i++) {
                Pixel pixel;
                pixel.B = 0;
                pixel.G = input[i];
                pixel.R = 0;
                _pixels.push_back(pixel);
            }
            return;
        }
        if (channel == "R") {
            for (int i = 0; i < input.size(); i++) {
                Pixel pixel;
                pixel.B = 0;
                pixel.G = 0;
                pixel.R = input[i];
                _pixels.push_back(pixel);
            }
            return;
        }
        else {
            throw runtime_error("SetChannel ERROR: Unknown channel type");
        }
    }
    if (channel == "B") {
        for (int i = 0; i < _pixels.size(); i++) {
            _pixels[i].B = input[i];
        }
    }
    else if (channel == "G") {
        for (int i = 0; i < _pixels.size(); i++) {
            _pixels[i].G = input[i];
        }
    }
    else if (channel == "R") {
        for (int i = 0; i < _pixels.size(); i++) {
            _pixels[i].R = input[i];
        }
    }
    else {
        throw runtime_error("SetChannel ERROR: Unknown channel type");
    }
}

// Performs SetChannel to all three channels using input channel
void Image::SetAllChannels(vector<unsigned char>& input) {
    SetChannel("B", input);
    SetChannel("G", input);
    SetChannel("R", input);
}


void Image::SetHeader(Header imageHeader) {
    _imageHeader = imageHeader;
    _hasHeader = true;
}

// Debugging

// Prints first 10 pixels of image
void Image::Print() {
    if (_pixels.size() == 0) {
        cout << "Cannot print an image without pixels.";
    }
    else if (_pixels.size() < 10) {
        cout << "Image has less than 10 pixels. Check for errors.";
    }
    cout << "B  |  G  |  R" << endl;
    for (int i = 0; i < 10; i++) {

        cout << static_cast<unsigned int>(_pixels[i].B) << " | ";
        cout << static_cast<unsigned int>(_pixels[i].G) << " | ";
        cout << static_cast<unsigned int>(_pixels[i].R) << " | ";
        cout << endl;
    }
}

void Image::PrintHeader() {
    if (!_hasHeader) {
        cout << "PrintHeader ERROR: No header exists.";
    }
    cout << _imageHeader.idLength << endl;
    cout << _imageHeader.idLength << endl;
    cout << _imageHeader.colorMapType << endl;
    cout << static_cast<int>(_imageHeader.dataTypeCode) << endl;
    cout << static_cast<int>(_imageHeader.colorMapOrigin) << endl;
    cout << _imageHeader. colorMapLength << endl;
    cout << static_cast<int>(_imageHeader.colorMapDepth) << endl;
    cout << static_cast<int>(_imageHeader.xOrigin) << endl;
    cout << static_cast<int>(_imageHeader.yOrigin) << endl;
    cout << static_cast<int>(_imageHeader. imageWidth) << endl;
    cout << _imageHeader. imageHeight << endl;
    cout << _imageHeader.bitsPerPixel << endl;
}

// Helper Functions

float Image::normalize(const unsigned char& val) {
    return val / 255.0f;
}

/* Val must be a normalized value within the range 0 < val < 1 */
unsigned char Image::denormalize(const float& val) {
    return (val * 255.0f) + 0.5f;
}

unsigned char Image::multiplyHelp(const unsigned char& a, const unsigned char& b) {
    return denormalize(normalize(a) * normalize(b));
}

/* Subtracts a - b, performing clamp as needed*/
unsigned char Image::subtractHelp(const unsigned char& a, const unsigned char& b) {
    if (a - b < 0) {
        return 0;
    }
    return a - b;
}

unsigned char Image::screenHelp(const unsigned char& a, const unsigned char& b) {
    return denormalize(1.0f - (1.0f - normalize(a)) * (1.0f - normalize(b)));
}

// a = background, b = foreground //
unsigned char Image::overlayHelp(const unsigned char& a, const unsigned char& b) {
    if (Image::normalize(a) <= 0.5f) {
        return Image::denormalize(2.0f * Image::normalize(a) * Image::normalize(b));
    }
    return Image::denormalize(1.0f - (2.0f * (1.0f - Image::normalize(a)) * (1.0f - Image::normalize(b))));
}

unsigned char Image::addHelp(const unsigned char& a, const unsigned char& b) {
    if (a + b > 255) {
        return 255;
    }
    return a + b;
}

unsigned char Image::scaleHelp(const unsigned char& a, const unsigned char& b) {
    if (a * b > 255) {
        return 255;
    }
    return a * b;
}



/* Blending Modes
* Assume invoking object is the "Base" layer
* * Requirements: Image dimensions must match,
* "product" Image object must have an empty _pixels vector
* "product" Image object must already have a header
* !! Blending does not modify the invoking object or layers
* used, but instead updates a separate "result" image
* which should be initialized with the proper header !! */


/* Parameter 1: Layer to multiply by
* Parameter 2: Separate Image object for results to be stored in */
void Image::Multiply(const Image& factor, Image& product) const {
    for (int i = 0; i < _pixels.size(); i++) {
        Pixel pixel;
        pixel.B = multiplyHelp(_pixels[i].B, factor._pixels[i].B);
        pixel.G = multiplyHelp(_pixels[i].G, factor._pixels[i].G);
        pixel.R = multiplyHelp(_pixels[i].R, factor._pixels[i].R);
        product._pixels.push_back(pixel);
    }
}


/* Parameter 1: Layer to subtract by ("top")
* Parameter 2: Separate Image object for results to be stored in */
void Image::Subtract(const Image& B, Image& difference) const {
    for (int i = 0; i < _pixels.size(); i++) {
        Pixel pixel;
        pixel.B = subtractHelp(_pixels[i].B, B._pixels[i].B);
        pixel.G = subtractHelp(_pixels[i].G, B._pixels[i].G);
        pixel.R = subtractHelp(_pixels[i].R, B._pixels[i].R);
        difference._pixels.push_back(pixel);
    }
}


/* Parameter 1: "B" layer in screen formula ("top")
* Parameter 2: Separate Image object for results to be stored in */
void Image::Screen(const Image& B, Image& result) const {
    for (int i = 0; i < _pixels.size(); i++) {
        Pixel pixel;
        pixel.B = screenHelp(_pixels[i].B, B._pixels[i].B);
        pixel.G = screenHelp(_pixels[i].G, B._pixels[i].G);
        pixel.R = screenHelp(_pixels[i].R, B._pixels[i].R);
        result._pixels.push_back(pixel);
    }
}


/* Parameter 1: Foreground layer in overlay formula ("top")
* Parameter 2: Separate Image object for resuls to be stored in */
void Image::Overlay(const Image& B, Image& result) const {
    for (int i = 0; i < _pixels.size(); i++) {
        Pixel pixel;
        pixel.B = overlayHelp(_pixels[i].B, B._pixels[i].B);
        pixel.G = overlayHelp(_pixels[i].G, B._pixels[i].G);
        pixel.R = overlayHelp(_pixels[i].R, B._pixels[i].R);
        result._pixels.push_back(pixel);
    }
}

/* ModifyChannel is an exception, modifying the invoking object and
 * assuming it has already been initialized with pixels */
void Image::ModifyChannel(const string& operation, const string& channel, unsigned char amount) {
    // Edge case: invalid arguments passed
    if (channel != "B" && channel != "G" && channel != "R") {
        throw runtime_error("ModifyChannel ERROR: Invalid channel");
    }
    if (operation != "Scale" && operation != "Add") {
        throw runtime_error("ModifyChannel ERROR: Unknown operation");
    }

    if (operation == "Add") {
        if (channel == "B") {
            for (int i = 0; i < _pixels.size(); i++) {
                _pixels[i].B = addHelp(_pixels[i].B, amount);
            }
        }
        else if (channel == "G") {
            for (int i = 0; i < _pixels.size(); i++) {
                _pixels[i].G = addHelp(_pixels[i].G, amount);
            }
        }
        if (channel == "R") {
            for (int i = 0; i < _pixels.size(); i++) {
                _pixels[i].R = addHelp(_pixels[i].R, amount);
            }
        }
    }
    else if (operation == "Scale") {
        if (channel == "B") {
            for (int i = 0; i < _pixels.size(); i++) {
                _pixels[i].B = scaleHelp(_pixels[i].B, amount);
            }
        }
        else if (channel == "G") {
            for (int i = 0; i < _pixels.size(); i++) {
                _pixels[i].G = scaleHelp(_pixels[i].G, amount);
            }
        }
        if (channel == "R") {
            for (int i = 0; i < _pixels.size(); i++) {
                _pixels[i].R = scaleHelp(_pixels[i].R, amount);
            }
        }
    }


}

// Exception: modifies invoking object
void Image::Flip() {
    // Edge cases: Missing Image data
    if (_pixels.size() == 0) {
        throw runtime_error("Flip ERROR: Cannot flip an image without pixels");
    }
    if (!_hasHeader) {
        throw runtime_error("Flip ERROR: File does not have a header");
    }


    int rows = _imageHeader.imageHeight;
    int cols = _imageHeader.imageWidth;

    // For every row (until halfway)
    for (int i = 1; i <= rows / 2; i++) {
        // For every column in every row
        for (int j = 1; j <= cols; j++) {
            int high_row = rows - (i - 1);
            int high_col = cols - (j - 1);
            int low_index = (cols * (i - 1) + j) - 1;
            int high_index = (cols * (high_row - 1) + high_col) - 1;
            // Store copy of low pixel
            Pixel temp_pixel(_pixels[low_index].B, _pixels[low_index].G, _pixels[low_index].R);
            // Copy high pixel into low pixel
            _pixels[low_index].B = _pixels[high_index].B;
            _pixels[low_index].G = _pixels[high_index].G;
            _pixels[low_index].R = _pixels[high_index].R;
            // Copy temp pixel into high pixel
            _pixels[high_index].B = temp_pixel.B;
            _pixels[high_index].G = temp_pixel.G;
            _pixels[high_index].R = temp_pixel.R;
        }
    }



}

/* Assumes each image has the same header information,
* importantly width and height dimensions
* Invoking object is Q1
* Combines images into the pattern
* Q1   Q2
* Q3   Q4
* With new dimensions originalHeight x 2, originalWidth x 2
* Storing output in a separate (empty - no header) image object */
void Image::Combine4(const Image& Q2, const Image& Q3, const Image& Q4, Image& output) const {
    short originalHeight = GetHeader().imageHeight;
    short originalWidth = GetHeader().imageWidth;

    // Write Q3, then Q4, then Q1, and finally Q3

    // For every row from 1 - 1024
    for (int i = 1; i <= originalHeight * 2; i++) {
        // For every column from 1 - 1024
        for (int j = 1; j <= originalWidth * 2; j++) {
            // For the first 512 rows, focus on loading Q3 and Q4 pixels
            if (i <= originalHeight) {
                // For the first 512 columns, load Q3 pixels
                if (j <= originalWidth) {
                    output._pixels.push_back(Q3._pixels[originalWidth * (i - 1) + j - 1]);
                }
                // For the remainder of columns (513 - 1024), load Q4 pixels
                else {
                    int col = j - originalWidth; // Reset col value to 1 (513 -> 1) to get the right index of Q4 pixels
                    output._pixels.push_back(Q4._pixels[originalWidth * (i - 1) + col - 1]);
                }
            }
            // For the remainder of rows (513 - 1024), focus on loading Q1 and Q2 pixels
            else {
                int row = i - originalHeight; // Reset row value to 1 (513 -> 1) to get the right index of Q1/Q2 pixels

                // For the first 512 columns, load Q1
                if (j <= originalWidth) {
                    output._pixels.push_back(this->_pixels[originalWidth * (row - 1) + j - 1]);
                }
                // For the remainder of columns (513 - 1024), load Q2 pixels
                else {
                    int col = j - originalWidth; // Reset col value to 1 (513 -> 1) to get the right index of Q1/Q2 pixels
                    output._pixels.push_back(Q2._pixels[originalWidth * (row - 1) + col - 1]);
                }
            }
        }
    }
    // Adjust header information for output image to reflect combination
    output.SetHeader(GetHeader());
    output._imageHeader.imageWidth *= 2;
    output._imageHeader.imageHeight *= 2;
}

/* Unpacks image data from inFile and updates Image
 * member variables using functions from FileOperations */
void Image::Deserialize(const string& inFilePath) {
    ifstream inFile(inFilePath, ios_base::binary);
    if (!inFile.is_open()) {
        throw runtime_error("ERROR: Invalid file path, deserialization failed");
    }
    _imageHeader = GetFileHeader(inFile);
    GetFilePixels(inFile, _imageHeader, _pixels);
    _hasHeader = true;
    inFile.close();
}

/* Packs header then pixel image data into outFile using
* functions from FileOperations */
void Image::Serialize(const string& outFilePath) {
    // Edge cases: missing internal data
    if (!_hasHeader && _pixels.size() == 0) {
        throw runtime_error("ERROR: Cannot serialize an empty image");
    }
    if (!_hasHeader) {
        throw runtime_error("ERROR: Cannot serialize image without a header");
    }
    if (_pixels.size() == 0) {
        throw runtime_error("ERROR: Cannot serialize image without pixels");
    }
    ofstream outFile(outFilePath, ios_base::binary);
    WriteHeader(outFile, _imageHeader);
    WritePixels(outFile, _pixels);
    outFile.close();
}
