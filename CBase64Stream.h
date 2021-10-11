#ifndef CBASE64STREAM_H_INCLUDED
#define CBASE64STREAM_H_INCLUDED

#include <stddef.h>

typedef struct {
  char quad[5];
} Base64Quard;

// Convert 3 digits to base64
// len < 3, there will be padding
// If len > 3 or == 0 function will return 0
int
base64_encode(const unsigned char* input, size_t len, Base64Quard* dest);

// Convert 4 base64 numbers into data, return the number of decoded byres.
// Dest must be an array of size > 3.
size_t
base64_decode(const Base64Quard* data, unsigned char dest[]);

#endif // CBASE64STREAM_H_INCLUDED
