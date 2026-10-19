#include <fstream>
#include <iostream>
#include "FileOperations.h"
#include "Header.h"
#include "Image.h"


// Testing

/* Parameter 1: File path of first file being compared (output)
 * Parameter 2: File path of second file being compared (examples)
 * Reads and compares the headers and pixels of two .tga files,
 * returning a message to indicate results */
string TestFileEquality(const string& file1Path, const string& file2Path);

int main() {
    //     // for (int i = 0; i < pixels.size(); i++) {
    //     //     cout << static_cast<unsigned int>(pixels[i].B) << " | ";
    //     //     cout << static_cast<unsigned int>(pixels[i].G) << " | ";
    //     //     cout << static_cast<unsigned int>(pixels[i].R) << " | ";
    //     //     cout << endl;
    //     // }

    // Input image objects
    Image car;
    Image circles;
    Image layer1;
    Image layer2;
    Image layer_blue;
    Image layer_green;
    Image layer_red;
    Image pattern1;
    Image pattern2;
    Image text;
    Image text2;

    // Deserialization of input image objects
    car.Deserialize("input/car.tga");
    circles.Deserialize("input/circles.tga");
    layer1.Deserialize("input/layer1.tga");
    layer2.Deserialize("input/layer2.tga");
    layer_blue.Deserialize("input/layer_blue.tga");
    layer_green.Deserialize("input/layer_green.tga");
    layer_red.Deserialize("input/layer_red.tga");
    pattern1.Deserialize("input/pattern1.tga");
    pattern2.Deserialize("input/pattern2.tga");
    text.Deserialize("input/text.tga");
    text2.Deserialize("input/text2.tga");

    // Extra Credit
    Image extraCredit;
    car.Combine4(circles, text, pattern1, extraCredit);
    extraCredit.Serialize("output/extracredit.tga");
    cout << "Extra Credit: " << TestFileEquality("output/extracredit.tga", "examples/EXAMPLE_extracredit.tga") << endl;

    // Part 1
    Image part1(pattern1.GetHeader());
    pattern1.Multiply(layer1, part1);
    part1.Serialize("output/part1.tga");
    cout << "Part 1: " << TestFileEquality("output/part1.tga", "examples/EXAMPLE_part1.tga") << endl;

    // // Part 2
    Image part2(car.GetHeader());
    car.Subtract(layer2, part2);
    part2.Serialize("output/part2.tga");
    cout << "Part 2: " << TestFileEquality("output/part2.tga", "examples/EXAMPLE_part2.tga") << endl;

    // Part 3
    Image part3_temp(pattern2.GetHeader());
    pattern2.Multiply(layer1, part3_temp);
    Image part3(pattern2.GetHeader());
    text.Screen(part3_temp, part3);
    part3.Serialize("output/part3.tga");
    cout << "Part 3: " << TestFileEquality("output/part3.tga", "examples/EXAMPLE_part3.tga") << endl;

    // Part 4
    Image part4_temp(circles.GetHeader());
    circles.Multiply(layer2, part4_temp);
    Image part4(circles.GetHeader());
    part4_temp.Subtract(pattern2, part4);
    part4.Serialize("output/part4.tga");
    cout << "Part 4: " << TestFileEquality("output/part4.tga", "examples/EXAMPLE_part4.tga") << endl;

    // Part 5
    Image part5(pattern1.GetHeader());
    pattern1.Overlay(layer1, part5);
    part5.Serialize("output/part5.tga");
    cout << "Part 5: " << TestFileEquality("output/part5.tga", "examples/EXAMPLE_part5.tga") << endl;

    // Part 6
    Image part6;
    part6.Deserialize("input/car.tga");
    part6.ModifyChannel("Add", "G", 200);
    part6.Serialize("output/part6.tga");
    cout << "Part 6: " << TestFileEquality("output/part6.tga", "examples/EXAMPLE_part6.tga") << endl;

    // Part 7
    Image part7;
    part7.Deserialize("input/car.tga");
    part7.ModifyChannel("Scale", "R", 4);
    part7.ModifyChannel("Scale", "B", 0);
    part7.Serialize("output/part7.tga");
    cout << "Part 7: " << TestFileEquality("output/part7.tga", "examples/EXAMPLE_part7.tga") << endl;

    // Part 8
    vector<unsigned char> carBlueChannel;
    vector<unsigned char> carGreenChannel;
    vector<unsigned char> carRedChannel;

    car.GetChannel("B", carBlueChannel);
    car.GetChannel("G", carGreenChannel);
    car.GetChannel("R", carRedChannel);

    Image part8_b(car.GetHeader());
    Image part8_g(car.GetHeader());
    Image part8_r(car.GetHeader());

    part8_b.SetAllChannels(carBlueChannel);
    part8_g.SetAllChannels(carGreenChannel);
    part8_r.SetAllChannels(carRedChannel);

    part8_b.Serialize("output/part8_b.tga");
    part8_g.Serialize("output/part8_g.tga");
    part8_r.Serialize("output/part8_r.tga");

    cout << "Part 8 Blue: " << TestFileEquality("output/part8_b.tga", "examples/EXAMPLE_part8_b.tga") << endl;
    cout << "Part 8 Green: " << TestFileEquality("output/part8_g.tga", "examples/EXAMPLE_part8_g.tga") << endl;
    cout << "Part 8 Red: " << TestFileEquality("output/part8_r.tga", "examples/EXAMPLE_part8_r.tga") << endl;

    // Part 9
    vector<unsigned char> layer_blueChannel;
    vector<unsigned char> layer_greenChannel;
    vector<unsigned char> layer_redChannel;


    layer_blue.GetChannel("B", layer_blueChannel);
    layer_green.GetChannel("G", layer_greenChannel);
    layer_red.GetChannel("R", layer_redChannel);

    Image part9(layer_blue.GetHeader());
    part9.SetChannel("B", layer_blueChannel);
    part9.SetChannel("G", layer_greenChannel);
    part9.SetChannel("R", layer_redChannel);
    part9.Serialize("output/part9.tga");
    cout << "Part 9: " << TestFileEquality("output/part9.tga", "examples/EXAMPLE_part9.tga") << endl;

    // Part 10
    text2.Flip();
    text2.Serialize("output/part10.tga");
    cout << "Part 10: " << TestFileEquality("output/part10.tga", "examples/EXAMPLE_part10.tga") << endl;

    return 0;

}

