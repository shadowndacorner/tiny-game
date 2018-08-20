#include <tiny_dll.hpp>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// I'm going to leave this for now, but it might
// make sense eventually to just get rid of it and
// just use the pointer
struct WindowsDLLData
{
	WindowsDLLData() : m_dll(0)
	{

	}

	HMODULE m_dll;
};

// This kind of blows, but meh it's not that big of a deal
#define DATA static_cast<WindowsDLLData*>(m_platform_data)
#define DATA_DLL DATA->m_dll

tiny::DynamicLibrary::DynamicLibrary()
{
	m_platform_data = new WindowsDLLData;
}

tiny::DynamicLibrary::DynamicLibrary(const char * path)
{
	m_platform_data = new WindowsDLLData;
	load(path);
}

tiny::DynamicLibrary::~DynamicLibrary()
{
	unload();
	delete m_platform_data;
}

void tiny::DynamicLibrary::load(const char * path)
{
	if (DATA_DLL == NULL)
	{
		DATA_DLL = LoadLibrary(path);
	}
}

void tiny::DynamicLibrary::unload()
{
	if (DATA_DLL != NULL)
	{
		FreeLibrary(DATA_DLL);
		DATA_DLL = NULL;
	}
}

void* tiny::DynamicLibrary::load_function(const char* procName)
{
	if (DATA_DLL != NULL)
	{
		GetProcAddress(DATA_DLL, procName);
	}
	return nullptr;
}
