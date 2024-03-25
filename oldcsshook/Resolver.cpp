#include "Main.h"


Resolver g_Resolver;

bool EdgeDetect(BasePlayer* Entity)
{
	bool ret = false;

	for (int y = 0; y < 360; y++)
	{
		QAngle tmp(10.f, Entity->m_angEyeAngles().y, 0.f);
		tmp.y += y;

		Vector forward;
		AngleVectors(tmp, &forward, NULL, NULL);
		float length = ((16 + 3) + ((16 + 3) * sin(DEG2RAD(10.f)))) + 5;
		forward *= length;

		Ray_t ray;
		trace_t traceData;

		TraceFilterSkipTwoEntities TraceFilter(Entity, 0);

		ray.Init(Entity->EyePosition(), (Entity->EyePosition() + forward));
		g_pEngineTrace->TraceRay(ray, 0x200400B, NULL, &traceData);

		if (traceData.fraction != 1.f)
		{
			QAngle angles;
			Vector Negate = traceData.plane.normal;
			Negate *= -1;
			VectorAngles(Negate, angles);

			tmp.y = angles.y;
			trace_t leftTrace, rightTrace;

			Vector left, right;
			AngleVectors(tmp + QAngle(0.f, 30.f, 0.f), &left, NULL, NULL);
			AngleVectors(tmp - QAngle(0.f, 30.f, 0.f), &right, NULL, NULL);

			left *= (length + (length * sin(DEG2RAD(30.f))));
			right *= (length + (length * sin(DEG2RAD(30.f))));

			ray.Init(Entity->EyePosition(), (Entity->EyePosition() + left));
			g_pEngineTrace->TraceRay(ray, 0x200400B, NULL, &leftTrace);

			ray.Init(Entity->EyePosition(), (Entity->EyePosition() + right));
			g_pEngineTrace->TraceRay(ray, 0x200400B, NULL, &rightTrace);

			if ((leftTrace.fraction == 1.f) && (rightTrace.fraction != 1.f)) tmp.y -= 90.f;
			else if ((leftTrace.fraction != 1.f) && (rightTrace.fraction == 1.f)) tmp.y += 90.f;

			if (g_Stuff.GuwopNormalize(tmp.y - Entity->m_angEyeAngles().y) < 1.f) ret = true;
		}
	}
	return ret;
}
float clamp3(float val, float minVal, float maxVal)
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
float normalizehehe(float angle, float angle1)
{
	if (angle < -180) angle += 360;
	if (angle > 180) angle -= 360;
	return angle;
}
bool build_server_abs_yaw(BasePlayer* m_player, float angle)
{
	Vector velocity = m_player->m_vecVelocity();
	auto anim_state = m_player->GetAnimstate();
	float m_flEyeYaw = angle;
	float m_flGoalFeetYaw = 0.f;

	float eye_feet_delta = g_Stuff.GuwopNormalize(m_flEyeYaw - m_flGoalFeetYaw);

	static auto GetSmoothedVelocity = [](float min_delta, Vector a, Vector b) {
		Vector delta = a - b;
		float delta_length = delta.Length();

		if (delta_length <= min_delta)
		{
			Vector result;

			if (-min_delta <= delta_length)
				return a;
			else
			{
				float iradius = 1.0f / (delta_length + FLT_EPSILON);
				return b - ((delta * iradius) * min_delta);
			}
		}
		else
		{
			float iradius = 1.0f / (delta_length + FLT_EPSILON);
			return b + ((delta * iradius) * min_delta);
		}
	};

	float spd = velocity.LengthSqr();

	if (spd > std::powf(1.2f * 260.0f, 2.f))
	{
		Vector velocity_normalized = velocity.Normalized();
		velocity = velocity_normalized * (1.2f * 260.0f);
	}

	float m_flChokedTime = anim_state->m_flLastAnimationStateClearTime;
	float v25 = (m_player->m_flDucktime(), 0.0f);
	float v26 = (m_player->m_flDucktime(), 1.0f);
	float v27 = m_flChokedTime * 6.0f;
	float v28;

	// clamp
	if ((v25 - v26) <= v27) {
		if (-v27 <= (v25 - v26))
			v28 = v25;
		else
			v28 = v26 - v27;
	}
	else {
		v28 = v26 + v27;
	}

	float flDuckAmount = clamp3(v28, 0.0f, 1.0f);

	Vector animationVelocity = GetSmoothedVelocity(m_flChokedTime * 2000.0f, velocity, m_player->m_vecVelocity());
	float speed = std::fminf(animationVelocity.Length(), 260.0f);

	float flMaxMovementSpeed = 260.0f;
	CSWeapon* pWeapon = (CSWeapon*)m_player->GetActiveBaseCombatWeapon();
	//CSWeapon* pWeapon = m_player->GetActiveBaseCombatWeapon();

	if (pWeapon && pWeapon->GetWeaponID())
		flMaxMovementSpeed = std::fmaxf(pWeapon->GetSpread(), 0.001f);

	float flRunningSpeed = speed / (flMaxMovementSpeed * 0.520f);
	float flDuckingSpeed = speed / (flMaxMovementSpeed * 0.340f);

	flRunningSpeed = clamp3(flRunningSpeed, 0.0f, 1.0f);

	float flYawModifier = ((0.19999999) * flRunningSpeed) + 1.0f;

	if (flDuckAmount > 0.0f)
	{
		float flDuckingSpeed = clamp3(flDuckingSpeed, 0.0f, 1.0f);
		flYawModifier += (flDuckAmount * flDuckingSpeed) * (0.5f - flYawModifier);
	}

	const float v60 = -58.f;
	const float v61 = 58.f;

	float flMinYawModifier = v60 * flYawModifier;
	float flMaxYawModifier = v61 * flYawModifier;

	if (eye_feet_delta <= flMaxYawModifier)
	{
		if (flMinYawModifier > eye_feet_delta)
			m_flGoalFeetYaw = fabs(flMinYawModifier) + m_flEyeYaw;
	}
	else
	{
		m_flGoalFeetYaw = m_flEyeYaw - fabs(flMaxYawModifier);
	}

	g_Stuff.GuwopNormalize(m_flGoalFeetYaw);

	if (speed > 0.1f || fabs(velocity.z) > 100.0f)
	{
		m_flGoalFeetYaw = normalizehehe(
			m_flEyeYaw,
			m_flGoalFeetYaw);
			//((anim_state->stop_to_full_running_fraction * 20.0f) + 30.0f)
		//	* m_flChokedTime);
	}
	else
	{
		m_flGoalFeetYaw = normalizehehe(
			//m_player->m_flLowerBodyYawTarget(),
			m_flGoalFeetYaw,
			m_flChokedTime * 100.0f);
	}

	return m_flGoalFeetYaw;
}
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
bool CurrentYaw(BasePlayer* LocalPlayer, BasePlayer* Target)
{
	Vector resultLocal = EyePosition;
	Vector resultentity = Target->EyePosition();
	Vector m_vTraceVector = Vector(resultLocal - resultentity);

	if (!resultLocal.IsValid() || !resultentity.IsValid() || !m_vTraceVector.IsValid()) return false;

	QAngle m_vAimAngles;
	VectorAngles(m_vTraceVector, m_vAimAngles);
	m_vAimAngles.x *= -1;

	if (!m_vAimAngles.IsValid()) return false;

	double variationPitch = double(m_vAimAngles.x - Target->m_angEyeAngles().x);
	double variationYaw = double(m_vAimAngles.y - Target->m_angEyeAngles().y);

	while (variationPitch < 0) variationPitch += 360.0;
	while (variationYaw < 0) variationYaw += 360.0;

	while (variationPitch > 180.0) variationPitch = 360.0 - variationPitch;
	while (variationYaw > 180.0) variationYaw = 360.0 - variationYaw;

	return (variationPitch <= 2) && (variationYaw <= 3);
}

