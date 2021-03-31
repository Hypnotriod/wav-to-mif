
/* 
 * File:   main.cpp
 * Author: Ilya Pikin
 */

#include <cstdlib>
#include <iostream>

#include "WavFileReader.h"
#include "MifFileWriter.h"

using namespace std;

int process(const char * input, const char * output, int samplesNum, int wordsNum) {
    float * buffer = new float[samplesNum];
    uint8_t * wordBuffer;
    size_t samplesRead;
    uint32_t bytesPerSample;
    uint32_t bitsPerSample;
    WavFileReader reader;

    if (reader.open(input) != WavFileReader::Status::OK) {
        cout << "Input file read error!" << endl;
        return -1;
    }

    bitsPerSample = reader.getHeader()->bitsPerSample;
    if (bitsPerSample != 8 && bitsPerSample != 16 && bitsPerSample != 24) {
        cout << "This tool only supports 8, 16 or 24 bits per sample." << endl;
        return -1;
    }

    bytesPerSample = bitsPerSample / 8;
    wordBuffer = new uint8_t[samplesNum * bytesPerSample];

    MifFileWriter writer(samplesNum * bitsPerSample, wordsNum);
    if (writer.open(output) != MifFileWriter::Status::OK ||
            writer.writeHeader() != MifFileWriter::Status::OK) {
        cout << "Can't create output file!" << endl;
        return -1;
    }

    while (true) {
        if (reader.getSamplesLeft())
            reader.read(samplesNum, buffer, &samplesRead);
        for (size_t i = 0; i < samplesNum * bytesPerSample; i += bytesPerSample) {
            if (bitsPerSample == 8) {
                wordBuffer[i] = samplesRead ? buffer[i / bytesPerSample] * INT8_MAX : 0;
            } else if (bitsPerSample == 16) {
                int16_t sample = samplesRead ? buffer[i / bytesPerSample] * INT16_MAX : 0;
                wordBuffer[i] = (sample >> 8);
                wordBuffer[i + 1] = (sample & 0xFF);
            } else {
                int32_t sample = samplesRead ? buffer[i / bytesPerSample] * INT32_MAX : 0;
                wordBuffer[i] = (sample >> 24) & 0xFF;
                wordBuffer[i + 1] = (sample >> 16) & 0xFF;
                wordBuffer[i + 2] = (sample >> 8) & 0xFF;
            }
            if (samplesRead) samplesRead--;
        }
        if (writer.writeWord(wordBuffer) == MifFileWriter::END_OF_FILE) break;
    }

    writer.writeEoF();
    delete[] buffer;
    delete[] wordBuffer;

    return 0;
}

int main(int argc, char** argv) {
    int samplesNum;
    int wordsNum;

    if (argc != 5) {
        cout << "Usage: wav-to-mif <path_to_wav_file> <path_to_mif_file> <samples_in_word_num> <words_num>" << endl;
        return 0;
    }

    try {
        samplesNum = stoi(argv[3]);
        wordsNum = stoi(argv[4]);
    } catch (invalid_argument e) {
        cout << "Invalid arguments!" << endl;
        return -1;
    } catch (out_of_range e) {
        cout << "Invalid arguments!" << endl;
        return -1;
    }

    return process(argv[1], argv[2], samplesNum, wordsNum);
}

