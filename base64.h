/*
 * Simple base64 encoding/decoding library.
 * <https://github.com/MisterEggnog/cBase64>
 *
 * Copyright 2022 Josiah Baldwin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */
#ifndef CBASE64STREAM_H_INCLUDED
#define CBASE64STREAM_H_INCLUDED

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#define static /**/
#endif // __cplusplus

// RFC 4648 §4
#define BASE64_DIGITS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
#define PADDING '='

// Convert 3 digits to base64
// len < 3, there will be padding
// If len > 3 or == 0 program may exit with an error.
//
// input must be an array of at least length 3.
// dest must be an array of at least length 4.
void
base64_encode(const unsigned char input[static 3], size_t len, char dest[static 4]);

// Convert 4 base64 numbers into 3 bytes, return the number of decoded byres.
// encoded must be a string of at least 4 characters
// raw must be an array of size > 3.
// return how many bytes were writen to raw
//
// Program may exit if input contains characters outside of BASE64_DIGITS or PADDING
size_t
base64_decode(const char encoded[], unsigned char raw[]);

#ifdef __cplusplus
#undef static
}
#endif // __cplusplus

#endif // CBASE64STREAM_H_INCLUDED
