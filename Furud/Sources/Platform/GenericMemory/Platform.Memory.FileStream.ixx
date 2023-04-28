//
// Platform.Memory.FileStream.ixx
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// IO utility.
//
module;

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <Furud.hpp>
#include <stdint.h>
#include <utility>



export module Furud.Platform.Memory.FileStream;

export import Furud.Platform.API.CharArray;



/** Stream mode. */
export namespace Furud
{
	class InputMode final
	{
	private:
		using TEnum = char;
		TEnum value;


	public:
		static constexpr TEnum ReadOnly   = TEnum('r');
		static constexpr TEnum ShareWrite = TEnum('s');


	public:
		constexpr InputMode(const TEnum& flag) noexcept
			: value(flag)
		{}

		constexpr InputMode& operator = (const TEnum& flag) noexcept
		{
			value = flag;
			return *this;
		}

		constexpr TEnum ToEnum() const noexcept
		{
			return value;
		}
	};



	class OutputMode final
	{
	private:
		using TEnum = char;
		TEnum value;


	public:
		static constexpr TEnum WriteOnly = TEnum('w');
		static constexpr TEnum ShareRead = TEnum('s');
		static constexpr TEnum Append    = TEnum('a');


	public:
		constexpr OutputMode(const TEnum& flag) noexcept
			: value(flag)
		{}

		constexpr OutputMode& operator = (const TEnum& flag) noexcept
		{
			value = flag;
			return *this;
		}

		constexpr TEnum ToEnum() const noexcept
		{
			return value;
		}
	};
}



/** Windows file position. */
namespace Furud::Internal
{
	/**
	 * @brief    The file position data used to seek operations.
	 * @details  文件流位置。
	 */
	class WinFilePosition
	{
	public:
		/** IO information. see https://learn.microsoft.com/en-us/windows/win32/api/minwinbase/ns-minwinbase-overlapped */
		OVERLAPPED overlapped{ 0 };

		/** The offset from the beginning of this file. */
		int64_t offset{ 0 };


	public:
		constexpr WinFilePosition& operator = (const int64_t& pos) noexcept
		{
			offset = pos;
			UpdateOverlapped();
			return *this;
		}

		constexpr WinFilePosition& operator += (const int64_t& pos) noexcept
		{
			offset += pos;
			UpdateOverlapped();
			return *this;
		}

		constexpr operator int64_t () noexcept
		{
			return offset;
		}


	private:
		constexpr void UpdateOverlapped() noexcept
		{
			ULARGE_INTEGER li;
			li.QuadPart = offset;
			overlapped.Offset = li.LowPart;
			overlapped.OffsetHigh = li.HighPart;
		}
	};
}



/** Input stream interface */
namespace Furud::Internal
{
	class FURUD_INTERFACE IInputFileStream
	{
	public:
		constexpr IInputFileStream() = default;

		/** Noncopyable. */
		IInputFileStream(const IInputFileStream&) = delete;

		/** Noncopyable. */
		IInputFileStream& operator = (const IInputFileStream&) = delete;

		/** Disable placement new. */
		void* operator new (size_t, void*) = delete;


	public:
		virtual bool Open(WidecharArrayView filename, InputMode mode = InputMode::ReadOnly) = 0;

		virtual bool IsOpen() = 0;

		virtual void Close() = 0;

		virtual int64_t Size() = 0;

		virtual int64_t Tell() = 0;

		virtual bool IsEOF() = 0;

		virtual void Seek(int64_t position) = 0;

		virtual bool Read(void* data, int64_t bytes) = 0;
	};
}



/** Ouput stream interface. */
namespace Furud::Internal
{
	class FURUD_INTERFACE IOutputFileStream
	{
	public:
		constexpr IOutputFileStream() = default;

		/** Noncopyable. */
		IOutputFileStream(const IOutputFileStream&) = delete;

		/** Noncopyable. */
		IOutputFileStream& operator = (const IOutputFileStream&) = delete;

		/** Disable placement new. */
		void* operator new (size_t, void*) = delete;


	public:
		virtual bool Open(WidecharArrayView filename, OutputMode mode = OutputMode::WriteOnly) = 0;

		virtual bool IsOpen() = 0;

		virtual void Close() = 0;


	public:
		virtual bool Write(const void* data, int64_t size) = 0;
	};
}



/** Input file stream. */
export namespace Furud
{
	/**
	 * @brief    Windows file input stream implementation.
	 * @details  文件输入流。
	 */
	class InputFileStream : public Internal::IInputFileStream
	{
	private:
		HANDLE handle { INVALID_HANDLE_VALUE };
		int64_t fileSize { 0 };
		Internal::WinFilePosition filePos {};


	public:
		constexpr InputFileStream() = default;

		virtual ~InputFileStream()
		{
			Close();
		}


