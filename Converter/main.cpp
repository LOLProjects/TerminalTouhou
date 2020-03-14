#include <SFML/Graphics.hpp>

#include <string>
#include <iostream>

int main()
{
    std::string test_image_path = "maxresdefault.jpg";
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

        for (int c = 0; c < 256; c++)
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

    return 0;
}
