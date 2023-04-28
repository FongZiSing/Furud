//
// Platform.Memory.FileSystem.ixx
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// File utility.
//
module;

#include <Furud.hpp>
#include <filesystem>



export module Furud.Platform.Memory.FileSystem;

export import Furud.Platform.API.CharArray;
namespace fs = std::filesystem;



/** Path string. */
export namespace Furud
{
	class PathString
	{
	private:
		friend struct IFileSystem;
		fs::path data;


	public:
		PathString() = default;

		PathString(const PathString&) = default;

		PathString(PathString&&) = default;

		~PathString() = default;

		PathString& operator= (const PathString&) = default;

		PathString& operator= (PathString&&) noexcept = default;


	public:
		PathString(wchar_t* path)
			: data(path)
		{}

		PathString(const WidecharArrayView& path)
			: data(path.Data(), path.Data() + path.Size())
		{}

		operator WidecharArrayView() noexcept
		{
			return { data.native().c_str(), data.native().size() };
		}
	};
}



/** File system. */
export namespace Furud
{
	struct IFileSystem
	{
		/**
		 * @brief    Checks if the given path corresponds to an existing file.
		 * @returns  False if file isn't exists or is directory.
		 * @details  检查文件是否存在。
		 */
		FURUD_NODISCARD FURUD_INLINE bool FileExists(const PathString& path) noexcept
		{
			fs::path p = path.data.lexically_normal();
			std::error_code error;
			auto fileStatus = fs::status(p, error);

			if (error)
			{
				return false;
			}

			if (!fs::exists(fileStatus))
			{
				return false;
			}

			if (fs::is_directory(fileStatus))
			{
				return false;
			}

			return true;
		}


		/**
		 * @brief    Checks if the given path corresponds to an existing directory.
		 * @returns  False if directory isn't exists or is file.
		 * @details  检查文件夹是否存在。
		 */
		FURUD_NODISCARD FURUD_INLINE static bool DirectoryExists(const PathString& path) noexcept
		{
			fs::path p = path.data.lexically_normal();
			std::error_code error;
			auto fileStatus = fs::status(p, error);

			if (error)
			{
				return false;
			}

			if (!fs::exists(fileStatus))
			{
				return false;
			}

			if (!fs::is_directory(fileStatus))
			{
				return false;
			}

			return true;
		}

	};
}