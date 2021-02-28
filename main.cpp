
/* 
 * File:   main.cpp
 * Author: Ilya Pikin
 */

#include <cstdlib>
#include <iostream>

#include "WavFileReader.h"
#include "MifFileWriter.h"

#define MIF_WORD_WIDTH  128
#define MIF_WORDS_NUM   1024
#define BITS_PER_SAMPLE 16
#define BYTES_PER_SAMPLE 2

#define SAMPLES_NUM (MIF_WORD_WIDTH / BITS_PER_SAMPLE)

using namespace std;

int main(int argc, char** argv) {
    float buffer[SAMPLES_NUM];
    uint8_t wordBuffer[SAMPLES_NUM * BYTES_PER_SAMPLE];
    size_t samplesRead;
    WavFileReader reader;
    MifFileWriter writer(MIF_WORD_WIDTH, MIF_WORDS_NUM);

    writer.open("data/impulse.mif");
    reader.open("data/impulse.wav");
    writer.writeHeader();

    while (true) {
        reader.read(SAMPLES_NUM, buffer, &samplesRead);
        if (samplesRead != SAMPLES_NUM) break;
        for (size_t i = 0; i < sizeof (wordBuffer); i += BYTES_PER_SAMPLE) {
            int16_t sample = buffer[i / BYTES_PER_SAMPLE] * INT16_MAX;
            wordBuffer[i] = (sample >> 8);
            wordBuffer[i + 1] = (sample & 0xFF);
        }
        if (writer.writeWord(wordBuffer) == MifFileWriter::END_OF_FILE) break;
    }
    writer.writeEoF();

    return 0;
}