	public:
		virtual bool Open(WidecharArrayView filename, InputMode mode = InputMode::ReadOnly) override
		{
			if (handle == INVALID_HANDLE_VALUE)
			{
				// Open a file.
				// see https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilew
				switch (mode.ToEnum())
				{
				case InputMode::ReadOnly:
					handle = ::CreateFileW
						( filename.Data()
						, GENERIC_READ /* desired access. */ 
						, FILE_SHARE_READ /* sharing mode. */
						, nullptr
						, OPEN_EXISTING /* Open only if exists. */
						, FILE_ATTRIBUTE_NORMAL
						, nullptr
						);
					break;

				case InputMode::ShareWrite:
					handle = ::CreateFileW
						( filename.Data()
						, GENERIC_READ /* desired access. */ 
						, FILE_SHARE_READ | FILE_SHARE_WRITE /* sharing mode. */
						, nullptr
						, OPEN_EXISTING /* Open only if exists. */
						, FILE_ATTRIBUTE_NORMAL
						, nullptr
						);
					break;

				default:
					return false;
				}
			}

			if (handle != INVALID_HANDLE_VALUE)
			{
				LARGE_INTEGER li;
				// Get the size of the specified file.
				// see https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getfilesizeex
				if (::GetFileSizeEx(handle, &li))
				{
					fileSize = li.QuadPart;
				}
				else
				{
					// This is a very abnormal result that occurs under extremely rare conditions.
					// But file size is important, I render the file handle is invalid if happen.
					Close();
				}
			}

			return handle != INVALID_HANDLE_VALUE;
		}

		virtual bool IsOpen() override
		{
			return handle != INVALID_HANDLE_VALUE;
		}

		virtual void Close() override
		{
			if (handle != INVALID_HANDLE_VALUE)
			{
				// Closes the file.
				// https://learn.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-closehandle
				::CloseHandle(handle);
				handle = INVALID_HANDLE_VALUE;
				fileSize = 0;
				filePos = Internal::WinFilePosition();
			}
		}

		virtual int64_t Size() override
		{
			return fileSize;
		}

		virtual int64_t Tell() override
		{
			return filePos;
		}

		virtual bool IsEOF() override
		{
			return filePos >= fileSize;
		}


	public:
		virtual void Seek(int64_t position) override
		{
			filePos = position;
		}

		virtual bool Read(void* data, int64_t bytes) override
		{
			uint8_t* FURUD_RESTRICT p = (uint8_t*)data;
			int64_t batch             = bytes / int64_t(UINT32_MAX);
			int64_t remainder         = bytes % int64_t(UINT32_MAX);

			while(batch --> 0)
			{
				if (!ReadFileInternal(p, (DWORD)UINT32_MAX))
				{
					return false;
				}
			}

			if (remainder > 0)
			{
				if (!ReadFileInternal(p, (DWORD)remainder))
				{
					return false;
				}
			}

			return true;
		}


	private:
		FURUD_INLINE bool ReadFileInternal(uint8_t*& data, DWORD bytesToRead)
		{
			DWORD bytesRead = 0;
			// Reads data from specified file.
			// see https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-readfile
			if (!::ReadFile(handle, data, bytesToRead, &bytesRead, &filePos.overlapped))
			{
				uint32_t code = ::GetLastError();
				if (code != ERROR_IO_PENDING)
				{
					return false;
				}

				bytesRead = 0;
				// Wait for the read to complete.
				if (!::GetOverlappedResult(handle, &filePos.overlapped, &bytesRead, true))
				{
					return false;
				}
			}

			data += bytesRead;
			filePos += bytesRead;

			// Unexpected bytes.
			if (bytesRead != bytesToRead)
			{
				return false;
			}

			return true;
		}
	};
}



/** Output file stream. */
export namespace Furud
{
	/**
	 * @brief    Windows file output stream implementation.
	 * @details  文件输出流。
	 */
	class OutputFileStream : public Internal::IOutputFileStream
	{
	private:
		HANDLE handle { INVALID_HANDLE_VALUE };
		Internal::WinFilePosition filePos {};
		int64_t fileSize { 0 };


	public:
		constexpr OutputFileStream() = default;

		virtual ~OutputFileStream()
		{
			Close();
		}


	public:
		virtual bool Open(WidecharArrayView filename, OutputMode mode = OutputMode::WriteOnly) override
		{
			if (handle == INVALID_HANDLE_VALUE)
			{
				fileSize = 0;

				// Open a file.
				// see https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilew
				switch (mode.ToEnum())
				{
				case OutputMode::WriteOnly:
					handle = ::CreateFileW
						( filename.Data()
						, GENERIC_WRITE /* desired access. */
						, 0 /* sharing mode. */
						, nullptr
						, CREATE_ALWAYS /* Create new file always. */
						, FILE_ATTRIBUTE_NORMAL
						, nullptr
					);
					break;

				case OutputMode::ShareRead:
					handle = ::CreateFileW
						( filename.Data()
						, GENERIC_WRITE /* desired access. */
						, FILE_SHARE_READ /* sharing mode. */
						, nullptr
						, CREATE_ALWAYS /* Create new file always. */
						, FILE_ATTRIBUTE_NORMAL
						, nullptr
					);
					break;

				case OutputMode::Append:
					handle = ::CreateFileW
						( filename.Data()
						, GENERIC_WRITE /* desired access. */
						, FILE_SHARE_READ /* sharing mode. */
						, nullptr
						, OPEN_ALWAYS /* Append new data to file. */
						, FILE_ATTRIBUTE_NORMAL
						, nullptr
					);
					if (handle != INVALID_HANDLE_VALUE)
					{
						LARGE_INTEGER li;
						// Get the size of the specified file.
						// see https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getfilesizeex
						if (::GetFileSizeEx(handle, &li))
						{
							fileSize = li.QuadPart;
							filePos = fileSize;
						}
						else
						{
							// This is a very abnormal result that occurs under extremely rare conditions.
							// But file size is important, I render the file handle is invalid if happen.
							Close();
							return false;
						}
					}
					break;

				default:
					return false;
				}
			}


			return handle != INVALID_HANDLE_VALUE;
		}

