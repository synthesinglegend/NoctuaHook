#include "Main.h"

LagCompensation g_LagCompensation;
float clamp89(float val, float minVal, float maxVal)
{
	if (maxVal < minVal)
		return maxVal;
	else if (val < minVal)
		return minVal;
	else if (val > maxVal)
		return maxVal;
	else
		return val;
}

float LagCompensation::LerpTime()
{
	static ConVar* cvar_cl_interp = g_pCvar->FindVar( /*cl_interp*/XorStr<0x24, 10, 0x7F9B18B0>("\x47\x49\x79\x4E\x46\x5D\x4F\x59\x5C" + 0x7F9B18B0).s);
	static ConVar* cvar_cl_updaterate = g_pCvar->FindVar( /*cl_updaterate*/XorStr<0xC6, 14, 0xD9FFF99F>("\xA5\xAB\x97\xBC\xBA\xAF\xAD\xB9\xAB\xBD\xB1\xA5\xB7" + 0xD9FFF99F).s);
	static ConVar* cvar_cl_interp_ratio = g_pCvar->FindVar( /*cl_interp_ratio*/XorStr<0xBF, 16, 0x298E884B>("\xDC\xAC\x9E\xAB\xAD\xB0\xA0\xB4\xB7\x97\xBB\xAB\xBF\xA5\xA2" + 0x298E884B).s);
	static ConVar* cvar_sv_minupdaterate = g_pCvar->FindVar( /*sv_minupdaterate*/XorStr<0xF6, 17, 0x99ECD573>("\x85\x81\xA7\x94\x93\x95\x89\x8D\x9A\x9E\x74\x64\x70\x62\x70\x60" + 0x99ECD573).s);
	static ConVar* cvar_sv_maxupdaterate = g_pCvar->FindVar( /*sv_maxupdaterate*/XorStr<0x6A, 17, 0x6C62999F>("\x19\x1D\x33\x00\x0F\x17\x05\x01\x16\x12\x00\x10\x04\x16\x0C\x1C" + 0x6C62999F).s);
	static ConVar* cvar_sv_client_min_interp_ratio = g_pCvar->FindVar( /*sv_client_min_interp_ratio*/XorStr<0x9B, 27, 0x783554E3>("\xE8\xEA\xC2\xFD\xF3\xC9\xC4\xCC\xD7\xFB\xC8\xCF\xC9\xF7\xC0\xC4\xDF\xC9\xDF\xDE\xF0\xC2\xD0\xC6\xDA\xDB" + 0x783554E3).s);
	static ConVar* cvar_sv_client_max_interp_ratio = g_pCvar->FindVar( /*sv_client_max_interp_ratio*/XorStr<0xAF, 27, 0xED44D950>("\xDC\xC6\xEE\xD1\xDF\xDD\xD0\xD8\xC3\xE7\xD4\xDB\xC3\xE3\xD4\xD0\xCB\xA5\xB3\xB2\x9C\xB6\xA4\xB2\xAE\xA7" + 0xED44D950).s);

	float lerp = cvar_cl_interp->GetFloat();
	int cl_updaterate = cvar_cl_updaterate->GetInt(),
		sv_maxupdaterate = cvar_sv_maxupdaterate->GetInt(),
		sv_minupdaterate = cvar_sv_minupdaterate->GetInt(),
		cl_interp_ratio = cvar_cl_interp_ratio->GetInt(),
		sv_client_min_interp_ratio = cvar_sv_client_min_interp_ratio->GetInt(),
		sv_client_max_interp_ratio = cvar_sv_client_max_interp_ratio->GetInt();

	if (cvar_sv_minupdaterate && cvar_sv_maxupdaterate) cl_updaterate = sv_maxupdaterate;

	if (cl_interp_ratio == 0.f) cl_interp_ratio = 1.f;
	if (cvar_sv_client_min_interp_ratio && cvar_sv_client_max_interp_ratio && sv_client_min_interp_ratio != 1.f) cl_interp_ratio = clamp89(cl_interp_ratio, sv_client_min_interp_ratio, sv_client_max_interp_ratio);

	return max(lerp, (cl_interp_ratio / cl_updaterate));
}

int LagCompensation::ServerTick()
{
	float out, in;
	int servertick;

	in = g_pEngineClient->GetNetChannelInfo()->GetLatency(1);
	out = g_pEngineClient->GetNetChannelInfo()->GetLatency(0);

	servertick = g_pGlobals->tickcount + 1 + TIME_TO_TICKS(in + out);

	return servertick;
}

