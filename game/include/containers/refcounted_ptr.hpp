#pragma once
#include <atomic>
#include <cassert>
#include "allocations.hpp"
namespace tiny
{
	namespace memory
	{
		template <typename T>
		class refcounted_ptr_t
		{
			template <typename A, typename V>
			friend bool operator ==(const refcounted_ptr_t<A>& lhs, const refcounted_ptr_t<V>& rhs);

			template <typename A, typename V>
			friend bool operator ==(const refcounted_ptr_t<A>& lhs, const A* rhs);

			template <typename A, typename V>
			friend bool operator !=(const refcounted_ptr_t<A>& lhs, const refcounted_ptr_t<V>& rhs);

			template <typename P>
			friend class refcounted_ptr_t;

			template <typename T, typename P>
			friend refcounted_ptr_t<T> create_refcounted_weak_ptr(const refcounted_ptr_t<P>& rhs);

			struct RefCount
			{
				std::atomic_int count;
				T* value;
			};

		public:
			inline refcounted_ptr_t() : m_refcount(NULL), m_is_weak(false) {}

			// TODO: ifdef c++11
			inline refcounted_ptr_t(const std::nullptr_t&) : m_refcount(NULL), m_destructor(NULL), m_is_weak(false) {}

			inline refcounted_ptr_t(const refcounted_ptr_t& rhs) : m_refcount(rhs.m_refcount), m_destructor(rhs.m_destructor), m_is_weak(false)
			{
				if (m_refcount)
					increment();
			}

			template <typename P>
			inline refcounted_ptr_t(const refcounted_ptr_t<P>& rhs) : m_refcount(reinterpret_cast<RefCount*>(rhs.m_refcount)), m_destructor(rhs.m_destructor), m_is_weak(false)
			{
				static_assert(std::is_base_of<T, P>::value, "Invalid graphics handle type assignment");
				if (m_refcount)
					increment();
			}

			inline refcounted_ptr_t(refcounted_ptr_t&& rhs) : m_refcount(rhs.m_refcount), m_destructor(rhs.m_destructor), m_is_weak(false) {
				rhs.m_refcount = NULL;
				rhs.m_destructor = NULL;
			}

			template <typename P>
			inline refcounted_ptr_t(P* rhs) : m_refcount(NULL), m_destructor(NULL), m_is_weak(false)
			{
				static_assert(std::is_base_of<T, P>::value, "Invalid graphics handle type assignment");
				m_refcount = min_new<RefCount>();
				m_refcount->value = rhs;
				
				if (std::is_polymorphic<P>::value)
					m_destructor = &destroy<P>;

				++m_refcount->count;
			}

			inline ~refcounted_ptr_t()
			{
				if (m_refcount)
					decrement();
			}

		public:
			inline operator bool() const 
			{
				return m_refcount != NULL;
			}

			inline T& operator*()
			{
				assert(m_refcount && m_refcount->value && "Attempted to dereference null graphics handle");
				return *(m_refcount->value);
			}

			inline T* operator->()
			{
				assert(m_refcount && m_refcount->value && "Attempted to dereference null graphics handle");
				return m_refcount->value;
			}

			inline const T& operator*() const
			{
				assert(m_refcount && m_refcount->value && "Attempted to dereference null graphics handle");
				return *(m_refcount->value);
			}

			inline const T* operator->() const
			{
				assert(m_refcount && m_refcount->value && "Attempted to dereference null graphics handle");
				return m_refcount->value;
			}

			inline refcounted_ptr_t& operator=(const refcounted_ptr_t& rhs)
			{
				if (m_refcount)
					decrement();
				m_refcount = rhs.m_refcount;
				m_destructor = rhs.m_destructor;
				if (m_refcount)
					increment();
				return *this;
			}

			inline refcounted_ptr_t& operator=(std::nullptr_t rhs)
			{
				if (m_refcount)
					decrement();
				m_refcount = NULL;
				m_destructor = NULL;
				return *this;
			}

			template <typename P>
			inline refcounted_ptr_t& operator=(const refcounted_ptr_t<P>& rhs)
			{
				static_assert(std::is_same<T, P>::value || std::is_base_of<T, P>::value, "Invalid graphics handle type assignment");
				decrement();
				m_refcount = reinterpret_cast<RefCount*>(rhs.m_refcount);
				m_destructor = rhs.m_destructor;
				if (m_refcount)
					increment();
				return *this;
			}

		public:
			inline bool is_valid() const
			{
				return m_refcount != NULL;
			}

			inline bool is_weak() const
			{
				return m_is_weak;
			}

			inline void clear()
			{
				if (m_refcount)
				{
					decrement();
					m_refcount = NULL;
				}
			}

			inline T* get_pointer()
			{
				if (!m_refcount)
					return NULL;

				return m_refcount->value;
			}

			inline const T* get_pointer() const
			{
				if (!m_refcount)
					return NULL;

				return m_refcount->value;
			}

			inline size_t get_hash() const
			{
				return (size_t)(m_refcount);
			}

		private:
			inline void increment()
			{
				assert(m_refcount);
				if (!m_is_weak)
					++m_refcount->count;
			}

			inline void decrement()
			{
				if (!m_is_weak && m_refcount) {
					if (--m_refcount->count == 0)
					{
						// TODO: std::is_polymorphic<T>::value
						if (false)// m_destructor != NULL)
							min_delete(m_refcount->value, m_destructor);
						else
							min_delete(m_refcount->value);

						min_delete(m_refcount);
					}
				}
			}

		private:
			RefCount * m_refcount;
			destructor_t* m_destructor;
			bool m_is_weak;
		};

		template <typename T, typename V>
		inline bool operator ==(const refcounted_ptr_t<T>& lhs, const refcounted_ptr_t<V>& rhs)
		{
			return lhs.m_refcount == rhs.m_refcount;
		}

		template <typename T, typename A>
		inline bool operator ==(const refcounted_ptr_t<T>& lhs, const A* rhs)
		{
			return lhs.m_refcount->value == rhs;
		}

		template <typename T, typename V>
		inline bool operator !=(const refcounted_ptr_t<T>& lhs, const refcounted_ptr_t<V>& rhs)
		{
			return lhs.m_refcount != rhs.m_refcount;
		}

		template <typename T>
		inline bool operator !=(const refcounted_ptr_t<T>& lhs, const T* rhs)
		{
			return lhs.m_refcount->value != rhs;
		}

		template <typename T, class... _Types>
		inline refcounted_ptr_t<T> create_refcounted_ptr(_Types&&... _Args)
		{
			return refcounted_ptr_t<T>(min_new<T>(std::forward<_Types>(_Args)...));
		}

		template <typename T, typename P>
		inline refcounted_ptr_t<T> create_refcounted_weak_ptr(const refcounted_ptr_t<P>& rhs)
		{
			refcounted_ptr_t<T> ret;
			ret.m_is_weak = true;
			ret = rhs;
			return ret;
		}
	}
}

namespace std
{
	template <typename T>
	struct hash<tiny::memory::refcounted_ptr_t<T>>
	{
		std::size_t operator()(const tiny::memory::refcounted_ptr_t<T>& k) const
		{
			return k.get_hash();
		}
	};
}