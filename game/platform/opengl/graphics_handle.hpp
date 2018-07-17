#pragma once
#include <containers/refcounted_ptr.hpp>
namespace tiny
{
	namespace graphics
	{
		template <typename T>
		using GraphicsHandle = memory::refcounted_ptr_t<T>;
		
		template <typename T>
		inline GraphicsHandle<T> create_handle()
		{
			return GraphicsHandle<T>(new T);
		}
    }
}

#define CREATE_HANDLE_TYPE(type) typedef tiny::graphics::GraphicsHandle<type> type##Handle;