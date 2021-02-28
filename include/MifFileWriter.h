
/* 
 * File:   MifFileWriter.h
 * Author: Ilya Pikin
 */

#ifndef MIFFILEWRITER_H
#define MIFFILEWRITER_H

#include <cstdint>
#include <cstdlib>
#include <fstream>

class MifFileWriter {
public:
    MifFileWriter(size_t width, size_t depth);
    MifFileWriter(const MifFileWriter& orig);
    virtual ~MifFileWriter();

    enum Status {
        OK = 0,
        END_OF_FILE,
        OPEN_ERROR,
        WRITE_ERROR,
    };

    Status open(const char * path);
    Status writeHeader();
    Status writeWord(uint8_t * data);
    Status writeEoF();
    void close();

private:
    const size_t width;
    const size_t depth;
    size_t wordIndex = 0;
    std::ofstream file;
};

#endif /* MIFFILEWRITER_H */

