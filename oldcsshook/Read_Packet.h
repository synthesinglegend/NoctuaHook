#pragma once
void* Original_Read_Packets_Caller;
#include <stdio.h>
#include <intrin.h>

#pragma intrinsic(_ReturnAddress)
void Redirected_Read_Packets(__int8 Final)
{
	static __int8 Parsed_Packets;

	if (Parsed_Packets == 0)
	{
		(decltype(&Redirected_Read_Packets)(Original_Read_Packets_Caller))(Final);
	}

	Parsed_Packets = _ReturnAddress() != (void*)537729110;
}