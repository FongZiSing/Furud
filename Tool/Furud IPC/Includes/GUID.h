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
	 *           broken down into 16 words composed of 32-bit each.
	 * @details  512-bit的`信息`, 即MD5算法的输入。字符串会被分组，以512 bit (64 byte)为一组。
	 */
	class MD5PaddedInfo
	{
	private:
		unsigned int data[16];


	private:
		constexpr static unsigned int InitPerBytes
			( char const* inputStr
			, int  const& inputLen
			, int  const& totalInt
			, int  const& bytesIdx
			) noexcept
		{
			// The following code is the padding works:
			// - Just copy string.
			if (bytesIdx < inputLen)
			{
				return unsigned int(inputStr[bytesIdx]) << ((bytesIdx % 4) << 3);
			}
			// - Appends a single bit, 1, to the end of the string.
			else if (bytesIdx == inputLen)
			{
				return unsigned int(0x80) << ((bytesIdx % 4) << 3);
			}
			// - Fill in the number of digits for the string.
			else if (bytesIdx / 4 == totalInt - 2)
			{
				return unsigned int(inputLen) << 3;
			}
			// - Else, fill in the zero.
			return unsigned int(0);
		}

		constexpr static unsigned int Init
			( char const* inputStr
			, int  const& inputLen
			, int  const& totalInt
			, int  const& bytesIdx
			) noexcept
		{
			// Combine 4 bytes into an integer.
			return
				InitPerBytes(inputStr, inputLen, totalInt, bytesIdx    ) |
				InitPerBytes(inputStr, inputLen, totalInt, bytesIdx + 1) |
				InitPerBytes(inputStr, inputLen, totalInt, bytesIdx + 2) |
				InitPerBytes(inputStr, inputLen, totalInt, bytesIdx + 3);
		}


	public:
		constexpr MD5PaddedInfo
			( char const* inputStr
			, int  const& inputLen
			, int  const& blockNum
			, int  const& blockIdx
			) noexcept
		{
			int totalInt = blockNum * 16; // 16 unsigned int per block.
			int bytesIdx = blockIdx * 64; // 64 bytes per block.

			for (int i = 0; i < 16; ++i)
			{
				data[i] = Init(inputStr, inputLen, totalInt, bytesIdx);
				bytesIdx += 4;
			}
		}


	public:
		constexpr unsigned int operator [] (int index) const noexcept
		{
			return data[index];
		}
	};



	/**
	 * @brief    The 128-bit `message digest`, 
	 *           that is the structure stores the MD5 hash algorithm's result.
	 * @details  128-bit的`信息摘要`, 即MD5算法的输出结果。
	 */
	class MD5CryptoHash
	{
	private:
		unsigned int data[4];


	public:
		constexpr MD5CryptoHash() noexcept
			: data{ 0 }
		{}

		template<typename... Args>
		constexpr MD5CryptoHash(Args&&... args) noexcept
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
	class MD5
	{
	protected:
		const MD5CryptoHash values;


	public:
		constexpr MD5(const char* inputString) noexcept
			: values(Encode(inputString))
		{}


	private:
		//****************************************************************
		// @brief    The main MD5 algorithm operates on a 128-bit state,
		//           divided into four 32-bit words, denoted A, B, C, and D. 
		//           And the followings is the initialize variables.
		//****************************************************************

		constexpr static unsigned int A = 0x67452301ui32;

		constexpr static unsigned int B = 0xEFCDAB89ui32;

		constexpr static unsigned int C = 0x98BADCFEui32;

		constexpr static unsigned int D = 0x10325476ui32;


	private:
		//****************************************************************
		// @brief    The look-up table that the algorithm used.
		//           S  -  specifies the per-round shift amounts.
		//           K  -  just precomputed table.
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


	private:
		constexpr MD5CryptoHash Encode(const char* inputStr) noexcept
		{
			// Calculates the length of string at compile time.
			// Only suitable for MSVC, need to support other compilers?
			auto inputLen = (int)__builtin_strlen(inputStr);

			// Divides the string into 512-bit blocks and process them.
			// The following code is to calculate the total number of blocks.
			auto blockNum = ((inputLen + 8) / 64) + 1;

			// Initialize hash variables.
			MD5CryptoHash hash { A, B, C, D };

			// The main loops for encoding message.
			for (int blockIdx = 0; blockIdx < blockNum; ++blockIdx)
			{
				IterateEncode({ inputStr, inputLen, blockNum, blockIdx }, hash);
			}
			
			return hash;
		}


	private:
		constexpr static void IterateEncode(const MD5PaddedInfo& info, MD5CryptoHash& hash) noexcept
		{
			// Initializes the hash value for this block.
			unsigned int a = hash[0];
			unsigned int b = hash[1];
			unsigned int c = hash[2];
			unsigned int d = hash[3];

			// Iterates 64 times.
			for (int i = 0; i < 64; ++i)
			{
				unsigned int f = 0;
				unsigned int g = 0;

				if (i < 16)
				{
					// Non-linear function F.
					f = (b & c) | ((~b) & d);
					g = i;
				}
				else if (i < 32)
				{
					// Non-linear function G.
					f = (b & d) | (c & (~d));
					g = (1 + 5 * i) % 16;
				}
				else if (i < 48)
				{
					// Non-linear function H.
					f = b ^ c ^ d;
					g = (5 + 3 * i) % 16;
				}
				else
				{
					// Non-linear function I.
					f = c ^ (b | (~d));
					g = (7 * i) % 16;
				}
				
				// Left rotation.
				unsigned int k = a + f + info[g] + K[i];
				unsigned int l = (k << S[i]) | (k >> (32 - S[i]));
				
				unsigned int e = d;
				d = c;
				c = b;
				b = b + l;
				a = e;
			}

			// Applies this block's hash value to result so far.
			hash[0] += a;
			hash[1] += b;
			hash[2] += c;
			hash[3] += d;
		}
	};
}



namespace Furud::IPC
{
	/**
	 * @brief    Compile-Time Globally Unique Identifier.
	 * @details  编译期GUID。
	 */
	class GUID final : private Concept::MD5
	{
	public:
		GUID() = delete;

		consteval GUID(const char* str) noexcept
			: Concept::MD5(str)
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