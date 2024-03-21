#include "Main.h"
#include "SendMove.h"
#include <limits>

DWORD dwReturnAddress = NULL;
DWORD dwCreateMove = 0x24087270;
static bool bSendPacket;
int sequence_number = 0;

float _clamp( float val, float minVal, float maxVal )
{
	if ( maxVal < minVal )
		return maxVal;
	else if( val < minVal )
		return minVal;
	else if( val > maxVal )
		return maxVal;
	else
		return val;
}

static bool pass = false;
static int queue = 0;
static bool angelfix = false;
float updateTime;
float lastUpdate;
float wasmoving;
bool performBreak;

void lbyBreaker(CUserCmd* pCmd, bool& bSendPacket) {
	BasePlayer* LocalPlayer = (BasePlayer*)g_pClientEntityList->GetClientEntity(g_pEngineClient->GetLocalPlayer());
	float flServerTime = (float)(LocalPlayer->m_nTickBase() * g_pGlobals->interval_per_tick);
	float Velocity = LocalPlayer->GetVelocity().Length2D();

	if (!performBreak) {
		if (Velocity > 1.f && (LocalPlayer->m_fFlags() & FL_ONGROUND)) {
			lastUpdate = flServerTime;
			wasmoving = true;
		}
		else {
			if (wasmoving && flServerTime - lastUpdate > 0.22f && (LocalPlayer->m_fFlags() & FL_ONGROUND)) {
				wasmoving = false;
				performBreak = true;
			}
			else if (flServerTime - lastUpdate > 1.1f && (LocalPlayer->m_fFlags() & FL_ONGROUND)) {
				performBreak = true;
			}
			else {
			}
		}
	}
	else {
		bSendPacket = false;
		pCmd->viewangles.y += 105.f;
		lastUpdate = flServerTime;
		performBreak = false;
	}
}
bool lbyUpdate()
{
	BasePlayer* LocalPlayer = (BasePlayer*)g_pClientEntityList->GetClientEntity(g_pEngineClient->GetLocalPlayer());
	float flServerTime = (float)(LocalPlayer->m_nTickBase() * g_pGlobals->interval_per_tick);
	float Velocity = LocalPlayer->GetVelocity().Length2D();

	if (Velocity > 1.0f) {
		updateTime = flServerTime + 0.22f;
	}

	if ((LocalPlayer->m_fFlags() & FL_ONGROUND) && flServerTime >= updateTime) {
		updateTime = flServerTime + 1.1f;
		return true;
		}
	else {
		return false;
	}
}
void AntiAimPitch( CUserCmd* pCmd, BasePlayer* LocalPlayer )
{
	// note: lisp doesnt do shit in css, needs max float

	switch( g_CVars.Miscellaneous.AntiAim.Pitch )
	{
		case 0: break;
		case 1: pCmd->viewangles.x = 180.f; break;																// normal
		case 2: pCmd->viewangles.x = -180.f; break; 															// inverse normal
		case 3: pCmd->viewangles.x = 89.f; break; 																// down
		case 4: pCmd->viewangles.x = -89.f; break;																// up
		case 5: pCmd->viewangles.x = rand() % 360; break; 					// jitter	
		case 6: pCmd->viewangles.x = 424.760605524934789702960929958; break;		//test 424,760605524934789702960929958
		case 7: pCmd->viewangles.x = (lbyBreaker) ? 89.f : -180.f; break; 								// flick up
		case 8:	pCmd->viewangles.x = g_CVars.Miscellaneous.AntiAim.PitchValue; break;
	}
}

static bool twitch, twitchfake, edgetwitch, edgetwitchfake;
void FakeWalk(BasePlayer* LocalPlayer, CUserCmd* cmd)
{
	if (LocalPlayer->m_fFlags() & FL_ONGROUND)
	{
		int speed = LocalPlayer->m_vecVelocity().Length2D();
		if (GetAsyncKeyState(VK_LSHIFT) && g_CVars.Miscellaneous.slowwalk)
		{
			if (speed < g_CVars.Miscellaneous.slowwalkspeed)return;
			if (GetAsyncKeyState(83))
				cmd->forwardmove = g_CVars.Miscellaneous.slowwalkspeed * -1;
			if (GetAsyncKeyState(87))
				cmd->forwardmove = g_CVars.Miscellaneous.slowwalkspeed;
			if (GetAsyncKeyState(68))
				cmd->sidemove = g_CVars.Miscellaneous.slowwalkspeed;
			if (GetAsyncKeyState(65))
				cmd->sidemove = g_CVars.Miscellaneous.slowwalkspeed * -1;
		}
	}
}