// Testing

string TestFileEquality(const string& file1Path, const string& file2Path) {
    // Open read streams
    ifstream file1(file1Path, ios_base::binary);
    ifstream file2(file2Path, ios_base::binary);

    // Read both files' components
    Header file1Header = GetFileHeader(file1);
    vector<Pixel> file1Pixels;
    GetFilePixels(file1, file1Header, file1Pixels);
    file1.close(); // IMPORTANT: AVOIDS PIXEL READ ERRORS NEAR END OF FILE

    Header file2Header = GetFileHeader(file2);
    vector<Pixel> file2Pixels;
    GetFilePixels(file2, file2Header, file2Pixels);
    file2.close(); // IMPORTANT: AVOIDS PIXEL READ ERRORS NEAR END OF FILE

    // Compare headers
    if (file1Header.idLength         !=  file2Header.idLength        ||
        file1Header.colorMapType     !=  file2Header.colorMapType    ||
        file1Header.dataTypeCode     !=  file2Header.dataTypeCode    ||
        file1Header.colorMapOrigin   !=  file2Header.colorMapOrigin  ||
        file1Header.colorMapLength   !=  file2Header.colorMapLength  ||
        file1Header.colorMapDepth    !=  file2Header.colorMapDepth   ||
        file1Header.xOrigin          !=  file2Header.xOrigin         ||
        file1Header.yOrigin          !=  file2Header.yOrigin         ||
        file1Header.imageWidth       !=  file2Header.imageWidth      ||
        file1Header.imageHeight      !=  file2Header.imageHeight     ||
        file1Header.bitsPerPixel     !=  file2Header.bitsPerPixel    ||
        file1Header.imageDescriptor  !=  file2Header.imageDescriptor  ) { return "FAIL: Header details do not match."; }

    // Compare every pixel
    if (file1Pixels.size() != file2Pixels.size()) { return "FAIL: Amount of pixels does not match."; }  // Pixel counts do not match

    for (int i = 0; i < file1Pixels.size(); i++) {
        if (file1Pixels[i].B != file2Pixels[i].B) {
            return "FAIL: The B channel of the " + to_string(i) + "th pixel does not match\n"
                    + "Current channel: " + to_string(static_cast<unsigned int>(file1Pixels[i].B)) + "\n"
                    + "Needed channel: " + to_string(static_cast<unsigned int>(file2Pixels[i].B));

        }
        if (file1Pixels[i].G != file2Pixels[i].G) {
            return "FAIL: The G channel of the " + to_string(i) + "th pixel does not match\n"
                    + "Current channel: " + to_string(static_cast<unsigned int>(file1Pixels[i].G)) + "\n"
                    + "Needed channel: " + to_string(static_cast<unsigned int>(file2Pixels[i].G));
        }
        if (file1Pixels[i].R != file2Pixels[i].R) {
            return "FAIL: The R channel of the " + to_string(i) + "th pixel does not match\n"
                    + "Current channel: " + to_string(static_cast<unsigned int>(file1Pixels[i].R)) + "\n"
                    + "Needed channel: " + to_string(static_cast<unsigned int>(file2Pixels[i].R));
        }

    }

    // Files are completely identical
    return "Success.";
}