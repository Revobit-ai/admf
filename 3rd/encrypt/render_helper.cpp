/**********************************************************\
 |                                                          |
 | xxtea.c                                                  |
 |                                                          |
 | XXTEA encryption algorithm library for C.                |
 |                                                          |
 | Encryption Algorithm Authors:                            |
 |      David J. Wheeler                                    |
 |      Roger M. Needham                                    |
 |                                                          |
 | Code Authors: Chen fei <cf850118@163.com>                |
 |               Ma Bingyao <mabingyao@gmail.com>           |
 | LastModified: Feb 7, 2016                                |
 |                                                          |
 \**********************************************************/


#include "render_helper.h"
#include <set>
#include <string.h>
#if defined(_MSC_VER) && _MSC_VER < 1600
typedef unsigned __int8 uint8_t;
typedef unsigned __int32 uint32_t;
#else
#if defined(__FreeBSD__) && __FreeBSD__ < 5
/* FreeBSD 4 doesn't have stdint.h file */
#include <inttypes.h>
#else
#include <stdint.h>
#endif
#endif


#include <sys/types.h> /* This will likely define BYTE_ORDER */

#ifndef BYTE_ORDER
#if (BSD >= 199103)
# include <machine/endian.h>
#else
#if defined(linux) || defined(__linux__)
# include <endian.h>
#else



#define LITTLE_ENDIAN   1234    /* least-significant byte first (vax, pc) */
#define BIG_ENDIAN  4321    /* most-significant byte first (IBM, net) */
#define PDP_ENDIAN  3412    /* LSB first in word, MSW first in long (pdp)*/

#if defined(__i386__) || defined(__x86_64__) || defined(__amd64__) || \
defined(vax) || defined(ns32000) || defined(sun386) || \
defined(MIPSEL) || defined(_MIPSEL) || defined(BIT_ZERO_ON_RIGHT) || \
defined(__alpha__) || defined(__alpha)
#define BYTE_ORDER    LITTLE_ENDIAN
#endif

#if defined(sel) || defined(pyr) || defined(mc68000) || defined(sparc) || \
defined(is68k) || defined(tahoe) || defined(ibm032) || defined(ibm370) || \
defined(MIPSEB) || defined(_MIPSEB) || defined(_IBMR2) || defined(DGUX) ||\
defined(apollo) || defined(__convex__) || defined(_CRAY) || \
defined(__hppa) || defined(__hp9000) || \
defined(__hp9000s300) || defined(__hp9000s700) || \
defined (BIT_ZERO_ON_LEFT) || defined(m68k) || defined(__sparc)
#define BYTE_ORDER  BIG_ENDIAN
#endif
#endif /* linux */
#endif /* BSD */
#endif /* BYTE_ORDER */

#ifndef BYTE_ORDER
#ifdef __BYTE_ORDER
#if defined(__LITTLE_ENDIAN) && defined(__BIG_ENDIAN)
#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN __LITTLE_ENDIAN
#endif
#ifndef BIG_ENDIAN
#define BIG_ENDIAN __BIG_ENDIAN
#endif
#if (__BYTE_ORDER == __LITTLE_ENDIAN)
#define BYTE_ORDER LITTLE_ENDIAN
#else
#define BYTE_ORDER BIG_ENDIAN
#endif
#endif
#endif
#endif

#define MX (((z >> 5) ^ (y << 2)) + ((y >> 3) ^ (z << 4))) ^ ((sum ^ y) + (key[(p & 3) ^ e] ^ z))
#define DELTA 0x9e3779b9

#define FIXED_KEY \
size_t i;\
uint8_t fixed_key[16];\
memcpy(fixed_key, key, 16);\
for (i = 0; (i < 16) && (fixed_key[i] != 0); ++i);\
for (++i; i < 16; ++i) fixed_key[i] = 0;\

