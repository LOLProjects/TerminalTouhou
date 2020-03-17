#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>

#define WIDTH 80
#define HEIGHT 24

const uint8_t* read_input_file(const char* filename, int* size)
{
    *size = 0;
    uint8_t *source = NULL;
    FILE *fp = fopen(filename, "rb");
    if (fp != NULL) {
        /* Go to the end of the file. */
        if (fseek(fp, 0L, SEEK_END) == 0) {
            /* Get the size of the file. */
            long bufsize = ftell(fp);
            if (bufsize == -1)
                return NULL;

            /* Allocate our buffer to that size. */
            source = (uint8_t*)malloc(sizeof(uint8_t) * bufsize);

            /* Go back to the start of the file. */
            if (fseek(fp, 0L, SEEK_SET) != 0) { /* Error */ }

            /* Read the entire file into memory. */
            size_t newLen = fread(source, sizeof(uint8_t), bufsize, fp);
            *size = newLen;
            if ( ferror( fp ) != 0 ) {
                fputs("Error reading file", stderr);
                return NULL;
            }
        }
        fclose(fp);
    }

    return source;
}

// return decode status
int rle_decode_frame(const uint8_t** input, const uint8_t* end_of_input, uint8_t* output)
{
    int total_bytes = 0;

    while (*input < end_of_input && total_bytes < WIDTH*HEIGHT)
    {
        int count     = 1 + (*input)[0];
        uint8_t byte  = (*input)[1];
        memset(output, byte, count);
        output += count;
        total_bytes += count;

        *input += 2;
    }

    if (total_bytes > WIDTH*HEIGHT)
    {
        // frame overflow
        return 1;
    }
    else if (total_bytes < WIDTH*HEIGHT)
    {
        // early file end
        return 2;
    }

    return 0;
}

// returns amount of decoded frames
int rle_decode_file(const char* filename, uint8_t* frame_data)
{
    int size; const uint8_t* eof;
    const uint8_t* data = read_input_file(filename, &size);
    eof = data + size;

    int decoded_frames = 0;
    while (data < eof)
    {
        int status = rle_decode_frame(&data, eof, frame_data);
        if (status != 0 && false)
        {
            fprintf(stderr, "error at frame %d : '%s'\n", decoded_frames, (status == 1 ? "frame data overflow" : status == 2 ? "early frame data end" : "unknown"));
            abort(); // abort
            return 0;
        }

        frame_data += WIDTH*HEIGHT;
        ++decoded_frames;
    }

    return decoded_frames;
}

int main()
{
    uint8_t* frame_data = (uint8_t*)malloc(WIDTH*HEIGHT * 30);
    int frames = rle_decode_file("result.bin", frame_data);

    printf("decoded frames : %d\n", frames);

    for (int i = 0; i < HEIGHT; ++i)
    {
        for (int j = 0; j < WIDTH; ++j)
        {

            printf("%02X", frame_data[i*WIDTH + j]);
        }
        printf("****\n");
    }

    return 0;
}
