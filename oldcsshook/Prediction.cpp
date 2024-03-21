#include "Main.h"
#include "detours.h"

Prediction g_Prediction;

static bool update = true;
void Prediction::Start(CUserCmd* pCmd, BasePlayer* LocalPlayer)
{
	CSWeapon* Weapon = (CSWeapon*)LocalPlayer->GetActiveBaseCombatWeapon();

	m_nTickBase = LocalPlayer->m_nTickBase();

	m_nOldTickBase = m_nTickBase;

	OldCurtime = g_pGlobals->curtime;
	OldFrametime = g_pGlobals->frametime;

	g_pGlobals->curtime = g_pGlobals->interval_per_tick * m_nTickBase;
	g_pGlobals->frametime = g_pGlobals->interval_per_tick;

	// m_pCurrentCommand
	*(DWORD*)((DWORD)LocalPlayer + 0xD60) = (DWORD)pCmd;

	// SetPredictionRandomSeed
	*(int*)((DWORD)BASE_CLIENT + 0x38FF74) = pCmd->random_seed;

	// SetPredictionPlayer
	*(BasePlayer**)((DWORD)BASE_CLIENT + 0x3B2EF8) = LocalPlayer;

	if (pCmd->weaponselect != 0)
	{
		CSWeapon* selectedweapon = dynamic_cast<CSWeapon*>(g_pClientEntityList->GetClientEntity(pCmd->weaponselect));
		if (selectedweapon) (*(void(__thiscall**)(const char*, int))((DWORD)LocalPlayer + 0x34C))(selectedweapon->GetWeaponAlias(), pCmd->weaponsubtype);
	}

	if (Weapon)
	{
		int WeaponID = Weapon->GetWeaponID();
		if (WeaponID == WEAPON_GLOCK)
		{
			if (Weapon->SpecialMode())
			{
				if (Weapon->m_flGlock18Shoot() > 0.f) update = false;
			}
		}
		else if (WeaponID == WEAPON_FAMAS)
		{
			if (Weapon->SpecialMode())
			{
				if (Weapon->m_flFamasShoot() > 0.f) update = false;
			}
		}
	}

	//if( update )
	typedef bool(__thiscall* PhysicsRunThink_t)(void*, BaseEntity::thinkmethods_t);
	if (((PhysicsRunThink_t)((DWORD)BASE_CLIENT + 0x1180E0))(LocalPlayer, BaseEntity::thinkmethods_t::THINK_FIRE_ALL_FUNCTIONS))
	{
		// ItemPreFrame
		//( *( void( __thiscall** )( void* ) )( ( DWORD ) LocalPlayer + 0x324 ) )( LocalPlayer );
	}

	typedef int(__thiscall* GetNextThinkTick)(void*, const char*);
	int valid = ((GetNextThinkTick)(((DWORD)BASE_CLIENT + 0x14DB0)))(LocalPlayer, 0);
	if (valid > 0 && valid <= m_nTickBase)
	{

		// SetNextThinkTick
		typedef void(__thiscall* SetNextThinkTick)(void*, float, const char*);
		((SetNextThinkTick)(((DWORD)BASE_CLIENT + 0x16030)))(LocalPlayer, -1, 0);
		(*(void(__thiscall**)(void*))((DWORD)LocalPlayer + 0x1B0))(LocalPlayer);
	}

	CMoveData Data;
	memset(&Data, 0, sizeof(Data));
	g_pPrediction->SetupMove(LocalPlayer, pCmd, Move, &Data);
	g_pGameMovement->ProcessMovement(LocalPlayer, &Data);
	g_pPrediction->FinishMove(LocalPlayer, pCmd, &Data);

	LocalPlayer->GetAbsVelocity() = (LocalPlayer->m_vecBaseVelocity() + LocalPlayer->m_vecVelocity());
}
int ServerTick1()
{
	float out, in;
	int servertick;

	in = g_pEngineClient->GetNetChannelInfo()->GetLatency(1);
	out = g_pEngineClient->GetNetChannelInfo()->GetLatency(0);

	servertick = g_pGlobals->tickcount + 1 + TIME_TO_TICKS(in + out);

	return servertick;
}
void Prediction::End(CUserCmd* pCmd, BasePlayer* LocalPlayer)
{
	// ItemPostFrame
	//( *( void( __thiscall** )( BasePlayer* ) )( ( DWORD ) LocalPlayer + 0x328 ) )( LocalPlayer );

	// m_pCurrentCommand
	*(DWORD*)((DWORD)LocalPlayer + 0xD60) = 0;

	// SetPredictionRandomSeed
	*(int*)((DWORD)BASE_CLIENT + 0x38FF74) = -1;

	// SetPredictionPlayer
	*(BasePlayer**)((DWORD)BASE_CLIENT + 0x3B2EF8) = 0;

	if ((g_CVars.Miscellaneous.Speedhack && !GetAsyncKeyState(0x45)) || !g_CVars.Miscellaneous.Speedhack)
	{
		if (m_nTickBase == m_nOldTickBase)
		{
			++PushTickBase;
			m_nTickBase += PushTickBase;
		}
		else PushTickBase = 0;

		if (m_nTickBase > ServerTick1()) m_nTickBase = ServerTick1();
		LocalPlayer->SetTickBase(m_nTickBase);
	}

	pLastCmd = pCmd;
}

