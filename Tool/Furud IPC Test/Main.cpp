
//#include <IPC.h>


#include <iomanip>
#include <cstdint>
#include <iostream>


template<typename T, int N>
class Array
{
private:
	const T arr[N];


public:
	template<typename ... Args>
	constexpr Array(T first, Args ... args) 
		: arr{ first, args... }
	{}

	constexpr Array() 
		: arr{ 0 }
	{}

	constexpr T operator[](int index) const 
	{
		return arr[index];
	}

	constexpr int size() const
	{
		return N;
	}
};



constexpr int get_string_size(const char* input, int i = 0) {
	return input[i] == '\0' ? i : get_string_size(input, i + 1);
}

constexpr uint32_t leftrotate(uint32_t x, int c) {
	return c == 0 ? x : (x << c) | (x >> (32 - c));
}

//constexpr uint32_t rightrotate(uint32_t x, int c) {
//	return c == 0 ? x : (x >> c) | (x << (32 - c));
//}

//constexpr uint64_t rightrotate64(uint64_t x, int c) {
//	return c == 0 ? x : (x >> c) | (x << (64 - c));
//}

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

constexpr uint32_t endianess_convertion(uint32_t value)
{
	return (get_byte<uint32_t>(value, 0) << 8 * 3) +
		(get_byte<uint32_t>(value, 1) << 8 * 2) +
		(get_byte<uint32_t>(value, 2) << 8) +
		(get_byte<uint32_t>(value, 3));
}



template<int HASH_SIZE>
class CryptoHash
{
private:
	const Array<uint32_t, HASH_SIZE> hashed_value;


protected:
	constexpr CryptoHash(Array<uint32_t, HASH_SIZE> _hashed_value) 
		: hashed_value(_hashed_value)
	{}


public:
	bool operator == (const CryptoHash<HASH_SIZE>& second_hash) const
	{
		for (int i = 0; i < HASH_SIZE; i++) 
		{
			if (hashed_value[i] != second_hash.hashed_value[i])
			{
				return false;
			}
		}
		return true;
	}

	bool operator != (const CryptoHash<HASH_SIZE>& second_hash) const
	{
		return !(*this == second_hash);
	}

	uint32_t operator [](int index)
	{
		return hashed_value[index];
	}


	friend std::ostream& operator<<(std::ostream& os, const CryptoHash<HASH_SIZE>& hash) {
		std::ios_base::fmtflags f(std::cout.flags());
		for (int i = 0; i < HASH_SIZE; i++)
			os << std::hex << std::setfill('0') << std::setw(8) << hash.hashed_value[i];
		std::cout.flags(f);
		return os;
	}

};



template<typename> class MD5;

template<typename H, typename Word = uint32_t>
class PaddedValue
{
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
		total_size(((size + 8) / byte_length + 1)* byte_length)
	{
		if (byte_length != 64)
		{
			*(volatile int*)0 = 0;
		}
	}


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

	constexpr uint64_t get_word_64(int i) const 
	{
		return
			((uint64_t)get_char(8 * i + 0) << 7 * 8) +
			((uint64_t)get_char(8 * i + 1) << 6 * 8) +
			((uint64_t)get_char(8 * i + 2) << 5 * 8) +
			((uint64_t)get_char(8 * i + 3) << 4 * 8) +
			((uint64_t)get_char(8 * i + 4) << 3 * 8) +
			((uint64_t)get_char(8 * i + 5) << 2 * 8) +
			((uint64_t)get_char(8 * i + 6) << 1 * 8) +
			((uint64_t)get_char(8 * i + 7));
	}



	constexpr Word operator [](int i) const {
		return byte_length == 64 ? get_word_32(i) : get_word_64(i);
	}

	friend class MD5<H>;
};

template<typename H = const char*>
class MD5 : public CryptoHash<4> 
{
private:
	constexpr static int s[64] = 
	{ 
		7, 12, 17, 22,
		7, 12, 17, 22,
		7, 12, 17, 22,
		7, 12, 17, 22,
		5,  9, 14, 20,
		5,  9, 14, 20,
		5,  9, 14, 20,
		5,  9, 14, 20,
		4, 11, 16, 23,
		4, 11, 16, 23,
		4, 11, 16, 23,
		4, 11, 16, 23,
		6, 10, 15, 21,
		6, 10, 15, 21,
		6, 10, 15, 21,
		6, 10, 15, 21
	};

