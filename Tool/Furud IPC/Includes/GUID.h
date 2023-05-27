//
// GUID.h
//
//       Copyright (c) Furud IPC Library. All rights reserved.
//       @Author FongZiSing
//
// Implemention of Globally Unique Identifier, which is generated at compile time.
//
#pragma once



namespace Furud::IPC::Concept
{
	/**
	 * @brief    The 512-bit `message`,
	 *           that is the structure stores the MD5 hash algorithm's iterable input.
	 *           The MD5 algorithm processes data in 512-bit blocks,
	 *           broken down into 16 words composed of 32 bits each.
	 * @details  128bit的`信息`, 即MD5算法的输入。字符串会被分组，以512 bit (64 byte)为一组。
	 */
	class IMD5PaddedInfo
	{
	private:
		const unsigned int data[16];


	private:
		constexpr static unsigned int GetInfoInternal(const char* str, const int& len, const int& num, const int& idx) noexcept
		{
			// The following code is the padding works.
			//~ Just copy string.
			if (idx < len)
			{
				return unsigned int(str[idx]) << (idx % 4) * 8;
			}
			//~ Appends a single bit, 1, to the end of the string.
			else if (idx == len)
			{
				return (0x80) << (idx % 4) * 8;
			}
			//~ Fill in the number of digits for the string.
			else if (idx / 4 == num - 2)
			{
				return len << 3;
			}
			//~ Else, fill in zero.
			return 0;
		}

		constexpr static unsigned int GetInfo(const char* str, const int& len, const int& num, const int& idx) noexcept
		{
			return
				GetInfoInternal(str, len, num, idx + 0) |
				GetInfoInternal(str, len, num, idx + 1) |
				GetInfoInternal(str, len, num, idx + 2) |
				GetInfoInternal(str, len, num, idx + 3);
		}


	public:
		/**
		 * @brief  Constructor.
		 * @param  str  -  The input constant string.
		 * @param  len  -  The length of input string.
		 * @param  num  -  The number of blocks.
		 * @param  idx  -  The index number of split.
		 */
		constexpr IMD5PaddedInfo
			( const char* str
			, const int& len
			, const int& num
			, const int& idx
		) noexcept
			: data
			{
				GetInfo(str, len, num, idx * 64 +  0), GetInfo(str, len, num, idx * 64 +  4),
				GetInfo(str, len, num, idx * 64 +  8), GetInfo(str, len, num, idx * 64 + 12),
				GetInfo(str, len, num, idx * 64 + 16), GetInfo(str, len, num, idx * 64 + 20),
				GetInfo(str, len, num, idx * 64 + 24), GetInfo(str, len, num, idx * 64 + 28),
				GetInfo(str, len, num, idx * 64 + 32), GetInfo(str, len, num, idx * 64 + 36),
				GetInfo(str, len, num, idx * 64 + 40), GetInfo(str, len, num, idx * 64 + 44),
				GetInfo(str, len, num, idx * 64 + 48), GetInfo(str, len, num, idx * 64 + 52),
				GetInfo(str, len, num, idx * 64 + 56), GetInfo(str, len, num, idx * 64 + 60),
			}
		{}

		constexpr unsigned int operator [] (int index) const noexcept
		{
			return data[index];
		}

		constexpr static unsigned int GetBlockNum(const int& len) noexcept
		{
			return ((len + 8) / 64) + 1;
		}
	};



	/**
	 * @brief    The 128-bit `message digest`, 
	 *           that is the structure stores the MD5 hash algorithm's result.
	 * @details  128bit的`信息摘要`, 即MD5算法的输出结果。
	 */
	class IMD5CryptoHash
	{
	private:
		unsigned int data[4];


	public:
		constexpr IMD5CryptoHash() noexcept
			: data{ 0 }
		{}

		template<typename... Args>
		constexpr IMD5CryptoHash(Args&&... args) noexcept
			: data{ args... }
		{}

		constexpr unsigned int& operator [] (int index) noexcept
		{
			return data[index];
		}

		constexpr unsigned int operator [] (int index) const noexcept
		{
			return data[index];
		}
	};



	/**
	 * @brief    Compile time MD5 message-digest algorithm.
	 * @details  编译期 MD5 哈希算法。
	 */
	class IMD5
	{
	protected:
		const IMD5CryptoHash values;


	public:
		constexpr IMD5(const char* inputString) noexcept
			: values(Encode(inputString))
		{}


	private:
		//****************************************************************
		// @brief    The main MD5 algorithm operates on a 128-bit state,
		//           divided into four 32-bit words, denoted A, B, C, and D. 
		//           And the followings is the initialize variables.
		// @details  初始的128位变量。
		//****************************************************************

		constexpr static unsigned int A = 0x67452301ui32;

		constexpr static unsigned int B = 0xEFCDAB89ui32;

		constexpr static unsigned int C = 0x98BADCFEui32;

		constexpr static unsigned int D = 0x10325476ui32;

		//****************************************************************
		// @brief    The look-up table that the algorithm used.
		//           S  -  specifies the per-round shift amounts.
		//           K  -  just precomputed table.
		// @details  加速计算的查找表。
		//****************************************************************

		constexpr static int S[64] =
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

