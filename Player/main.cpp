#include <SFML/Audio/Music.hpp>

#include "FakeTerminal.h"

#include "rle_decode.h"

const uint8_t* eof;
const uint8_t* file_data;
int decoded_frames = 0;

bool decode_next_frame(uint8_t* data)
{
    int status = rle_decode_frame(&file_data, eof, data);
    if (status != 0)
    {
        fprintf(stderr, "error at frame %d : '%s'\n", decoded_frames, (status == 1 ? "frame data overflow" : status == 2 ? "early frame data end" : "unknown"));
        return false;
    }

    ++decoded_frames;

    return true;
}

int main()
{
    FakeTerminal ft(sf::Vector2u(80, 24), decode_next_frame);

    int size;
    file_data = read_input_file("result.bin", &size);
    eof = file_data + size;

    /*
    for (int i = 0; i < HEIGHT; ++i)
    {
        for (int j = 0; j < WIDTH; ++j)
        {
            ft.setChar(sf::Vector2u(j, i), frame_data[i*WIDTH + j]);
        }
        printf("****\n");
    }
    */

    sf::Music music;
    music.openFromFile("bad_apple_song.ogg");

    if (!ft.init())
        return 1;

    music.play();
    ft.run();

    ft.stop();

    return 0;
}
