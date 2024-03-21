#include "Main.h"

void AddToTail(void* thisptr, BasePlayer* Ent)
{
	typedef void(__thiscall* AddToTail_t)(void*, BasePlayer*);
	((AddToTail_t)((DWORD)BASE_CLIENT + 0x4C040))(thisptr, Ent);
}

typedef bool(__thiscall* InPrediction_t)(void*);
bool __fastcall Hooked_InPrediction(void* ptr, int edx)
{
	//PredictionVMT->SetHookEnabled( false );
	bool ret = PredictionVMT->Function< InPrediction_t >(8)(ptr); //g_pPrediction->InPrediction( );
	//PredictionVMT->SetHookEnabled( true );
	return ret;
}

void __stdcall Gate_SetViewAngles(PFLOAT pfAngles, BaseEntity* ent, DWORD dwReturnAddress)
{
	PFLOAT pfPunchAngle;

	if (dwReturnAddress == 0x240192BE)
	{
		if (ent && ent->entindex() == g_pEngineClient->GetLocalPlayer() && pfAngles)
		{
			pfPunchAngle = (PFLOAT)((DWORD)ent + 0xBB0);

			if (g_CVars.Visuals.NoVisualRecoil)
			{
				pfAngles[0] -= pfPunchAngle[0];
				pfAngles[1] -= pfPunchAngle[1];
				pfAngles[2] -= pfPunchAngle[2];
			}
		}
	}


}

BOOL g_bSetViewAngles;

__declspec (naked) void Hooked_SetViewAngles(void)
{
	__asm
	{
		XOR EAX, EAX

		MOV AL, BYTE PTR[ECX + 0x8]
		MOV[g_bSetViewAngles], EAX

		MOV EAX, DWORD PTR[ESP]
		PUSH EAX
		PUSH ESI
		PUSH EDI
		CALL Gate_SetViewAngles

		MOV EAX, [g_bSetViewAngles]
		RETN
	}
}