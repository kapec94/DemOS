#include <stddef.h>
#include <string.h>
#include "bda.h"

int bda_query(u8 addr, size_t len, void* out)
{
	void* ptr = (void*)(BDA_ADDRESS + addr);
	memcpy(out, ptr, len);

	return S_OK;
}
