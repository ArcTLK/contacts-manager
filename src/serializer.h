#ifndef _serializer_h
#define _serializer_h
#include "manager.h"

char* serialize(contact*);
contact* unserialize(char*);

#endif