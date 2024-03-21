#include "Main.h"

BoneCacheHelper g_BoneCacheHelper;
BoneCache g_BoneCache;

BoneCache* BoneCache::CreateBoneCache(BaseEntity* pEntity, matrix3x4_t* pBoneToWorld, unsigned int& handle)
{
	CStudioHdr* pStudioHdr = *(CStudioHdr**)((DWORD_PTR)pEntity + 0x6A8);
	if (!pStudioHdr) return nullptr;

	bonecacheparams_t params;

	params.pStudioHdr = pStudioHdr;
	params.pBoneToWorld = pBoneToWorld;
	params.curtime = pEntity->m_flSimulationTime(); // g_pGlobals->curtime;
	params.boneMask = 0x100;

	static unsigned int(__cdecl * pStudio_CreateBoneCache)(bonecacheparams_t*) = (unsigned int(__cdecl*)(bonecacheparams_t*))((DWORD_PTR)GetModuleHandleA("client.dll") + 0x2D3F0);
	handle = pStudio_CreateBoneCache(&params);

	static BoneCache* (__cdecl * pStudio_GetBoneCache)(unsigned int) = (BoneCache * (__cdecl*)(unsigned int))((DWORD_PTR)GetModuleHandleA("client.dll") + 0x2A690);
	return pStudio_GetBoneCache(handle);
}

void BoneCache::DestroyBoneCache(unsigned int handle)
{
	static void(__cdecl * pStudio_DestroyBoneCache)(unsigned int) = (void(__cdecl*)(unsigned int))((DWORD_PTR)GetModuleHandleA("client.dll") + 0x24430);
	return pStudio_DestroyBoneCache(handle);
}

void BoneCacheHelper::BackupCache(BaseEntity* pEntity)
{
	m_CacheBackup.m_hitboxCacheHandle = pEntity->m_hitboxBoneCacheHandle();

	m_CacheBackup.m_vecOrigin = pEntity->GetAbsOrigin();
	//m_CacheBackup.m_vecAngles = pEntity->GetAbsAngles();
	m_CacheBackup.m_vecMins = pEntity->m_vecMins();
	m_CacheBackup.m_vecMaxs = pEntity->m_vecMaxs();
}

void BoneCacheHelper::RestoreCache(BaseEntity* pEntity)
{
	if (pEntity->m_hitboxBoneCacheHandle() != m_CacheBackup.m_hitboxCacheHandle) g_BoneCache.DestroyBoneCache(pEntity->m_hitboxBoneCacheHandle());
	pEntity->m_hitboxBoneCacheHandle() = m_CacheBackup.m_hitboxCacheHandle;

	g_Stuff.SetAbsOrigin((BasePlayer*)pEntity, m_CacheBackup.m_vecOrigin);
	//g_Stuff.SetAbsAngles((BasePlayer*)pEntity, m_CacheBackup.m_vecAngles);
	pEntity->m_vecMins() = m_CacheBackup.m_vecMins;
	pEntity->m_vecMaxs() = m_CacheBackup.m_vecMaxs;
}

void BoneCacheHelper::OverrideCache(BaseEntity* pEntity, CTickRecord* pRecord)
{
	if (pEntity->m_hitboxBoneCacheHandle() != m_CacheBackup.m_hitboxCacheHandle) g_BoneCache.DestroyBoneCache(pEntity->m_hitboxBoneCacheHandle());

	unsigned int handle;
	if (g_BoneCache.CreateBoneCache(pEntity, pRecord->m_BoneCache, handle))
	{
		pEntity->m_hitboxBoneCacheHandle() = handle;
		g_Stuff.ApplyTickRecord((BasePlayer*)pEntity, pRecord);
	}
	else printconsole("couldn't apply tick record");
}