__forceinline float angle_diff(float destAngle, float srcAngle)
{
	float delta;

	delta = fmodf(destAngle - srcAngle, 360.0f);
	if (destAngle > srcAngle)
	{
		if (delta >= 180)
			delta -= 360;
	}
	else
	{
		if (delta <= -180)
			delta += 360;
	}
	return delta;
}
template <typename T> __forceinline int signum(T val)
{
	return (T(0) < val) - (val < T(0));
}
void AntiAimYaw( CUserCmd* pCmd, BasePlayer* LocalPlayer, bool fake, bool half )
{
	float server_time = LocalPlayer->m_nTickBase() * g_pGlobals->interval_per_tick;
	static float yaw = server_time + 0.0f;
	static int SpinCounter = 0;
	bool Side = false;
	auto target_direction = 0.f;
	int i = g_pGlobals->maxClients;
	const auto target_delta = (target_direction / 100.f);
	const auto min_max = target_delta > 0.f;
	const auto min_max_inv = target_delta <= 0.f;
	BaseEntity* Ent = (BaseEntity*)g_pClientEntityList->GetClientEntity(i);
	float bodyyawdelta = *(float*)((DWORD)Ent + 0x518 + (0x4 * 2));
	float yaw1 = Ent->m_angEyeAngles().y;
	//BasePlayer* LocalPlayer = (BasePlayer*)g_pClientEntityList->GetClientEntity(g_pEngineClient->GetLocalPlayer());
	static bool bSwitch = false;
	float m_flGoalFeetYaw = *(float*)((DWORD)Ent + 0xF80 + 0x20);
	auto intermediate = 0.f;
	float delta = g_Stuff.GuwopNormalize(yaw1 - m_flGoalFeetYaw);
	bool inair = !(LocalPlayer->m_fFlags() & FL_ONGROUND);
	if (g_CVars.Miscellaneous.AntiAim.lowdelta)
	{
		static constexpr auto flop_tolerance = 6.f;
		static constexpr auto flip_range = -152.f;
		const auto opposite = g_Stuff.GuwopNormalize(flip_range - fabsf(min_max_inv));
		intermediate += opposite + (fabsf(min_max_inv) - fabsf(m_flGoalFeetYaw)) * .75f;
		const auto delta = angle_diff(g_Stuff.GuwopNormalize(pCmd->viewangles.y + intermediate * -signum(target_delta)), m_flGoalFeetYaw);

		pCmd->viewangles.y* delta;
	}
	if (g_CVars.Miscellaneous.AntiAim.opposite)
	{
		static constexpr auto flop_tolerance = 6.f;
		static constexpr auto flip_range = -152.f;

		const auto opposite = g_Stuff.GuwopNormalize(flip_range - fabsf(min_max_inv));
		intermediate += opposite + (fabsf(min_max_inv) - fabsf(m_flGoalFeetYaw)) * .75f;
		const auto delta = angle_diff(g_Stuff.GuwopNormalize(pCmd->viewangles.y + intermediate * -signum(target_delta)), m_flGoalFeetYaw);

		if (fabsf(delta) <= opposite)
			intermediate = 180.f + -signum(target_delta) * (fabsf(min_max_inv) + flop_tolerance);
		else
			intermediate *= -signum(target_delta);
	}
	Vector Velocity = LocalPlayer->m_vecVelocity( );
	if (g_CVars.Miscellaneous.AntiAim.lbybreaker) lbyUpdate(), lbyBreaker(pCmd, bSendPacket);
	if( fake )
	{
		switch( g_CVars.Miscellaneous.AntiAim.Yaw )
		{
			case 0: 
			{
				switch( g_CVars.Miscellaneous.AntiAim.Variation )
				{
					case 0: break;
					case 1: pCmd->viewangles.y += ( half ) ? 270.f : 181.f; break;
					case 2: pCmd->viewangles.y += ( half ) ? 90.f : 179.f; break;
					case 3: pCmd->viewangles.y += 180.f; break;
				}
				break;
			}
			case 1:
			{
				switch( g_CVars.Miscellaneous.AntiAim.Variation )
				{
					case 0: pCmd->viewangles.y += 180.f; break;
					case 1: pCmd->viewangles.y += ( half ) ? 90.f : 1.f; break;
					case 2: pCmd->viewangles.y += ( half ) ? 270.f : 359.f; break;
					case 3: break;
				}
				break;
			}
			case 2:
			{
				switch( g_CVars.Miscellaneous.AntiAim.Variation )
				{
					case 0: pCmd->viewangles.y += 270.f; break;
					case 1: pCmd->viewangles.y += ( half ) ? 180.f : 91.f; break;
					case 2: pCmd->viewangles.y += ( half ) ? 360.f : 89.f; break;
					case 3: pCmd->viewangles.y += 90.f; break;
				}
				break;
			}			
			case 3:
			{
				static bool bFlip = false;
				switch( g_CVars.Miscellaneous.AntiAim.Variation )
				{
				case 0: pCmd->viewangles.y += g_CVars.Miscellaneous.AntiAim.FakeValue; break;
					case 1:	pCmd->viewangles.y += bFlip ? g_CVars.Miscellaneous.AntiAim.FakeValue - 15.f : g_CVars.Miscellaneous.AntiAim.FakeValue + 15.f; break;
					case 2: pCmd->viewangles.y = g_CVars.Miscellaneous.AntiAim.FakeValue; break;
				}
				break;
			}
			case 4:
			{
				static bool bFlip = false;
				switch (g_CVars.Miscellaneous.AntiAim.Variation)
				{
				case 0:

					pCmd->viewangles.y += bFlip ? g_CVars.Miscellaneous.AntiAim.JitterReal : g_CVars.Miscellaneous.AntiAim.JitterFake;
					bFlip = !bFlip;
				case 1:

					pCmd->viewangles.y += bFlip ? g_CVars.Miscellaneous.AntiAim.JitterReal * 3.14 : g_CVars.Miscellaneous.AntiAim.JitterFake * 3.14;
					bFlip = !bFlip;

				case 2:

					pCmd->viewangles.y += bFlip ? g_CVars.Miscellaneous.AntiAim.JitterReal + M_PI : g_CVars.Miscellaneous.AntiAim.JitterFake + M_PI;
					bFlip = !bFlip;
				}
			}
		}
	}
	else
	{
		switch( g_CVars.Miscellaneous.AntiAim.Yaw )
		{
			case 0: break;
			case 1: pCmd->viewangles.y += 180.f; break;
			case 2: pCmd->viewangles.y += 270.f; break;
			case 3:
			{ 
				static bool bFlip = false;
				switch( g_CVars.Miscellaneous.AntiAim.Variation )
				{ 
					case 0: pCmd->viewangles.y += GetAsyncKeyState((g_CVars.Miscellaneous.AntiAim.inverterkey)) ? g_CVars.Miscellaneous.AntiAim.RealValue : g_CVars.Miscellaneous.AntiAim.FakeValue; break;
					case 1:	pCmd->viewangles.y += bFlip ? g_CVars.Miscellaneous.AntiAim.RealValue -15.f : g_CVars.Miscellaneous.AntiAim.RealValue + 15.f; break;
					case 2: pCmd->viewangles.y = g_CVars.Miscellaneous.AntiAim.RealValue; break;
				}
				break;
			}
			case 4:
			{
				static bool bFlip = false;
				switch (g_CVars.Miscellaneous.AntiAim.Variation)
				{
				case 0:

					pCmd->viewangles.y += bFlip ? g_CVars.Miscellaneous.AntiAim.JitterReal : g_CVars.Miscellaneous.AntiAim.JitterFake;
					bFlip = !bFlip;
				case 1:

					pCmd->viewangles.y += bFlip ? g_CVars.Miscellaneous.AntiAim.JitterReal * 3.14 : g_CVars.Miscellaneous.AntiAim.JitterFake * 3.14;
					bFlip = !bFlip;
				
				case 2:

					pCmd->viewangles.y += bFlip ? g_CVars.Miscellaneous.AntiAim.JitterReal + M_PI : g_CVars.Miscellaneous.AntiAim.JitterFake + M_PI;
					bFlip = !bFlip;
				}
			}
		}
	}
}

