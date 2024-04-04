#include <cstdint>
#include <cstdio>

typedef unsigned char u4; // for nibble
typedef uint16_t u16;
typedef uint64_t u64;

void s_layer(u64 &, u64 &);
void inv_s_layer(u64 &, u64 &);
void sr_layer(u64 &, u64 &);
void inv_sr_layer(u64 &, u64 &);
void m_layer(u64 &prev_state, u64 &curr_state);

// round constant
// u64 RC[12] = {
// 0x0000000000000000,
// 0x13198a2e03707344,
// 0xa4093822299f31d0,
// 0x082efa98ec4e6c89,
// 0x452821e638d01377,
// 0xbe5466cf34e90c6c,
// 0x7ef84f78fd955cb1,
// 0x85840851f1ac43aa,
// 0xc882d32f25323c54,
// 0x64a51195e0e3610d,
// 0xd3b5a399ca0c2399,
// 0xc0ac29b7c97c50dd
//};

/* S Layer ================================================================ */
u4 sBox[16] = {0xB, 0xF, 0x3, 0x2, 0xA, 0xC, 0x9, 0x1,
               0x6, 0x7, 0x8, 0x0, 0xE, 0x5, 0xD, 0x4};
u4 inv_sBox[16] = {0xB, 0x7, 0x3, 0x2, 0xF, 0xD, 0x8, 0x9,
                   0xA, 0x6, 0x4, 0x0, 0x5, 0xE, 0xC, 0x1};

void s_layer(u64 &prev_state, u64 &curr_state) {
  u64 tmp = 0x0;
  for (int i = 0; i < 16; i++) {
    tmp = tmp << 4;
    u4 nibble;
    nibble = (prev_state & 0xf000000000000000) >> 60;
    //		printf("%d nibble : %llx\n", i, nibble);
    //// get nibble
    /* Substitution */
    nibble = sBox[nibble];
    tmp = tmp | nibble;
    //		printf("%d tmp : %llx\n", i, tmp);
    prev_state = prev_state << 4;
  }
  curr_state = tmp;
}
void inv_s_layer(u64 &prev_state, u64 &curr_state) {
  u64 tmp = 0x0;
  for (int i = 0; i < 16; i++) {
    tmp = tmp << 4;
    u4 nibble;
    nibble = (prev_state & 0xf000000000000000) >> 60;
    //		printf("%d nibble : %llx\n", i, nibble);
    //// get nibble
    /* Substitution */
    nibble = inv_sBox[nibble];
    tmp = tmp | nibble;
    //		printf("%d tmp : %llx\n", i, tmp);
    prev_state = prev_state << 4;
  }
  curr_state = tmp;
}
/* S Layer ================================================================ */

/* SR Layer ================================================================*/
u64 mask_col[4] = {
    0xf000f000f000f000,
    0x0f000f000f000f00,
    0x00f000f000f000f0,
    0x000f000f000f000f,
};
u64 left_rotate_u64(u64 &column, unsigned int n) {
  return (column >> static_cast<u64>(64 - n)) | (column << static_cast<u64>(n));
}
u64 right_rotate_u64(u64 &column, unsigned int n) {
  return (column << static_cast<u64>(64 - n)) | (column >> static_cast<u64>(n));
}
void sr_layer(u64 &prev_state, u64 &curr_state) {
  u64 tmp[4];
  //	printf("[+] prev_state : %llx\n", prev_state);
  for (int i = 0; i < 4; i++) {
    tmp[i] = prev_state & mask_col[i];
    //		printf("[+] tmp[%d] : %016llx\n", i, tmp[i]);
  }

  tmp[0] = tmp[0];
  tmp[1] = left_rotate_u64(tmp[1], 4 * 4 * 1);
  tmp[2] = left_rotate_u64(tmp[2], 4 * 4 * 2);
  tmp[3] = left_rotate_u64(tmp[3], 4 * 4 * 3);

  u64 result = 0x0;
  //	printf("\n");
  for (int i = 0; i < 4; i++) {
    //		printf("[*] tmp[%d] : %016llx\n", i, tmp[i]);
    result |= tmp[i];
  }
  curr_state = result;
}
void inv_sr_layer(u64 &prev_state, u64 &curr_state) {
  u64 tmp[4];
  //	printf("[+] prev_state : %llx\n", prev_state);
  for (int i = 0; i < 4; i++) {
    tmp[i] = prev_state & mask_col[i];
    //		printf("[+] tmp[%d] : %016llx\n", i, tmp[i]);
  }

  tmp[0] = tmp[0];
  tmp[1] = right_rotate_u64(tmp[1], 4 * 4 * 1);
  tmp[2] = right_rotate_u64(tmp[2], 4 * 4 * 2);
  tmp[3] = right_rotate_u64(tmp[3], 4 * 4 * 3);

  u64 result = 0x0;
  //	printf("\n");
  for (int i = 0; i < 4; i++) {
    //		printf("[*] tmp[%d] : %016llx\n", i, tmp[i]);
    result |= tmp[i];
  }
  curr_state = result;
}
/* SR Layer ================================================================*/