int get_dmgmod_player(const float& constdamage, bool isHeadshot, bool isFriendly, bool hasHelmet, CSWeapon* Weapon, BaseEntity* targetEntity)
{
	WeaponInfo WeaponInfo = g_NoSpread.GetWeaponInfo(Weapon);

	float damage = constdamage;
	if (isFriendly) damage *= .35f;

	int armor = targetEntity->m_ArmorValue();
	if ((armor > 0) && (!isHeadshot || (isHeadshot && hasHelmet)))
	{
		float weaponArmorRatio = (.5f * WeaponInfo.ArmorRatio);

		float newdamage = (weaponArmorRatio * damage);
		float armordamage = ((damage - newdamage) * .5f);

		if (armordamage <= (float)armor)
		{
			armordamage = floor(armordamage);
		}
		else
		{
			newdamage = (damage + ((float)armor * -2.f));
			armordamage = (int)armor;
		}

		damage = floor(newdamage);
	}
	else damage = floor(damage);
	return (int)damage;
}

float get_dmgmod_hitgroup(float dmg, int hitgroup)
{
	static float hitgroupModifiers[] = { 1.f, 4.f, 1.f, 1.25f, 1.f, 1.f, .75f, .75f };
	return(dmg * hitgroupModifiers[hitgroup]);
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

void SimulatePlayerMovement(BasePlayer* Entity, CTickRecord* Record, bool InAir)
{
	if (!(Record->m_Flags & FL_ONGROUND)) Record->m_Velocity.z -= g_pCvar->FindVar("sv_gravity")->GetFloat() * g_pGlobals->interval_per_tick;
	else if (InAir) Record->m_Velocity.z = sqrtf(91200.f);

	BasePlayer* LocalPlayer = (BasePlayer*)g_pClientEntityList->GetClientEntity(g_pEngineClient->GetLocalPlayer());

	trace_t Trace;
	Ray_t ray1, ray2, ray3;
	TraceFilterSkipTwoEntities TraceFilter(LocalPlayer, 0);

	auto src = Record->m_Origin, end = src + (Record->m_Velocity * g_pGlobals->interval_per_tick);

	ray1.Init(src, end);
	g_pEngineTrace->TraceRay(ray1, CONTENTS_SOLID, &TraceFilter, &Trace);

	if (Trace.fraction != 1.f)
	{
		for (int i = 0; i < 2; ++i)
		{
			Record->m_Velocity -= Trace.plane.normal * Trace.plane.normal.Dot(end);

			float dot = Trace.plane.normal.Dot(Record->m_Velocity);
			if (dot < 0.f) Record->m_Velocity -= dot * Trace.plane.normal;

			end = Trace.endpos + (Record->m_Velocity * (g_pGlobals->interval_per_tick * (1.f - Trace.fraction)));

			ray2.Init(Trace.endpos, end);
			g_pEngineTrace->TraceRay(ray2, CONTENTS_SOLID, &TraceFilter, &Trace);

			if (Trace.fraction == 1.f) break;
		}
	}

	Record->m_Origin = Trace.endpos;
	end = Trace.endpos;
	end.z -= 2.f;

	ray3.Init(Record->m_Origin, end);
	g_pEngineTrace->TraceRay(ray3, CONTENTS_SOLID, &TraceFilter, &Trace);

	Record->m_Flags &= ~FL_ONGROUND;

	if (Trace.fraction != 1.f && Trace.plane.normal.z > 0.7f) Record->m_Flags |= FL_ONGROUND;
}

float GetAverageMovementVelocityDelta(CTickRecord* oldtick, CTickRecord* newtick)
{
	float oldtickvel = oldtick->m_Velocity.Length();
	float newtickvel = newtick->m_Velocity.Length();
	float delta = newtickvel - oldtickvel;

	return sqrtf(delta * delta);
}
int LagCompensation::GetBestRecord(BasePlayer* LocalPlayer, CSWeapon* Weapon, BasePlayer* Entity)
{
	int i = Entity->entindex();
	static int best_record[64];
	static int maxdmg[64];

	for (int k = 16; --k; k >= 0)
	{
		CTickRecord backup;
		CTickRecord* record = &pPlayerHistory[i][k];
		CTickRecord* prevrecord = &pPlayerHistory[i][k + 1];

		if (!CheckDelta(record))
		{
			best_record[i] = 0;
			break;
		}

		if ((record->m_Origin - prevrecord->m_Origin).LengthSqr() > 4096.f)
		{
			best_record[i] = 0;
			break;
		}

		g_Stuff.StoreTickRecord(Entity, &backup);
		g_Stuff.ApplyTickRecord(Entity, record);

		Vector dir = get_hitbox(g_CVars.Aimbot.Hitbox, Entity);
		int temp = INT_MAX, rate;
		rate = LocalPlayer->GetAbsOrigin().DistTo(Entity->GetAbsOrigin());

		if (rate <= temp)
		{
			if (is_visible(LocalPlayer, Weapon, Entity, EyePosition, dir))
			{
				best_record[i] = k;
				temp = rate;
			}
		}

		g_Stuff.ApplyTickRecord(Entity, &backup);
	}

	return best_record[i];
}

bool LagCompensation::CheckDelta(CTickRecord* record)
{
	float out, dt, correct;

	out = g_pEngineClient->GetNetChannelInfo()->GetLatency(0);

	correct = out + TICKS_TO_TIME(TIME_TO_TICKS(LerpTime()));
	correct = clamp89(correct, 0.f, 1.f);

	/*
	auto PredictedCmdArrivalTick = cmdTickcount + 1 + TIME_TO_TICKS(netChannelInfo->GetAvgLatency(FLOW_INCOMING) + netChannelInfo->GetAvgLatency(FLOW_OUTGOING));
	auto Correct = clamp(lerpTime + netChannelInfo->GetLatency(FLOW_OUTGOING), 0.f, 1.f) - TICKS_TO_TIME(PredictedCmdArrivalTick + TIME_TO_TICKS(lerpTime) - GetTickcountForEntity(tickrecord.m_SimulationTime));
	*/

	int entitytick = TIME_TO_TICKS(record->m_SimulationTime) + TIME_TO_TICKS(LerpTime());
	dt = correct - TICKS_TO_TIME(ServerTick() + TIME_TO_TICKS(LerpTime()) - entitytick);

	return (fabs(dt) <= 0.2f);
}

void LagCompensation::Store()
{
	BasePlayer* LocalPlayer = (BasePlayer*)g_pClientEntityList->GetClientEntity(g_pEngineClient->GetLocalPlayer());
	if (!LocalPlayer) return;

	for (auto Index = g_pGlobals->maxClients; Index >= 1; --Index)
	{
		if (Index == g_pEngineClient->GetLocalPlayer()) continue;

		BasePlayer* Entity = (BasePlayer*)g_pClientEntityList->GetClientEntity(Index);
		if (!Entity) continue;

		if (Entity->m_lifeState() != 0)
		{
			for (int Tick = 31; Tick > 0; Tick--) HistoryData[Index][Tick].Reset();
			continue;
		}

		if (Entity->m_iHealth() > 0 && Entity->m_iHealth() < 500);

		if (!g_CVars.Aimbot.FriendlyFire)
		{
			if (Entity->m_iTeamNum() == LocalPlayer->m_iTeamNum()) continue;
		}
		if (Entity->IsDormant()) continue;

		if (HistoryData[Index][0].m_SimulationTime > Entity->m_flSimulationTime()) HistoryData[Index][0].m_SimulationTime = 0.f;
		if (HistoryData[Index][0].m_SimulationTime == Entity->m_flSimulationTime()) return;

		for (int Tick = 31; Tick > 0; Tick--) HistoryData[Index][Tick] = HistoryData[Index][Tick - 1];
		g_Stuff.StoreTickRecord(Entity, &HistoryData[Index][0]);
	}
}

void LagCompensation::Restore(BasePlayer* Entity)
{
	if (!Entity) return;
	int Index = Entity->entindex();

	if (HistoryData[Index][0].m_SimulationTime == 0.f) return;

	static ConVar* cvar_cl_interp = g_pCvar->FindVar( /*cl_interp*/XorStr<0x24, 10, 0x7F9B18B0>("\x47\x49\x79\x4E\x46\x5D\x4F\x59\x5C" + 0x7F9B18B0).s);
	static ConVar* cvar_cl_updaterate = g_pCvar->FindVar( /*cl_updaterate*/XorStr<0xC6, 14, 0xD9FFF99F>("\xA5\xAB\x97\xBC\xBA\xAF\xAD\xB9\xAB\xBD\xB1\xA5\xB7" + 0xD9FFF99F).s);
	static ConVar* cvar_cl_interp_ratio = g_pCvar->FindVar( /*cl_interp_ratio*/XorStr<0xBF, 16, 0x298E884B>("\xDC\xAC\x9E\xAB\xAD\xB0\xA0\xB4\xB7\x97\xBB\xAB\xBF\xA5\xA2" + 0x298E884B).s);
	static ConVar* cvar_sv_minupdaterate = g_pCvar->FindVar( /*sv_minupdaterate*/XorStr<0xF6, 17, 0x99ECD573>("\x85\x81\xA7\x94\x93\x95\x89\x8D\x9A\x9E\x74\x64\x70\x62\x70\x60" + 0x99ECD573).s);
	static ConVar* cvar_sv_maxupdaterate = g_pCvar->FindVar( /*sv_maxupdaterate*/XorStr<0x6A, 17, 0x6C62999F>("\x19\x1D\x33\x00\x0F\x17\x05\x01\x16\x12\x00\x10\x04\x16\x0C\x1C" + 0x6C62999F).s);
	static ConVar* cvar_sv_client_min_interp_ratio = g_pCvar->FindVar( /*sv_client_min_interp_ratio*/XorStr<0x9B, 27, 0x783554E3>("\xE8\xEA\xC2\xFD\xF3\xC9\xC4\xCC\xD7\xFB\xC8\xCF\xC9\xF7\xC0\xC4\xDF\xC9\xDF\xDE\xF0\xC2\xD0\xC6\xDA\xDB" + 0x783554E3).s);
	static ConVar* cvar_sv_client_max_interp_ratio = g_pCvar->FindVar( /*sv_client_max_interp_ratio*/XorStr<0xAF, 27, 0xED44D950>("\xDC\xC6\xEE\xD1\xDF\xDD\xD0\xD8\xC3\xE7\xD4\xDB\xC3\xE3\xD4\xD0\xCB\xA5\xB3\xB2\x9C\xB6\xA4\xB2\xAE\xA7" + 0xED44D950).s);
	static ConVar* cvar_sv_max_usercmd_future_ticks = g_pCvar->FindVar("sv_max_usercmd_future_ticks");

	float cl_interp = cvar_cl_interp->GetFloat();
	int cl_updaterate = cvar_cl_updaterate->GetInt(),
		sv_maxupdaterate = cvar_sv_maxupdaterate->GetInt(),
		sv_minupdaterate = cvar_sv_minupdaterate->GetInt(),
		cl_interp_ratio = cvar_cl_interp_ratio->GetInt(),
		sv_client_min_interp_ratio = cvar_sv_client_min_interp_ratio->GetInt(),
		sv_client_max_interp_ratio = cvar_sv_client_max_interp_ratio->GetInt(),
		sv_max_usercmd_future_ticks = cvar_sv_max_usercmd_future_ticks->GetInt();

	if (sv_client_min_interp_ratio > cl_interp_ratio) cl_interp_ratio = sv_client_min_interp_ratio;
	if (cl_interp_ratio > sv_client_max_interp_ratio) cl_interp_ratio = sv_client_max_interp_ratio;
	if (sv_maxupdaterate <= cl_updaterate) cl_updaterate = sv_maxupdaterate;
	if (sv_minupdaterate > cl_updaterate) cl_updaterate = sv_minupdaterate;

	float interp = cl_interp_ratio / cl_updaterate;
	if (interp > cl_interp) cl_interp = interp;

	INetChannelInfo* nci = g_pEngineClient->GetNetChannelInfo();
	int predCmdArrivTick = g_TickCount + 1 + TIME_TO_TICKS(nci->GetAvgLatency(1) + nci->GetAvgLatency(0));
	int iLerpTicks = TIME_TO_TICKS(cl_interp);
	int iTargetTickCount = TIME_TO_TICKS(HistoryData[Index][0].m_SimulationTime) + iLerpTicks;
	float flCorrect = clamp89(cl_interp + nci->GetLatency(0), 0.f, 1.f) - TICKS_TO_TIME(predCmdArrivTick + TIME_TO_TICKS(cl_interp) - iTargetTickCount);
	if (fabs(flCorrect) > 0.2f) return;

	float delta = (HistoryData[Index][0].m_Origin - HistoryData[Index][1].m_Origin).Length2DSqr();

	int iter = 0;
	int tickrate = 1 / g_pGlobals->interval_per_tick;
	int maxtickcount = g_TickCount + sv_max_usercmd_future_ticks;

	float v57 = RAD2DEG(atan2(HistoryData[Entity->entindex()][0].m_Velocity.x, HistoryData[Entity->entindex()][0].m_Velocity.y));

	while (true)
	{
		if (sv_client_min_interp_ratio > cl_interp_ratio) cl_interp_ratio = sv_client_min_interp_ratio;
		if (cl_interp_ratio > sv_client_max_interp_ratio) cl_interp_ratio = sv_client_max_interp_ratio;
		if (sv_maxupdaterate <= cl_updaterate) cl_updaterate = sv_maxupdaterate;
		if (sv_minupdaterate > cl_updaterate) cl_updaterate = sv_minupdaterate;

		float interp = cl_interp_ratio / cl_updaterate;
		if (interp > cl_interp) cl_interp = interp;

		int v30 = (int)v57;
		int tick = TIME_TO_TICKS(v30 * tickrate) + TIME_TO_TICKS(cl_interp * tickrate);
		if (tick <= maxtickcount) break;
		++iter;

		cl_updaterate = cvar_cl_updaterate->GetInt();
		v30 = (int)v57 + 37;
		v57 += 37;

		if (iter >= 30) break;
	}

	g_Stuff.StoreTickRecord(Entity, &BackupData[Index]);
	ShouldRestoreBackup[Index] = true;

	if (g_CVars.Aimbot.Interpolation.LagPrediction == 2 && delta > 4096.f)
	{
		float simulationtimedelta = HistoryData[Entity->entindex()][0].m_SimulationTime - HistoryData[Entity->entindex()][1].m_SimulationTime;
		int simulationticksdelta = clamp89(TIME_TO_TICKS(HistoryData[Entity->entindex()][0].m_SimulationTime - HistoryData[Entity->entindex()][1].m_SimulationTime), 1, 15);
		int totaldelta = (TIME_TO_TICKS(simulationtimedelta) - simulationticksdelta);

		float movementvalue = GetAverageMovementVelocityDelta(&HistoryData[Entity->entindex()][1], &HistoryData[Entity->entindex()][0]);
		Vector backuporigin = HistoryData[Entity->entindex()][0].m_Origin;

		SimulationData[Entity->entindex()] = HistoryData[Entity->entindex()][0];

		if (totaldelta >= 0)
		{
			if (simulationticksdelta > 0)
			{
				for (int simulationticks = simulationticksdelta; simulationticks > 0; simulationticks--)
				{
					auto v50 = v57 + (movementvalue * g_pGlobals->interval_per_tick);
					SimulationData[Entity->entindex()].m_Velocity.x = cos(DEG2RAD(v50)) * SimulationData[Entity->entindex()].m_Velocity.Length2D();
					SimulationData[Entity->entindex()].m_Velocity.y = sin(DEG2RAD(v50)) * SimulationData[Entity->entindex()].m_Velocity.Length2D();
					SimulatePlayerMovement(Entity, &SimulationData[Entity->entindex()], !(Entity->m_fFlags() & FL_ONGROUND));

					v57 = v50;
				}

				HistoryData[Entity->entindex()][0].m_Origin = SimulationData[Entity->entindex()].m_Origin;
			}
		}
		else HistoryData[Entity->entindex()][0].m_Origin = backuporigin;
	}

	g_Stuff.ApplyTickRecord(Entity, &HistoryData[Index][0]);

	int m_iAccumulatedBoneMask = *(int*)((DWORD)Entity + 0x49C + 0x4);
	int m_nReadableBones = *(int*)((DWORD)Entity + 0x4A8 + 0x4);
	int m_nWritableBones = *(int*)((DWORD)Entity + 0x4AC + 0x4);
	int m_iPrevBoneMask = *(int*)((DWORD)Entity + 0x498 + 0x4);

	*(int*)((DWORD)Entity + 0x4A8 + 0x4) = 0;							// baseanimating + 0x4A8
	*(int*)((DWORD)Entity + 0x4AC + 0x4) = 0;							// baseanimating + 0x4AC
	*(int*)((DWORD)Entity + 0x498 + 0x4) = m_iAccumulatedBoneMask;		// baseanimating + 0x498
	*(int*)((DWORD)Entity + 0x49C + 0x4) = 0;							// baseanimating + 0x49C
}

void LagCompensation::RestoreBackup(BasePlayer* Entity)
{
	int Index = Entity->entindex();
	if (!ShouldRestoreBackup[Index]) return;
	g_Stuff.ApplyTickRecord(Entity, &BackupData[Index]);
	ShouldRestoreBackup[Index] = false;
}
