/************************************************************************
*					Portable v34 SDK by InUrFace						*
*							15. 03. 2014								*
*					 Not for commercial purposes						*
*																		*
*																		*
************************************************************************/
#ifndef __BASECLIENT_H__
#define __BASECLIENT_H__
#include "VfuncTemplates.h"
#include "ClientClass.h"
#include "Vector.h"

namespace Valve
{
	enum ClientFrameStage_t
	{
		FRAME_UNDEFINED=-1,			// (haven't run any frames yet)
		FRAME_START,

		// A network packet is being recieved
		FRAME_NET_UPDATE_START,
			// Data has been received and we're going to start calling PostDataUpdate
			FRAME_NET_UPDATE_POSTDATAUPDATE_START,
			// Data has been received and we've called PostDataUpdate on all data recipients
			FRAME_NET_UPDATE_POSTDATAUPDATE_END,
		// We've received all packets, we can now do interpolation, prediction, etc..
		FRAME_NET_UPDATE_END,

		// We're about to start rendering the scene
		FRAME_RENDER_START,
		// We've finished rendering the scene.
		FRAME_RENDER_END
	};

	class ClientClass;

	class CViewSetup
	{
	public:
		CViewSetup()
		{
			m_flAspectRatio = 0.0f;
			m_bRenderToSubrectOfLargerScreen = false;
			m_bDoBloomAndToneMapping = true;
			m_bOffCenter = false;
			m_bCacheFullSceneState = false;
			//		m_bUseExplicitViewVector = false;
		}

		// shared by 2D & 3D views
		int context;
		// left side of view window
		int			x;
		// top side of view window
		int			y;
		// width of view window
		int			width;
		// height of view window
		int			height;

		// the rest are only used by 3D views

			// Orthographic projection?
		bool		m_bOrtho;
		// View-space rectangle for ortho projection.
		float		m_OrthoLeft;
		float		m_OrthoTop;
		float		m_OrthoRight;
		float		m_OrthoBottom;

		// horizontal FOV in degrees
		float		fov;
		// horizontal FOV in degrees for in-view model
		float		fovViewmodel;

		// 3D origin of camera
		Vector		origin;

		// heading of camera (pitch, yaw, roll)
		QAngle		angles;
		// local Z coordinate of near plane of camera
		float		zNear;
		// local Z coordinate of far plane of camera
		float		zFar;

		// local Z coordinate of near plane of camera ( when rendering view model )
		float		zNearViewmodel;
		// local Z coordinate of far plane of camera ( when rendering view model )
		float		zFarViewmodel;

		// set to true if this is to draw into a subrect of the larger screen
		// this really is a hack, but no more than the rest of the way this class is used
		bool		m_bRenderToSubrectOfLargerScreen;

		// The aspect ratio to use for computing the perspective projection matrix
		// (0.0f means use the viewport)
		float		m_flAspectRatio;

		// Controls for off-center projection (needed for poster rendering)
		bool		m_bOffCenter;
		float		m_flOffCenterTop;
		float		m_flOffCenterBottom;
		float		m_flOffCenterLeft;
		float		m_flOffCenterRight;

		// Control that the SFM needs to tell the engine not to do certain post-processing steps
		bool		m_bDoBloomAndToneMapping;

		// Cached mode for certain full-scene per-frame varying state such as sun entity coverage
		bool		m_bCacheFullSceneState;

	};

#define CLASSNAME IBaseClientDLL
	class IBaseClientDLL
	{
	public:
		VFUNCR0(GetAllClasses, 0x14, Valve::ClientClass*);
		VFUNCR3(IN_KeyEvent, 0x44, int, int, int, const char* );
		VFUNCN3(CreateMove, 0x48, int, float, bool);
	//	VFUNCN3(CUserCmd + 0xC4, int, int, int, bool);
		VFUNCN1(FrameStageNotify, 0x80, ClientFrameStage_t);
		VFUNCN3(RenderView, 0x60, CViewSetup&, int, bool );
	};
};

#endif //__BASECLIENT_H__