#include "Main.h"
#define clamp(val, min, max) (((val) > (max)) ? (max) : (((val) < (min)) ? (min) : (val)))

Aimbot g_Aimbot;

int GetPlayerModifiedDamage(const float& constdamage, bool isHeadshot, bool isFriendly, bool hasHelmet, CSWeapon* Weapon, BaseEntity* targetEntity)
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

float GetHitgroupModifiedDamage(float dmg, int hitgroup)
{
	static float hitgroupModifiers[] = { 1.f, 4.f, 1.f, 1.25f, 1.f, 1.f, .75f, .75f };
	return(dmg * hitgroupModifiers[hitgroup]);
}



int Aimbot::GetTotalDamage(BaseEntity* LocalPlayer, CSWeapon* Weapon, BaseEntity** ppPlayerHit)
{
	if (!LocalPlayer) return -1;

	trace_t traceData, wallTraceData;
	Ray_t ray;

	WeaponInfo WeaponInfo = g_NoSpread.GetWeaponInfo(Weapon);

	int currentPenetration = WeaponInfo.Penetration;
	float currentPenetrationPower = WeaponInfo.PenetrationPower;
	float currentDamage = (float)WeaponInfo.Damage;
	float currentMaxRange = WeaponInfo.MaxRange;

	Vector start = EyePosition, wall;
	Vector end;
	float tracedDistance = 0.f;
	float multiplier = 24.f;
	BaseEntity* skipPlayer = 0;
	BaseEntity* tmp = 0;
	TraceFilterSkipTwoEntities TraceFilter(LocalPlayer, skipPlayer);
	int totalDamage = 0;
	float wallThickness;
	bool isGrate;
	int material;

	float penetrationPowerModifier = 1.f;
	float damageModifier = .99f;

	Vector clip;
	static ConVar* mp_friendlyfire = g_pCvar->FindVar( /*mp_friendlyfire*/XorStr<0x7A, 16, 0x24CED9A4>("\x17\x0B\x23\x1B\x0C\x16\xE5\xEF\xE6\xEF\xFD\xE3\xEF\xF5\xED" + 0x24CED9A4).s);
	float tmpDistance;

	while (true)
	{
		end = (start + (vecDirection * currentMaxRange));
		ray.Init(start, end);
		g_pEngineTrace->TraceRay(ray, 0x4600400B, (ITraceFilter*)&TraceFilter, &traceData);
		clip = (end + (vecDirection * 40.0f));
		g_Stuff.ClipTraceToPlayers(start, clip, 0x4600400B, &TraceFilter, &traceData);
		tmp = traceData.m_pEnt;

		if (tmp && tmp->entindex() > 0 && tmp->entindex() <= g_pGlobals->maxClients)
			skipPlayer = tmp;
		else
			skipPlayer = 0;

		if (traceData.fraction == 1.f) break;

		tmpDistance = (tracedDistance + (currentMaxRange * traceData.fraction));
		currentDamage *= pow(WeaponInfo.RangeModifier, (tmpDistance * .002f));

		if (skipPlayer)
		{
			if ((g_CVars.Aimbot.FriendlyFire && (skipPlayer->m_iTeamNum() == LocalPlayer->m_iTeamNum())) || (skipPlayer->m_iTeamNum() != LocalPlayer->m_iTeamNum()))
			{
				totalDamage += GetPlayerModifiedDamage(GetHitgroupModifiedDamage(currentDamage, traceData.hitgroup),
					(traceData.hitgroup == 1),
					(skipPlayer->m_iTeamNum() == LocalPlayer->m_iTeamNum()),
					skipPlayer->m_bHasHelmet(),
					Weapon,
					skipPlayer);

				if (ppPlayerHit && !*ppPlayerHit) *ppPlayerHit = skipPlayer;
			}
		}

		isGrate = (traceData.contents & CONTENTS_GRATE);
		material = (int)g_pPhysicsSurfaceProps->GetSurfaceData(traceData.surface.surfaceProps)->game.material;

		if (!isGrate) Weapon->GetMaterialParameters(material, penetrationPowerModifier, damageModifier);
		if (tmpDistance > WeaponInfo.PenetrationRange) currentPenetration = (currentPenetration <= 0) ? currentPenetration : 0;
		if ((currentPenetration < 0) || ((currentPenetration == 0) && !isGrate)) break;

		while (true)
		{
			multiplier += 24.f;
			wall = (traceData.endpos + (vecDirection * multiplier));
			if (!(g_pEngineTrace->GetPointContents((traceData.endpos + (vecDirection * multiplier)), 0) & 0x200400B)) break;
			if (multiplier > 128.f)
			{
				multiplier = -1.f;
				break;
			}
		}

		if (multiplier == -1.f) break;

		ray.Init(wall, traceData.endpos);
		g_pEngineTrace->TraceRay(ray, 0x4600400B, 0, &wallTraceData);

		if (wallTraceData.m_pEnt && (wallTraceData.m_pEnt != traceData.m_pEnt))
		{
			g_Stuff.UTIL_TraceLine(wall, traceData.endpos, 0x4600400B, wallTraceData.m_pEnt, 0, &wallTraceData);
		}

		wallThickness = (wallTraceData.endpos - traceData.endpos).Length();

		if ((material == (int)g_pPhysicsSurfaceProps->GetSurfaceData(wallTraceData.surface.surfaceProps)->game.material) && ((material == 'W') || (material == 'M'))) penetrationPowerModifier += penetrationPowerModifier;
		if (wallThickness > (currentPenetrationPower * penetrationPowerModifier)) break;

		currentPenetrationPower -= (wallThickness / penetrationPowerModifier);
		tracedDistance = (tmpDistance + wallThickness);
		start = wallTraceData.endpos;
		currentMaxRange = ((currentMaxRange - tracedDistance) * .5f);
		currentDamage *= damageModifier;
		currentPenetration--;
	}

	if (totalDamage == 0) totalDamage = -1;
	return totalDamage;
}

