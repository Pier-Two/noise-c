/* ed25519-randombytes-custom.h
 *
 * This header satisfies ed25519-donna when ED25519_CUSTOMRANDOM is
 * defined.  The actual implementation lives in Noise-C’s rand_os.c.
 */
#ifndef ED25519_RANDOMBYTES_CUSTOM_H
#define ED25519_RANDOMBYTES_CUSTOM_H
#include <stddef.h>
void ed25519_randombytes_unsafe(void *p, size_t len);
#endif