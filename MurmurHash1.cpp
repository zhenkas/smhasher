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

static inline void
MurMix22 (const MURMUR22_CTX &c, const MURMUR22_CTX &d, MURMUR22_CTX &s)
{
  s.lo ^= _umul128 (c.lo ^ s.lo ^ d.lo, c.hi ^ s.hi ^ d.hi, &s.hi);
}
static inline uint64_t
MurmurHash22_Finalize (const MURMUR22_CTX &s)
{
  return s.lo ^ s.hi;
}

uint64_t
MurmurHash22 (const void *key, int len, uint64_t s)
{
  const MURMUR22_CTX c1 = { 0x9C54531DAD5396B3ull, 0xD75CB969AE36C967ull };
  const MURMUR22_CTX c2 = { 0xAD9CACEA526A6539ull, 0xC69A72D2E2ABAC4Dull };
  const MURMUR22_CTX c3 = { 0xB54DCA3146A5496Dull, 0x93A68CE45D5AB273ull };
  const MURMUR22_CTX c4 = { 0xD8DA264EB198DA47ull, 0xB4ED6A237327239Bull };

       
       
       
      
  MURMUR22_CTX seed = { s, s };

  const uint8_t *p = (const uint8_t *)key;

  int len2 = len;
  while (true) {
      if (len < 64)
        {
          while (len >= 16)
            {
              MurMix22 (c1, _wyr16 (p, 16), seed);
              p += 16;
              len -= 16;
            }

          if (len == 0)
            {
              MurMix22 (c4, { (uint64_t)len2, 0xD58973A518CECB6Dull }, seed);
              return MurmurHash22_Finalize(seed);
            }
          if (len < 4)
            {
              MurMix22 (c1, { _wyr3 (p, len), 0xE9D8CE56ED95A589ull }, seed);
            }
          else if (len <= 8)
            {
              MurMix22 (c1, { _wyr8 (p, len), 0xE9D8CE56ED95A589ull }, seed);
            }
          else if (len <= 12)
            {
              MurMix22 (c1, _wyr12 (p, len), seed);
            }
          else
            { // 13..16
              MurMix22 (c1, _wyr16 (p, len), seed);
            }
          MurMix22 (c4, { (uint64_t)len2, 0xD58973A518CECB6Dull }, seed);
          return MurmurHash22_Finalize(seed);
      } 
      MURMUR22_CTX see1 = seed;
      MURMUR22_CTX see2 = seed;
      MURMUR22_CTX see3 = seed;
      do
        {
          MurMix22 (c1, (p + 16 * 0), seed);
          MurMix22 (c2, (p + 16 * 1), see1);
          MurMix22 (c3, (p + 16 * 2), see2);
          MurMix22 (c4, (p + 16 * 3), see3);
          std::swap (seed, see1);
          std::swap (see2, see3);
          p += 4*16;
          len -= 4 * 16;
        }
      while (len >= 64);
      seed.lo ^= see1.lo ^ see2.lo ^ see3.lo;
      seed.hi ^= see1.hi ^ see2.hi ^ see3.hi;
  }
}
