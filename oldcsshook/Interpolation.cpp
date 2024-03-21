#include "Main.h"

void* Original_Interpolate_Caller;

__int8 Redirected_Interpolate(void* Entity, void* Unknown_Parameter)
{
	if (Entity != *(void**)607867332)
	{
		if (*(void**)Entity == (void*)607350148)
		{
			return 1;
		}
	}

	return (decltype(&Redirected_Interpolate)(Original_Interpolate_Caller))(Entity, Unknown_Parameter);
}

void ClientInterpolation(void)
{
	Redirected_Interpolate;
//	BaseInterpolatePart1 = (BaseInterpolatePart1_t)g_Detour.DetourFunction(((DWORD)BASE_CLIENT + 0x47170), Hooked_BaseInterpolatePart1);
//	SetInterpolationAmount = (SetInterpolationAmount_t)g_Detour.DetourFunction(((DWORD)BASE_CLIENT + 0x34670), Hooked_SetInterpolationAmount);
}