bool IsAimingAtMe(BasePlayer* LocalPlayer, BasePlayer* Target)
{
	Vector resultLocal = EyePosition;
	Vector resultentity = Target->EyePosition();
	Vector m_vTraceVector = Vector(resultLocal - resultentity);

	if (!resultLocal.IsValid() || !resultentity.IsValid() || !m_vTraceVector.IsValid()) return false;

	QAngle m_vAimAngles;
	VectorAngles(m_vTraceVector, m_vAimAngles);
	m_vAimAngles.x *= -1;

	if (!m_vAimAngles.IsValid()) return false;

	double variationPitch = double(m_vAimAngles.x - Target->m_angEyeAngles().x);
	double variationYaw = double(m_vAimAngles.y - Target->m_angEyeAngles().y);

	while (variationPitch < 0) variationPitch += 360.0;
	while (variationYaw < 0) variationYaw += 360.0;

	while (variationPitch > 180.0) variationPitch = 360.0 - variationPitch;
	while (variationYaw > 180.0) variationYaw = 360.0 - variationYaw;

	return (variationPitch <= 2) && (variationYaw <= 3);
}

static float BruteAngles[] = { 0.f, -61.f, 61.f };
float Resolver::SelectAngle(BasePlayer* Entity, int flag)
{
	auto animstate = Entity->GetAnimstate();
	int i = Entity->entindex();

	static int old[64];

	if (old[i] > shotsfired[i]) old[i] = 0;

	float delta = g_Stuff.GuwopNormalize(Entity->m_angEyeAngles().y - g_CVars.PlayerList.ViewAngles[i].y / 1.5);
	float bodyyawdelta = *(float*)((DWORD)Entity + 0x518 + (0x4 * 2));

	bool twoshots = ((shotsfired[i] - old[i]) >= 2);
	bool animationupdated = (fabs(g_Stuff.GuwopNormalize(angle[i][flag] - delta)) < 5.f);

	if (!animationupdated) ShouldLogShots[i] = false;

	if (shotsfired[i] >= 2)
	{
		if (twoshots && animationupdated)
		{
			angleindex[i][flag]++;
			if (angleindex[i][flag] > 2) angleindex[i][flag] = 0;
			angle[i][flag] = BruteAngles[angleindex[i][flag]];
			angle_old[i][flag] = angle[i][flag];

			old[i] = shotsfired[i];
		}
		else angle[i][flag] = angle_old[i][flag];
	}
	else angle[i][flag] = angle_old[i][flag];

	return angle[i][flag];
	animstate->m_flGoalFeetYaw = animstate->m_flCurrentFeetYaw;
	animstate->m_flLastTurnTime = 0.f;
	animstate->m_bCurrentFeetYawInitialized = true;
}
void Resolver::detect_side(BasePlayer* player, int* side) {
	bool ret = false;

	for (int y = 0; y < 360; y++)
	{
		QAngle tmp(10.f, player->m_angEyeAngles().y, 0.f);
		tmp.y += y;

		Vector forward;
		AngleVectors(tmp, &forward, NULL, NULL);
		float length = ((16 + 3) + ((16 + 3) * sin(DEG2RAD(10.f)))) + 5;
		forward *= length;

		Ray_t ray;
		trace_t traceData;

		TraceFilterSkipTwoEntities TraceFilter(player, 0);

		ray.Init(player->EyePosition(), (player->EyePosition() + forward));
		g_pEngineTrace->TraceRay(ray, 0x200400B, NULL, &traceData);

		if (traceData.fraction != 1.f)
		{
			QAngle angles;
			Vector Negate = traceData.plane.normal;
			Negate *= -1;
			VectorAngles(Negate, angles);

			tmp.y = angles.y;
			trace_t leftTrace, rightTrace;

			Vector left, right;
			AngleVectors(tmp + QAngle(0.f, 30.f, 0.f), &left, NULL, NULL);
			AngleVectors(tmp - QAngle(0.f, 30.f, 0.f), &right, NULL, NULL);

			left *= (length + (length * sin(DEG2RAD(30.f))));
			right *= (length + (length * sin(DEG2RAD(30.f))));

			ray.Init(player->EyePosition(), (player->EyePosition() + left));
			g_pEngineTrace->TraceRay(ray, 0x200400B, NULL, &leftTrace);

			ray.Init(player->EyePosition(), (player->EyePosition() + right));
			g_pEngineTrace->TraceRay(ray, 0x200400B, NULL, &rightTrace);

			/* fix side */

			if (leftTrace.fraction > rightTrace.fraction) {
				*side = -1;
			}
			else if (rightTrace.fraction > leftTrace.fraction) {
				*side = 1;
			}
			else
				*side = 0;
		}
	}
}
void MakeVector(QAngle pfIn, Vector& pfOut)
{
	float pitch;
	float yaw;
	float tmp;

	pitch = (float)(pfIn[0] * M_PI / 180);
	yaw = (float)(pfIn[1] * M_PI / 180);
	tmp = (float)cos(pitch);

	pfOut[0] = (float)(-tmp * -cos(yaw));
	pfOut[1] = (float)(sin(yaw) * tmp);
	pfOut[2] = (float)-sin(pitch);
}

