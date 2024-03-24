#include "Main.h"
#include "Resolver.h"
#include <deque>
#define clamp(val, min, max) (((val) > (max)) ? (max) : (((val) < (min)) ? (min) : (val)))
#define delta(angle1, angle2) remainderf(fabsf(angle1 - angle2), 360.0f)
#define n(yaw) g_Stuff.GuwopNormalize(fabsf(yaw))
#define ca(angle1, angle2) g_Stuff.CalculateAngles(angle1, angle2)


int shotsfired[64], shotsfired_old[64], angleindex[64][3];
float angle[64][3], angle_old[64][3];
float flResAngle[64];
int bestrecord[64];
std::deque<float>angle_history;
int iResolverSide;
float get_backward_side(BasePlayer* Entity) {
	if (!g_pEngineClient->IsInGame())
		return -1.f;
	float TmpDistance = 999999.f;
	QAngle tmp;
	Vector dst = Entity->EyePosition();
	float Distance = (dst - EyePosition).Length();
	if (Distance < TmpDistance)
	{
		TmpDistance = Distance;
		g_Stuff.CalculateAngles(EyePosition, dst, tmp);
		Normalize(tmp);
		Entity->m_angEyeAngles().y = tmp.y;
	}
}
float get_angle(BasePlayer* Entity) {
	return g_Stuff.GuwopNormalize(Entity->m_angEyeAngles().y);
}
float get_foword_yaw(BasePlayer* Entity) {
	return g_Stuff.GuwopNormalize(get_backward_side(Entity) - 180.f);
}



Vector get_hitbox(int iHitbox, BasePlayer* Entity)
{
	matrix3x4_t matrix[128];
	if (!(Entity->SetupBones(matrix, 128, 0x100, g_pGlobals->curtime))) return Vector(0, 0, 0);
	void* pModel = Entity->GetModel();
	if (!pModel) return Vector(0, 0, 0);
	studiohdr_t* studiohdr = g_pModelInfo->GetStudiomodel(pModel);
	mstudiohitboxset_t* studiohitboxset = studiohdr->pHitboxSet(Entity->m_nHitboxSet());
	if (!studiohitboxset) return Vector(0, 0, 0);
	mstudiobbox_t* studiobbox = studiohitboxset->pHitbox(iHitbox);
	if (!studiobbox) return Vector(0, 0, 0);
	Vector points = ((studiobbox->bbmin + studiobbox->bbmax) * .5f), ret;
	VectorTransform(points, matrix[studiobbox->bone], ret);
	return ret;
}



bool is_visible(BaseEntity* LocalPlayer, CSWeapon* Weapon, BasePlayer* Entity, Vector& vecAbsStart, Vector& vecAbsEnd)
{
	if (!LocalPlayer) return false;
	if (!Weapon) return false;

	trace_t Trace;
	Ray_t Ray;
	TraceFilterSkipTwoEntities TraceFilter(Entity, 0);

	Ray.Init(vecAbsStart, vecAbsEnd);
	g_pEngineTrace->TraceRay(Ray, 0x46004003, (ITraceFilter*)&TraceFilter, &Trace);
	return (Trace.fraction == 1.f);
}



void animation_fix(BasePlayer* local_player)
{
	if (g_pEngineClient->IsInGame() && g_pEngineClient->IsConnected())
	{

		if (!local_player)
			return;

		if (local_player->m_lifeState() != 0)
			return;


		CCSPlayerAnimState* animation_state = local_player->GetAnimstate();
		if (!animation_state)
			return;
		//float m_flGoalFeetYaw = *(float*)((DWORD)local_player + 0xF80 + 0x20);
		local_player->m_bClientSideAnimation() = true;

		float old_curtime = g_pGlobals->curtime;
		float old_frametime = g_pGlobals->frametime;

		static ConVar* host_timescale = g_pCvar->FindVar("host_timescale");
		g_pGlobals->frametime = g_pGlobals->interval_per_tick * host_timescale->GetFloat();
		g_pGlobals->curtime = local_player->m_flSimulationTime() + g_pGlobals->interval_per_tick;

		local_player->UpdateClientSideAnimation();
		animation_state->Update(animation_state->m_flEyeYaw, animation_state->m_flEyePitch);
		g_pGlobals->curtime = old_curtime;
		g_pGlobals->frametime = old_frametime;

		local_player->SetLocalAngles(QAngle(0.f, animation_state->m_flGoalFeetYaw, 0.f));

		local_player->m_bClientSideAnimation() = false;
	}
}

float getspecialangle(float angle)
{
	return ((angle * angle) * 1.5f * 3.1415926535897932384626433832795f * 3.0f / angle);
}