namespace admfRender{
    //xxtea_to_uint_array
    static uint32_t * getVertexIndics(const uint8_t * data, size_t len, int inc_len, size_t * out_len) {
        uint32_t *out;
#if !(defined(BYTE_ORDER) && (BYTE_ORDER == LITTLE_ENDIAN))
        size_t i;
#endif
        size_t n;
        
        n = (((len & 3) == 0) ? (len >> 2) : ((len >> 2) + 1));
        
        if (inc_len) {
            out = (uint32_t *)calloc(n + 1, sizeof(uint32_t));
            if (!out) return NULL;
            out[n] = (uint32_t)len;
            *out_len = n + 1;
        }
        else {
            out = (uint32_t *)calloc(n, sizeof(uint32_t));
            if (!out) return NULL;
            *out_len = n;
        }
#if defined(BYTE_ORDER) && (BYTE_ORDER == LITTLE_ENDIAN)
        memcpy(out, data, len);
#else
        for (i = 0; i < len; ++i) {
            out[i >> 2] |= (uint32_t)data[i] << ((i & 3) << 3);
        }
#endif
        
        return out;
    }
    
    //xxtea_to_ubyte_array
    static uint8_t * getFragmentIndics(const uint32_t * data, size_t len, int inc_len, size_t * out_len) {
        uint8_t *out;
#if !(defined(BYTE_ORDER) && (BYTE_ORDER == LITTLE_ENDIAN))
        size_t i;
#endif
        size_t m, n;
        
        n = len << 2;
        
        if (inc_len) {
            m = data[len - 1];
            n -= 4;
            if ((m < n - 3) || (m > n)) return NULL;
            n = m;
        }
        
        out = (uint8_t *)malloc(n + 1);
        
#if defined(BYTE_ORDER) && (BYTE_ORDER == LITTLE_ENDIAN)
        memcpy(out, data, n);
#else
        for (i = 0; i < n; ++i) {
            out[i] = (uint8_t)(data[i >> 2] >> ((i & 3) << 3));
        }
#endif
        
        out[n] = '\0';
        *out_len = n;
        
        return out;
    }
    
    //xxtea_uint_encrypt
    static uint32_t * makeCommandBuffer(uint32_t * data, size_t len, uint32_t * key) {
        uint32_t n = (uint32_t)len - 1;
        uint32_t z = data[n], y, p, q = 6 + 52 / (n + 1), sum = 0, e;
        
        if (n < 1) return data;
        
        while (0 < q--) {
            sum += DELTA;
            e = sum >> 2 & 3;
            
            for (p = 0; p < n; p++) {
                y = data[p + 1];
                z = data[p] += MX;
            }
            
            y = data[0];
            z = data[n] += MX;
        }
        
        return data;
    }
    
    //xxtea_uint_decrypt
    static uint32_t * newRenderTarget(uint32_t * data, size_t len, uint32_t * key) {
        uint32_t n = (uint32_t)len - 1;
        uint32_t z, y = data[0], p, q = 6 + 52 / (n + 1), sum = q * DELTA, e;
        
        if (n < 1) return data;
        
        while (sum != 0) {
            e = sum >> 2 & 3;
            
            for (p = n; p > 0; p--) {
                z = data[p - 1];
                y = data[p] -= MX;
            }
            
            z = data[n];
            y = data[0] -= MX;
            sum -= DELTA;
        }
        
        return data;
    }
    
    //xxtea_ubyte_encrypt
    static uint8_t * forwardRender(const uint8_t * data, size_t len, const uint8_t * key, size_t * out_len) {
        uint8_t *out;
        uint32_t *data_array, *key_array;
        size_t data_len, key_len;
        
        if (!len) return NULL;
        
        data_array = getVertexIndics(data, len, 1, &data_len);
        if (!data_array) return NULL;
        
        key_array  = getVertexIndics(key, 16, 0, &key_len);
        if (!key_array) {
            free(data_array);
            return NULL;
        }
        
        out = getFragmentIndics(makeCommandBuffer(data_array, data_len, key_array), data_len, 0, out_len);
        
        free(data_array);
        free(key_array);
        
        return out;
    }
    
