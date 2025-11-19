#include <stdlib.h>

void * operator new(size_t size) { return malloc(size); }
void operator delete(void * ptr) { free(ptr); }
void * operator new[](size_t size) { return operator new(size); }
void operator delete[](void * ptr) { operator delete(ptr); }
void operator delete(void* ptr, size_t size) noexcept { (void)size; free(ptr); }

extern "C" void __cxa_pure_virtual(void)
{
	while (1)
	{
		;
	}
}


extern "C" int __cxa_guard_acquire (long *g)
{
	char* flag = (char*)g;
	
	if (*flag == 0)
	{
		
		*flag = 1;
		
		return 1;
	}
	
	return 0;
}

extern "C" void __cxa_guard_release (long *)
{
	
}

extern "C" void __cxa_guard_abort (long *)
{
	
}


extern "C" void *__dso_handle __attribute__ ((weak));
void *__dso_handle = NULL;