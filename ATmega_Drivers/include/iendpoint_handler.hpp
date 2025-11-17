#pragma once

// Interfa?a pentru un handler de endpoint.
// Toate fi?ierele (gpio_handler, timer_handler) vor implementa asta
class IEndpointHandler
{
	public:
	// O func?ie care prime?te cererea parsatâ
	// Returneaz? 'true' dac? a gestionat cererea, 'false' altfel.
	virtual bool Handle(const char* method, const char* path) = 0;
	
	virtual ~IEndpointHandler() = default;
};