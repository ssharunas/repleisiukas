#ifndef OPENMODES_H
#define OPENMODES_H

#include <QString>

#define MODE_READ "r"
#define MODE_WRITE "w"
#define MODE_READ_WRITE "rw"
#define MODE_APPEND "a"

bool is_valid_open_mode(QString mode);

#endif // OPENMODES_H
