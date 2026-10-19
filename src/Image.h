#pragma once

#include <vector>
#include "Header.h"
#include "Pixel.h"
using namespace std;

class Image {
    Header _imageHeader;
    vector<Pixel> _pixels;
    bool _hasHeader;

    // Helper functions
    static float normalize(const unsigned char& val);
    static unsigned char denormalize(const float& val);
    static unsigned char multiplyHelp(const unsigned char& a, const unsigned char& b);
    static unsigned char subtractHelp(const unsigned char& a, const unsigned char& b);
    static unsigned char screenHelp(const unsigned char& a, const unsigned char& b);
    static unsigned char overlayHelp(const unsigned char& a, const unsigned char& b);
    static unsigned char addHelp(const unsigned char& a, const unsigned char& b);
    static unsigned char scaleHelp(const unsigned char& a, const unsigned char& b);


public:
    // Constructors, Assignment Operators
    Image();
    Image(Header imageHeader);
    Image(Header& imageHeader, vector<Pixel>& pixels);

    // Accessors, Mutators
    Header GetHeader() const;
    vector<Pixel> GetPixels() const;

    // Get all values of "R" or "G" or "B" and store in output parameter
    void GetChannel(const string& channel, vector<unsigned char>& output);

    /* Set all values of "R" or "G" or "B" based on input channel
     * if no pixels exist, create pixels using the input channel and
     * with other channels each set to 0 */
    void SetChannel(const string& channel, vector<unsigned char>& input);

    // Performs SetChannel to all three channels using input channel
    void SetAllChannels(vector<unsigned char>& input);

    void SetHeader(Header imageHeader);


    // Debugging

    // Prints first 10 pixels of image
    void Print();
    void PrintHeader();

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
    void Multiply(const Image& factor, Image& product) const;


    /* Parameter 1: Layer to subtract by ("top") (this - B)
    * Parameter 2: Separate Image object for results to be stored in */
    void Subtract(const Image& B, Image& difference) const;


    /* Parameter 1: "B" layer in screen formula ("top")
    * Parameter 2: Separate Image object for results to be stored in */
    void Screen(const Image& B, Image& result) const;


    /* Parameter 1: "B" layer in overlay formula ("top")
     * Parameter 2: Separate Image object for resuls to be stored in */
    void Overlay(const Image& B, Image& result) const;

    /* Parameter 1: Applies indicated operation ("Add", "Scale") - must be capitalized
     * Parameter 2: Channel to modify ("B", "G", "R") - must be capitalized
     * Parameter 3: Separate Image object for results to be stored in
     * Flexible function capable of performing various operations to a single channel
     * ModifyChannel is an exception, modifying the invoking object and
     * assuming it has already been initialized with pixels */
    void ModifyChannel(const string& operation, const string& channel, unsigned char amount);

    // Exception: modifies invoking object
    void Flip();

    /* Assumes each image has the same header information,
    * importantly width and height dimensions
    * Invoking object is Q1
    * Combines images into the pattern
    * Q1   Q2
    * Q3   Q4
    * With new dimensions originalHeight x 2, originalWidth x 2
    * Storing output in a separate (empty - no header) image object */
    void Combine4(const Image& Q2, const Image& Q3, const Image& Q4, Image& output) const;


    // File Operations

    /* Unpacks image data from inFilePath and updates Image
    * member variables using functions from FileOperations */
    void Deserialize(const string& inFilePath);

    /* Packs header then pixel image data into outFilePath using
     * functions from FileOperations */
    void Serialize(const string& outFilePath);

};