		constexpr static unsigned int K[64] =
		{
			0xD76AA478ui32, 0xE8C7B756ui32, 0x242070DBui32, 0xC1BDCEEEui32,
			0xF57C0FAFui32, 0x4787C62Aui32, 0xA8304613ui32, 0xFD469501ui32,
			0x698098D8ui32, 0x8B44F7AFui32, 0xFFFF5BB1ui32, 0x895CD7BEui32,
			0x6B901122ui32, 0xFD987193ui32, 0xA679438Eui32, 0x49B40821ui32,
			0xF61E2562ui32, 0xC040B340ui32, 0x265E5A51ui32, 0xE9B6C7AAui32,
			0xD62F105Dui32, 0x02441453ui32, 0xD8A1E681ui32, 0xE7D3FBC8ui32,
			0x21E1CDE6ui32, 0xC33707D6ui32, 0xF4D50D87ui32, 0x455A14EDui32,
			0xA9E3E905ui32, 0xFCEFA3F8ui32, 0x676F02D9ui32, 0x8D2A4C8Aui32,
			0xFFFA3942ui32, 0x8771F681ui32, 0x6D9D6122ui32, 0xFDE5380Cui32,
			0xA4BEEA44ui32, 0x4BDECFA9ui32, 0xF6BB4B60ui32, 0xBEBFBC70ui32,
			0x289B7EC6ui32, 0xEAA127FAui32, 0xD4EF3085ui32, 0x04881D05ui32,
			0xD9D4D039ui32, 0xE6DB99E5ui32, 0x1FA27CF8ui32, 0xC4AC5665ui32,
			0xF4292244ui32, 0x432AFF97ui32, 0xAB9423A7ui32, 0xFC93A039ui32,
			0x655B59C3ui32, 0x8F0CCC92ui32, 0xFFEFF47Dui32, 0x85845DD1ui32,
			0x6FA87E4Fui32, 0xFE2CE6E0ui32, 0xA3014314ui32, 0x4E0811A1ui32,
			0xF7537E82ui32, 0xBD3AF235ui32, 0x2AD7D2BBui32, 0xEB86D391ui32
		};

		constexpr static unsigned int LeftRotate(unsigned int a, unsigned int n) noexcept
		{
			return (a << n) | (a >> (32 - n));
		}


	private:
		constexpr IMD5CryptoHash Encode(const char* inputString) noexcept
		{
			const int inputNum = (int)__builtin_strlen(inputString);
			const int loopsNum = IMD5PaddedInfo::GetBlockNum(inputNum);
			const int loopsInt = loopsNum * 16;

			IMD5CryptoHash hash { A, B, C, D };
			for (int i = 0; i < loopsNum; ++i)
			{
				IteratePaddedInfo({ inputString, inputNum, loopsInt, i }, hash);
			}

			return hash;
		}


	private:
		constexpr void IteratePaddedInfo(const IMD5PaddedInfo& info, IMD5CryptoHash& hash) noexcept
		{
			unsigned int a = hash[0];
			unsigned int b = hash[1];
			unsigned int c = hash[2];
			unsigned int d = hash[3];


			for (int i = 0; i < 64; ++i)
			{
				unsigned int g = 0;
				unsigned int h = 0;

				if (i < 16)
				{
					g = (b & c) | ((~b) & d);
					h = i;
				}
				else if (i < 32)
				{
					g = (b & d) | (c & (~d));
					h = (1 + 5 * i) % 16;
				}
				else if (i < 48)
				{
					g = b ^ c ^ d;
					h = (5 + 3 * i) % 16;
				}
				else
				{
					g = c ^ (b | (~d));
					h = (7 * i) % 16;
				}

				unsigned int e = d;
				d = c;
				c = b;
				b = b + LeftRotate(a + g + info[h] + K[i], S[i]);
				a = e;
			}

			hash[0] += a;
			hash[1] += b;
			hash[2] += c;
			hash[3] += d;
		}
	};
}



namespace Furud::IPC
{
	class GUID final : private Concept::IMD5
	{
	public:
		GUID() = delete;

		consteval GUID(const char* str)
			: Concept::IMD5(str)
		{}


		constexpr bool operator == (const GUID& rhs) const noexcept
		{
			return
				values[0] == rhs.values[0] &&
				values[1] == rhs.values[1] &&
				values[2] == rhs.values[2] &&
				values[3] == rhs.values[3];
		}

		constexpr bool operator != (const GUID& rhs) const noexcept
		{
			return !(*this == rhs);
		}

#ifdef _IOSTREAM_
		friend std::ostream& operator << (std::ostream& os, const GUID& guid)
		{
			constexpr char lut[] = "0123456789abcdef";
			char buffer[9] = { 0 };
			buffer[8] = 0;
			for (int i = 0; i < 4; ++i)
			{
				const unsigned int& hash = guid.values[i];
				
				// Endianess convertion.
				for (int j = 0, k = 0, m = 4, n = 0; j < 4; ++j)
				{
					const char l = lut[(hash >> m) & 0x0f];
					const char h = lut[(hash >> n) & 0x0f];
					buffer[k++] = l;
					buffer[k++] = h;
					m += 8;
					n += 8;
				}

				os << buffer;
			}

			return os;
		}
#endif
	};
}