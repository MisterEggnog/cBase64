#ifndef CBASE64STREAM_H_INCLUDED
#define CBASE64STREAM_H_INCLUDED

#include <stddef.h>

#define BASE64_DIGITS "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz+/"
#define PADDING "="

typedef struct {
  char quad[5];
} Base64Quard;

// Convert 3 digits to base64
// len < 3, there will be padding
// If len > 3 or == 0 program may exit with an error.
//
// Note that dest must be a string of at least length 4.
void
base64_encode(const unsigned char input[], size_t len, char dest[]);

// Convert 4 base64 numbers into data, return the number of decoded byres.
// Dest must be an array of size > 3.
size_t
base64_decode(const Base64Quard* data, unsigned char dest[]);

#endif // CBASE64STREAM_H_INCLUDED
