#include <SFML/Graphics.hpp>

#include <string>
#include <iostream>
#include <cstring>
#include <fstream>
#include <array>

#include <assert.h>

int giveHelp()
{
    std::cout <<
    "=== Manual for TT Converter ===\n"
    "Usage : converter (file path) [flags]\n\n"

    "List of flags : \n"
    "  -h or --help       : Show this page :)\n"
    "  -v or --verbose    : Enable verbose mode, gives more information during the processing.\n"
    "  -s or --single     : Process one single image instead of a folder of images (path has to contain filename).\n"

    << std::endl;
    return 0;
}

int invalidFlag(std::string& arg)
{
    std::cerr << "Invalid flag given : \"" << arg << "\".\n"
        << "Use -h or --help to get usage." << std::endl;
    return 1;
}

int noArguments()
{
    std::cerr << "No arguments given. You need to specify a path!\n"
        << "Use -h or --help to get usage." << std::endl;
    return 1;
}

bool exists(std::string& path)
{
    std::ifstream f(path);
    return f.good();
}

int fileNotFound(std::string& path)
{
    std::cerr << "Invalid path given : \"" << path << "\".\n"
        << "Use -h or --help to get usage." << std::endl;
    return 1;
}

/**
* Converts one image to RLE compressed data for the terminal to read
* Keep in mind when allocating a buffer that the data can be up to 80 * 24 * 2 bytes, 3840
* @param image : the image to convert. It HAS to be 640 by 384 pixels
* @param destination : where the data is going to be written
* @return how many bytes have been written
*/
size_t convertImage(const sf::Image& image, uint8_t* destination);

/**
* Resizes one image to the right size : 640 by 384 pixels.
* @param texture : the image to convert. The image to resize
* @param destination : where the data is going to be written
* @return a new image of the right size
*/
sf::Image resizeImage(const sf::Texture& texture);

sf::Image chars;

int main(int argc, char** argv)
{
    //Program argument variables
    std::string path;
    bool single_image = false;
    bool verbose = false;

    if (argc <= 1) return noArguments();

    //The first argument has to be the path
    //No need to check if path is a flag, since it won't be a file or a dir.
    path = argv[1];
    if (path == "-h" || path == "--help") return giveHelp();

    if (!exists(path)) return fileNotFound(path);

    while (argc-- != 2)
    {
        std::string arg = argv[argc];

        if (arg == "-h" || arg == "--help") return giveHelp();
        else if (arg == "-v" || arg == "--verbose") verbose = true;
        else if (arg == "-s" || arg == "--single") single_image = true;
        else return invalidFlag(arg);
    }

    if (verbose)
        std::cout << "Verbose mode is enabled." << std::endl;

    if (!single_image)
    {
        std::cerr << "Only single image mode is supported right now. Use -s to enable it." << std::endl;
        return 1;
    }

    //Load the image
    sf::Texture test_texture;

    if (!test_texture.loadFromFile(path))
    {
        std::cerr << "Couldn't open file" << std::endl;
        return 1;
    }

    sf::Image test_image = resizeImage(test_texture);

    //Load the charset
    if (!chars.loadFromFile("chars.png"))
        return 1;

    //The data array : 2 * 1920 bytes is the biggest it could ever be
    std::array<uint8_t, 2 * 24 * 80> frame_data;
    size_t pointer = 0;

    size_t s = convertImage(test_image, frame_data.begin());

    std::cout << "Frame took " << s << " bytes." << std::endl;

    pointer += s;

    {
        std::ofstream file("result.bin", std::ios::binary);
        file.write((const char*)frame_data.begin(), pointer);
    }

    return 0;
}

size_t convertImage(const sf::Image& image, uint8_t* destination)
{
    if (image.getSize() != sf::Vector2u(80 * 8, 24 * 16))
    {
        std::cerr << "Could not convert frame! It is not of the expected size (640 * 384)." << std::endl;
        return 0;
    }

    size_t data_size = 0;

    //Compression is done on the spot
    //For each char of the terminal
    for (int y = 0; y < 24; y++)
    for (int x = 0; x < 80; x++)
    {
        uint8_t choosed_char = 0;
        unsigned int choosed_char_score = 0;

        //For each char in the charset
        for (int c = 3; c < 127; c++)
        {
            unsigned int this_score = 0;

            int ccx = 9 * (c % 32) + 8;
            int ccy = 16 * (c / 32) + 8;

            //For each pixel of the char in the charset
            for (int cx = 0; cx < 8; cx++)
            for (int cy = 0; cy < 16; cy++)
            {
                //Make a score of how much that char fits the original image pixels
                bool image_pixel = image.getPixel(x * 8 + cx, y * 16 + cy).r > 127;
                bool char_pixel = chars.getPixel(ccx + cx, ccy + cy).r > 127;

                this_score += !(image_pixel ^ char_pixel);
            }

            if (this_score > choosed_char_score)
            {
                choosed_char_score = this_score;
                choosed_char = c;
            }
        }

        //Take care of RLE compression on the spot
        if (data_size == 0 || destination[data_size - 1] == 0xFF || destination[data_size - 2] != choosed_char) //Need to create a new RLE block
        {
            destination[data_size] = choosed_char;
            destination[data_size + 1] = 0;
            data_size += 2;
        }
        else    //Increment the current RLE block
            destination[data_size - 1]++;
    }

    assert(data_size <= 3840);

    return data_size;
}

sf::Image resizeImage(const sf::Texture& texture)
{
    sf::RenderTexture render_texture;
    render_texture.create(80 * 8, 24 * 16);

    {
        render_texture.clear();

        sf::RectangleShape rectangle_shape;
        rectangle_shape.setTexture(&texture);
        rectangle_shape.setSize(sf::Vector2f(80 * 8, 24 * 16));

        render_texture.draw(rectangle_shape);

        render_texture.display();
    }

    sf::Texture result = render_texture.getTexture();

    return result.copyToImage();
}