bool Aimbot::CheckVisible(Vector& vecAbsStart, Vector& vecAbsEnd, BasePlayer* Target, BasePlayer* LocalPlayer)
{
	CSWeapon* Weapon = (CSWeapon*)LocalPlayer->GetActiveBaseCombatWeapon();
	if (!Weapon) return false;

	trace_t Trace;
	Ray_t Ray;
	TraceFilterSkipTwoEntities TraceFilter(Target, 0);

	Ray.Init(vecAbsStart, vecAbsEnd);
	g_pEngineTrace->TraceRay(Ray, 0x46004003, (ITraceFilter*)&TraceFilter, &Trace);
	return (Trace.fraction == 1.f);
}

bool Aimbot::CheckVisibleAWallCheck(Vector& vecAbsStart, Vector& vecAbsEnd, BasePlayer* Target, BasePlayer* LocalPlayer)
{
	CSWeapon* Weapon = (CSWeapon*)LocalPlayer->GetActiveBaseCombatWeapon();
	if (!Weapon) return false;

	trace_t Trace;
	Ray_t Ray;
	TraceFilterSkipTwoEntities traceFilter(Target, 0);

	if (g_CVars.Aimbot.Active)
	{
		BaseEntity* pPlayerHit = nullptr;
		if (GetTotalDamage(LocalPlayer, Weapon, &pPlayerHit) >= g_CVars.Aimbot.MinDamage) return true;
		else
		{
			Ray.Init(vecAbsStart, vecAbsEnd);
			g_pEngineTrace->TraceRay(Ray, 0x46004003, (ITraceFilter*)&traceFilter, &Trace);
			return (Trace.fraction == 1.f);
		}
	}
	else
	{
		Ray.Init(vecAbsStart, vecAbsEnd);
		g_pEngineTrace->TraceRay(Ray, 0x46004003, (ITraceFilter*)&traceFilter, &Trace);
		return (Trace.fraction == 1.f);
	}
}

matrix3x4_t ConstructMatrixFromOriginAndAngles(Vector origin, QAngle angles)
{
	matrix3x4_t matrix;
	Vector forward, left, up;

	AngleVectors(angles, &forward, &left, &up);

	matrix[0][0] = forward[0];
	matrix[1][0] = forward[1];
	matrix[2][0] = forward[2];

	matrix[0][1] = left[0];
	matrix[1][1] = left[1];
	matrix[2][1] = left[2];

	matrix[0][2] = up[0];
	matrix[1][2] = up[1];
	matrix[2][2] = up[2];

	matrix[0][3] = origin.x;
	matrix[1][3] = origin.y;
	matrix[2][3] = origin.z;

	return matrix;
}

void AdjustYawPosition(BasePlayer* Ent, Vector& adj, float addYaw)
{
	CTickRecord current = pPlayerHistory[Ent->entindex()][0];
	Vector pos(adj), tmp(current.m_Origin), vMin, vMax, vForward;
	QAngle ang, fix;
	float fApproxDist;

	matrix3x4_t matrix = ConstructMatrixFromOriginAndAngles(tmp, QAngle(0, current.m_EyeAngles.y, 0));
	VectorTransform(g_Aimbot.mins[Ent->entindex()], matrix, vMin);
	VectorTransform(g_Aimbot.maxs[Ent->entindex()], matrix, vMax);

	tmp = (vMax + vMin) * .5f;
	tmp.z = pos.z;

	VectorAngles(pos - tmp, ang);
	fApproxDist = tmp.DistTo(pos);

	fix = QAngle(0, g_Stuff.GuwopNormalize(ang.y + addYaw), 0);
	AngleVectors(fix, &vForward);
	tmp += vForward * fApproxDist;
	adj = tmp;
}

