#include <cstdio>
#include <cstdint>
#include <cstring>

#include "prince.h"

using namespace std;

#define MASK 0x7fffffffffffffff

int main() {
	// key = {k0, k1, k2}				// 'k0' and 'k2' are the Whitening Keys.
										// 'k1' is for CORE Function.
//	u64 key[3];
//	key[0] = 0xf234567812345678;		// k0
//	key[1] = 0x8765432187654321;		// k1
//	key_expansion(key[0], key[2]);		// k2
	
//	FILE* fKey = fopen("key.in", "rb");
//	fgets(&key[0], 8, 1, fKey);			// k0	
//	fread(&key[1], 8, 1, fKey);			// k1
//	key_expansion(key[0], key[2]);		// k2
	
	u64 key[2];
	key[0] = 0x0000000000000000;
	key[1] = 0x0000000000000000;
	u64 pt = 0x0000000000000000;
	u64 ct = 0x0;
	
	encrypt(key, pt, ct);
	
	printf("%llx\n", ct);


	
//	u64 prev_state = 0x12345678abcdef01;
//	printf("%llx\n", prev_state);
//	
//	u64 tmp = 0x0;
//	m_layer(prev_state, tmp);
//	printf("%llx\n", tmp);
//	
//	u64 tmp2 = 0x0;
//	m_layer(tmp, tmp2);
//	printf("%llx\n", tmp2);
	
//	printf("[+] state : %llx\n", prev_state);
//	s_layer(prev_state, prev_state);
//	printf("[+] state : %llx\n", prev_state);
//	inv_s_layer(prev_state, prev_state);
//	printf("[+] state : %llx\n", prev_state);
//	
//	FILE *test = fopen("key.out", "wb");
//	fwrite(&key[2], 8, 1, test);	
//	fclose(fKey);

	return 0;
}
