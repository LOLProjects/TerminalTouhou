#pragma once

#include <SFML/Graphics.hpp>
#include <array>

class FakeTerminal
{

    public:
        static const uint8_t test_string[];


        FakeTerminal(sf::Vector2u terminal_size);
        virtual ~FakeTerminal();

        bool init();

        void run();

        void stop();

        void setChar(sf::Vector2u pos, uint8_t character);

    protected:
        sf::Vector2u size;

        sf::RenderWindow window;

        sf::VertexArray vertices;

        sf::Texture chars;

        void prepareVertices();

        std::array<sf::Vector2f, 4> getCharQuad(uint8_t character);

        virtual void update(float delta);

    private:
};