void Aimbot::GetHitbox(int iHitbox, BasePlayer* Entity)
{
	matrix3x4_t matrix[128];
	if (!(Entity->SetupBones(matrix, 128, 0x100, g_pGlobals->curtime))) return; // pPlayerHistory[ Entity->entindex( ) ][ 0 ].m_UpdateTime
	void* pModel = Entity->GetModel();
	if (!pModel) return;
	studiohdr_t* studiohdr = g_pModelInfo->GetStudiomodel(pModel);
	mstudiohitboxset_t* studiohitboxset = studiohdr->pHitboxSet(Entity->m_nHitboxSet());
	if (!studiohitboxset) return;
	mstudiobbox_t* studiobbox = studiohitboxset->pHitbox(iHitbox);
	if (!studiobbox) return;

	float pointscale = g_CVars.Aimbot.PointScale / 100;
	float scalecenter = g_pGlobals->interval_per_tick * pointscale;

	mins[Entity->entindex()] = studiobbox->bbmin;
	maxs[Entity->entindex()] = studiobbox->bbmax;

	Vector points[] = { ((studiobbox->bbmin + studiobbox->bbmax) * .5f),
		Vector(studiobbox->bbmin.x + (studiobbox->bbmax.x * (1 - pointscale) * .5f), studiobbox->bbmin.y + (studiobbox->bbmax.y * (1 - pointscale)), studiobbox->bbmin.z + (studiobbox->bbmax.z * (1 - pointscale))),
		Vector(studiobbox->bbmin.x + (studiobbox->bbmax.x * (1 - pointscale) * .5f), studiobbox->bbmax.y - (studiobbox->bbmax.y * (1 - pointscale)), studiobbox->bbmin.z + (studiobbox->bbmax.z * (1 - pointscale))),
		Vector(studiobbox->bbmax.x - (studiobbox->bbmax.x * (1 - pointscale) * .5f), studiobbox->bbmax.y - (studiobbox->bbmax.y * (1 - pointscale)), studiobbox->bbmin.z + (studiobbox->bbmax.z * (1 - pointscale))),
		Vector(studiobbox->bbmax.x - (studiobbox->bbmax.x * (1 - pointscale) * .5f), studiobbox->bbmin.y + (studiobbox->bbmax.y * (1 - pointscale)), studiobbox->bbmin.z + (studiobbox->bbmax.z * (1 - pointscale))),
		Vector(studiobbox->bbmax.x - (studiobbox->bbmax.x * (1 - pointscale) * .5f), studiobbox->bbmax.y - (studiobbox->bbmax.y * (1 - pointscale)), studiobbox->bbmax.z - (studiobbox->bbmax.z * (1 - pointscale))),
		Vector(studiobbox->bbmin.x + (studiobbox->bbmax.x * (1 - pointscale) * .5f), studiobbox->bbmax.y - (studiobbox->bbmax.y * (1 - pointscale)), studiobbox->bbmax.z - (studiobbox->bbmax.z * (1 - pointscale))),
		Vector(studiobbox->bbmin.x + (studiobbox->bbmax.x * (1 - pointscale) * .5f), studiobbox->bbmin.y + (studiobbox->bbmax.y * (1 - pointscale)), studiobbox->bbmax.z - (studiobbox->bbmax.z * (1 - pointscale))),
		Vector(studiobbox->bbmax.x - (studiobbox->bbmax.x * (1 - pointscale) * .5f), studiobbox->bbmin.y + (studiobbox->bbmax.y * (1 - pointscale)), studiobbox->bbmax.z - (studiobbox->bbmax.z * (1 - pointscale))) };

	float flPitch = Entity->m_angEyeAngles().x;

	if (iHitbox == 12)
	{
		if (g_CVars.Aimbot.HitboxMode == 0)
		{
			if (g_CVars.Aimbot.AutoHeightMode[Entity->entindex()] == 1)
			{
				if (Entity->m_vecVelocity().Length2D() < 40.f && !(Entity->m_fFlags() & FL_DUCKING))
				{
					Vector a = ((points[3] + points[5]) * .5f);

					if ((flPitch > 50.f) && (flPitch < 91.f))
					{
						Vector b = (((a - points[0]) / 3) * 4);
						Vector c = (points[0] + (b * .7f)); //( flPitch / 89.f ) ) );
						points[0] = c;
					}
					else if ((flPitch >= -91.f) && (flPitch <= -50.f)) points[0].z -= 1.f;
				}
				else
				{
					if ((flPitch > 50.f) && (flPitch < 91.f))
					{
						points[0].x = studiobbox->bbmin.x * .75f;
						points[0].y = studiobbox->bbmax.y * .75f;
						points[0].z = (studiobbox->bbmin.z + studiobbox->bbmax.z) * .5f;
					}
					else if ((flPitch >= -91.f) && (flPitch <= -50.f))
					{
						//points[ 0 ].x = studiobbox->bbmin.x * .75f;
						//points[ 0 ].y = studiobbox->bbmax.y * .75f;
						points[0].z -= 1.f;
					}
				}
			}
		}
		else
		{
			if (g_CVars.Aimbot.HitboxMode == 2 || g_CVars.Aimbot.HitboxMode == 3)
			{
				if ((flPitch > 50.f) && (flPitch < 91.f))
				{
					points[0].x = studiobbox->bbmin.x * .75f;
					points[0].y = studiobbox->bbmax.y * .75f;
					points[0].z = (studiobbox->bbmin.z + studiobbox->bbmax.z) * .5f;

					if (g_CVars.Aimbot.HitboxMode == 2) points[0] += Vector(0, .9f, .5f);
				}
				else if ((flPitch >= -91.f) && (flPitch <= -50.f))
				{
					//points[ 0 ].x = studiobbox->bbmin.x * .75f;
					//points[ 0 ].y = studiobbox->bbmax.y * .75f;
					points[0].z -= 1.f;
				}
			}
			else if (g_CVars.Aimbot.HitboxMode == 4)
			{
				Vector a = ((points[3] + points[5]) * .5f);

				if ((flPitch > 50.f) && (flPitch < 91.f))
				{
					Vector b = (((a - points[0]) / 3) * 4);
					Vector c = (points[0] + (b * .7f)); //( flPitch / 89.f ) ) );
					points[0] = c;
				}
				else if ((flPitch >= -91.f) && (flPitch <= -50.f)) points[0].z -= 1.f;
			}
		}
	}
	else points[0] += points[0] * .5f;

	for (int index = 0; index <= 8; ++index) VectorTransform(points[index], matrix[studiobbox->bone], vecCorners[index]);
}
bool BoneFilter(int bone)
{
	if (bone > 18 && bone < 28) return false;
	else if (bone > 31 && bone < 41) return false;
	else return true;
}