void MakeVector(QAngle pfIn, QAngle& pfOut)
{
	float pitch;
	float yaw;
	float tmp;

	pitch = (float)(pfIn[0] * M_PI / 180);
	yaw = (float)(pfIn[1] * M_PI / 180);
	tmp = (float)cos(pitch);

	pfOut[0] = (float)(-tmp * -cos(yaw));
	pfOut[1] = (float)(sin(yaw) * tmp);
	pfOut[2] = (float)-sin(pitch);
}

float AngleDifference(QAngle angle, Vector src, Vector dst)
{
	QAngle ang;
	Vector aim;
	float difference;

	g_Stuff.CalculateAngles(src, dst, ang);
	MakeVector(angle, aim);
	MakeVector(ang, ang);

	float mag_s = sqrt((aim[0] * aim[0]) + (aim[1] * aim[1]) + (aim[2] * aim[2]));
	float mag_d = sqrt((aim[0] * aim[0]) + (aim[1] * aim[1]) + (aim[2] * aim[2]));

	float u_dot_v = aim[0] * ang[0] + aim[1] * ang[1] + aim[2] * ang[2];

	difference = acos(u_dot_v / (mag_s * mag_d)) * (180.0f / M_PI);

	return difference;
}
float normalizehehe(float angle)
{
	if (angle < -180) angle += 360;
	if (angle > 180) angle -= 360;
	return angle;
}
double angleDiff(double a, double b) {
	double dif = fmod(b - a + 180, 360);
	if (dif < 0)
		dif += 360;
	return dif - 180;
}

