#include "openmodes.h"

bool is_valid_open_mode(QString mode){
	return mode == MODE_READ || mode == MODE_WRITE || mode == MODE_APPEND || mode == MODE_READ_WRITE;
}
