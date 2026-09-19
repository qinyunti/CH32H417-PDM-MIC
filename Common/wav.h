#ifndef WAV_H
#define WAV_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>

#define CHUNK_RIFF "RIFF"
#define CHUNK_WAVE "WAVE"
#define CHUNK_FMT "fmt "
#define CHUNK_DATA "data"

#define CHUNK_RIFF "RIFF"
#define CHUNK_WAVE "WAVE"
#define CHUNK_FMT "fmt "
#define CHUNK_DATA "data"

typedef struct
{
    uint32_t off;
    uint32_t chunksize;
    uint16_t audioformat;
    uint16_t numchannels;
    uint32_t samplerate;
    uint32_t byterate;
    uint16_t blockalign;
    uint16_t bitspersample;
    uint32_t datasize;
}wav_t;

int wav_decode(uint8_t* addr, wav_t* wav);
void wav_encode(uint8_t* wav, int samples, int chnum, int freq);

#ifdef __cplusplus
}
#endif

#endif