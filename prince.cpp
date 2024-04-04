#include <cstdio>

#include "key_expansion.h"
#include "prince_core.h"

/* round constant at prince_core.cpp */
u64 RC[12] = {0x0000000000000000, 0x13198a2e03707344, 0xa4093822299f31d0,
              0x082efa98ec4e6c89, 0x452821e638d01377, 0xbe5466cf34e90c6c,
              0x7ef84f78fd955cb1, 0x85840851f1ac43aa, 0xc882d32f25323c54,
              0x64a51195e0e3610d, 0xd3b5a399ca0c2399, 0xc0ac29b7c97c50dd};

typedef unsigned int u4;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

void encrypt(u64 key[2], u64 &plaintext, u64 &ciphertext);
void decrypt(u64 key[2], u64 &ciphertext, u64 &plaintext);

void encrypt(u64 key[2], u64 &plaintext, u64 &ciphertext) {
  u64 k0 = key[0];
  u64 k1 = key[1];
  u64 k2 = 0x0;
  key_expansion(key[0], k2);

  u64 prev_state = plaintext;
  u64 curr_state = plaintext;

  /* Whitening with k0 */
  curr_state = prev_state ^ k0;
  prev_state = curr_state;

  /* === PRINCE CORE === */
  /* k1 */
  curr_state = prev_state ^ k1;
  prev_state = curr_state;

  /* RC[0] */
  curr_state = prev_state ^ RC[0];
  prev_state = curr_state;

  for (int i = 1; i <= 5; i++) {
    /* S */
    s_layer(prev_state, curr_state);
    prev_state = curr_state;

    /* M */
    sr_layer(prev_state, curr_state);
    prev_state = curr_state;
    m_layer(prev_state, curr_state);
    prev_state = curr_state;

    /* k1 */
    curr_state = prev_state ^ k1;
    prev_state = curr_state;
  }

  s_layer(prev_state, curr_state);
  prev_state = curr_state;

  m_layer(prev_state, curr_state);
  prev_state = curr_state;

  inv_s_layer(prev_state, curr_state);
  prev_state = curr_state;

  for (int i = 6; i <= 10; i++) {
    /* k1 */
    curr_state = prev_state ^ k1;
    prev_state = curr_state;

    /* RC[i] */
    curr_state = prev_state ^ RC[i];
    prev_state = curr_state;

    /* inv M */
    m_layer(prev_state, curr_state);
    prev_state = curr_state;
    inv_sr_layer(prev_state, curr_state);
    prev_state = curr_state;

    inv_s_layer(prev_state, curr_state);
    prev_state = curr_state;
  }

  /* RC[i] */
  curr_state = prev_state ^ RC[11];
  prev_state = curr_state;

  /* k1 */
  curr_state = prev_state ^ k1;
  prev_state = curr_state;

  /* Whitening with k2 */
  curr_state = prev_state ^ k2;
  prev_state = curr_state;

  ciphertext = curr_state;
  /* === PRINCE CORE === */
}

void decrypt(u64 key[2], u64 &ciphertext, u64 &plaintext) {
  u64 k0 = key[0];
  u64 k1 = key[1];
  u64 k2 = 0x0;
  key_expansion(key[0], k2); // k2 생성

  u64 prev_state = ciphertext;
  u64 curr_state = ciphertext;

  // k2를 사용한 Whitening (역방향)
  curr_state = prev_state ^ k2;
  prev_state = curr_state;

  // PRINCE CORE 역순으로 실행
  // k1
  curr_state = prev_state ^ k1;
  prev_state = curr_state;

  // RC[11]
  curr_state = prev_state ^ RC[11];
  prev_state = curr_state;

  for (int i = 10; i >= 6; i--) {
    // inv_S
    inv_s_layer(prev_state, curr_state);
    prev_state = curr_state;

    // inv_M
    inv_sr_layer(prev_state, curr_state);
    prev_state = curr_state;
    m_layer(prev_state, curr_state);
    prev_state = curr_state;

    // RC[i] & k1
    curr_state = prev_state ^ RC[i];
    prev_state = curr_state;
    curr_state = prev_state ^ k1;
    prev_state = curr_state;
  }

  inv_s_layer(prev_state, curr_state);
  prev_state = curr_state;

  m_layer(prev_state, curr_state);
  prev_state = curr_state;

  s_layer(prev_state, curr_state);
  prev_state = curr_state;

  for (int i = 5; i >= 1; i--) {
    // k1
    curr_state = prev_state ^ k1;
    prev_state = curr_state;

    // M & S
    m_layer(prev_state, curr_state);
    prev_state = curr_state;
    sr_layer(prev_state, curr_state);
    prev_state = curr_state;
    s_layer(prev_state, curr_state);
    prev_state = curr_state;

    // RC[i]
    curr_state = prev_state ^ RC[i];
    prev_state = curr_state;
  }

  // k1 & RC[0]
  curr_state = prev_state ^ k1;
  prev_state = curr_state;
  curr_state = prev_state ^ RC[0];
  prev_state = curr_state;

  // k0를 사용한 Whitening (역방향)
  curr_state = prev_state ^ k0;
  prev_state = curr_state;

  plaintext = curr_state;
}
