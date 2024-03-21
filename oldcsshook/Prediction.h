#pragma once

#ifndef _PREDICTION_H_
#define _PREDICTION_H_

class Prediction
{
public:
	struct compression_t
	{
		Vector m_Origin, m_NetworkOrigin, m_BaseVelocity, m_ViewOffset, m_Mins, m_Maxs, m_AbsVelocity, m_Velocity;
		QAngle m_PunchAngle, m_ViewAngles;
		float m_FallVelocity, m_DuckJumpTime, m_JumpTime, m_DuckTime;
		int m_Flags, m_WaterLevel;
		bool m_Ducking;
		EHANDLE m_GroundEntity;
	}
	vars[90];

	void Start(CUserCmd*, BasePlayer*);
	void End(CUserCmd*, BasePlayer*);
	void PredictOrigin(Vector&, BasePlayer*);
	void UpdatePlayerHistory(BasePlayer*);
	void SetVars(BasePlayer*, int);
	void GetVars(BasePlayer*, int);
	int m_nOldTickBase, PushTickBase, m_nTickBase;
	float OldFrametime, OldCurtime;
	IMoveHelper* Move;
	QAngle m_punchAngle;
	Vector m_velocity;
	CUserCmd* pLastCmd;
};
extern Prediction g_Prediction;

#endif // _PREDICTION_H_