void AntiAim( BasePlayer* LocalPlayer, CUserCmd* pCmd, int LagValue )
{

	if (pCmd->buttons & IN_USE)	return;

	if (g_CVars.Miscellaneous.AntiAim.manuals)
	{
	//	return;
		if (GetAsyncKeyState(g_CVars.Miscellaneous.AntiAim.RightManualValue))
		{
			pCmd->viewangles.y += 90.f;
		} 
		else if (GetAsyncKeyState(g_CVars.Miscellaneous.AntiAim.LeftManualValue))
		{
			pCmd->viewangles.y -= 90.f;
		}
	}
	//Redirected_Send_Move();
//	Naked_Redirected_Send_Move();
	int MoveType = LocalPlayer->m_MoveType( );
	Vector Velocity = LocalPlayer->m_vecVelocity( );
	int perfectrandomticksorder[] = { 14, 13, 12, 11, 10, 8, 13, 14, 12, 8 };
	static int perfectcounter = 0;
	bool WallDTC = false;
	bool ret = true;
	bool ShouldChoke = false;

	bool inair = !( LocalPlayer->m_fFlags( ) & FL_ONGROUND );

	int tmpLagticks;
	if( g_CVars.Miscellaneous.Fakelag.AirOnly )
	{
		tmpLagticks = ( inair ) ? LagValue : 1;
	}
	else tmpLagticks = LagValue;

	// creds to machete for giving me this brilliant idea lol
	int DeltaTicks = _clamp( abs( queue - tmpLagticks ), 0, 15 );

	if( g_CVars.Miscellaneous.Fakelag.Active )
	{
		if( g_CVars.Miscellaneous.Fakelag.Mode == 0 )
		{
			{	//maxmimum
				static int queued = 0;
				if (queued >= 0)
				{
					queued++;

					//up to 21
					if ((queued < 2))
					{
						bSendPacket = true;
					}
					else
					{
						// cmd up
						bSendPacket = false;
					}
				}
				else
				{
					// cmd up
					bSendPacket = true;
				}

				// warping and reseting
				if ((queued == 21))
				{
					queued = 0;
				}
			}

			if (DeltaTicks > 0) ShouldChoke = true;
		}
		else if( g_CVars.Miscellaneous.Fakelag.Mode == 1 )//aimware
		{
			int wish_ticks{};
			int adaptive_ticks{};
			const int units_per_tick = g_pGlobals->interval_per_tick; Velocity.Length2D() >= 15.0f ? Velocity.Length2D() : 0.0f;

			if (wish_ticks * units_per_tick > 68)
				goto LABEL_64;
			if ((adaptive_ticks - 1) * units_per_tick > 68)
			{
				++wish_ticks;
			LABEL_64:
				ShouldChoke = DeltaTicks > tmpLagticks < wish_ticks;
			}
			else if (adaptive_ticks * units_per_tick > 68)
			{
				ShouldChoke = DeltaTicks > tmpLagticks < wish_ticks + 2;
			}
			else if ((adaptive_ticks + 1) * units_per_tick > 68)
			{
				ShouldChoke = DeltaTicks > tmpLagticks < wish_ticks + 3;
			}
			else
			{
				if ((adaptive_ticks + 2) * units_per_tick <= 68)
				{
					adaptive_ticks += 5;
					wish_ticks += 5;
					if (adaptive_ticks > 16)
					{
						ShouldChoke = DeltaTicks > tmpLagticks < wish_ticks;
					}
				}
				ShouldChoke = DeltaTicks > tmpLagticks < wish_ticks + 4;
			}
			tmpLagticks = wish_ticks;
			if (DeltaTicks > 0) ShouldChoke = true;
		}
		else if( g_CVars.Miscellaneous.Fakelag.Mode == 2 ) // thx dadli
		{
			tmpLagticks = perfectrandomticksorder[perfectcounter];
			tmpLagticks += rand() % 10, 14;
			if (DeltaTicks > 0) ShouldChoke = true;
		}
		else if (g_CVars.Miscellaneous.Fakelag.Mode == 3) // thx HENZO
			{
				static int ticks = 0;
				int packetsToChoke;
				if (Velocity.Length() > 0.f)
				{
					packetsToChoke = (int)((64.f / g_pGlobals->interval_per_tick) / Velocity.Length() + 1);
					if (packetsToChoke >= 15)
						packetsToChoke = 14;
					if (packetsToChoke < g_CVars.Miscellaneous.Fakelag.Value)
						packetsToChoke = g_CVars.Miscellaneous.Fakelag.Value;
				}
				else
					packetsToChoke = 0;

				bSendPacket = ticks < 16 - packetsToChoke;
				if (DeltaTicks > 0) ShouldChoke = true;
			}
		
		else if (g_CVars.Miscellaneous.Fakelag.Mode == 4)
		{
				//evvware
				//stable fakelag but i wont update it
				float Velocity2D = Velocity.Length2D() * g_pGlobals->interval_per_tick;
				if (Velocity2D > 0.0f)
				{
					tmpLagticks = (int)((64.f / g_pGlobals->interval_per_tick) * Velocity2D) - g_CVars.Miscellaneous.Fakelag.Value;
					if (tmpLagticks >= g_CVars.Miscellaneous.Fakelag.Value)
						tmpLagticks - 2;

					if (tmpLagticks < g_CVars.Miscellaneous.Fakelag.Value)
						tmpLagticks = g_CVars.Miscellaneous.Fakelag.Value;

					bSendPacket = ShouldChoke < DeltaTicks - tmpLagticks;
				}
				if (DeltaTicks > 0) ShouldChoke = true;
		}

		else if (g_CVars.Miscellaneous.Fakelag.Mode == 5)
		{
			{	//fastest
				static int queued = 0;
				if (queued >= 0)
				{
					queued++;

					//up to 21
					if ((queued < 2))
					{
						bSendPacket = true;
					}
					else
					{
						// cmd up
						bSendPacket = false;
					}
				}
				else
				{
					// cmd up
					bSendPacket = true;
				}

				// warping and reseting
				if ((queued == 4))
				{
					queued = 0;
				}
			}

			if (DeltaTicks > 0) ShouldChoke = true;
			}
	}
	else
	{
		if( g_CVars.Miscellaneous.AntiAim.Active )
		{
			static bool flip;
			flip = !flip;
			if( flip ) ShouldChoke = true;
		}
	}

	if( pass ) ShouldChoke = false;

	bSendPacket = ( ShouldChoke ) ? false : true;
	if( !bSendPacket )
	{
		//Redirected_Send_Move();
		//CL_SendMove();
		SendMove();
		if( queue >= 14 )
		{
			
			bSendPacket = true;
			queue = 0;
		}
		else ++queue;
	}
	else queue = 0;

	if( g_CVars.Miscellaneous.AntiAim.Active )
	{
		for( int i = g_pGlobals->maxClients; i >= 1; i-- )
		{
			if( i == g_pEngineClient->GetLocalPlayer( ) ) continue;			
			CSWeapon* Weapon = (CSWeapon*)LocalPlayer->GetActiveBaseCombatWeapon();
			if (Weapon->GetWeaponID() == WEAPON_KNIFE)
				continue;
			BasePlayer* Ent = ( BasePlayer* )g_pClientEntityList->GetClientEntity( i );
			if( !Ent ) continue;
			if( !( *( int* )( ( DWORD ) Ent + 0x87 ) == 0 ) ) continue;
			if( Ent->m_iTeamNum( ) == LocalPlayer->m_iTeamNum( ) ) continue;
			 
			ret = false;
		}

		if( g_CVars.Miscellaneous.AntiAim.TurnOff )
		{
			if( ret ) return;
		}

		if( ( MoveType == Valve::MoveType_t::MOVETYPE_LADDER ) && ( pCmd->buttons & IN_DUCK ) )
		{
			if( !bSendPacket ) pCmd->buttons &= ~IN_DUCK;
		}

		if (g_CVars.Miscellaneous.AntiAim.AtTargets == 1) g_Stuff.AntiAim.AtTargets(LocalPlayer, pCmd);
		else if (g_CVars.Miscellaneous.AntiAim.AtTargets== 2) g_Stuff.AntiAim.AtTargets2(LocalPlayer, pCmd);

		if( g_CVars.Miscellaneous.AntiAim.WallDetection && Velocity.Length( ) < 300.f )
		{
			if( g_CVars.Miscellaneous.AntiAim.WallDetectionMode == 0 ) WallDTC = g_Stuff.AntiAim.WallDetection( LocalPlayer, pCmd, 0.f );
			else if( g_CVars.Miscellaneous.AntiAim.WallDetectionMode == 1 ) WallDTC = g_Stuff.AntiAim.WallDetection( LocalPlayer, pCmd, ( bSendPacket ) ? 0.f : 180.f );
			else if( g_CVars.Miscellaneous.AntiAim.WallDetectionMode == 2 ) WallDTC = g_Stuff.AntiAim.WallDetection( LocalPlayer, pCmd, ( bSendPacket ) ? 180.f : 0.f );
			else if( g_CVars.Miscellaneous.AntiAim.WallDetectionMode == 3 )
			{
				if( bSendPacket )
				{
					edgetwitch = !edgetwitch;
					WallDTC = g_Stuff.AntiAim.WallDetection( LocalPlayer, pCmd, ( edgetwitch ) ? 0.f : 180.f );
				}
				else
				{
					edgetwitchfake = !edgetwitch;
					edgetwitchfake = !edgetwitchfake;
					WallDTC = g_Stuff.AntiAim.WallDetection( LocalPlayer, pCmd, ( edgetwitch ) ? 0.f : 180.f );
				}
			}
		}

		if( MoveType != Valve::MoveType_t::MOVETYPE_LADDER )
		{
			if( WallDTC ) pCmd->viewangles.x = 89.f;
			else
			{
				AntiAimPitch( pCmd, LocalPlayer );
				if( bSendPacket ) AntiAimYaw( pCmd, LocalPlayer, false, false );
				else
				{
					AntiAimYaw( pCmd, LocalPlayer, true, true );
					if( g_CVars.Miscellaneous.AntiAim.DuckInAir && LocalPlayer->GetVelocity( ).z > 0 ) pCmd->buttons |= IN_DUCK;
				}
			}
		}
	}

	pass = false;
}