	constexpr static uint32_t k[64] = 
	{
		0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
		0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
		0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
		0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
		0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
		0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
		0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
		0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
		0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
		0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
		0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
		0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
		0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
		0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
		0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
		0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
	};

	constexpr static int g_array[4][2] = 
	{
		{ 1, 0 },
		{ 5, 1 },
		{ 3, 5 },
		{ 7, 0 }
	};


private:
	using Hash_T = MD5<H>;
	using PaddedValue_T = PaddedValue<H>;

	struct hash_parameters
	{
		const Array<uint32_t, 4> arr;
		const uint32_t f_array[4][5];

		template <typename ... Args>
		constexpr hash_parameters(Args ... args)
			: arr{ args... }
			, f_array{
				{arr[3],0,arr[2],arr[1],arr[3]},
				{arr[2],0,arr[1],arr[3],arr[2]},
				{arr[3],0,arr[2],0xffffffff,arr[1]},
				{~arr[3],arr[1],0,0xffffffff,arr[2]}}
		{}

		constexpr hash_parameters() 
			: hash_parameters(
				(uint32_t)0x67452301,
				(uint32_t)0xefcdab89,
				(uint32_t)0x98badcfe,
				(uint32_t)0x10325476)
		{}

		constexpr uint32_t operator [](int index) const { return arr[index]; }
	};

	constexpr Array<uint32_t, 4> create_hash(PaddedValue_T value, hash_parameters h, int block_index = 0)
	{
		return block_index * 64 == value.total_size 
			? Array<uint32_t, 4>{
				endianess_convertion(h[0]),
				endianess_convertion(h[1]),
				endianess_convertion(h[2]),
				endianess_convertion(h[3]) }
			: create_hash(
				value,
				hash_block(value, h, h, block_index),
				block_index + 1);
	}

	constexpr uint32_t f_expr(int i, struct hash_parameters h) const {
		return ((h.f_array[i][0] | h.f_array[i][1]) ^ h.f_array[i][2]) & h.f_array[i][3] ^ h.f_array[i][4];
	}

	constexpr uint32_t f(PaddedValue_T value, int block_index, int i, struct hash_parameters h) const {
		return f_expr(i / 16, h) + h[0] + k[i] + value[block_index * 16 + g(i)];
	}

	constexpr int g(int i) const {
		return (g_array[i / 16][0] * i + g_array[i / 16][1]) & 0xf;
	}

	constexpr hash_parameters hash_block(PaddedValue_T value, hash_parameters const& h, hash_parameters const& prev_h, int block_index, int i = 0) const
	{
		return i == 64 
			? hash_parameters {
				prev_h[0] + h[0], prev_h[1] + h[1], prev_h[2] + h[2], prev_h[3] + h[3] }
			: hash_block(
				value,
				{
					h[3],
					h[1] + leftrotate(f(value, block_index, i, h), s[i]),
					h[1],
					h[2]
				},
				prev_h, block_index, i + 1);
	}

public:

	constexpr MD5(H input)
		: CryptoHash<4>(create_hash(PaddedValue_T(input, false), {})) 
	{}

};


typedef MD5<> MD5String;


int main(int argc, char** argv)
{
	MD5 sha = MD5String("hello world");
	std::cout << sha << '\n';
	//TestIPC(argc, argv);
	return 0;
}

