//-----------------------------------------------------------------------------
// MurmurHash was written by Austin Appleby, and is placed in the public
// domain. The author hereby disclaims copyright to this source code.

// Note - This code makes a few assumptions about how your machine behaves -

// 1. We can read a 4-byte value from any address without crashing
// 2. sizeof(int) == 4

// And it has a few limitations -

// 1. It will not work incrementally.
// 2. It will not produce the same results on little-endian and big-endian
//    machines.

#include "MurmurHash1.h"
#include <cstring>
#include <cstdlib>
#include "t1ha/t1ha_bits.h"
#include <algorithm>

static inline uint32_t
_wyr4 (const uint32_t *p, uint32_t k)
{
  const uint8_t *pb = (const uint8_t *)p;
  return (((uint32_t)pb[0]) << 16) | (((uint32_t)pb[k >> 1]) << 8) | pb[k - 1];
}

static inline uint32_t
_wyr4_shift (const uint32_t *p, int shift)
{
  const uint8_t *pb = (const uint8_t *)p;
  return *(uint32_t *)(pb + shift);
}

#define murmix1(c1, c2, seed, data)                                           \
  ((c1 ^ seed.Seed.lo ^ data) * (c2 ^ seed.Seed.hi))
#define murmix2(c1, c2, seed, data1, data2)                                   \
  ((c1 ^ data1 ^ seed.Seed.lo) * (c2 ^ data2 ^ seed.Seed.hi))

//murmur22

uint32_t
MurmurHash11 (const void *key, int len, uint32_t s)
{
  const uint64_t c1 = 0x8b174952c8e6e651;
  const uint64_t c2 = 0xc46257516d31d2ab;
  const uint64_t c3 = 0xd9649531d138d397;
  const uint64_t c4 = 0xa25d6cdc4d54eacd;
  const uint64_t c5 = 0x2b6764ba56c934a3;

  MURMUR11_CTX seed = { s + 1, s - 1};

  const uint32_t *data = (const uint32_t *)key;
  int len2 = len;
  if (len <= 16)
  {     
      if (len < 4)
      {
          if (len != 0)
          {
              seed.Seed.val = murmix1 (c1, c2, seed, _wyr4 (data, len));
              seed.Seed.val = murmix1 (c2, c4, seed, len);
          }
          else          
          {
              seed.Seed.val = murmix1 (c2, c4, seed, 0);
          }          
          return seed.Seed.hi ^ seed.Seed.lo;
      }
      if (len <= 8)
      {
          seed.Seed.val = murmix2 (c1, c2, seed, data[0], _wyr4_shift(data, len - 4));
          seed.Seed.val = murmix1 (c3, c4, seed, len2);
          return seed.Seed.hi ^ seed.Seed.lo;
      }
      MURMUR11_CTX see1 = seed;
      if (len <= 12)
      {
          seed.Seed.val = murmix2 (c1, c2, seed, data[0], data[1]);
          see1.Seed.val = murmix1 (c3, c4, see1, _wyr4_shift (data, len - 4));
          seed.Seed.val ^= see1.Seed.val;
          seed.Seed.val = murmix1 (c2, c4, seed, len2);
          return seed.Seed.hi ^ seed.Seed.lo;
      }
      seed.Seed.val = murmix2 (c1, c2, seed, data[0], data[1]);
      see1.Seed.val = murmix2 (c3, c4, see1, data[2], _wyr4_shift (data, len - 4));
      seed.Seed.val ^= see1.Seed.val;
      seed.Seed.val = murmix1 (c2, c4, seed, len2);
      return seed.Seed.hi ^ seed.Seed.lo;
  }
  if (len >= 32)
    {
      MURMUR11_CTX see1 = seed;
      MURMUR11_CTX see2 = seed;
      MURMUR11_CTX see3 = seed;

      do
        {

          seed.Seed.val = murmix2 (c1, c2, seed, data[0], data[1]);
          see1.Seed.val = murmix2 (c3, c4, see1, data[2], data[3]);
          see2.Seed.val = murmix2 (c2, c3, see2, data[4], data[5]);
          see3.Seed.val = murmix2 (c4, c1, see3, data[6], data[7]);

          data += 8;
          len -= 32;
        }
      while (len >= 32);
      seed.Seed.val ^= see1.Seed.val ^ see2.Seed.val ^ see3.Seed.val;
    }
  if (len >= 24)
    {
      MURMUR11_CTX see1 = seed;
      MURMUR11_CTX see2 = seed;

      seed.Seed.val = murmix2 (c1, c2, seed, data[0], data[1]);
      see1.Seed.val = murmix2 (c3, c4, see1, data[2], data[3]);
      see2.Seed.val = murmix2 (c2, c3, see2, data[4], data[5]);
      data += 6;
      len -= 24;
      seed.Seed.val ^= see1.Seed.val ^ see2.Seed.val;
    }
  else if (len >= 16)
    {
      MURMUR11_CTX see1 = seed;

      seed.Seed.val = murmix2 (c1, c2, seed, data[0], data[1]);
      see1.Seed.val = murmix2 (c3, c4, see1, data[2], data[3]);
      data += 4;
      len -= 16;
      seed.Seed.val ^= see1.Seed.val;
    } 
  else if (len >= 8)
    {

      seed.Seed.val = murmix2 (c1, c2, seed, data[0], data[1]);
      data += 2;
      len -= 8;
    } 
    if (len >= 4)
    {
      seed.Seed.val = murmix1 (c1, c2, seed, data[0]);
      data++;
      len -= 4;
    }
  //----------
  if (len != 0)
  {
      seed.Seed.val = murmix1 (c1, c2, seed, _wyr4_shift(data, len - 4));
  }

  seed.Seed.val = murmix1 (c2, c4, seed, len2);

  return seed.Seed.hi ^ seed.Seed.lo;
}