		virtual bool IsOpen() override
		{
			return handle != INVALID_HANDLE_VALUE;
		}

		virtual void Close() override
		{
			if (handle != INVALID_HANDLE_VALUE)
			{
				// Closes the file.
				// https://learn.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-closehandle
				::CloseHandle(handle);
				handle = INVALID_HANDLE_VALUE;
				fileSize = 0;
			}
		}


	public:
		virtual bool Write(const void* data, int64_t bytes) override
		{
			uint8_t* FURUD_RESTRICT p  = (uint8_t*)data;
			int64_t batch              = bytes / int64_t(UINT32_MAX);
			int64_t remainder          = bytes % int64_t(UINT32_MAX);

			while (batch --> 0)
			{
				if (!WriteFileInternal(p, (DWORD)UINT32_MAX))
				{
					return false;
				}
			}

			if (remainder > 0)
			{
				if (!WriteFileInternal(p, (DWORD)remainder))
				{
					return false;
				}
			}

			return true;
		}


	private:
		FURUD_INLINE bool WriteFileInternal(uint8_t*& data, DWORD bytesToWrite)
		{
			DWORD bytesWritten = 0;
			// Writes data from specified file.
			// see https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-writefile
			if (!::WriteFile(handle, data, bytesToWrite, &bytesWritten, &filePos.overlapped))
			{
				uint32_t code = ::GetLastError();
				if (code != ERROR_IO_PENDING)
				{
					return false;
				}

				bytesWritten = 0;
				// Wait for the write to complete.
				if (!::GetOverlappedResult(handle, &filePos.overlapped, &bytesWritten, true))
				{
					return false;
				}
			}

			data += bytesWritten;
			filePos += bytesWritten;
			fileSize = std::max(fileSize, filePos.offset);

			// Unexpected bytes.
			if (bytesWritten != bytesToWrite)
			{
				return false;
			}

			return true;
		}
	};
}



/** Text stream. */
export namespace Furud
{
	/**
	 * @brief    Text file input stream implementation.
	 * @details  文本输入流。
	 */
	class AnsicharStream : private InputFileStream
	{
	private:
		AnsicharArray buffer;


	public:
		AnsicharStream()
			: InputFileStream()
			, buffer(255)
		{}

		void ResizeBuffer(uint32_t inSize)
		{
			buffer = AnsicharArray(inSize);
		}

		virtual bool Open(WidecharArrayView filename, InputMode mode = InputMode::ReadOnly) override
		{
			bool bFileValid = InputFileStream::Open(filename, mode);

			if (bFileValid)
			{
				uint8_t bom[3] = { 0 };
				Read(bom, 3);

				// utf-8 with bom.
				if (bom[0] == 0xef && bom[1] == 0xbb && bom[2] == 0xbf)
				{
					// ignore bom, do nothing.
				}
				else
				{
					Seek(0);
				}
			}

			return bFileValid;
		}


	public:
		using InputFileStream::Close;
		using InputFileStream::IsEOF;
		using InputFileStream::Read;
		using InputFileStream::Size;
		using InputFileStream::Tell;


	public:
		bool GetLine(AnsicharArray& line)
		{
			// Reserve at least 32 bytes of memory.
			const size_t numLeastCapacity = line.Capacity() >= 32 ? 0 : 32;
			line.Empty(numLeastCapacity);

			bool success = false;
			bool finished = false;
			do
			{
				GetlineInternal(finished);

				// Overflow?
				if (line.Size() >= AnsicharArray::npos - buffer.Size())
				{
					break;
				}
				else
				{
					line.Append(AnsicharArrayView { buffer });
				}

				if (finished)
				{
					break;
				}
			}
			while (true);


			return success;
		}


	private:
		FURUD_INLINE void GetlineInternal(bool& finished)
		{
			AnsicharArray::TSize counter = buffer.Capacity() - 1;

			finished = false;
			buffer.Clear();

			while (counter--)
			{
				char ch;

				if (!Read(&ch, 1))
				{
					// EOF or other error.
					finished = true;
					break;
				}
				// Ignores carriage return character.
				else if (ch == 0x0d /* \r */)
				{
					continue;
				}
				// Meets line feed.
				else if (ch == 0x0a /* \n */)
				{
					finished = true;
					break;
				}

				buffer.PushUnsafe(ch);
			}
		}
	};
}