float side(CCSPlayerAnimState* animstate)
{
	float eye_goalfeet_delta = angleDiff(g_Stuff.GuwopNormalize(animstate->m_flEyeYaw) - g_Stuff.GuwopNormalize(animstate->m_flGoalFeetYaw), 360.f);

	float new_body_yaw_pose = 0.f;

	if (eye_goalfeet_delta < 0.f)
	{
		new_body_yaw_pose = (eye_goalfeet_delta / -180.f) * -75.f;
	}
	else
	{
		new_body_yaw_pose = (eye_goalfeet_delta / 180.f) * 75.f;
	}
	return new_body_yaw_pose;
}
void smart(BasePlayer* Entity)
{
	int i = Entity->entindex();
	auto animstate = Entity->GetAnimstate(); 
	int best_record = g_LagCompensation.bestrecord[i];
	CTickRecord* current = &pPlayerHistory[i][best_record];
	CTickRecord* previous = &pPlayerHistory[i][best_record + 1];
	CTickRecord simulation = pSimulationData[i];
	float pitch = Entity->m_angEyeAngles().x;
	float yaw = Entity->m_angEyeAngles().y;
	bool twitchpitch = ((current->m_EyeAngles.x - previous->m_EyeAngles.x) > 40.f);
	bool lag = ((TIME_TO_TICKS(current->m_SimulationTime - previous->m_SimulationTime)) > 2);
	/*
	//maybe side fix by this
	bool twitchyaw = ((current->m_EyeAngles.y - previous->m_EyeAngles.y) >= 180.f);
	if (IsAimingAtMe)
	{
		if (yaw >= 90.f)
		{
			yaw -= 90.f;
		}
		if (yaw <= 90.f)
		{
			yaw += 90.f;
		}
	}
	*/
	if (((pitch < 45.f || pitch > -45.f) && !lag) || twitchpitch)
	{
		Entity->m_angEyeAngles().y += 0;
	}

}

