#ifndef CBASE64STREAM_H_INCLUDED
#define CBASE64STREAM_H_INCLUDED

#include <stdio.h>

// Needs shim for `restrict` keyword

typedef struct {
  char quad[5];
} Base64Quard;

// Request 4 base64 numbers, when the end of the file is found, end will be .
// If C had sum types this would return an option.
int
base64_encode(FILE restrict* source, Base64Quard restrict* data);

// Convert 4 base64 numbers into data, then write to file.
void
base64_decode(FILE restrict* dest, const Base64Data restrict* data);

#endif // CBASE64STREAM_H_INCLUDED
