#pragma once
using namespace cmft;

struct ClrAllocator : AllocatorI
{
	void* realloc(void* _ptr, size_t _size, size_t _align, const char* _file, size_t _line)
	{
		// do nothing
		// (else the cmft allocator will try to delete our 
		//  CLR image and we wont be able to save)
		return nullptr;
	}
};
