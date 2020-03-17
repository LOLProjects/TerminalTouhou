#ifndef RLE_DECODE_H
#define RLE_DECODE_H

#define WIDTH 80
#define HEIGHT 24

const uint8_t* read_input_file(const char* filename, int* size);
// return decode status
int rle_decode_frame(const uint8_t** input, const uint8_t* end_of_input, uint8_t* output);
// returns amount of decoded frames
int rle_decode_file(const char* filename, uint8_t* frame_data);

#endif // RLE_DECODE_H