/*
#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>

using namespace std;

#define A 0x67452301
#define B 0xefcdab89
#define C 0x98badcfe
#define D 0x10325476

const char str16[] = "0123456789abcdef";

const unsigned int T[] = {
	0xd76aa478,0xe8c7b756,0x242070db,0xc1bdceee,
	0xf57c0faf,0x4787c62a,0xa8304613,0xfd469501,
	0x698098d8,0x8b44f7af,0xffff5bb1,0x895cd7be,
	0x6b901122,0xfd987193,0xa679438e,0x49b40821,
	0xf61e2562,0xc040b340,0x265e5a51,0xe9b6c7aa,
	0xd62f105d,0x02441453,0xd8a1e681,0xe7d3fbc8,
	0x21e1cde6,0xc33707d6,0xf4d50d87,0x455a14ed,
	0xa9e3e905,0xfcefa3f8,0x676f02d9,0x8d2a4c8a,
	0xfffa3942,0x8771f681,0x6d9d6122,0xfde5380c,
	0xa4beea44,0x4bdecfa9,0xf6bb4b60,0xbebfbc70,
	0x289b7ec6,0xeaa127fa,0xd4ef3085,0x04881d05,
	0xd9d4d039,0xe6db99e5,0x1fa27cf8,0xc4ac5665,
	0xf4292244,0x432aff97,0xab9423a7,0xfc93a039,
	0x655b59c3,0x8f0ccc92,0xffeff47d,0x85845dd1,
	0x6fa87e4f,0xfe2ce6e0,0xa3014314,0x4e0811a1,
	0xf7537e82,0xbd3af235,0x2ad7d2bb,0xeb86d391 };

const unsigned int s[] = { 7,12,17,22,7,12,17,22,7,12,17,22,7,12,17,22,
						   5,9,14,20,5,9,14,20,5,9,14,20,5,9,14,20,
						   4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23,
						   6,10,15,21,6,10,15,21,6,10,15,21,6,10,15,21 };
class MD5 {
private:
	unsigned int tempA, tempB, tempC, tempD, strlength;
public:
	MD5() {
		tempA = A;
		tempB = B;
		tempC = C;
		tempD = D;
		strlength = 0;
	}
	// F函数
	unsigned int F(unsigned int b, unsigned int c, unsigned int d) {
		return (b & c) | ((~b) & d);
	}
	// G函数
	unsigned int G(unsigned int b, unsigned int c, unsigned int d) {
		return (b & d) | (c & (~d));
	}
	// H函数
	unsigned int H(unsigned int b, unsigned int c, unsigned int d) {
		return b ^ c ^ d;
	}
	// I函数
	unsigned int I(unsigned int b, unsigned int c, unsigned int d) {
		return c ^ (b | (~d));
	}
	// 移位操作函数
	unsigned int shift(unsigned int a, unsigned int n) {
		return (a << n) | (a >> (32 - n));
	}
	// 编码函数
	string encode(string src) {
		vector<unsigned int> rec = padding(src);
		for (unsigned int i = 0; i < strlength / 16; i++) {
			unsigned int num[16];
			for (int j = 0; j < 16; j++) {
				num[j] = rec[i * 16 + j];
			}
			iterateFunc(num, 16);
		}
		return format(tempA) + format(tempB) + format(tempC) + format(tempD);
	}
	// 循环压缩
	void iterateFunc(unsigned int* X, int size = 16) {
		unsigned int a = tempA,
			b = tempB,
			c = tempC,
			d = tempD,
			rec = 0,
			g, k;
		for (int i = 0; i < 64; i++) {
			if (i < 16) {
				// F迭代
				g = F(b, c, d);
				k = i;
			}
			else if (i < 32) {
				// G迭代
				g = G(b, c, d);
				k = (1 + 5 * i) % 16;
			}
			else if (i < 48) {
				// H迭代
				g = H(b, c, d);
				k = (5 + 3 * i) % 16;
			}
			else {
				// I迭代
				g = I(b, c, d);
				k = (7 * i) % 16;
			}
			rec = d;
			d = c;
			c = b;
			b = b + shift(a + g + X[k] + T[i], s[i]);
			a = rec;
		}
		tempA += a;
		tempB += b;
		tempC += c;
		tempD += d;
	}
	// 填充字符串
	vector<unsigned int> padding(string src) {
		// 以512位,64个字节为一组
		unsigned int num = ((src.length() + 8) / 64) + 1;
		vector<unsigned int> rec(num * 16);
		strlength = num * 16;
		for (unsigned int i = 0; i < src.length(); i++) {
			// 一个unsigned int对应4个字节，保存4个字符信息
			rec[i >> 2] |= (int)(src[i]) << ((i % 4) * 8);
		}
		// 补充1000...000
		rec[src.length() >> 2] |= (0x80 << ((src.length() % 4) * 8));
		// 填充原文长度
		// K bit
		rec[rec.size() - 2] = (src.length() << 3);
		return rec;
	}
	// 整理输出
	string format(unsigned int num) {
		string res = "";
		unsigned int base = 1 << 8;
		for (int i = 0; i < 4; i++) {
			string tmp = "";
			unsigned int b = (num >> (i * 8)) % base & 0xff;
			for (int j = 0; j < 2; j++) {
				tmp = str16[b % 16] + tmp;
				b /= 16;
			}
			res += tmp;
		}
		return res;
	}
};

int main() {
	MD5 test;
	string a = "hello world";
	cout << "result: " << test.encode(a) << endl;
	return 0;
}
*/