//-----------------------------------------------------------------------------

// objsize: 0-0x157: 343
uint32_t
MurmurHash1 (const void *key, int len, uint32_t seed)
{
  const unsigned int m = 0xc6a4a793;

  const int r = 16;

  unsigned int h = seed ^ (len * m);

  //----------

  const unsigned char *data = (const unsigned char *)key;

  while (len >= 4)
    {
      unsigned int k = *(unsigned int *)data;

      h += k;
      h *= m;
      h ^= h >> 16;

      data += 4;
      len -= 4;
    }

  //----------

  switch (len)
    {
    case 3:
      h += data[2] << 16;
    case 2:
      h += data[1] << 8;
    case 1:
      h += data[0];
      h *= m;
      h ^= h >> r;
    };

  //----------

  h *= m;
  h ^= h >> 10;
  h *= m;
  h ^= h >> 17;

  return h;
}

//-----------------------------------------------------------------------------
// MurmurHash1Aligned, by Austin Appleby

// Same algorithm as MurmurHash1, but only does aligned reads - should be safer
// on certain platforms.

// Performance should be equal to or better than the simple version.
// objsize: 0x160-0x4e3: 899

unsigned int
MurmurHash1Aligned (const void *key, int len, unsigned int seed)
{
  const unsigned int m = 0xc6a4a793;
  const int r = 16;

  const unsigned char *data = (const unsigned char *)key;

  unsigned int h = seed ^ (len * m);

  int align = (uint64_t)data & 3;

  if (align && (len >= 4))
    {
      // Pre-load the temp registers

      unsigned int t = 0, d = 0;

      switch (align)
        {
        case 1:
          t |= data[2] << 16;
        case 2:
          t |= data[1] << 8;
        case 3:
          t |= data[0];
        }

      t <<= (8 * align);

      data += 4 - align;
      len -= 4 - align;

      int sl = 8 * (4 - align);
      int sr = 8 * align;

      // Mix

      while (len >= 4)
        {
          d = *(unsigned int *)data;
          t = (t >> sr) | (d << sl);
          h += t;
          h *= m;
          h ^= h >> r;
          t = d;

          data += 4;
          len -= 4;
        }

      // Handle leftover data in temp registers

      int pack = len < align ? len : align;

      d = 0;

      switch (pack)
        {
        case 3:
          d |= data[2] << 16;
        case 2:
          d |= data[1] << 8;
        case 1:
          d |= data[0];
        case 0:
          h += (t >> sr) | (d << sl);
          h *= m;
          h ^= h >> r;
        }

      data += pack;
      len -= pack;
    }
  else
    {
      while (len >= 4)
        {
          h += *(unsigned int *)data;
          h *= m;
          h ^= h >> r;

          data += 4;
          len -= 4;
        }
    }

  //----------
  // Handle tail bytes

  switch (len)
    {
    case 3:
      h += data[2] << 16;
    case 2:
      h += data[1] << 8;
    case 1:
      h += data[0];
      h *= m;
      h ^= h >> r;
    };

  h *= m;
  h ^= h >> 10;
  h *= m;
  h ^= h >> 17;

  return h;
}


