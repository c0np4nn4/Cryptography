#include <cstdint>
#define MASK 0x7fffffffffffffff

typedef uint64_t u64;

void key_expansion(u64& k0, u64& k2) {
	// k2
	k2 = ( ((k0 >> static_cast<u64>(1)) & MASK ) | (k0 << static_cast<u64>(63)) ) ^ ( k0 >> static_cast<u64>(63) );
}