void Resolver::memesolve(BasePlayer* Entity) {
	int i = Entity->entindex();
	int best_record = g_LagCompensation.bestrecord[i];
	CTickRecord* current = &pPlayerHistory[i][best_record];
	CTickRecord* previous = &pPlayerHistory[i][best_record + 1];
	CTickRecord simulation = pSimulationData[i];

	float poseparam2 = -90.f + current->m_PoseParameter[2] * 180.f;
	float poseparam3 = -90.f + current->m_PoseParameter[3] * 180.f;
	float poseparam4 = -90.f + current->m_PoseParameter[4] * 180.f;

	float g_flLastBodyYaw = *(float*)((DWORD)0x243A2474);
	float m_flGoalFeetYaw = *(float*)((DWORD)Entity + 0xF80 + 0x20);


	for (int i = 0; i < 15; i++)
	{
		float weight = current->m_AnimationLayer[i].flWeight;
		float cycle = current->m_AnimationLayer[i].flCycle;

		if (weight == 0.f && cycle >= .97f) printconsole("yo");
	}


	float pitch = Entity->m_angEyeAngles().x;
	bool twitch = ((current->m_EyeAngles.x - previous->m_EyeAngles.x) > 40.f);
	bool lag = ((TIME_TO_TICKS(current->m_SimulationTime - previous->m_SimulationTime)) > 6);

	if (((pitch < 45.f || pitch > -45.f) && !lag) || twitch)
	{
		ShouldLogShots[i] = false;
		return;
	}
	else ShouldLogShots[i] = true;

	float temp = g_CVars.PlayerList.ViewAngles[i].y;
	static float temp_j[64], delta[64], angle, angletowardslocal[64];
	Vector eyedelta = Vector(EyePosition - Entity->EyePosition());
	QAngle aimangles;

	// --- calc delta for jitter ----------------------------------------------------------

	delta[i] = g_Stuff.GuwopNormalize(temp - temp_j[i]);
	delta[i] -= 180.f;
	if (delta[i] < 0.f) delta[i] = -delta[i];

	temp_j[i] = temp;

	// --- calc delta for static aa -------------------------------------------------------

	VectorAngles(eyedelta, aimangles);
	aimangles.x *= -1.f;

	if (!aimangles.IsValid()) return;

	angletowardslocal[i] = aimangles.y - temp;
	angletowardslocal[i] = g_Stuff.GuwopNormalize(angletowardslocal[i]);
	if (angletowardslocal[i] < 0.f) angletowardslocal[i] += 360.f;

	// --- check for movement types -------------------------------------------------------

	int k = 0;

	if (Entity->m_fFlags() & FL_ONGROUND)
	{
		if (Entity->m_vecVelocity().Length2D() < 30)
		{
			//if (EdgeDetect(Entity) && Entity->m_vecVelocity().Length() < 300.f) k = 5;
			//else
			k = (Entity->m_fFlags() & FL_DUCKING) ? 1 : 2;
		}
		else k = 3;
	}
	else k = 4;

	// ------------------------------------------------------------------------------------

	angle = SelectAngle(Entity, k); // other cases
	flResAngle[i] = angle;
	//if (fabsf(delta[i]) < 15.f) angle = 180.f; // jitter

	QAngle orig = QAngle(Entity->m_angEyeAngles().x, temp, Entity->m_angEyeAngles().z);
	QAngle final = QAngle(Entity->m_angEyeAngles().x, temp + angle, Entity->m_angEyeAngles().z);

	current->m_EyeAngles.x = final.y;
	simulation.m_EyeAngles.y = final.y;
	Entity->m_angEyeAngles().y = final.y;
	//Entity->SetAbsAngles(final);
	//Entity->SetLocalAngles(final);

	//Entity->UpdateClientSideAnimation();

	//Entity->m_angEyeAngles().y = temp;
	//Entity->SetAbsAngles(orig);
	//Entity->SetLocalAngles(orig);
	//Entity->UpdateClientSideAnimation();
}

