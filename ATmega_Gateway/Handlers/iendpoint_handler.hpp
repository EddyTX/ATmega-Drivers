#pragma once


class IEndpointHandler
{
	public:
	
	virtual bool Handle(const char* method, const char* path) = 0;
	
	virtual ~IEndpointHandler() = default;
};