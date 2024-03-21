#include "Main.h"

typedef void(__thiscall* OverrideView_t)(void*, CViewSetup*);
void __stdcall Hooked_OverrideView(CViewSetup* setup)
{
    BasePlayer* LocalPlayer = (BasePlayer*)g_pClientEntityList->GetClientEntity(g_pEngineClient->GetLocalPlayer());
    setup->fov = 90.f; //g_CVars.Miscellaneous.Fov.Value;
    setup->fovViewmodel = 120.f;//g_CVars.Miscellaneous.Fov.ViewModel;

    if (LocalPlayer && g_pInput->m_fCameraInThirdPerson())
    {
        setup->origin.z = LocalPlayer->GetAbsOrigin().z + 64.f;
    }

    ClientModeVMT->Function< OverrideView_t >(16)(g_pClientMode, setup);
}