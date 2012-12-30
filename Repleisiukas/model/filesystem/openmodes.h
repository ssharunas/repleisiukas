#ifndef OPENMODES_H
#define OPENMODES_H

#include <QString>

const char* MODE_READ = "r";
const char* MODE_WRITE = "w";
const char* MODE_APPEND = "a";

bool is_valid_open_mode(QString mode){
    return mode == MODE_READ || mode == MODE_WRITE || mode == MODE_APPEND;
}

#endif // OPENMODES_H
