#pragma once
#include <memory>
#include <cassert>

#ifndef MINGINE_MEMORY_CHECKS
//#ifndef NDEBUG
#define MINGINE_MEMORY_CHECKS
//#endif
#endif

namespace tiny
{
	namespace memory
	{
		namespace internal
		{
			class RawMemoryAllocation
			{
			private:
				struct MemoryHeader
				{
					char* memory;
					size_t size;
#ifdef MINGINE_MEMORY_CHECKS
					size_t magic;
#endif
				};

			public:
				static void* allocate_memory(size_t size);

				static void free_memory(void* memory);

			private:
				static std::allocator<MemoryHeader> m_header_allocator;
				static std::allocator<char> m_memory_allocator;
			};

			// Utility class to validate heap allocations with
			// a magic number (deadbeef, of course).
			template <typename T>
			struct HeapValidator
			{
				template<class... _Types>
				inline HeapValidator(_Types&&... _Args) :
					value(std::forward<_Types>(_Args)...),
					magic(0xdeadbeef)
				{}

				T value;
				size_t magic;
			};
		}

		template<typename T, class... _Types>
		inline void construct(T * _Ptr, _Types&&... _Args)
		{
			::new (const_cast<void *>(static_cast<const volatile void *>(_Ptr)))
				T(std::forward<_Types>(_Args)...);
		}

		typedef void destructor_t(void*);

		template<typename T>
		inline void destroy(void* _Ptr)
		{
			reinterpret_cast<T*>(_Ptr)->~T();
		}

/*
		template <typename T>
		inline void get_vtable(T* value, void** ptr_out, size_t* size_out)
		{
			static_assert(std::is_polymorphic<T>::value, "Attempted to get vtable pointer of non-polymorphic type");
			static_assert(false, "VTable calculations not implemented");
			auto ptr_out = *value;
		}
*/

		// TODO: If heap validation is enabled (define?), allocate a HeapAllocator instead
		template <typename T, class... _Types>
		inline T* min_new(_Types&&... _Args)
		{
			T* ptr = reinterpret_cast<T*>(internal::RawMemoryAllocation::allocate_memory(sizeof(T)));
			assert(ptr && "Failed to allocate memory");
			construct(ptr, std::forward<_Types>(_Args)...);
			return ptr;
		}

		// TODO: If heap validation is enabled (define?), validate and destroy the HeapAllocator instead
		template <typename T>
		inline void min_delete(T* ptr)
		{
			assert(ptr && "Attempted to delete null pointer");
			destroy<T>(ptr);
			internal::RawMemoryAllocation::free_memory(ptr);
		}

		template <typename T>
		inline void min_delete(T* ptr, destructor_t* dest)
		{
			assert(ptr && "Attempted to delete null pointer");
			dest(ptr);
			internal::RawMemoryAllocation::free_memory(ptr);
	}
	}
}

#ifdef MINGINE_CUSTOM_NEWDELETE
#define MIN_NEW(T, ...) mingine::memory::min_new<T>(__VA_ARGS__)
#define MIN_DELETE(...) mingine::memory::min_delete(__VA_ARGS__)
#else
#define MIN_NEW(T, ...) new T(__VA_ARGS__)
#define MIN_DELETE(...) delete __VA_ARGS__
#endif