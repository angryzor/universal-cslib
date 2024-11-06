#pragma once
#include <cstring>
#include <ucsl/memory/iallocator.h>

namespace ucsl::strings {
	const char* StrDup(const char* src, memory::IAllocator* allocator) {
		auto len = strlen(src);
		auto* buf = new (allocator) char[len + 1];

		if (buf) {
			memcpy(buf, src, len);
			buf[len] = '\0';
		}

		return buf;
	}

	const char* StrNdup(const char* src, size_t size, memory::IAllocator* allocator) {
		auto len = strnlen(src, size);
		auto* buf = new (allocator) char[len + 1];

		if (buf) {
			memcpy(buf, src, len);
			buf[len] = '\0';
		}

		return buf;
	}
}
