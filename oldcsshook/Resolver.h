#ifndef __RESOLVER__
#define __RESOLVER__
//#include "Main.h"


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
};
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