static inline uint32_t
load32u (const uint8_t *p)
{
  uint32_t v;
  memcpy (&v, p, 4);
  return v;
}
static inline uint64_t
load64u (const uint8_t *p)
{
  uint64_t v;
  memcpy (&v, p, 8);
  return v;
}

// k = 1..3  (classic wyhash pattern)
static inline uint64_t
_wyr3 (const uint8_t *p, uint32_t k)
{
  return ((uint64_t)p[0] << 16) | ((uint64_t)p[k >> 1] << 8) | p[k - 1];
}

// k = 4..8
static inline uint64_t
_wyr8 (const uint8_t *p, uint32_t k)
{
  return (uint64_t)load32u (p) | ((uint64_t)load32u (p + k - 4) << 32);
}

// k = 9..12
static inline MURMUR22_CTX
_wyr12 (const uint8_t *p, uint32_t k)
{
  uint64_t lo = load64u (p);
  uint64_t hi = 0;
  memcpy (&hi, p + k - 4, 4);
  return { lo, hi };
}

// k = 13..16
static inline MURMUR22_CTX
_wyr16 (const uint8_t *p, uint32_t k)
{
  uint64_t lo = load64u (p);
  uint64_t hi = load64u (p + k - 8);
  return { lo, hi };
}
static inline uint64_t
_rot64 (const uint64_t x, int k)
{
  return (x << k) | (x >> (64 - k));
}


    static inline void
MurMix22 (const MURMUR22_CTX &c, const MURMUR22_CTX &d, MURMUR22_CTX &s)
{
//   uint64_t v1 = c.lo ^ d.lo ^ s.lo;
//   uint64_t v2 = c.hi ^ d.hi ^ s.hi;
// 
//   uint64_t v1h = v1^(v1 >> 32);
//   uint64_t v2h = v2^(v2 >> 32);
// 
//   uint64_t r1 = v1 * v2h;
//   uint64_t r2 = v2 * v1h;
//   s.lo ^= r1 ^_rot64 (r2, 42);
//   s.hi = r2;
  s.lo ^= _umul128 (c.lo ^ s.lo ^ d.lo, c.hi ^ s.hi ^ d.hi, &s.hi);
}

static inline void
MurMix22x4 (const MURMUR22_CTX *c, const uint8_t *d, MURMUR22_CTX *s)
{
    for (int i = 0; i < 4; i++)
    {
        MurMix22 (c[i], _wyr16 (d + i * 16, 16), s[i]);
    }
//     std::swap (s[0], s[1]);
//     std::swap (s[2], s[3]);
}

static inline void
MurMix22x8 (const MURMUR22_CTX * c, const uint8_t *d, MURMUR22_CTX * s)
{
    /*
      uint64_t v1 = c.lo ^ d.lo ^ s.lo;
      uint64_t v2 = c.hi ^ d.hi ^ s.hi;

      uint64_t v1h = v1^(v1 >> 32);
      uint64_t v2h = v2^(v2 >> 32);

      uint64_t r1 = v1 * v2h;
      uint64_t r2 = v2 * v1h;
      s.lo ^= r1 ^_rot64 (r2, 42);
      s.hi = r2;
    */
  __m512i dl = _mm512_loadu_si512 ((const __m512i *)d);
  __m512i dh = _mm512_loadu_si512 ((const __m512i *)d + 1);
  __m512i cl = _mm512_loadu_si512 ((const __m512i *)c);
  __m512i ch = _mm512_loadu_si512 ((const __m512i *)c + 1);
  __m512i sl = _mm512_loadu_si512 ((const __m512i *)s);
  __m512i sh = _mm512_loadu_si512 ((const __m512i *)s + 1);
  __m512i v1 = _mm512_xor_si512 (_mm512_xor_si512 (cl, sl), dl);
  __m512i v2 = _mm512_xor_si512 (_mm512_xor_si512 (ch, sh), dh);
  __m512i v1h = _mm512_xor_si512 (v1, _mm512_srli_epi64 (v1, 32));
  __m512i v2h = _mm512_xor_si512 (v2, _mm512_srli_epi64 (v2, 32));
  __m512i r1 = _mm512_mullo_epi64 (v1, v2h);
  __m512i r2 = _mm512_mullo_epi64 (v2, v1h);
  sl = _mm512_xor_si512 (sl, _mm512_xor_si512 (r1, _mm512_ror_epi64 (r2, 42)));
  sh = r2;

  _mm512_storeu_si512 ((__m512i *)s, sl);
  _mm512_storeu_si512 ((__m512i *)s + 1, sh);
}