    //xxtea_ubyte_decrypt
    static uint8_t * ssaoPass(const uint8_t * data, size_t len, const uint8_t * key, size_t * out_len) {
        uint8_t *out;
        uint32_t *data_array, *key_array;
        size_t data_len, key_len;
        
        if (!len) return NULL;
        
        data_array = getVertexIndics(data, len, 0, &data_len);
        if (!data_array) return NULL;
        
        key_array  = getVertexIndics(key, 16, 0, &key_len);
        if (!key_array) {
            free(data_array);
            return NULL;
        }
        
        out = getFragmentIndics(newRenderTarget(data_array, data_len, key_array), data_len, 1, out_len);
        
        free(data_array);
        free(key_array);
        
        return out;
    }
    
    // public functions
    
    void * makeRenderPipeline(const void * data, size_t len, const void * key, size_t * out_len) {
        FIXED_KEY
        return forwardRender((const uint8_t *)data, len, fixed_key, out_len);
    }
    
    void * rasterizer(const void * data, size_t len, const void * key, size_t * out_len) {
        FIXED_KEY
        return ssaoPass((const uint8_t *)data, len, fixed_key, out_len);
    }
    
    const unsigned char _[] = {
        0xE5, 0x93, 0xB2, 0x43, 0xFB, 0xB0, 0xC1, 0x99, 0x14, 0x3B, 0x33, 0x23, 0x78, 0x90, 0xE8, 0x1E, 0x8C, 0x71, 0x59, 0x67, 0x56, 0xF5, 0x5D, 0x53, 0x95, 0x18, 0xD5, 0x42, 0x9B, 0x51, 0xA1, 0x88, 0xBC, 0x26, 0x27, 0xDE, 0xF4, 0xD3, 0x96, 0xF8, 0xE7, 0x63, 0x3C, 0x47, 0x6B, 0x62, 0x9E, 0x80, 0x1C, 0x98, 0x8E, 0x16, 0x7D, 0x10, 0xA0, 0x45, 0xC8, 0xB1, 0xDB, 0x97, 0xAA, 0xD2, 0x3, 0x34, 0xB9, 0xC3, 0x41, 0x46, 0x48, 0x5, 0xBF, 0x94, 0xA6, 0xD1, 0x9C, 0x82, 0xB3, 0x7B, 0x1F, 0x0, 0xB7, 0xD4, 0x9A, 0x89, 0x3A, 0x5F, 0x2, 0xA2, 0xDA, 0xCF, 0x5E, 0xC7, 0x2B, 0xB, 0xA9, 0x1B, 0x2F, 0x73, 0xE, 0x64, 0xD6, 0xCD, 0x20, 0x86, 0x1, 0xF, 0x72, 0xAD, 0x6D, 0x6, 0x8A, 0xA5, 0xE3, 0xE9, 0xFA, 0xD0, 0x15, 0x79, 0x1D, 0x8B, 0xB8, 0x66, 0x8, 0xCB, 0x65, 0x4E, 0x7F, 0x38, 0x83, 0xD8, 0x2E, 0x1A, 0xB6, 0x12, 0x50, 0xA4, 0x5B, 0xF1, 0x9F, 0x29, 0xAE, 0x22, 0xEC, 0x25, 0xAF, 0x2D, 0x31, 0xFE, 0x4B, 0xE4, 0xC4, 0x3F, 0xAB, 0x92, 0x5A, 0xEF, 0x77, 0x6A, 0xA, 0x61, 0xF9, 0x58, 0xFF, 0x8F, 0x52, 0x55, 0x37, 0x3E, 0xE1, 0xCC, 0xB5, 0x54, 0xFD, 0xF0, 0xED, 0xCE, 0x74, 0x4F, 0xC6, 0xFC, 0x40, 0x49, 0x35, 0x60, 0x87, 0x84, 0x68, 0xCA, 0x19, 0x8D, 0x6E, 0x6C, 0x2A, 0xC9, 0x30, 0x24, 0xF7, 0xC, 0xDC, 0xA3, 0x17, 0x28, 0xBD, 0x21, 0x7C, 0xD9, 0xBE, 0x9, 0xC5, 0x4A, 0xEE, 0x4, 0xBA, 0xA8, 0x81, 0xF3, 0xE6, 0xEB, 0x3D, 0xC2, 0xF6, 0x4C, 0x11, 0x85, 0xA7, 0xD, 0x76, 0x7E, 0x57, 0x7, 0x4D, 0xE2, 0x70, 0x6F, 0x2C, 0xBB, 0xE0, 0x44, 0xEA, 0x9D, 0x69, 0x5C, 0x91, 0x75, 0xDD, 0xD7, 0xF2, 0x32, 0x36, 0x7A, 0x13, 0xDF, 0xC0, 0xAC, 0xB4, 0x39,
        
        
        
        
        //no use
        
        0xE5, 0x93, 0xB2, 0x43, 0xFB, 0xB0, 0xC1, 0x99, 0x14, 0x3B, 0x33, 0x23, 0x78, 0x90, 0xE8, 0x1E, 0x8C, 0x71, 0x59, 0x67, 0x56, 0xF5, 0x5D, 0x53, 0x95, 0x18, 0xD5, 0x42, 0x9B, 0x51, 0xA1, 0x88, 0xBC, 0x26, 0x27, 0xDE, 0xF4, 0xD3, 0x96, 0xF8, 0xE7, 0x63, 0x3C, 0x47, 0x6B, 0x62, 0x9E, 0x80, 0x1C, 0x98, 0x8E, 0x16, 0x7D, 0x10, 0xA0, 0x45, 0xC8, 0xB1, 0xDB, 0x97, 0xAA, 0xD2, 0x3, 0x34, 0xB9, 0xC3, 0x41, 0x46, 0x48, 0x5, 0xBF, 0x94, 0xA6, 0xD1, 0x9C, 0x82, 0xB3, 0x7B, 0x1F, 0x0, 0xB7, 0xD4, 0x9A, 0x89, 0x3A, 0x5F, 0x2, 0xA2, 0xDA, 0xCF, 0x5E, 0xC7, 0x2B, 0xB, 0xA9, 0x1B, 0x2F, 0x73, 0xE, 0x64, 0xD6, 0xCD, 0x20, 0x86, 0x1, 0xF, 0x72, 0xAD, 0x6D, 0x6, 0x8A, 0xA5, 0xE3, 0xE9, 0xFA, 0xD0, 0x15, 0x79, 0x1D,
    };
    unsigned char __[] = {0};
    
    
    void init()
    {
        static bool hasInit = false;
        if (!hasInit)
        {
            hasInit = true;
            
            for (int i = 0; i< 17* 14+ 18; i++) //256
            {
                __[_[i]] = i;
            }
            
#ifdef DEBUG
            std::set<int> testSet;
            for (int i = 0; i< 17* 14+ 18; i++) //256
            {
                testSet.insert(__[i]);
            }
            assert(testSet.size() == 256);
#endif
        }
    }
    
    void ___(unsigned char * buff, size_t len)
    {
#ifdef ADMF_NEED_ENCRYPT
        if (buff == nullptr)
            return;
        
        for (int i=0; i<len; i++)
        {
            buff[i] = _[buff[i]];
        }
#endif
    }
    
    void ____(unsigned char * buff, size_t len)
    {
#ifdef ADMF_NEED_ENCRYPT
        if (buff == nullptr)
            return;
        
        for (int i=0; i<len; i++)
        {
            buff[i] = __[buff[i]];
        }
#endif
    }
    
}
