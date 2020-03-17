#pragma once

#include <SFML/Graphics.hpp>
#include <array>

class FakeTerminal
{

    public:
        using frame_callback_t = bool(*)(uint8_t*);

        FakeTerminal(sf::Vector2u terminal_size, frame_callback_t callback);
		
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

        void load_next_frame();

        std::array<sf::Vector2f, 4> getCharQuad(uint8_t character);

        virtual void update(float delta);

    private:
        frame_callback_t next_frame_callback = nullptr;
};