void Prediction::PredictOrigin(Vector& Origin, BasePlayer* Entity)
{
	Origin -= Entity->GetAbsOrigin();
	Origin += Entity->m_vecOrigin() + Entity->m_vecVelocity() * g_pEngineClient->GetNetChannelInfo()->GetPing();
}

struct player
{
	int index;
	Vector data[128];
}player_s[64];


void Prediction::UpdatePlayerHistory(BasePlayer* Player)
{
	if (g_CVars.Miscellaneous.OriginCorrection)
	{
		for (int i = 1; i <= g_pGlobals->maxClients; i++)
		{
			if (!Player) return;
			if (i == g_pEngineClient->GetLocalPlayer()) return;
			int tick = TIME_TO_TICKS(Player->m_flSimulationTime());
			player_s[i].data[tick % HISTORY_MAX] = Player->GetAbsOrigin();
		}
	}
}

bool Validate(const float& stored, const float& current)
{
	return (abs(stored - current) < .5f);
}

bool Validate(const Vector& stored, const Vector& current)
{
	return ((abs(stored.x - current.x) < .5f) && (abs(stored.y - current.y) < .5f) && (abs(stored.z - current.z) < .5f));
}

bool Validate(const QAngle& stored, const QAngle& current)
{
	return ((abs(stored.x - current.x) < .5f) && (abs(stored.y - current.y) < .5f) && (abs(stored.z - current.z) < .5f));
}

void Prediction::SetVars(BasePlayer* LocalPlayer, int TickBase)
{
	if (Validate(vars[TickBase % MULTIPLAYER_BACKUP].m_BaseVelocity, LocalPlayer->m_vecBaseVelocity())) LocalPlayer->m_vecBaseVelocity() = vars[TickBase % MULTIPLAYER_BACKUP].m_BaseVelocity;
	if (Validate(vars[TickBase % MULTIPLAYER_BACKUP].m_ViewOffset, LocalPlayer->m_vecViewOffset())) LocalPlayer->m_vecViewOffset() = vars[TickBase % MULTIPLAYER_BACKUP].m_ViewOffset;
	if (Validate(vars[TickBase % MULTIPLAYER_BACKUP].m_AbsVelocity, LocalPlayer->GetAbsVelocity())) LocalPlayer->GetAbsVelocity() = vars[TickBase % MULTIPLAYER_BACKUP].m_AbsVelocity;
	if (Validate(vars[TickBase % MULTIPLAYER_BACKUP].m_Velocity, LocalPlayer->m_vecVelocity())) LocalPlayer->m_vecVelocity() = vars[TickBase % MULTIPLAYER_BACKUP].m_Velocity;
	if (Validate(vars[TickBase % MULTIPLAYER_BACKUP].m_PunchAngle, LocalPlayer->GetPunchAngle())) LocalPlayer->GetPunchAngle() = vars[TickBase % MULTIPLAYER_BACKUP].m_PunchAngle;
	if (Validate(vars[TickBase % MULTIPLAYER_BACKUP].m_FallVelocity, LocalPlayer->m_flFallVelocity())) LocalPlayer->m_flFallVelocity() = vars[TickBase % MULTIPLAYER_BACKUP].m_FallVelocity;
}

void Prediction::GetVars(BasePlayer* LocalPlayer, int TickBase)
{
	vars[TickBase % MULTIPLAYER_BACKUP].m_BaseVelocity = LocalPlayer->m_vecBaseVelocity();
	vars[TickBase % MULTIPLAYER_BACKUP].m_ViewOffset = LocalPlayer->m_vecViewOffset();
	vars[TickBase % MULTIPLAYER_BACKUP].m_AbsVelocity = LocalPlayer->GetAbsVelocity();
	vars[TickBase % MULTIPLAYER_BACKUP].m_Velocity = LocalPlayer->m_vecVelocity();
	vars[TickBase % MULTIPLAYER_BACKUP].m_PunchAngle = LocalPlayer->GetPunchAngle();
	vars[TickBase % MULTIPLAYER_BACKUP].m_FallVelocity = LocalPlayer->m_flFallVelocity();
}

