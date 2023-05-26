#pragma once
#ifndef  GUID_H
#define GUID_H


#include <iomanip>
#include <cstdint>
//#include <iostream>

#ifndef ARRAY2_H
#define ARRAY2_H

template<typename T, int N, int TRUNCATED_SIZE = N, int Counter = N>
class Array {

private:
    const T arr[N];

public:

    template<typename ... Args>
    constexpr Array(T first, Args ... args) : arr{ first, args... } {}

    constexpr Array() : arr{ 0 } {}

    constexpr T operator[](int index) const {
        return arr[index];
    }

    constexpr int size() const {
        return N;
    }

    constexpr Array<T, TRUNCATED_SIZE> truncate() const {
        return Array<T, TRUNCATED_SIZE>().__truncate(&arr[0]);
    }

    template<typename ... Elements>
    constexpr Array<T, N> __truncate(const T* source, Elements ... elements) const {
        return Array<T, N, TRUNCATED_SIZE, Counter - 1>().__truncate(source, elements..., source[N - Counter]);
    }
};

template<typename T, int N, int TRUNCATED_SIZE>
class Array<T, N, TRUNCATED_SIZE, 0> {

public:
    template<typename ... Elements>
    constexpr Array<T, N> __truncate(const T* source, Elements ... elements) {
        return Array<T, N>{ elements... };
    }

};



#endif

#ifndef CIRCULAR_QUEUE
#define CIRCULAR_QUEUE

template<typename T, size_t N, int Counter = N>
class CircularQueue {
public:
    const Array<T, N> arr;

    template<typename ... Args>
    constexpr CircularQueue(T new_item, CircularQueue<T, N> queue, Args ... args) :
        arr(CircularQueue<T, N, Counter - 1>(new_item, queue, args..., queue[N - Counter]).arr) {}

    constexpr CircularQueue() :
        arr{ 0 } {}


    constexpr T operator[](int index) const {
        return arr[index];
    }

    constexpr size_t size() const {
        return N;
    }

};

template<typename T, size_t N>
class CircularQueue<T, N, 0> {

public:
    const Array<T, N> arr;

    template<typename ... Args>
    constexpr CircularQueue(T new_item, CircularQueue<T, N> queue, Args ... args) :
        arr{ add_item(new_item,args...) } {}


    template<typename ... Args>
    constexpr Array<T, N> add_item(T new_item, T first, Args... args) const {
        return Array<T, N>{args..., new_item };
    }

};
#endif

#ifndef UTIL_H
#define UTIL_H

constexpr int get_string_size(const char* input, int i = 0) {
    return input[i] == '\0' ? i : get_string_size(input, i + 1);
}

constexpr uint32_t leftrotate(uint32_t x, int c) {
    return c == 0 ? x : (x << c) | (x >> (32 - c));
}

constexpr uint32_t rightrotate(uint32_t x, int c) {
    return c == 0 ? x : (x >> c) | (x << (32 - c));
}

constexpr uint64_t rightrotate64(uint64_t x, int c) {
    return c == 0 ? x : (x >> c) | (x << (64 - c));
}

template<typename T>
constexpr unsigned char get_byte(T input, int i) {
    return (input >> 8 * i) & 0xff;
}


template<>
constexpr unsigned char get_byte(const char* input, int i) {
    return input[i];
}

template<>
constexpr unsigned char get_byte(char* input, int i) {
    return input[i];
}

template<>
unsigned char get_byte(std::string input, int i) {
    return input[i];
}


template<typename T>
constexpr size_t get_size(T input) {
    return sizeof(T);
}

template<>
constexpr size_t get_size(const char* input) {
    return get_string_size(input);
}

template<>
constexpr size_t get_size(char* input) {
    return get_string_size(input);
}

template<>
size_t get_size(std::string input) {
    return input.size();
}

constexpr uint32_t endianess_convertion(uint32_t value) {
    return (get_byte<uint32_t>(value, 0) << 8 * 3) +
        (get_byte<uint32_t>(value, 1) << 8 * 2) +
        (get_byte<uint32_t>(value, 2) << 8) +
        (get_byte<uint32_t>(value, 3));
}

#endif

#ifndef PADDED_VALUE_H
#define PADDED_VALUE_H

template<typename, typename>class Section;

template<typename> class MD5;
template<typename> class SHA1;
template<typename, int> class SHA224_256;
template<typename, int> class SHA384_512;

template<typename H, typename Word = uint32_t>
class PaddedValue {

    constexpr static Word W = 0;

private:
    const int byte_length = sizeof(W) * 16;
    const bool is_big_endian;
    H input;
    const int size, total_size;

    constexpr PaddedValue(H _input, bool _is_big_endian) :
        is_big_endian(_is_big_endian),
        input(_input),
        size(get_size(input)),
        total_size(((size + 8) / byte_length + 1)* byte_length) {}


    constexpr unsigned char get_char(int i) const {
        return i < size ? get_char_from_input<H>(i) : get_padded_byte(i);
    }