bool Aimbot::GetBone(int iBone, BasePlayer* Target, Vector& vBonePos)
{
	static matrix3x4_t matrix[128];

	if (!Target->SetupBones(matrix, 128, 0x100, Target->m_flSimulationTime())) return false;

	vBonePos.x = matrix[iBone][0][3];
	vBonePos.y = matrix[iBone][1][3];
	vBonePos.z = matrix[iBone][2][3];

	return true;
}
void Aimbot::Reset( )
{
	Distance = INT_MAX;
	Temp = INT_MAX;
	TargetIndex = -1;
	vecDirection.Init( 0, 0, 0 );
	for( int i = 0; i <= 8; ++i ) vecCorners[ i ].Init( 0, 0, 0 );
	IsAimbotting = false;
	qFinalAngle = QAngle( 0, 0, 0 );
}

int next_shot;
int Rate( BasePlayer* LocalPlayer, BasePlayer* Ent )
{
	int rate = 0;

	if( g_CVars.Aimbot.TargetSelection == 0 ) rate = LocalPlayer->GetAbsOrigin( ).DistTo( Ent->GetAbsOrigin( ) ); // distance
	if( g_CVars.Aimbot.TargetSelection == 1 ) rate = Ent->m_iHealth( ); // health
	if( g_CVars.Aimbot.TargetSelection == 2 || g_CVars.Aimbot.TargetSelection == 3 ) rate = Ent->entindex( ) > next_shot ? 0 : 1; // next shot, random

	return rate;
}
int GetBestRecord(BasePlayer* Entity)
{
	float diff, v8, best_diff = 1000.f;
	int tick = -1;

	for (int i = 31; i >= 0; --i)
	{
		diff = abs(pPlayerHistory[Entity->entindex()][i].m_SimulationTime); //- m_LastBodyYawUpdate[ Entity->entindex( ) ] );

		if (best_diff > diff)
		{
			best_diff = diff;
			tick = i;
		}
	}

	if (tick == -1) return 0;

	static ConVar* cvar_cl_interp = g_pCvar->FindVar( /*cl_interp*/XorStr<0x24, 10, 0x7F9B18B0>("\x47\x49\x79\x4E\x46\x5D\x4F\x59\x5C" + 0x7F9B18B0).s);
	static ConVar* cvar_cl_updaterate = g_pCvar->FindVar( /*cl_updaterate*/XorStr<0xC6, 14, 0xD9FFF99F>("\xA5\xAB\x97\xBC\xBA\xAF\xAD\xB9\xAB\xBD\xB1\xA5\xB7" + 0xD9FFF99F).s);
	static ConVar* cvar_cl_interp_ratio = g_pCvar->FindVar( /*cl_interp_ratio*/XorStr<0xBF, 16, 0x298E884B>("\xDC\xAC\x9E\xAB\xAD\xB0\xA0\xB4\xB7\x97\xBB\xAB\xBF\xA5\xA2" + 0x298E884B).s);
	static ConVar* cvar_sv_minupdaterate = g_pCvar->FindVar( /*sv_minupdaterate*/XorStr<0xF6, 17, 0x99ECD573>("\x85\x81\xA7\x94\x93\x95\x89\x8D\x9A\x9E\x74\x64\x70\x62\x70\x60" + 0x99ECD573).s);
	static ConVar* cvar_sv_maxupdaterate = g_pCvar->FindVar( /*sv_maxupdaterate*/XorStr<0x6A, 17, 0x6C62999F>("\x19\x1D\x33\x00\x0F\x17\x05\x01\x16\x12\x00\x10\x04\x16\x0C\x1C" + 0x6C62999F).s);
	static ConVar* cvar_sv_client_min_interp_ratio = g_pCvar->FindVar( /*sv_client_min_interp_ratio*/XorStr<0x9B, 27, 0x783554E3>("\xE8\xEA\xC2\xFD\xF3\xC9\xC4\xCC\xD7\xFB\xC8\xCF\xC9\xF7\xC0\xC4\xDF\xC9\xDF\xDE\xF0\xC2\xD0\xC6\xDA\xDB" + 0x783554E3).s);
	static ConVar* cvar_sv_client_max_interp_ratio = g_pCvar->FindVar( /*sv_client_max_interp_ratio*/XorStr<0xAF, 27, 0xED44D950>("\xDC\xC6\xEE\xD1\xDF\xDD\xD0\xD8\xC3\xE7\xD4\xDB\xC3\xE3\xD4\xD0\xCB\xA5\xB3\xB2\x9C\xB6\xA4\xB2\xAE\xA7" + 0xED44D950).s);

	float cl_interp = cvar_cl_interp->GetFloat();
	int cl_updaterate = cvar_cl_updaterate->GetInt(),
		sv_maxupdaterate = cvar_sv_maxupdaterate->GetInt(),
		sv_minupdaterate = cvar_sv_minupdaterate->GetInt(),
		cl_interp_ratio = cvar_cl_interp_ratio->GetInt(),
		sv_client_min_interp_ratio = cvar_sv_client_min_interp_ratio->GetInt(),
		sv_client_max_interp_ratio = cvar_sv_client_max_interp_ratio->GetInt();

	if (sv_client_min_interp_ratio > cl_interp_ratio) cl_interp_ratio = sv_client_min_interp_ratio;
	if (cl_interp_ratio > sv_client_max_interp_ratio) cl_interp_ratio = sv_client_max_interp_ratio;
	if (sv_maxupdaterate <= cl_updaterate) cl_updaterate = sv_maxupdaterate;
	if (sv_minupdaterate > cl_updaterate) cl_updaterate = sv_minupdaterate;

	float interp = cl_interp_ratio / cl_updaterate;
	if (interp > cl_interp) cl_interp = interp;

	v8 = pPlayerHistory[Entity->entindex()][0].m_SimulationTime + cl_interp - pPlayerHistory[Entity->entindex()][tick].m_SimulationTime;
	if (abs(v8) > 0.15f) return 0;

	return tick;
}
float LerpTime()
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
	if (cvar_sv_client_min_interp_ratio && cvar_sv_client_max_interp_ratio && sv_client_min_interp_ratio != 1.f) cl_interp_ratio = clamp(cl_interp_ratio, sv_client_min_interp_ratio, sv_client_max_interp_ratio);

	return max(lerp, (cl_interp_ratio / cl_updaterate));
}
int ServerTick()
{
	float out, in;
	int servertick;

	in = g_pEngineClient->GetNetChannelInfo()->GetLatency(1);
	out = g_pEngineClient->GetNetChannelInfo()->GetLatency(0);

	servertick = g_pGlobals->tickcount + 1 + TIME_TO_TICKS(in + out);

	return servertick;
}

