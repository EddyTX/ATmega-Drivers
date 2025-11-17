#include <stdlib.h>

// 1. Operatorii de memorie (New/Delete)
void * operator new(size_t size) { return malloc(size); }
void operator delete(void * ptr) { free(ptr); }
void * operator new[](size_t size) { return operator new(size); }
void operator delete[](void * ptr) { operator delete(ptr); }
void operator delete(void* ptr, size_t size) noexcept { (void)size; free(ptr); }

// 2. Handler pentru func?ii pur-virtuale
extern "C" void __cxa_pure_virtual(void)
{
	// Eroare fatal?, bucl? infinit?
	while (1)
	{
		;
	}
}

// 3. Static Initialization Guards (IMPLEMENTARE CORECT?)
// ==========================================================
// 'g' este un pointer la un flag de gard?.
// Îl trat?m ca pe un simplu byte.
//
extern "C" int __cxa_guard_acquire (long *g)
{
	// Transformat pointerul într-un tip de 1 byte
	char* flag = (char*)g;
	
	// Verific?m dac? flag-ul este 0 (neini?ializat)
	if (*flag == 0)
	{
		// Îl set?m pe 1 (ini?ializat)
		*flag = 1;
		// Return?m 1 pentru a-i spune compilatorului S? RULEZE constructorul
		return 1;
	}
	
	// Altfel, flag-ul e deja 1.
	// Return?m 0 pentru a-i spune compilatorului S? SAR? constructorul
	return 0;
}

extern "C" void __cxa_guard_release (long *)
{
	// Într-un sistem single-thread, nu trebuie s? facem nimic aici
}

extern "C" void __cxa_guard_abort (long *)
{
	// Nu este relevant pentru noi
}
// ==========================================================

// 4. DSO Handle
extern "C" void *__dso_handle __attribute__ ((weak));
void *__dso_handle = NULL;