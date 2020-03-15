#include <SFML/Graphics.hpp>

#include <string>
#include <iostream>
#include <cstring>

void giveHelp()
{
    std::cout <<
    "=== Manual for TT Converter ===\n"
    "Usage : converter (file path) [flags]\n\n"

    "List of flags : \n"
    "  -h or --help       : Show this page :)\n"
    "  -v or --verbose    : Enable verbose mode, gives more information during the processing.\n"
    "  -s or --single     : Process one single image instead of a folder of images (path has to contain filename).\n"

    << std::endl;
}

int main(int argc, char** argv)
{
    std::string path;
    bool single_image = false;

    bool verbose = false;

    //Arguments parsing
    //It's ugly but it works.

    if (argc <= 1)
    {
        std::cerr << "No arguments given. You need to specify a path!\n"
            << "Use -h or --help to get usage." << std::endl;
        return 1;
    }

    std::string last_flag = "";
    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];

        if (!arg.length())
            continue;

        //std::clog << "Parsing argument #" << i << " : \"" << arg << '\"' << std::endl;

        if (arg.at(0) == '-')
        {
            if (arg.length() == 1)
            {
                std::cerr << "Invalid flag given : \"-\".\n"
                    << "Use -h or --help to get usage." << std::endl;
                return 1;
            }

            if (arg.length() != 2 && arg.at(1) != '-')
            {
                std::cerr << "Invalid flag given : \"" << arg << "\".\n"
                    << "Use -h or --help to get usage." << std::endl;
                return 1;
            }

            switch (arg.at(1))
            {
            case '-':
                if (arg.length() == 2)
                {
                    std::cerr << "Invalid flag given : \"" << arg << "\".\n"
                        << "Use -h or --help to get usage." << std::endl;
                    return 1;
                }

                if (arg == "--help")
                {
                    giveHelp();
                    return 0;
                }

                if (arg == "--verbose")
                    verbose = true;

                if (arg == "--single")
                    single_image = true;

                break;

            case 'h':
                giveHelp(); return 0;

            case 'v':
                verbose = true; break;

            case 's':
                single_image = true; break;

            default:
                std::cerr << "Unknown flag given : \"" << arg << "\".\n"
                    << "Use -h or --help to get usage." << std::endl;
                return 1;
            }
        }
        else
        {
            if (last_flag == "")
            {
                if (path.length())
                {
                    std::cerr << "Two paths were given in the argument list!\n"
                        << "Use -h or --help to get usage." << std::endl;
                    return 1;
                }
                path = arg;
            }
        }
    }

    if (verbose)
        std::cout << "Verbose mode is enabled." << std::endl;

    return 0; //TEMPORARY

    /*

    std::string test_image_path = "";
    sf::Texture test_texture;

    if (!test_texture.loadFromFile(test_image_path))
        return 1;

    sf::RenderTexture render_texture;
    render_texture.create(80 * 8, 24 * 16);

    {
        render_texture.clear();

        sf::RectangleShape rectangle_shape;
        rectangle_shape.setTexture(&test_texture);
        rectangle_shape.setSize(sf::Vector2f(80 * 8, 24 * 16));

        render_texture.draw(rectangle_shape);

        render_texture.display();
    }

    sf::Texture result = render_texture.getTexture();

    sf::Image test_image = result.copyToImage();

    test_image.saveToFile("test.png");

    sf::Image chars;
    chars.loadFromFile("chars.png");

    std::cout << "{ ";

    for (int y = 0; y < 24; y++)
    for (int x = 0; x < 80; x++)
    {
        uint8_t choosed_char = 0;
        unsigned int choosed_char_score = 0;

        for (int c = 3; c < 127; c++)
        {
            unsigned int this_score = 0;

            int ccx = 9 * (c % 32) + 8;
            int ccy = 16 * (c / 32) + 8;

            for (int cx = 0; cx < 8; cx++)
            for (int cy = 0; cy < 16; cy++)
            {
                bool image_pixel = test_image.getPixel(x * 8 + cx, y * 16 + cy).r > 127;
                bool char_pixel = chars.getPixel(ccx + cx, ccy + cy).r > 127;

                this_score += !(image_pixel ^ char_pixel);
            }

            if (this_score > choosed_char_score)
            {
                choosed_char_score = this_score;
                choosed_char = c;
            }
        }

        std::cout << (int)choosed_char;
        std::cout << ", ";
    }

    std::cout << std::endl;
    */

    return 0;
}
