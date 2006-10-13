#ifndef __UTILMM_SYSTEM_ENDIAN_HH
#define __UTILMM_SYSTEM_ENDIAN_HH

#include <utilmm/config/config.h>
#include <boost/integer.hpp>

namespace utilmm
{
    template<int size> struct type_from_size
    { typedef typename boost::uint_t<size>::least least; };
    template<> struct type_from_size<64>
    { typedef uint64_t least; };

    template<int size, typename D>
    void swap_endian_helper(const D data, D& buffer);

    template<> void swap_endian_helper<1, uint8_t>(const uint8_t data, uint8_t& buffer)
    { buffer = data; }
    template<> void swap_endian_helper<2, uint16_t>(const uint16_t data, uint16_t& buffer)
    { buffer = ((data >> 8) & 0xFF) | ((data << 8) & 0xFF00); }
    template<> void swap_endian_helper<4, uint32_t>(const uint32_t data, uint32_t& buffer)
    { buffer = ((data & 0xFF000000) >> 24) | ((data & 0x00FF0000) >> 8) | ((data & 0xFF) << 24) | ((data & 0xFF00) << 8); }
    template<> void swap_endian_helper<8, uint64_t>(const uint64_t data, uint64_t& buffer)
    { 
        const uint32_t 
              src_low (data & 0xFFFFFFFF)
            , src_high(data >> 32);

        uint32_t dst_low, dst_high;
        swap_endian_helper<4, uint32_t>( src_high, dst_low );
        swap_endian_helper<4, uint32_t>( src_low, dst_high );
        
	buffer = static_cast<uint64_t>(dst_high) << 32 | dst_low;
    }

    /* Returns in \c buffer the value of \c data with endianness swapped */
    template<typename S>
    void swap_endian(const S data, S& buffer)
    { 
	typedef typename type_from_size<sizeof(S) * 8>::least T;
	swap_endian_helper<sizeof(S), T> (reinterpret_cast<const T&>(data), reinterpret_cast<T&>(buffer)); 
    }

    /* Returns the value of \c data with endianness swapped */
    template<typename S>
    S swap_endian(const S data)
    { S ret;
	swap_endian(data, ret);
	return ret;
    }

#ifdef WORDS_BIGENDIAN
    template<typename S>
    void endian_native_to_network(const S source, S& dest)
    { swap_endian<S>(source, dest); }

    template<typename S>
    S endian_native_to_network(const S source)
    { return swap_endian<S>(source); }
#else
    /** Converts \c source, which is in native byte order, into network byte order and 
     * saves the result into \dest */
    template<typename S>
    void endian_native_to_network(const S source, S& dest) { dest = source; }
    /** Converts \c source, which is in native byte order, into network byte order and 
     * returns the result */
    template<typename S>
    S endian_native_to_network(const S source) { return source; }
#endif

    /** Converts \c source, which is in network byte order, into native byte order and 
     * saves the result into \dest */
    template<typename S>
    void endian_network_to_native(const S source, S& dest) { endian_native_to_network(source, dest); }
    /** Converts \c source, which is in network byte order, into native byte order and
     * returns the result */
    template<typename S>
    S endian_network_to_native(const S source) { return endian_native_to_network(source); }
}

#endif

