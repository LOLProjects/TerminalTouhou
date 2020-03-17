#include "FakeTerminal.h"

#include <cstdlib>

FakeTerminal::FakeTerminal(sf::Vector2u terminal_size, FakeTerminal::frame_callback_t callback) :
    size(terminal_size),
    window(sf::VideoMode(terminal_size.x * 8 * 2, terminal_size.y * 16 * 2), "TerminalTouhou", sf::Style::Close),
    vertices(sf::Quads, terminal_size.x * terminal_size.y * 4),
      next_frame_callback(callback)
{
    //ctor
}

FakeTerminal::~FakeTerminal()
{
    //dtor
}

bool FakeTerminal::init()
{
    window.setFramerateLimit(0);

    if (!chars.loadFromFile("chars.png"))
        return false;

    prepareVertices();

    load_next_frame();

    return true;
}

void FakeTerminal::run()
{
    const sf::Time frame_time = sf::microseconds(1000000/24);
    sf::Time time_acc;

    sf::Clock clk;
    sf::Time delta;

    int frames = 0;

    while (window.isOpen())
    {
        {   //Event handling
            sf::Event e;

            while (window.pollEvent(e))
            {
                if (e.type == sf::Event::Closed)
                    window.close();
            }
        }

        delta = clk.restart();

        time_acc += delta;
        ++frames;
        if (time_acc >= frame_time)
        {
            load_next_frame();
            time_acc = sf::Time::Zero;
            frames = 0;
        }

        update(delta.asSeconds());

        //window.clear();

        window.draw(vertices, &chars);

        window.display();
    }
}

void FakeTerminal::stop()
{

}

void FakeTerminal::setChar(sf::Vector2u pos, uint8_t character)
{
    if (pos.x >= size.x || pos.y >= size.y)
        return;

    auto quad = getCharQuad(character);

    vertices[(pos.x + pos.y * size.x) * 4 + 0] = sf::Vertex(sf::Vector2f(8.f * 2.f * pos.x,         16.f * 2.f * pos.y),        quad[0]);
    vertices[(pos.x + pos.y * size.x) * 4 + 1] = sf::Vertex(sf::Vector2f(8.f * 2.f * (pos.x + 1),   16.f * 2.f * pos.y),        quad[1]);
    vertices[(pos.x + pos.y * size.x) * 4 + 2] = sf::Vertex(sf::Vector2f(8.f * 2.f * (pos.x + 1),   16.f * 2.f * (pos.y + 1)),  quad[2]);
    vertices[(pos.x + pos.y * size.x) * 4 + 3] = sf::Vertex(sf::Vector2f(8.f * 2.f * pos.x,         16.f * 2.f * (pos.y + 1)),  quad[3]);
}


void FakeTerminal::prepareVertices()
{
    uint8_t def = 8;

    for (unsigned int x = 0; x < size.x; x++)
    for (unsigned int y = 0; y < size.y; y++)
    {
        setChar(sf::Vector2u(x, y), test_string[x + y * size.x]);
    }
}

void FakeTerminal::load_next_frame()
{
    if (next_frame_callback)
    {
        uint8_t frame_data[24][80];
        bool status = next_frame_callback((uint8_t*)frame_data);

        if (status)
            for (int i = 0; i < 24; ++i)
            {
                for (int j = 0; j < 80; ++j)
                {
                    setChar(sf::Vector2u(j, i), frame_data[i][j]);
                }
            }
    }
}

std::array<sf::Vector2f, 4> FakeTerminal::getCharQuad(uint8_t character)
{
    sf::Vector2f a = sf::Vector2f(9.f * (character % 32) + 8.f, 16.f * (character / 32) + 8.f);
    return
    {
        a,
        a + sf::Vector2f(8.f, 0),
        a + sf::Vector2f(8.f, 16.f),
        a + sf::Vector2f(0, 16.f)
    };
}

void FakeTerminal::update(float delta)
{
    //setChar(sf::Vector2u(std::rand() % size.x, std::rand() % size.y), std::rand() % 256);
}
