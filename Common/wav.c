
#include <wav.h>

int wav_decode(uint8_t* addr, wav_t* wav)
{
    uint8_t* p = addr;
    uint32_t chunksize;
    uint32_t subchunksize;
    if(0 != memcmp(p,CHUNK_RIFF,4))
    {
        return -1;
    }
    p += 4;
    chunksize = (uint32_t)p[0] | ((uint32_t)p[1]<<8) | ((uint32_t)p[2]<<16) | ((uint32_t)p[3]<<24);
    wav->chunksize = chunksize;
    p += 4;
    if(0 != memcmp(p,CHUNK_WAVE,4))
    {
        return -2;
    }
    p += 4;

    do
    {
        if(0 == memcmp(p,CHUNK_FMT,4))
        {
            p += 4;
            subchunksize = (uint32_t)p[0] | ((uint32_t)p[1]<<8) | ((uint32_t)p[2]<<16) | ((uint32_t)p[3]<<24);
            p += 4;
            /* 解析参数 */
            wav->audioformat = (uint16_t)p[0] | ((uint16_t)p[1]<<8);
            if((wav->audioformat == 0x0001) || (wav->audioformat == 0xFFFE))
            {
                p += 2;
                wav->numchannels = (uint16_t)p[0] | ((uint16_t)p[1]<<8);
                p += 2;
                wav->samplerate = (uint32_t)p[0] | ((uint32_t)p[1]<<8) | ((uint32_t)p[2]<<16) | ((uint32_t)p[3]<<24);
                p += 4;
                wav->byterate = (uint32_t)p[0] | ((uint32_t)p[1]<<8) | ((uint32_t)p[2]<<16) | ((uint32_t)p[3]<<24);
                p += 4;
                wav->blockalign = (uint16_t)p[0] | ((uint16_t)p[1]<<8);
                p += 2;
                wav ->bitspersample = (uint16_t)p[0] | ((uint16_t)p[1]<<8);
                p += 2;

                if(subchunksize >16)
                {
                    /* 有ext区域 */
                    uint16_t cbsize = (uint16_t)p[0] | ((uint16_t)p[1]<<8);
                    p += 2;
                    if(cbsize > 0)
                    {
                        /* ext数据 2字节有效bits wValidBitsPerSample ，4字节dwChannelMask 16字节SubFormat */
                        p += 2;
                        p += 4;
                        /* 比对subformat */
                        p += 16;       
                    }
                }
            }
            else
            {
                p += subchunksize;
            }
        }
        else if(0 == memcmp(p,CHUNK_DATA,4))
        {
            p += 4;
            subchunksize = (uint32_t)p[0] | ((uint32_t)p[1]<<8) | ((uint32_t)p[2]<<16) | ((uint32_t)p[3]<<24);
            wav->datasize = subchunksize;
            p += 4;
            wav->off = (uint32_t)(p- addr);
            return 0;
        }
        else
        {
            p += 4;
            subchunksize = (uint32_t)p[0] | ((uint32_t)p[1]<<8) | ((uint32_t)p[2]<<16) | ((uint32_t)p[3]<<24);
            p += 4;
            p += subchunksize;
        }
    }while((uint32_t)(p - addr) < (chunksize + 8));
    return -3;
}

void wav_encode(uint8_t* wav, int samples, int chnum, int freq)
{
    /*
     * 添加wav头信息
     */
    uint32_t chunksize = 44-8+samples*chnum*16/8;
    uint8_t* p = (uint8_t*)wav;
    uint32_t bps = freq*chnum*16/8;
    uint32_t datalen = samples*chnum*16/8;
    p[0] = 'R';
    p[1] = 'I';
    p[2] = 'F';
    p[3] = 'F';
    p[4] = chunksize & 0xFF;
    p[5] = (chunksize>>8) & 0xFF;
    p[6] = (chunksize>>16) & 0xFF;
    p[7] = (chunksize>>24) & 0xFF;
    p[8] = 'W';
    p[9] = 'A';
    p[10] = 'V';
    p[11] = 'E';

    p[12] = 'f';
    p[13] = 'm';
    p[14] = 't';
    p[15] = ' ';

    p[16] = 16;  /* Subchunk1Size */
    p[17] = 0;
    p[18] = 0;
    p[19] = 0;

    p[20] = 1;  /* PCM */
    p[21] = 0;

    p[22] = chnum; /* 通道数 */
    p[23] = 0;

    p[24] = freq & 0xFF;
    p[25] = (freq>>8) & 0xFF;
    p[26] = (freq>>16) & 0xFF;
    p[27] = (freq>>24) & 0xFF; 

    p[28] = bps & 0xFF;      /* ByteRate */
    p[29] = (bps>>8) & 0xFF;
    p[30] = (bps>>16) & 0xFF;
    p[31] = (bps>>24) & 0xFF; 

    p[32] = chnum*16/8; /* BlockAlign */
    p[33] = 0;

    p[34] = 16;  /* BitsPerSample */
    p[35] = 0;

    p[36] = 'd';
    p[37] = 'a';
    p[38] = 't';
    p[39] = 'a';

    p[40] = datalen & 0xFF;
    p[41] = (datalen>>8) & 0xFF;
    p[42] = (datalen>>16) & 0xFF;
    p[43] = (datalen>>24) & 0xFF; 

}