    template<typename F>
    constexpr unsigned char get_char_from_input(int i) const {
        return get_byte<F>(input, i);
    }

    constexpr unsigned char get_padded_byte(int i) const {
        return (i == size || i < total_size - 8) ?
            0x80 * (i == size) :
            get_byte<uint64_t>(size * 8, is_big_endian ?
                (byte_length - i % byte_length - 1) :
                (8 - byte_length + i % byte_length));
    }

    constexpr uint32_t get_word_32(int i) const {
        return is_big_endian ?
            (get_char(4 * i) << 3 * 8) + (get_char(4 * i + 1) << 2 * 8)
            + (get_char(4 * i + 2) << 8) + get_char(4 * i + 3)
            : (get_char(4 * i + 3) << 3 * 8) + (get_char(4 * i + 2) << 2 * 8)
            + (get_char(4 * i + 1) << 8) + get_char(4 * i);
    }

    constexpr uint64_t get_word_64(int i) const {
        return  ((uint64_t)get_char(8 * i) << 7 * 8) + ((uint64_t)get_char(8 * i + 1) << 6 * 8)
            + ((uint64_t)get_char(8 * i + 2) << 5 * 8) + ((uint64_t)get_char(8 * i + 3) << 4 * 8)
            + ((uint64_t)get_char(8 * i + 4) << 3 * 8) + ((uint64_t)get_char(8 * i + 5) << 2 * 8)
            + ((uint64_t)get_char(8 * i + 6) << 8) + (uint64_t)get_char(8 * i + 7);
    }



    constexpr Word operator [](int i) const {
        return byte_length == 64 ? get_word_32(i) : get_word_64(i);
    }

    friend class Section<H, Word>;
    friend class MD5<H>;
    friend class SHA1<H>;
    template<typename, int> friend class SHA224_256;
    template<typename, int> friend class SHA384_512;
};

#endif

#ifndef SECTION_H
#define SECTION_H

template<typename> class MD5;
template<typename> class SHA1;
template<typename, int> class SHA224_256;
template<typename, int> class SHA384_512;

template<typename H, typename Word = uint32_t>
class Section {

    typedef CircularQueue<Word, 16> CircularQueueInts;
    typedef Word(*scheduled_func)(CircularQueueInts);
    const int start_index;
    const int block_start_index;
    const PaddedValue<H, Word> main_value;
    const CircularQueueInts queue;


    constexpr Section(PaddedValue<H, Word> value, int _start_index, scheduled_func scheduled) :
        start_index(_start_index),
        block_start_index(_start_index),
        main_value(value),
        queue(fill_next_ints(CircularQueueInts(), scheduled)) {}


    constexpr Section(const Section& prev_section, int _start_index, scheduled_func scheduled) :
        start_index(_start_index),
        block_start_index(prev_section.block_start_index),
        main_value(prev_section.main_value),
        queue(fill_next_ints(prev_section.queue, scheduled)) {}


    constexpr CircularQueueInts fill_next_ints(CircularQueueInts queue, scheduled_func scheduled,
        int i = 0) const {
        return i == 16 ? queue
            : fill_next_ints(
                CircularQueueInts(
                    get_int(queue, start_index + i, scheduled)
                    , queue)
                , scheduled, i + 1);
    }

    constexpr Word get_int(CircularQueueInts queue, int index, scheduled_func scheduled) const {
        return index - block_start_index < 16 ?
            main_value[index]
            : scheduled(queue);
    }

    constexpr Word operator[](int index) const {
        return queue[index];
    }

    friend class MD5<H>;
    friend class SHA1<H>;
    template<typename, int> friend class SHA224_256;
    template<typename, int> friend class SHA384_512;
};

#endif

#ifndef CRYPTO_HASH_H
#define CRYPTO_HASH_H


//HASH_SIZE = (hash size in bits)/32

template<int HASH_SIZE>
class CryptoHash {


private:

    const Array<uint32_t, HASH_SIZE> hashed_value;

protected:

    constexpr CryptoHash(Array<uint32_t, HASH_SIZE> _hashed_value) :
        hashed_value(_hashed_value) {}


public:

    bool operator ==(const CryptoHash<HASH_SIZE>& second_hash) const {
        for (int i = 0; i < HASH_SIZE; i++) {
            if (hashed_value[i] != second_hash.hashed_value[i]) return false;
        }
        return true;
    }


    bool operator !=(const CryptoHash<HASH_SIZE>& second_hash) const {
        return !(*this == second_hash);
    }

    uint32_t operator [](int index) {
        return hashed_value[index];
    }


    //friend std::ostream& operator<<(std::ostream& os, const CryptoHash<HASH_SIZE>& hash) {
    //    std::ios_base::fmtflags f(std::cout.flags());
    //    for (int i = 0; i < HASH_SIZE; i++)
    //        os << std::hex << std::setfill('0') << std::setw(8) << hash.hashed_value[i];
    //    std::cout.flags(f);
    //    return os;
    //}

};
#endif