void RunSimulation(CUserCmd* pCmd, BasePlayer* LocalPlayer)
{
	g_pGlobals->curtime = LocalPlayer->m_nTickBase() * g_pGlobals->interval_per_tick;
	g_pGlobals->frametime = g_pGlobals->interval_per_tick;

	typedef void(__thiscall* RunSimulationFn)(void*, int, float, CUserCmd*, BasePlayer*);
	((RunSimulationFn)((DWORD)BASE_CLIENT + 0x12B650))(g_pPrediction, pCmd->command_number, g_pGlobals->curtime, pCmd, LocalPlayer);
}

void* Original_Run_Simulation_Caller_Location;

void Redirected_Run_Simulation(void* Unknown_Parameter_1, void* Unknown_Parameter_2, void* Unknown_Parameter_3, void* Unknown_Parameter_4, void* Player)
{
	(decltype(&Redirected_Run_Simulation)(Original_Run_Simulation_Caller_Location))(Unknown_Parameter_1, Unknown_Parameter_2, Unknown_Parameter_3, Unknown_Parameter_4, Player);

	using Set_Collision_Bounds_Type = void(__thiscall*)(void* Collision_Property, float* Minimum, float* Maximum);

	__int8 In_Crouch = (*(__int32*)((unsigned __int32)Player + 692) & 2) == 2;

	Set_Collision_Bounds_Type(604660272)((void*)((unsigned __int32)Player + 352), (float*)((unsigned __int32)608155796 + 24 * In_Crouch), (float*)((unsigned __int32)608155808 + 24 * In_Crouch));
}

typedef void(*CL_RunPrediction_t)(void);
CL_RunPrediction_t _CL_RunPrediction;

typedef void(*CL_PreprocessEntities_t)(void);
CL_PreprocessEntities_t _CL_PreprocessEntities;

DWORD ply_speed_addy = (DWORD)((uintptr_t)BASE_ENGINE + 0x4D2420);

float* ply_speed = (float*)ply_speed_addy;
float i = 0.01;
float x = 0.00001;
float ply_speed_orig = *ply_speed;

void Hooked_CL_RunPrediction()
{
	/*
	if( GetAsyncKeyState( VK_DELETE ) )
	{
		i += 0.0001;
		x += 0.00000001;

		if( *ply_speed )
		{
			*ply_speed = ( i + x );

			if( *ply_speed >= 0.025 )
			{
				*ply_speed = 0.01;
				i = 0.01;
				x = 0.01;
			}
		}
	}
	else *ply_speed = ply_speed_orig;
	*/

	//*( int* )( ( DWORD ) BASE_ENGINE + 0x367060 ) -= 2;

	if (*(int*)((DWORD)BASE_ENGINE + 0x390AA8) != 6) return; // m_nSignonState != SIGNONSTATE_FULL
	if (*(int*)((DWORD)BASE_ENGINE + 0x390B0C) < 0) return; // bool valid = m_nDeltaTick > 0

	g_pPrediction->Update(*(int*)((DWORD)BASE_ENGINE + 0x390B0C), // m_nDeltaTick
		*(int*)((DWORD)BASE_ENGINE + 0x390B0C) > 0, // m_nDeltaTick > 0
		*(int*)((DWORD)BASE_ENGINE + 0x3953A4), // last_command_ack
		*(int*)((DWORD)BASE_ENGINE + 0x39539C) + *(int*)((DWORD)BASE_ENGINE + 0x3953A0)); // lastoutgoingcommand + chokedcommands
}

void Hooked_CL_PreprocessEntities()
{
	g_pPrediction->Update(*(int*)((DWORD)BASE_ENGINE + 0x390B0C), // m_nDeltaTick
		*(int*)((DWORD)BASE_ENGINE + 0x390B0C) > 0, // m_nDeltaTick > 0
		*(int*)((DWORD)BASE_ENGINE + 0x3953A4), // last_command_ack
		*(int*)((DWORD)BASE_ENGINE + 0x39539C) + *(int*)((DWORD)BASE_ENGINE + 0x3953A0)); // lastoutgoingcommand + chokedcommands

	//*( int* ) OffModules->E_Unknown = *( int* ) OffModules->E_Unknown2;

	g_pPrediction->PreEntityPacketReceived(*(int*)((DWORD)BASE_ENGINE + 0x3953A8) - *(int*)((DWORD)BASE_ENGINE + 0x3953A4), *(int*)((DWORD)BASE_ENGINE + 0x390ABC));
}

void CL_RunPrediction(void)
{
	_CL_RunPrediction = (CL_RunPrediction_t)DetourFunction((PBYTE)((DWORD)BASE_ENGINE + 0x464C0), (PBYTE)Hooked_CL_RunPrediction);
	_CL_PreprocessEntities = (CL_PreprocessEntities_t)DetourFunction((PBYTE)((DWORD)BASE_ENGINE + 0x3EC70), (PBYTE)Hooked_CL_PreprocessEntities);
}
