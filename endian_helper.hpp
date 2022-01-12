#ifndef ENDIAN_HELPER_HPP
#define ENDIAN_HELPER_HPP

enum endian_t {
    OS_BIG_ENDIAN      = 0x00000001,
    OS_LITTLE_ENDIAN   = 0x01000000,
    OS_PDP_ENDIAN      = 0x00010000,
    OS_UNKNOWN_ENDIAN  = 0xFFFFFFFF
};

#ifdef __BIG_ENDIAN__
enum { OS_BYTE_ORDER = OS_BIG_ENDIAN };
#else /* NO __BIG_ENDIAN__ */
#ifdef __LITTLE_ENDIAN__
enum { OS_BYTE_ORDER = OS_LITTLE_ENDIAN };
#else /* NO __LITTLE_ENDIAN__ */
#ifdef BSD
#include <sys/endian.h>
#else // BSD
#include <endian.h>
#endif // BSD
#if __BYTE_ORDER == __BIG_ENDIAN
enum { OS_BYTE_ORDER = OS_BIG_ENDIAN };
#elif __BYTE_ORDER == __LITTLE_ENDIAN
enum { OS_BYTE_ORDER = OS_LITTLE_ENDIAN };
#else
#error "Unsupported Endianness!"
#endif /* __BYTE_ORDER */
#endif /* __LITTLE_ENDIAN__ */
#endif /* __BIG_ENDIAN__ */

#include <stdint.h>

template<int ... Args>
struct EndianConverter {};
    
template<int Same>
struct EndianConverter<Same, Same>
{
    template<typename T>
    constexpr inline T operator() (T t) const
    {
        return t;
    }
};

template <int T, int U>
struct EndianConverter<T,U> {
    template<typename V>
    constexpr inline V operator() (V t) const
    {
        return t;
    }
};

template<>
struct EndianConverter<OS_BIG_ENDIAN, OS_LITTLE_ENDIAN>
{
    constexpr inline uint32_t operator() (uint32_t i) const
    {
        return  (i & 0xff << 0u)  << 24 |
                (i & 0xff << 8u)  << 8  |
                (i & 0xff << 16u) >> 8  |
                (i & 0xff << 24u) >> 24 ;
    }
    
    constexpr inline uint16_t operator() (uint16_t i) const
    {
        return  (i & 0xff << 0u) << 8 |
                (i & 0xff << 8u) >> 8 ;
    }
};

template<>
struct EndianConverter<OS_PDP_ENDIAN, OS_LITTLE_ENDIAN>
{
    constexpr inline uint32_t operator() (uint32_t i) const
    {
        return  (i & 0xffff << 0u)  << 16 |
                (i & 0xffff << 16u) >> 16 ;
    }
    
    constexpr inline uint16_t operator() (uint16_t i) const
    {
        return i;
    }
};

template<>
struct EndianConverter<OS_PDP_ENDIAN, OS_BIG_ENDIAN>
{
    constexpr inline uint32_t operator() (uint32_t i) const
    {
        return  (i & 0xff << 0u)  << 8 |
                (i & 0xff << 8u)  >> 8 |
                (i & 0xff << 16u) << 8 |
                (i & 0xff << 24u) >> 8;
    }
    
    constexpr inline uint16_t operator() (uint16_t i) const
    {
        return  (i & 0xff << 0u) << 8 |
                (i & 0xff << 8u) >> 8 ;
    }
};

EndianConverter<OS_BYTE_ORDER, OS_LITTLE_ENDIAN> ToLittleEndian;

#endif // ENDIAN_HELPER_HPP
