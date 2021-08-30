#include <cstdint>

typedef uint64_t u64;

void s_layer(u64&, u64&);
void inv_s_layer(u64&, u64&);

void sr_layer(u64&, u64&);
void inv_sr_layer(u64&, u64&);

void m_layer(u64& prev_state, u64& curr_state);
