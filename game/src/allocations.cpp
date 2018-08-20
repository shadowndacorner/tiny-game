#include <containers/allocations.hpp>
using namespace tiny::memory;
using namespace tiny::memory::internal;

//#ifndef TINY_IS_DLL
std::allocator<RawMemoryAllocation::MemoryHeader> RawMemoryAllocation::m_header_allocator;
std::allocator<char> internal::RawMemoryAllocation::m_memory_allocator;
//#endif

void *RawMemoryAllocation::allocate_memory(size_t size)
{
	if (true)
	{
		return malloc(size);
	}

	auto head = m_header_allocator.allocate(1);
	head->memory = m_memory_allocator.allocate(size);
	head->size = size;
#ifdef TINY_MEMORY_CHECKS
	head->magic = 0xdeadbeef;
#endif
	return head->memory;
}

void RawMemoryAllocation::free_memory(void * memory)
{
	if (true)
	{
		free(memory);
		return;
	}

	MemoryHeader* head = reinterpret_cast<MemoryHeader*>(memory);
#ifdef TINY_MEMORY_CHECKS
	assert(memory && "Attempted to free null");
	assert((head->magic == 0xdeadbeef) && "Invalid pointer passed to free");
	if ((head->magic != 0xdeadbeef)) {
		fprintf(stderr, "Failed to free memory\n");
		return;
	}
#endif
	m_memory_allocator.deallocate(head->memory, head->size);
	m_header_allocator.deallocate(head, 1);
}