void Resolver::Think(BasePlayer* Entity)
{
	auto animstate = Entity->GetAnimstate();

	float m_flLowerBodyYawTarget = Entity->m_angEyeAngles().y - animstate->m_flGoalFeetYaw + 360;
	bool fl_foword = fabsf( g_Stuff.GuwopNormalize( get_angle( Entity ) - get_foword_yaw( Entity ) ) ) < 90.f;
	int fl_shots = ( g_iBulletsFired[ Entity->entindex( ) ] );
	switch (g_iBulletsFired[Entity->entindex()] % 4)
	{
	case 0:
		Entity->m_angEyeAngles().y -= 89.f;
		break;
	case 1:
		Entity->m_angEyeAngles().y += 89.f;
		break;
	case 2:
		Entity->m_angEyeAngles().y -= 45.f;
		break;
	case 3:
		Entity->m_angEyeAngles().y += 45.f;
		break;
	}
	animstate->m_flGoalFeetYaw = animstate->m_flCurrentFeetYaw;
	animstate->m_flLastTurnTime = 0.f;
	animstate->m_bCurrentFeetYawInitialized = true;
	/* fix goalfeet yaw */
	//player->m_PlayerAnimState()->goal_feet_yaw = fl_eye_yaw + brute;
}

// ====================================
// i dont use that 
// ====================================
// i dont use that 
/*{
	auto animstate = Entity->GetAnimstate();
	auto i = g_pGlobals->maxClients; i >= 1; --i;
	static float fl_stored_yaw = Entity->m_angEyeAngles().y;
	float resolve_value = 50.f;
	static float brute = 0.f;
	float fl_max_rotation = 90.f;
	float fl_eye_yaw = Entity->GetAnimstate()->m_flEyeYaw;
	float perfect_resolve_yaw = resolve_value;
	bool fl_foword = fabsf(g_Stuff.GuwopNormalize(get_angle(Entity) - get_foword_yaw(Entity))) < 90.f;
	int fl_shots = (g_iBulletsFired[Entity->entindex()]);
	static int side{ 89 };

	if (fl_shots == 0) {
		brute = perfect_resolve_yaw * (fl_foword ? -60 : 60);
	}
	else {
		switch (g_iBulletsFired[Entity->entindex()] % 3) {
		case 0: {
			brute = perfect_resolve_yaw * (fl_foword ? -35 : 35);
		} break;
		case 1: {
			brute = perfect_resolve_yaw * (fl_foword ? 15 : -15);
		} break;
		case 2: {
			brute = 0;
		} break;
		}
	}
	Entity->m_angEyeAngles().y += brute;
	/* fix goalfeet yaw 
	//      Entity->GetAnimstate()->m_flGoalFeetYaw = fl_eye_yaw + brute;
}
*/