static inline uint64_t
MurmurHash22_Finalize (const MURMUR22_CTX &s)
{
  return s.lo ^ s.hi;
}

uint64_t
MurmurHash22 (const void *key, int len, uint64_t s)
{
  const MURMUR22_CTX c[] = {
    { 0x9C54531DAD5396B3ull, 0xD75CB969AE36C967ull },
    { 0xAD9CACEA526A6539ull, 0xC69A72D2E2ABAC4Dull },
    { 0xB54DCA3146A5496Dull, 0x93A68CE45D5AB273ull },
    { 0xD8DA264EB198DA47ull, 0xB4ED6A237327239Bull },
    { 0xE92B6C56CD6D2745ull, 0x9B38D394B2B465C5ull },
    { 0xB631D69D4656526Dull, 0xB6C5BA2B694D9289ull },
    { 0xA4E4E323A5A4DA97ull, 0x89169CB124EB5C6Dull },
    { 0x925C59128A32B9C9ull, 0xD4A769C9D5C6AB27ull },    
  };              
      
  MURMUR22_CTX seed[8];
  seed[0] = { s, s };

  const uint8_t *p = (const uint8_t *)key;

  int len2 = len;
  while (true) {
      if (len < 128)
        {
//           if (len >= 64) {
//               seed[1] = seed[0];
//               seed[2] = seed[0];
//               seed[3] = seed[0];
//               MurMix22x4 (c, p, seed);
//               seed[0].lo ^= seed[1].lo ^ seed[2].lo ^ seed[3].lo;
//               seed[0].hi ^= seed[1].hi ^ seed[2].hi ^ seed[3].hi;
//               p += 4 * 16;
//               len -= 4 * 16;
//           }
          while (len >= 16)
            {
              MurMix22 (c[0], _wyr16 (p, 16), seed[0]);
              p += 16;
              len -= 16;
            }

          if (len == 0)
            {
              MurMix22 (c[3], { (uint64_t)len2, 0xD58973A518CECB6Dull }, seed[0]);
              return MurmurHash22_Finalize(seed[0]);
            }
          if (len < 4)
            {
              MurMix22 (c[0], { _wyr3 (p, len), 0xE9D8CE56ED95A589ull }, seed[0]);
            }
          else if (len <= 8)
            {
              MurMix22 (c[0], { _wyr8 (p, len), 0xE9D8CE56ED95A589ull }, seed[0]);
            }
          else if (len <= 12)
            {
              MurMix22 (c[0], _wyr12 (p, len), seed[0]);
            }
          else
            { // 13..16
              MurMix22 (c[0], _wyr16 (p, len), seed[0]);
            }
          MurMix22 (c[3], { (uint64_t)len2, 0xD58973A518CECB6Dull }, seed[0]);
          return MurmurHash22_Finalize(seed[0]);
      } 
      seed[1] = seed[0];
      seed[2] = seed[0];
      seed[3] = seed[0];
      seed[4] = seed[0];
      seed[5] = seed[0];
      seed[6] = seed[0];
      seed[7] = seed[0];
      do
        {
          MurMix22x8 (c, p, seed);
//           MurMix22 (c[0], (p + 16 * 0), seed[0]);
//           MurMix22 (c[1], (p + 16 * 1), seed[1]);
//           MurMix22 (c[2], (p + 16 * 2), seed[2]);
//           MurMix22 (c[3], (p + 16 * 3), seed[3]);
          //std::swap (seed[0], seed[1]);
          //std::swap (seed[2], seed[3]);
          p += 8*16;
          len -= 8 * 16;
        }
      while (len >= 128);

      MurMix22x4 (c, (const uint8_t*) (seed+4), seed);
      MurMix22 (c[4], seed[1], seed[0]);
      MurMix22 (c[5], seed[2], seed[3]);
      MurMix22 (c[6], seed[3], seed[0]);
  }
}
