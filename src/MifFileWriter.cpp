
/* 
 * File:   MifFileWriter.cpp
 * Author: Ilya Pikin
 */

#include <fstream>
#include <iomanip>

#include "MifFileWriter.h"

MifFileWriter::MifFileWriter(size_t width, size_t depth) : width(width), depth(depth) {
}

MifFileWriter::~MifFileWriter() {
}

MifFileWriter::Status MifFileWriter::open(const char* path) {
    file.open(path);
    if (!file.is_open())
        return OPEN_ERROR;
    return OK;
}

void MifFileWriter::close() {
    if (file.is_open()) {
        file.close();
    }
}

MifFileWriter::Status MifFileWriter::writeHeader() {
    file << "WIDTH = " << width << ";                  -- The size of data in bits\n";
    file << "DEPTH = " << depth << ";                  -- The size of memory in words\n";
    file << "ADDRESS_RADIX = HEX;          -- The radix for address values\n";
    file << "DATA_RADIX = HEX;             -- The radix for data values\n";
    file << "CONTENT                       -- start of (address : data pairs)\n";
    file << "BEGIN\n";

    return OK;
}

MifFileWriter::Status MifFileWriter::writeWord(uint8_t* data) {
    if (wordIndex == depth) return END_OF_FILE;

    size_t dataIndex = 0;
    file << std::setfill('0') << std::setw(2) << std::uppercase << std::hex << wordIndex++ << " : ";
    while (dataIndex < width / 8) {
        file << std::setfill('0') << std::setw(2) << std::uppercase << std::hex << (size_t) data[dataIndex++];
    }
    file << ";\n";
    return (wordIndex == depth) ? END_OF_FILE : OK;
}

MifFileWriter::Status MifFileWriter::writeEoF() {
    if (wordIndex == depth) {
        file << "END;";
        return OK;
    }
    return WRITE_ERROR;
}