/* M` Layer ================================================================*/
void m_0(u4 x[4], u4 y[4]) {
  /* MSB				LSB */
  /*   0    1     2  	  3 */

  /* M */
  u64 M[4] = {0x08, 0x04, 0x02, 0x01};

  /* y0 */
  y[0] |= (x[1] ^ x[2] ^ x[3]) & M[0];
  y[0] |= (x[0] ^ x[2] ^ x[3]) & M[1];
  y[0] |= (x[0] ^ x[1] ^ x[3]) & M[2];
  y[0] |= (x[0] ^ x[1] ^ x[2]) & M[3];

  /* y1 */
  y[1] |= (x[0] ^ x[1] ^ x[2]) & M[0];
  y[1] |= (x[1] ^ x[2] ^ x[3]) & M[1];
  y[1] |= (x[0] ^ x[2] ^ x[3]) & M[2];
  y[1] |= (x[0] ^ x[1] ^ x[3]) & M[3];

  /* y2 */
  y[2] |= (x[0] ^ x[1] ^ x[3]) & M[0];
  y[2] |= (x[0] ^ x[1] ^ x[2]) & M[1];
  y[2] |= (x[1] & M[2]) ^ (x[2] & M[2]) ^ (x[3] & M[2]);
  y[2] |= (x[0] & M[3]) ^ (x[2] & M[3]) ^ (x[3] & M[3]);

  /* y3 */
  y[3] |= (x[0] & M[0]) ^ (x[2] & M[0]) ^ (x[3] & M[0]);
  y[3] |= (x[0] & M[1]) ^ (x[1] & M[1]) ^ (x[3] & M[1]);
  y[3] |= (x[0] & M[2]) ^ (x[1] & M[2]) ^ (x[2] & M[2]);
  y[3] |= (x[1] & M[3]) ^ (x[2] & M[3]) ^ (x[3] & M[3]);
}
void m_1(u4 x[4], u4 y[4]) {
  /* MSB				LSB */
  /*   0    1     2  	  3 */

  /* M */
  u64 M[4] = {0x08, 0x04, 0x02, 0x01};

  /* y0 */
  y[0] |= (x[0] & M[0]) ^ (x[1] & M[0]) ^ (x[2] & M[0]);
  y[0] |= (x[1] & M[1]) ^ (x[2] & M[1]) ^ (x[3] & M[1]);
  y[0] |= (x[0] & M[2]) ^ (x[2] & M[2]) ^ (x[3] & M[2]);
  y[0] |= (x[0] & M[3]) ^ (x[1] & M[3]) ^ (x[3] & M[3]);

  /* y1 */
  y[1] |= (x[0] & M[0]) ^ (x[1] & M[0]) ^ (x[3] & M[0]);
  y[1] |= (x[0] & M[1]) ^ (x[1] & M[1]) ^ (x[2] & M[1]);
  y[1] |= (x[1] & M[2]) ^ (x[2] & M[2]) ^ (x[3] & M[2]);
  y[1] |= (x[0] & M[3]) ^ (x[2] & M[3]) ^ (x[3] & M[3]);

  /* y2 */
  y[2] |= (x[0] & M[0]) ^ (x[2] & M[0]) ^ (x[3] & M[0]);
  y[2] |= (x[0] & M[1]) ^ (x[1] & M[1]) ^ (x[3] & M[1]);
  y[2] |= (x[0] & M[2]) ^ (x[1] & M[2]) ^ (x[2] & M[2]);
  y[2] |= (x[1] & M[3]) ^ (x[2] & M[3]) ^ (x[3] & M[3]);

  /* y3 */
  y[3] |= (x[1] & M[0]) ^ (x[2] & M[0]) ^ (x[3] & M[0]);
  y[3] |= (x[0] & M[1]) ^ (x[2] & M[1]) ^ (x[3] & M[1]);
  y[3] |= (x[0] & M[2]) ^ (x[1] & M[2]) ^ (x[3] & M[2]);
  y[3] |= (x[0] & M[3]) ^ (x[1] & M[3]) ^ (x[2] & M[3]);
}
void m_layer(u64 &prev_state, u64 &curr_state) {

  u64 MASK = 0x0f;

  u64 tmp_state = prev_state;
  // 4 nibbles * 16 = 64bits
  u4 nibble[4][4];
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      u4 tmp = (tmp_state >> static_cast<u64>(60)) & MASK;
      nibble[j][i] = tmp;
      tmp_state = tmp_state << static_cast<u64>(4);
    }
  }

  u4 result[4][4] = {
      {
          0x0,
      },
  };
  m_0(nibble[0], result[0]);
  m_1(nibble[1], result[1]);
  m_1(nibble[2], result[2]);
  m_0(nibble[3], result[3]);

  tmp_state = 0x0;
  curr_state = tmp_state;

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      curr_state = curr_state << static_cast<u64>(4);
      curr_state |= result[j][i];
    }
  }
}
/* M` Layer ================================================================*/