bool CheckDelta(CTickRecord* record)
{
	float out, dt, correct;

	out = g_pEngineClient->GetNetChannelInfo()->GetLatency(0);

	correct = out + TICKS_TO_TIME(TIME_TO_TICKS(LerpTime()));
	correct = clamp(correct, 0.f, 1.f);

	/*
	auto PredictedCmdArrivalTick = cmdTickcount + 1 + TIME_TO_TICKS(netChannelInfo->GetAvgLatency(FLOW_INCOMING) + netChannelInfo->GetAvgLatency(FLOW_OUTGOING));
	auto Correct = clamp(lerpTime + netChannelInfo->GetLatency(FLOW_OUTGOING), 0.f, 1.f) - TICKS_TO_TIME(PredictedCmdArrivalTick + TIME_TO_TICKS(lerpTime) - GetTickcountForEntity(tickrecord.m_SimulationTime));
	*/

	int entitytick = TIME_TO_TICKS(record->m_SimulationTime) + TIME_TO_TICKS(LerpTime());
	dt = correct - TICKS_TO_TIME(ServerTick() + TIME_TO_TICKS(LerpTime()) - entitytick);

	return (fabs(dt) <= 0.2f);
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
void Aimbot::Main(CUserCmd* pCmd, BasePlayer* LocalPlayer)
{
	CSWeapon* Weapon = (CSWeapon*)LocalPlayer->GetActiveBaseCombatWeapon();
	if (!Weapon || !Weapon->IsWeapon()) return;

	WeaponInfo wpnInfo = g_NoSpread.GetWeaponInfo(Weapon);

	Reset();

	static int iSpot;
	static int Choose[] = { 12, 11, 5, 0, 1, 9, 10, 13, 14, 16, 17, 18, 8, 7, 6, 4, 3, 2, 15 };
	static int limbs[] = { iSpot, HITBOX_L_CALF, HITBOX_L_FOOT, HITBOX_L_FOREARM, HITBOX_L_HAND, HITBOX_L_THIGH, HITBOX_L_UPPERARM, HITBOX_R_CALF, HITBOX_R_FOOT, HITBOX_R_FOREARM, HITBOX_R_HAND, HITBOX_R_THIGH, HITBOX_R_UPPERARM };
	static int centralbody[] = { iSpot, HITBOX_CHEST, HITBOX_PELVIS, HITBOX_STOMACH };
	static int fullhitboxes[] = { HITBOX_HEAD, HITBOX_NECK, HITBOX_CHEST, HITBOX_STOMACH, HITBOX_R_TOE0, HITBOX_L_UPPERARM, HITBOX_R_UPPERARM, HITBOX_L_THIGH, HITBOX_R_THIGH, HITBOX_L_HAND, HITBOX_R_HAND, HITBOX_L_FOOT, HITBOX_R_FOOT, HITBOX_L_FOREARM, HITBOX_R_FOREARM, HITBOX_L_CALF, HITBOX_R_CALF, HITBOX_PELVIS };



	int m_iWeaponID = Weapon->GetWeaponID();


	if (Weapon->GetWeaponID() == 17 && g_CVars.Aimbot.BodyAWP)
	{
		if (g_CVars.Aimbot.BodyAWP) iSpot = HITBOX_CHEST;
		else iSpot = g_CVars.Aimbot.Hitbox;
	}
	else iSpot = g_CVars.Aimbot.Hitbox;

	Choose[0] = iSpot;

	for (int i = 18; i >= 1; i--)
	{
		if (Choose[i] == iSpot) Choose[i] = g_CVars.Aimbot.Hitbox;
	}

	if (g_CVars.Aimbot.Key > 0)
	{
		if (!GetAsyncKeyState(g_CVars.Aimbot.Key)) return;
	}

	for (auto i = g_pGlobals->maxClients; i >= 1; --i)
	{
		if (i == g_pEngineClient->GetLocalPlayer()) continue;

		BasePlayer* Ent = (BasePlayer*)g_pClientEntityList->GetClientEntity(i);
		if (Ent == 0) continue;
		if (Ent->IsDormant()) continue;
		if (Ent->m_lifeState() != 0) continue;
		if (g_CVars.Aimbot.baiminair)
		{
			if (~Ent->m_fFlags() & FL_ONGROUND)
			{
				iSpot = HITBOX_CHEST;
			}
			else iSpot = g_CVars.Aimbot.Hitbox;

		}
		else iSpot = g_CVars.Aimbot.Hitbox;

		if (g_CVars.Aimbot.HitboxOwns == 1)
		{
			iSpot = limbs[11];
		}
		else if (g_CVars.Aimbot.HitboxOwns == 2)
		{
			iSpot = centralbody[2];
		}
		else if (g_CVars.Aimbot.HitboxOwns == 3)
		{
			iSpot = fullhitboxes[17];
		}
		else iSpot = 12;

		GetAsyncKeyState(g_CVars.Aimbot.mindmgpressedvalue) ? g_CVars.Aimbot.mindmgpressed : g_CVars.Aimbot.MinDamage;

		if (g_CVars.Aimbot.baimincrouch)
		{
			if (Ent->m_fFlags() & FL_DUCKING)
			{
				iSpot = 9;
			}
			else iSpot = 12;

		}
		else iSpot = 12;

		if (g_CVars.Aimbot.baimafterhp)
		{
			if (Ent->m_iHealth() <= g_CVars.Aimbot.baimafterhpslider) iSpot = 9;
			else iSpot = 12;
		}
		else iSpot = 12;

		if ( g_CVars.Aimbot.BodyAim && GetAsyncKeyState( g_CVars.Aimbot.BodyAimValue ) )
		{
			iSpot = 9;
		}
		else iSpot = 12;
		/*
		if (true)
		{
			if (Ent->m_iHealth() <= 78) iSpot = 10;
			else iSpot = g_CVars.Aimbot.Hitbox;
			if (g_iBulletsFired[Ent->entindex()] >= 5)
			{
				iSpot = 10;
			}
			else iSpot = g_CVars.Aimbot.Hitbox;
			if (animstate->m_bJumping)
			{
				iSpot = 10;
			}
			else iSpot = g_CVars.Aimbot.Hitbox;
		}
		*/
		if (!g_CVars.Aimbot.FriendlyFire)
		{
			if (Ent->m_iTeamNum() == LocalPlayer->m_iTeamNum()) continue;
		}

		if (Ent->m_iHealth() > 500) continue;
		if (Ent->IsSpawnProtectedPlayer()) continue;
		if (g_CVars.PlayerList.Friend[i]) continue;
		if (Ent->m_vecOrigin().DistTo(EyePosition) > wpnInfo.MaxRange) continue;
		if (g_Whitelist.List(i)) iSpot = HITBOX_HEAD;
		int rate = Rate(LocalPlayer, Ent);
		if (rate > Temp) continue;

		if (Weapon->GetWeaponID() == 17) // awp
		{
			GetHitbox(iSpot, Ent);

			if (CheckVisible(EyePosition, vecCorners[0], Ent, LocalPlayer))
			{
				VectorSubtract(vecCorners[0], EyePosition, vecDirection);
				VectorNormalizeFast(vecDirection);

				if (g_Stuff.IsReadyToShoot(LocalPlayer, Weapon))
				{
					VectorAngles(vecDirection, pCmd->viewangles);
					IsAimbotting = true;
				}

				TargetIndex = i;
				Temp = rate;
			}
		}
		else
		{
			int size = (g_CVars.Aimbot.HitScan == 0) ? 0 : ((g_CVars.Aimbot.HitScan == 1) ? 4 : ((g_CVars.Aimbot.HitScan == 2) ? 8 : 17));
			for (int m_iHitbox = size; m_iHitbox >= 0; m_iHitbox--)
			{
				int hitbox = (g_CVars.Aimbot.HitScan == 0) ? iSpot : fullhitboxes[m_iHitbox];
				GetHitbox(hitbox, Ent);

				if ((g_CVars.Aimbot.MultiSpot == 1 && hitbox == iSpot) || g_CVars.Aimbot.MultiSpot == 2)
				{
					for (int m_iCorners = 8; m_iCorners > 0; m_iCorners--)
					{
						VectorSubtract(vecCorners[m_iCorners], EyePosition, vecDirection);
						VectorNormalizeFast(vecDirection);

						if (CheckVisibleAWallCheck(EyePosition, vecCorners[m_iCorners], Ent, LocalPlayer))
						{
							if (g_Stuff.IsReadyToShoot(LocalPlayer, Weapon))
							{
								VectorAngles(vecDirection, pCmd->viewangles);
								IsAimbotting = true;
							}

							TargetIndex = i;
							Temp = rate;
						}
					}
				}

				VectorSubtract(vecCorners[0], EyePosition, vecDirection);
				VectorNormalizeFast(vecDirection);

				if (CheckVisibleAWallCheck(EyePosition, vecCorners[0], Ent, LocalPlayer))
				{
					if (g_Stuff.IsReadyToShoot(LocalPlayer, Weapon))
					{
						VectorAngles(vecDirection, pCmd->viewangles);
						IsAimbotting = true;
					}

					TargetIndex = i;
					Temp = rate;
				}
			}
			for (int m_iHitbox = size; m_iHitbox >= 0; m_iHitbox--)
			{
				int hitbox = (g_CVars.Aimbot.HitboxOwns == 1) ? iSpot : fullhitboxes[m_iHitbox];
				GetHitbox(hitbox, Ent);

				if ((g_CVars.Aimbot.HitboxOwns == 1 && hitbox == iSpot))
				{
					for (int m_iCorners = 8; m_iCorners > 0; m_iCorners--)
					{
						VectorSubtract(vecCorners[m_iCorners], EyePosition, vecDirection);
						VectorNormalizeFast(vecDirection);

						if (CheckVisibleAWallCheck(EyePosition, vecCorners[m_iCorners], Ent, LocalPlayer))
						{
							if (g_Stuff.IsReadyToShoot(LocalPlayer, Weapon))
							{
								VectorAngles(vecDirection, pCmd->viewangles);
								IsAimbotting = true;
							}

							TargetIndex = i;
							Temp = rate;
						}
					}
				}

				VectorSubtract(vecCorners[0], EyePosition, vecDirection);
				VectorNormalizeFast(vecDirection);

				if (CheckVisibleAWallCheck(EyePosition, vecCorners[0], Ent, LocalPlayer))
				{
					if (g_Stuff.IsReadyToShoot(LocalPlayer, Weapon))
					{
						VectorAngles(vecDirection, pCmd->viewangles);
						IsAimbotting = true;
					}

					TargetIndex = i;
					Temp = rate;
				}
			}
			for (int m_iHitbox = size; m_iHitbox >= 0; m_iHitbox--)
			{
				int hitbox = (g_CVars.Aimbot.HitboxOwns == 2) ? iSpot : fullhitboxes[m_iHitbox];
				GetHitbox(hitbox, Ent);

				if ((g_CVars.Aimbot.HitboxOwns == 2 && hitbox == iSpot))
				{
					for (int m_iCorners = 8; m_iCorners > 0; m_iCorners--)
					{
						VectorSubtract(vecCorners[m_iCorners], EyePosition, vecDirection);
						VectorNormalizeFast(vecDirection);

						if (CheckVisibleAWallCheck(EyePosition, vecCorners[m_iCorners], Ent, LocalPlayer))
						{
							if (g_Stuff.IsReadyToShoot(LocalPlayer, Weapon))
							{
								VectorAngles(vecDirection, pCmd->viewangles);
								IsAimbotting = true;
							}

							TargetIndex = i;
							Temp = rate;
						}
					}
				}

				VectorSubtract(vecCorners[0], EyePosition, vecDirection);
				VectorNormalizeFast(vecDirection);

				if (CheckVisibleAWallCheck(EyePosition, vecCorners[0], Ent, LocalPlayer))
				{
					if (g_Stuff.IsReadyToShoot(LocalPlayer, Weapon))
					{
						VectorAngles(vecDirection, pCmd->viewangles);
						IsAimbotting = true;
					}

					TargetIndex = i;
					Temp = rate;
				}
			}
			for (int m_iHitbox = size; m_iHitbox >= 0; m_iHitbox--)
			{
				int hitbox = (g_CVars.Aimbot.HitboxOwns == 3) ? iSpot : fullhitboxes[m_iHitbox];
				GetHitbox(hitbox, Ent);

				if ((g_CVars.Aimbot.HitboxOwns == 3 && hitbox == iSpot))
				{
					for (int m_iCorners = 8; m_iCorners > 0; m_iCorners--)
					{
						VectorSubtract(vecCorners[m_iCorners], EyePosition, vecDirection);
						VectorNormalizeFast(vecDirection);

						if (CheckVisibleAWallCheck(EyePosition, vecCorners[m_iCorners], Ent, LocalPlayer))
						{
							if (g_Stuff.IsReadyToShoot(LocalPlayer, Weapon))
							{
								VectorAngles(vecDirection, pCmd->viewangles);
								IsAimbotting = true;
							}

							TargetIndex = i;
							Temp = rate;
						}
					}
				}

				VectorSubtract(vecCorners[0], EyePosition, vecDirection);
				VectorNormalizeFast(vecDirection);

				if (CheckVisibleAWallCheck(EyePosition, vecCorners[0], Ent, LocalPlayer))
				{
					if (g_Stuff.IsReadyToShoot(LocalPlayer, Weapon))
					{
						VectorAngles(vecDirection, pCmd->viewangles);
						IsAimbotting = true;
					}

					TargetIndex = i;
					Temp = rate;
				}
			}
		}
		if (IsAimbotting && TargetIndex != -1)
		{
			if (g_CVars.Aimbot.AutoShoot) pCmd->buttons |= IN_ATTACK;
		}

		if (g_CVars.Aimbot.TargetSelection == 3) next_shot = TargetIndex;


		if (IsAimbotting)
		{
			if (g_CVars.Aimbot.TargetSelection != 3) next_shot = TargetIndex;
		}
		else if (Weapon->ShouldReload()) pCmd->buttons |= IN_RELOAD;

		if (!g_CVars.Aimbot.Silent) g_pEngineClient->SetViewAngles(pCmd->viewangles);

		// force baim
		CTickRecord* backup = &pBackupData[i];
		//	CTickRecord* previous = &pPlayerHistory[i][GetBestRecord(Ent) + 1];



		if (g_CVars.Aimbot.BodyAim && GetAsyncKeyState(g_CVars.Aimbot.BodyAimValue))
		{
			iSpot = 9;
		}
		else
		{
			iSpot = 12;
		}

	}
}