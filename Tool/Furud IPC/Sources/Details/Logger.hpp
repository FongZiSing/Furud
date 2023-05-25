//
// Logger.hpp
//
//       Copyright (c) Furud IPC Library. All rights reserved.
//       @Author FongZiSing
//
// Implemention of logger.
//
#pragma once

#include <cstdio>
#include <utility>



#if _DEBUG
#define FURUD_IPC_LOGGER 1
#endif

namespace Furud::IPC
{
	struct Logger
	{
	private:
		template <typename O>
		__forceinline static void Print(O out, char const* str)
		{
			std::fprintf(out, "[Furud] ");
			std::fprintf(out, "%s\n", str);
		}

		template <typename O, typename Arg0, typename ...Args>
		__forceinline static void Print(O out, char const* fmt, Arg0&& arg0, Args&&... args)
		{
			std::fprintf(out, "[Furud] ");
			std::fprintf(out, fmt, std::forward<Arg0>(arg0), std::forward<Args>(args)...);
			std::fprintf(out, "\n");
		}


	public:
		inline static void Log(char const* str) noexcept
		{
		#if FURUD_IPC_LOGGER
			Print(stdout, str);
		#endif
		}

		inline static void Error(char const* str) noexcept
		{
		#if FURUD_IPC_LOGGER
			Print(stderr, str);
		#endif
		}

		template <typename Arg0, typename ...Args>
		inline static void Log(char const* fmt, Arg0&& arg0, Args&&... args)
		{
		#if FURUD_IPC_LOGGER
			Print(stdout, fmt, std::forward<Arg0>(arg0), std::forward<Args>(args)...);
		#endif
		}

		template <typename Arg0, typename ...Args>
		inline static void Error(char const* fmt, Arg0&& arg0, Args&&... args)
		{
		#if FURUD_IPC_LOGGER
			Print(stderr, fmt, std::forward<Arg0>(arg0), std::forward<Args>(args)...);
		#endif
		}
	};
}