#ifndef COMPILE_TIME_SHA224_256_H
#define COMPILE_TIME_SHA224_256_H


#define MASK 0xffffffff

template<typename H>
class SHA224;

template<typename H>
class SHA256;

template<typename H = const char*, int HASH_SIZE = 8>
class SHA224_256 : public CryptoHash<HASH_SIZE> {
private:

    constexpr static uint32_t ssig0(uint32_t x) {
        return rightrotate(x, 7) ^ rightrotate(x, 18) ^ (x >> 3);
    }

    constexpr static uint32_t ssig1(uint32_t x) {
        return rightrotate(x, 17) ^ rightrotate(x, 19) ^ (x >> 10);
    }

    constexpr static uint32_t scheduled(CircularQueue<uint32_t, 16> queue) {
        return ssig1(queue[14]) + queue[9] + ssig0(queue[1]) + queue[0];
    }

    constexpr static uint32_t k[64] = { 0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070, 0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2, };



    struct hash_parameters {
        const Array<uint32_t, 8, HASH_SIZE> arr;

        template <typename ... Args>
        constexpr hash_parameters(Args ... args) : arr{ args... } {}

        constexpr uint32_t operator [](int index) { return arr[index]; }
    };


    using Hash_T = SHA224_256<H, HASH_SIZE>;
    using PaddedValue_T = PaddedValue<H>;
    using Section_T = Section<H>;

    constexpr uint32_t ch(uint32_t x, uint32_t y, uint32_t z) const {
        return (x & y) ^ (~x & z);
    }

    constexpr uint32_t maj(uint32_t x, uint32_t y, uint32_t z) const {
        return (x & y) ^ (x & z) ^ (y & z);
    }

    constexpr uint32_t bsig0(uint32_t x) const {
        return rightrotate(x, 2) ^ rightrotate(x, 13) ^ rightrotate(x, 22);
    }

    constexpr uint32_t bsig1(uint32_t x) const {
        return rightrotate(x, 6) ^ rightrotate(x, 11) ^ rightrotate(x, 25);
    }




    constexpr Array<uint32_t, HASH_SIZE> create_hash(PaddedValue_T value, hash_parameters h, int block_index = 0) const {
        return block_index * 64 == value.total_size
            ? h.arr.truncate()
            : create_hash(
                value,
                hash_block(Section_T(value, block_index * 16, scheduled), h, h, block_index * 16),
                block_index + 1);
    }

    constexpr uint32_t t1(Section_T section, hash_parameters h, int index) const {
        return h[7] + bsig1(h[4]) + ch(h[4], h[5], h[6]) + k[index] + section[index % 16];
    }

    constexpr uint32_t t2(hash_parameters h) const {
        return bsig0(h[0]) + maj(h[0], h[1], h[2]);
    }

    constexpr hash_parameters hash_section(Section_T section, hash_parameters h, int start, int i = 0) const {
        return i == 16 ? h :
            hash_section(section, {
                t1(section,h,start + i) + t2(h),
                h[0],
                h[1],
                h[2],
                h[3] + t1(section,h,start + i),
                h[4],
                h[5],
                h[6]
                }, start, i + 1);
    }

    constexpr hash_parameters hash_block(Section_T section, hash_parameters h, hash_parameters prev_h,
        int start_index, int i = 0) const {
        return i == 64 ?
            hash_parameters {
            prev_h[0] + h[0],
                prev_h[1] + h[1],
                prev_h[2] + h[2],
                prev_h[3] + h[3],
                prev_h[4] + h[4],
                prev_h[5] + h[5],
                prev_h[6] + h[6],
                prev_h[7] + h[7]
        } : hash_block(
            Section_T(section, start_index + i + 16, scheduled),
            hash_section(section, h, i),
            prev_h, start_index, i + 16);
    }

    template <typename ... InitialValues>
    constexpr SHA224_256(H input, InitialValues ... initial_values) :
        CryptoHash<HASH_SIZE>(create_hash(PaddedValue_T(input, true), { initial_values... })) {}

    friend class SHA224<H>;
    friend class SHA256<H>;

};

template<typename T, int HASH_SIZE> constexpr uint32_t SHA224_256<T, HASH_SIZE>::k[64];

#endif

#ifndef COMPILE_TIME_SHA256_H
#define COMPILE_TIME_SHA256_H

template<typename H = const char*>
class SHA256 : public SHA224_256<H, 8> {

public:

    constexpr SHA256(H input) : SHA224_256<H, 8>(input,
        (uint32_t)0x6a09e667, (uint32_t)0xbb67ae85,
        (uint32_t)0x3c6ef372, (uint32_t)0xa54ff53a,
        (uint32_t)0x510e527f, (uint32_t)0x9b05688c,
        (uint32_t)0x1f83d9ab, (uint32_t)0x5be0cd19) {}
};

typedef SHA256<> SHA256String;

#endif

#endif // ! GUID