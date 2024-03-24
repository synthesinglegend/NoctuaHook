#ifndef __RESOLVER__
#define __RESOLVER__
//#include "Main.h"

enum EResolverStages {
	RESOLVE_MODE_NONE, // not currently resolving
	RESOLVE_MODE_STAND,
	RESOLVE_MODE_MOVE,
	RESOLVE_MODE_AIR,
	RESOLVE_MODE_PRED,
	RESOLVE_MODE_OVERRIDE
};

struct ResolverData_t {
	void ResetMoveData() {
		if (!m_vecLastMoveOrigin.IsZero())
			m_vecLastMoveOrigin.Init();

		if (m_flLastMoveBody != FLT_MAX)
			m_flLastMoveBody = FLT_MAX;

		if (m_flLastMoveAnimTime != FLT_MAX)
			m_flLastMoveAnimTime = FLT_MAX;
	}

	void ResetPredData() {
		if (m_flLowerBodyRealignTimer != FLT_MAX)
			m_flLowerBodyRealignTimer = FLT_MAX;

		if (m_flAnimationTime != FLT_MAX)
			m_flAnimationTime = FLT_MAX;

		if (m_flPreviousLowerBodyYaw != FLT_MAX)
			m_flPreviousLowerBodyYaw = FLT_MAX;

		if (m_bInPredictionStage != false)
			m_bInPredictionStage = false;

		if (m_iDetectedBodyUpdate != 0)
			m_iDetectedBodyUpdate = 0;
	}

	void Reset() {
		m_iMissedShots = m_iMissedShotsLBY = m_iMissedShotsSpread = 0;
		m_eCurrentStage = RESOLVE_MODE_NONE;
		ResetMoveData();
		ResetPredData();
	}

	EResolverStages m_eCurrentStage;

	int m_iMissedShots;
	int m_iMissedShotsSpread;
	int m_iMissedShotsLBY;

	bool m_bIsOverriding = false;

	// last move data
	Vector m_vecLastMoveOrigin = { 0, 0, 0 };
	float m_flLastMoveBody = FLT_MAX;
	float m_flLastMoveAnimTime = FLT_MAX;

	// lby pred data
	float m_flPreviousLowerBodyYaw = FLT_MAX;
	int m_flLastLbyActivity = 979;
	float m_flLowerBodyYawTarget;
	int m_iDetectedBodyUpdate = 0;
	bool m_bInPredictionStage = false;
	float m_flLowerBodyRealignTimer = FLT_MAX;
	float m_flAnimationTime = FLT_MAX;
};
class AdaptiveAngle {
public:
	float m_yaw;
	float m_dist;

public:
	// ctor.
	__forceinline AdaptiveAngle(float yaw, float penalty = 0.f) {
		// set yaw.
		m_yaw = g_Stuff.GuwopNormalize(yaw);

		// init distance.
		m_dist = 0.f;

		// remove penalty.
		m_dist -= penalty;
	}
};
class Resolver
{
public:
	int shotsfired[64], shotsfired_old[64], angleindex[64][3];
	float angle[64][3], angle_old[64][3];
	float flResAngle[64];
	void detect_side(BasePlayer*, int*);
	float SelectAngle(BasePlayer*, int);
	bool ShouldLogShots[64];
	void Think(BasePlayer*);
	void memesolve(BasePlayer*);

};
std::array< ResolverData_t, 65 > m_arrResolverData;
extern Resolver g_Resolver;

class LagCompensation
{
public:
	CTickRecord HistoryData[64][32];
	CTickRecord SimulationData[64];
	CTickRecord BackupData[64];

	void Store();
	void Restore(BasePlayer*);
	void RestoreBackup(BasePlayer*);
	bool ShouldRestoreBackup[64];
	float LerpTime();
	int ServerTick();
	int GetBestRecord(BasePlayer*, CSWeapon*, BasePlayer*);
	int bestrecord[64];
	bool CheckDelta(CTickRecord*);
};
extern LagCompensation g_LagCompensation;

#endif //__RESOLVER__