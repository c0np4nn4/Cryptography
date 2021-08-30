#include "prince_core.h"
#include "key_expansion.h"
					
typedef unsigned int u4;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

void encrypt(u64 key[2], u64& plaintext, u64& ciphertext);
void decrypt(u64 key[2], u64& ciphertext);