void sendcmd( const char* input, ... )
{ 
	va_list va_alist;
	char buf[ 256 ];

	va_start( va_alist, input );
	vsprintf( buf, input, va_alist );
	va_end( va_alist );

	g_pEngineClient->ExecuteClientCmd( buf );
}

void ForceFullUpdate( BasePlayer* Ent )
{
	typedef void( __thiscall* ForceFullUpdate_t )( void* );
	( ( ForceFullUpdate_t )( ( DWORD ) BASE_ENGINE + 0x9E0D0 ) )( Ent );
}
#define clamp(val, min, max) (((val) > (max)) ? (max) : (((val) < (min)) ? (min) : (val)))
void CorrectTickCount(CUserCmd* pCmd)
{
	float updaterate = g_pCvar->FindVar("cl_updaterate")->GetFloat();
	ConVar* minupdate = g_pCvar->FindVar("sv_minupdaterate");
	ConVar* maxupdate = g_pCvar->FindVar("sv_maxupdaterate");

	if (minupdate && maxupdate)
		updaterate = maxupdate->GetFloat();

	float ratio = g_pCvar->FindVar("cl_interp_ratio")->GetFloat();

	if (ratio == 0)
		ratio = 1.0f;

	float lerp = g_pCvar->FindVar("cl_interp")->GetFloat();
	ConVar* cmin = g_pCvar->FindVar("sv_client_min_interp_ratio");
	ConVar* cmax = g_pCvar->FindVar("sv_client_max_interp_ratio");

	if (cmin && cmax && cmin->GetFloat() != 1)
		ratio = clamp(ratio, cmin->GetFloat(), cmax->GetFloat());

	max(lerp, ratio / updaterate);

	int tick;
	if (g_Aimbot.TargetIndex != -1)
	{
		tick = TIME_TO_TICKS(pPlayerHistory[g_Aimbot.TargetIndex][0].m_SimulationTime);
		bool timeout = (tick < (pCmd->tick_count - 50));
		if (!timeout) pCmd->tick_count = tick;
	}
}


