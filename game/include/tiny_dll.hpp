#pragma once

namespace tiny 
{
	// DLL class
	// Each platform can implement this as necessary
	class DynamicLibrary
	{
	public:
		DynamicLibrary();
		DynamicLibrary(const char* path);
		~DynamicLibrary();

	public:
		void load(const char* path);
		void unload();

		void* load_function(const char* procName);

		template <typename T>
		T load_function_t(const char* procName)
		{
			return load_function(const char* procName);
		}

	private:
		void* m_platform_data;
	};
}