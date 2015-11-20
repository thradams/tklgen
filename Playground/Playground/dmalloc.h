
#pragma once

#include <stddef.h>

void Free(void* p);
void* Realloc(void* ptr, size_t len);
void* Malloc(size_t len);