typedef void( __thiscall* CreateMove_t )( void*, int, float, bool );
void __fastcall CreateMove( void* ecx, void* edx, int sequence_number, float input_sample_frametime, bool active )
{
	CreateMoveVMT->Function< CreateMove_t >( 18 )( edx, sequence_number, input_sample_frametime, active );

	BasePlayer* LocalPlayer = ( BasePlayer* ) g_pClientEntityList->GetClientEntity( g_pEngineClient->GetLocalPlayer( ) );
	CSWeapon* Weapon = ( CSWeapon* ) LocalPlayer->GetActiveBaseCombatWeapon( );
	if( !LocalPlayer ) return;

	if( !g_pInput ) return;

	bSendPacket = true;
	CUserCmd* pCmd = g_pInput->GetUserCmd( sequence_number );

	g_TickCount = pCmd->tick_count;

	if( g_GUI.ShouldDisableInput( ) )
	{
		pCmd->buttons &= ~IN_ATTACK;
		pCmd->buttons &= ~IN_ATTACK2;
		pCmd->buttons &= ~IN_FORWARD;
		pCmd->buttons &= ~IN_BACK;
		pCmd->buttons &= ~IN_MOVELEFT;
		pCmd->buttons &= ~IN_MOVERIGHT;
	}

	g_Stuff.sidemove_old = pCmd->sidemove;
	g_Stuff.forwardmove_old = pCmd->forwardmove;
	g_pEngineClient->GetViewAngles(g_Stuff.radarangles);
	g_Stuff.viewangles_old = pCmd->viewangles;
	g_Stuff.ForceCVars();

	g_iGameTicks++;
	g_TickCount = pCmd->tick_count;

	if (g_CVars.MovementRecorder.Active)
	{
		if (GetAsyncKeyState(VK_F6)) MovementRecorder.Situation = RECORDING; // record
		if (GetAsyncKeyState(VK_F7)) // save
		{
			g_Macro.CurrentName = /*demo_1*/XorStr<0x15, 7, 0xEF9CCFF8>("\x71\x73\x7A\x77\x46\x2B" + 0xEF9CCFF8).s;
			g_Macro.Save = true;
			g_Macro.Load = false;
		}
		if (GetAsyncKeyState(VK_F8)) // load
		{
			g_Macro.CurrentName = /*demo_1*/XorStr<0x3E, 7, 0x37290FEA>("\x5A\x5A\x2D\x2E\x1D\x72" + 0x37290FEA).s;
			g_Macro.Load = true;
			g_Macro.Save = false;
		}
		if (GetAsyncKeyState(VK_F9)) MovementRecorder.Situation = PLAYING; // play
		if (GetAsyncKeyState(VK_F10)) MovementRecorder.Situation = NOTHING; // stop playing/recording
		if (GetAsyncKeyState(VK_F11)) MovementRecorder.Situation = STARTPOS; // find startposition

		MovementRecorder.RecordMovement(pCmd, LocalPlayer, pCmd->viewangles);
	}

	if (LocalPlayer->m_lifeState() != 0) return; // something sometimes crashed so i had to put this shit lol

	if (g_CVars.Aimbot.AutoPistol) g_Stuff.AutoPistol(pCmd, Weapon, LocalPlayer);
	if( g_CVars.Miscellaneous.BunnyHop ) g_Stuff.BunnyHop( pCmd, LocalPlayer );
	if (g_CVars.Miscellaneous.AutoPeek && g_CVars.Miscellaneous.AutoPeekValue) g_Stuff.AutoPeek(pCmd, LocalPlayer);
	//if( g_CVars.Miscellaneous.EdgeJump ) g_Stuff.EdgeJump( pCmd, LocalPlayer );

	Vector ViewOffsetZ = *reinterpret_cast<Vector*>(reinterpret_cast<DWORD>(LocalPlayer) + 0x0D4);

	("DT_BasePlayer", "ViewOffset[0]");
	if (g_CVars.Miscellaneous.FakeStand)
	{
		if (GetAsyncKeyState(g_CVars.Miscellaneous.FakeStandValue))
		{
			static bool bDuck = false;

			if (bDuck)
			{
				bDuck = false;
				pCmd->buttons &= ~IN_DUCK;
				bSendPacket = true;
			}
			else
			{
				if (pCmd->buttons & IN_DUCK)
				{
					bSendPacket = false;
					bDuck = true;
					static bool bFakeCrouch = true;
					bFakeCrouch = !bFakeCrouch;
				}
			}
		}
	}
	if (g_CVars.Miscellaneous.FakeDuck && GetAsyncKeyState(g_CVars.Miscellaneous.FakeDuckValue))
	{
		BasePlayer* LocalPlayer = (BasePlayer*)g_pClientEntityList->GetClientEntity(g_pEngineClient->GetLocalPlayer());
		static bool isducked;

		if (pCmd->buttons & IN_DUCK && LocalPlayer->m_fFlags() & FL_ONGROUND)

		{
			if (isducked == false)
			{
				if (ViewOffsetZ.z == 47)
				{
					isducked = true;
				}
			}
			else
			{
				if (ViewOffsetZ.z == 64)
				{
					pCmd->buttons |= IN_DUCK;
				}
				else if (ViewOffsetZ.z <= 63)
				{
					pCmd->buttons &= ~IN_DUCK;
				}
			}
		}
		else
		{
			isducked = false;
		}
	}

	g_Prediction.Start( pCmd, LocalPlayer );
	EyePosition = LocalPlayer->EyePosition( );

	if( g_CVars.Miscellaneous.AutoKnife ) g_Stuff.Knifebot.Main( pCmd, LocalPlayer, Weapon );

	if( Weapon && Weapon->IsWeapon( ) )
	{
		if( g_CVars.Triggerbot.Active ) g_Stuff.SeedTrigger( pCmd, LocalPlayer, Weapon );
		if( g_CVars.Aimbot.Active )
		{
			QAngle tmp = pCmd->viewangles;
			g_Aimbot.Main( pCmd, LocalPlayer );

			if( g_CVars.Aimbot.SnapLimiter )
			{
				float delta_x, delta_y, limit;
				delta_x = g_Stuff.GuwopNormalize( pCmd->viewangles.x - tmp.x );
				delta_y = g_Stuff.GuwopNormalize( pCmd->viewangles.y - tmp.y );

				if( g_CVars.Aimbot.AngleLimit >= 180 ) limit = 180;
				else limit = float( g_CVars.Aimbot.AngleLimit ) + g_CVars.Aimbot.AngleLimitTens; // eks dee

				if( !( ( delta_x < limit && delta_x > -limit ) && ( delta_y < limit && delta_y > -limit ) ) ) pCmd->viewangles = tmp;
			}
		}

		if( pCmd->buttons & IN_ATTACK )
		{
			if( g_Stuff.IsReadyToShoot( LocalPlayer, Weapon ) )	
			{
				if( g_Aimbot.TargetIndex != -1 ) g_iBulletsFired[ g_Aimbot.TargetIndex ]++;
				angelfix = false;
				pass = true;
				queue = 0;

				bool trigger = ( g_CVars.Triggerbot.Active && !g_CVars.Triggerbot.IsShooting );

				if( !g_CVars.Triggerbot.Active || trigger )
				{
					if( g_CVars.Accuracy.ForceSeed ) g_Stuff.ForceSeed( pCmd );
					if( g_CVars.Accuracy.PerfectAccuracy )
					{
						switch( g_CVars.Accuracy.NoSpreadMode )
						{
							case 1: g_NoSpread.Main( pCmd, pCmd->viewangles, LocalPlayer, Weapon, g_CVars.Miscellaneous.AntiAim.Static ); break;
							case 2: g_NoSpread.Iterative( pCmd, pCmd->viewangles, LocalPlayer, Weapon, g_CVars.Miscellaneous.AntiAim.Static ); break;
							case 3: g_NoSpread.CoolNospreee( pCmd, pCmd->viewangles, LocalPlayer, Weapon, g_CVars.Miscellaneous.AntiAim.Static ); break;
							default: g_NoSpread.Main( pCmd, pCmd->viewangles, LocalPlayer, Weapon, g_CVars.Miscellaneous.AntiAim.Static ); break;
						}
						g_Stuff.NoRecoil( pCmd, LocalPlayer, g_CVars.Miscellaneous.AntiAim.Static );
					}
				}

				if( g_CVars.Aimbot.PerfectSilent ) bSendPacket = false;

				CorrectTickCount( pCmd );
			}
			else
			{
				if( g_CVars.Aimbot.AntiSMAC ) pCmd->viewangles = QAngle( 0, 0, 0 );
				else AntiAim( LocalPlayer, pCmd, ( g_CVars.Miscellaneous.Fakelag.InAttack ) ? g_CVars.Miscellaneous.Fakelag.Value : 1 );

				pCmd->buttons &= ~IN_ATTACK;
			}
		}
		else
		{
			if( g_CVars.Aimbot.AntiSMAC ) pCmd->viewangles = QAngle( 0, 0, 0 );
			else AntiAim( LocalPlayer, pCmd, g_CVars.Miscellaneous.Fakelag.Value );
		}
	}

	if( g_CVars.Miscellaneous.AutoStrafe ) g_Stuff.AutoStrafe( pCmd, LocalPlayer );
	if (g_CVars.Miscellaneous.slowwalk) FakeWalk(LocalPlayer, pCmd);
	int i = g_pGlobals->maxClients; i >= 1; i--;
	BasePlayer* Ent = (BasePlayer*)g_pClientEntityList->GetClientEntity(i);
	if (g_CVars.Miscellaneous.FastThrow) g_Stuff.FastTrow(pCmd, LocalPlayer, Ent);

	g_Stuff.MovementFix.FixMove( LocalPlayer, pCmd, angelfix );

	g_Prediction.End( pCmd, LocalPlayer );

	if( g_CVars.Miscellaneous.AirStuck )
	{
		// todo: fix local pos so the shots while stuck are accurate
		if( GetAsyncKeyState( 'F' ) & 1 ) g_CVars.Miscellaneous.AirStuckPress = !g_CVars.Miscellaneous.AirStuckPress;

		if( g_CVars.Miscellaneous.AirStuckPress )
		{
			if( !( pCmd->buttons & IN_ATTACK ) ) pCmd->tick_count = INT_MAX;
		}
	}
	if (g_CVars.Miscellaneous.LagExploit)
	{

		if (GetAsyncKeyState(g_CVars.Miscellaneous.LagExploitValue))

		{
			bSendPacket = (sequence_number % 10) == 0;
			pCmd->command_number += 3 * 150;
			*(int*)(GetModuleHandleA("engine.dll") + (DWORD)0x39539C) += 3 * 150;
			CNetChannel* m_pNet = *(CNetChannel**)0x20390990;
			m_pNet->m_nOutSequenceNr += 3 * 150;


		}
	}
	auto command = 0;
	if (GetAsyncKeyState(g_CVars.Miscellaneous.DefensiveValue))
	{
		int nTickBase = LocalPlayer->m_nTickBase();
		if ((pCmd->buttons & IN_ATTACK))
		{
			bSendPacket = ((pCmd->command_number % 20) == 1);
			command = (MULTIPLAYER_BACKUP / 14);
			//	command = (MULTIPLAYER_BACKUP / 14);
			nTickBase % 14;
		}

	}
	if (GetAsyncKeyState(g_CVars.Miscellaneous.DefensiveValue1))
	{
		int nTickBase = LocalPlayer->m_nTickBase();
		if ((pCmd->buttons & IN_ATTACK))
		{
			bSendPacket = (sequence_number % 10) == 0;
			pCmd->command_number += 3 * 150;
			*(int*)(GetModuleHandleA("engine.dll") + (DWORD)0x39539C) += 3 * 150;
			command = (MULTIPLAYER_BACKUP / 14);
			nTickBase % 14;
		}
	}

		if (GetAsyncKeyState(g_CVars.Miscellaneous.FakeWalkValue))
		{
			static int choked = 0;
			choked = choked > 7 ? 0 : choked + 1;
			pCmd->forwardmove = choked < 2 || choked > 5 ? 0 : pCmd->forwardmove;
			pCmd->sidemove = choked < 2 || choked > 5 ? 0 : pCmd->sidemove;
			bSendPacket = choked < 1;
		}
	
	if( bSendPacket ) g_qThirdPerson = pCmd->viewangles;
}

void __declspec( naked ) __fastcall Hooked_CreateMove( void* ecx, void* edx, int sequence_number, float input_sample_frametime, bool active )
{
	__asm
	{
		push ebp
		mov ebp, esp
		mov bSendPacket, bl			   
		movzx eax, active
		push eax
		mov eax, input_sample_frametime
		push eax
		mov eax, sequence_number
		push eax
		call CreateMove			   
		mov bl, bSendPacket			   
		mov esp, ebp
		pop ebp			   
		retn 0xC
	}
}