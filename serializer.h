#ifndef SERIALIZER_H_INCLUDED
#define SERIALIZER_H_INCLUDED
#include "manager.h"

void serialize(contact*, char*, int);
int unserialize(char*, contact*);

#endif
