#include "Main.h"
#ifndef __BONECACHEHELPER_H__
#define __BONECACHEHELPER_H__

struct bonecacheparams_t
{
	CStudioHdr* pStudioHdr;
	matrix3x4_t* pBoneToWorld;
	float           curtime;
	int             boneMask;
};

class BoneCache
{
public:
	BoneCache* CreateBoneCache(BaseEntity*, matrix3x4_t*, unsigned int&);
	void DestroyBoneCache(unsigned int);
};

extern BoneCache g_BoneCache;

class BoneCacheHelper
{
public:
	void BoneCacheHelper::BackupCache(BaseEntity*);
	void BoneCacheHelper::RestoreCache(BaseEntity*);
	void BoneCacheHelper::OverrideCache(BaseEntity*, CTickRecord*);

	struct BoneCacheBackup
	{
		unsigned int m_hitboxCacheHandle;
		Vector m_vecOrigin;
		QAngle m_vecAngles;
		Vector m_vecMins;
		Vector m_vecMaxs;
	};

	BoneCacheBackup m_CacheBackup;
};

extern BoneCacheHelper g_BoneCacheHelper;

#endif //__BONECACHEHELPER_H__