float yawdelta(float value)
{
	float yawdelta = ((value * value) * 1.5 * 3.1415926535897932384626433832795 / value);

	return yawdelta;
}
void TraceHull(const Vector& vec_start, const Vector& vec_end, const unsigned int n_mask, const Vector& extents, trace_t* p_trace)
{
	CTraceFilterWorldOnly Filter;

	Ray_t ray;
	ray.Init(vec_start, vec_end);
	ray.m_Extents = extents;
	ray.m_IsRay = false;

	g_pEngineTrace->TraceRay(ray, n_mask, &Filter, p_trace);
}

typedef void(__thiscall* FrameStageNotify_t)(void*, ClientFrameStage_t);
void __fastcall Hooked_FrameStageNotify(void* ecx, void* edx, ClientFrameStage_t curStage)
{
	BasePlayer* LocalPlayer = (BasePlayer*)g_pClientEntityList->GetClientEntity(g_pEngineClient->GetLocalPlayer());
	QAngle* pPunchAngle;
	QAngle PunchAngle, PunchAngle_old;

	if (g_pEngineClient->IsInGame())
	{
		if (curStage == FRAME_UNDEFINED) return;
	}

	if (curStage == FRAME_RENDER_START)
	{
		if (LocalPlayer && LocalPlayer->m_lifeState() == 0)
		{
			static bool boole = false;
			if (GetAsyncKeyState(4) & 1)
				boole = !boole;

			if (boole)
			{
				*(float*)((DWORD)LocalPlayer + 0xD14) = g_qThirdPerson.y;
				g_pEngineClient->ExecuteClientCmd("thirdperson");
			}
			else
				g_pEngineClient->ExecuteClientCmd("firstperson");
		} 
			//animation_fix(LocalPlayer);
	}


	CreateMoveVMT->Function< FrameStageNotify_t >(32)(ecx, curStage);

	if (!g_pEngineClient->IsInGame()) return;

	static float tempYaw[64];

	if (curStage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		for (auto Index = g_pGlobals->maxClients; Index >= 1; --Index)
		{
			BasePlayer* Entity = (BasePlayer*)g_pClientEntityList->GetClientEntity(Index);
			if (Entity == 0) continue;
			if (Index == g_pEngineClient->GetLocalPlayer()) continue;
			if (Entity->m_lifeState() != 0) continue;
			if (Entity->m_iHealth() > 0 && Entity->m_iHealth() < 500);
			if (!g_CVars.Aimbot.FriendlyFire)
			{
				if (Entity->m_iTeamNum() == LocalPlayer->m_iTeamNum()) continue;
			}
			if (Entity->IsDormant()) continue;

			if (!g_Whitelist.List(Index) && (!g_CVars.Aimbot.Resolver.memeAct) && g_CVars.Aimbot.Resolver.defaultAct) g_Resolver.Think(Entity);
			if (g_CVars.Aimbot.Resolver.memeAct) {
				g_CVars.Aimbot.Resolver.defaultAct == 0;
				g_Resolver.memesolve(Entity);
			}
			if (pPlayerHistory[Index][0].m_SimulationTime != Entity->m_flSimulationTime())
			{
				for (int tick = 1; tick > 0; tick--) pPlayerHistory[Index][tick] = pPlayerHistory[Index][tick - 1];
				g_Stuff.StoreTickRecord(Entity, &pPlayerHistory[Index][0]);
			}
		}
	}
}

/*
TODO:

	class CCSPlayerAnimState;
	class ICSPlayerAnimState; 

	inline CCSPlayerAnimState* GetAnimstate()
	{
		return reinterpret_cast<CCSPlayerAnimState*>(((uint32_t)GetAnimstateInterface()) - 0x94);
	}

	inline ICSPlayerAnimState* GetAnimstateInterface()
	{
		return *reinterpret_cast<ICSPlayerAnimState**>(this + 0xF80);
	}
						{
							auto animstate = Entity->GetAnimstate();
						//	animstate->m_flEyeYaw = Entity->m_angEyeAngles().y;
							animstate->m_flEyePitch = Entity->m_angEyeAngles().x;

							const float angle_diff = !angle_history.empty() ? clamp(animstate->m_flEyeYaw - angle_history.front(), -40.f, 40.f) : 0.f;

							angle_history.push_front(animstate->m_flEyeYaw);

							if (angle_history.size() > 4)
								angle_history.pop_back();

							if (g_iBulletsFired[Entity->entindex()] % 2 > 2)
							{
								animstate->m_flCurrentFeetYaw = Entity->m_angEyeAngles().y + (89 - angle_diff);
							}
							else
							{
								animstate->m_flCurrentFeetYaw = animstate->m_flEyeYaw - angle_diff;
							}

							animstate->m_flGoalFeetYaw = animstate->m_flCurrentFeetYaw;
							animstate->m_flLastTurnTime = 0.f;
							animstate->m_bCurrentFeetYawInitialized = true;
						}


*/