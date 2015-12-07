#ifndef  __KINECTSENSOR_H 
#define __KINECTSENSOR_H



#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <ole2.h>

#include <mmsystem.h>

#pragma comment ( lib, "winmm.lib" )

#include <stdint.h>

#include "NuiApi.h"



#define PI 3.14159265


typedef struct _pos
{
    float x;
    float y;
    float z;
} pos;

class KinectSensor
{
public:
	KinectSensor(void);
    ~KinectSensor(void);
	
	HRESULT							invokeKinectSensor(void);

public:
	bool									bSavePos;
	int									num_person;
	Vector4							HumanBody[20];       
	bool									bFoundSkeleton, bDepth;

private:
	void										Nui_Zero(void);
	void										Nui_UnInit(void);
	
	static DWORD WINAPI		Nui_ProcessThread(LPVOID pParam);
	DWORD WINAPI					Nui_ProcessThread(void);

	void										Nui_GotDepthAlert(void);
	void                                     Nui_GotVideoAlert(void);
	void                                     Nui_GotSkeletonAlert(void);
	RGBQUAD                           Nui_ShortToQuad_Depth( USHORT s );

private:
	INuiSensor						*m_pNuiSensor;
	BSTR								m_instanceId;      

	NUI_SKELETON_FRAME	SkeletonFrame;
	BYTE								buf[640*480*3];

	uint16_t         t_gamma[2048];
	int                  UU, VV;

	HDC               m_SkeletonDC;

	POINT            m_Points[NUI_SKELETON_POSITION_COUNT], m_Points_dep[NUI_SKELETON_POSITION_COUNT];

	DWORD         m_SkeletonIds[NUI_SKELETON_COUNT];
	HANDLE        m_hNextDepthFrameEvent;
	HANDLE        m_hNextColorFrameEvent;
	HANDLE        m_hNextVideoFrameEvent;
	HANDLE        m_hNextSkeletonEvent;
	HANDLE        m_pDepthStreamHandle;
	HANDLE        m_pVideoStreamHandle;
	HANDLE        m_hThNuiProcess;
	HANDLE        m_hEvNuiProcessStop;
	DWORD         m_LastDepthFPStime;
	DWORD         m_LastSkeletonFoundTime;

	int                  m_DepthFramesTotal;
	int                  m_LastDepthFramesTotal;
	bool               m_bScreenBlanked;
	bool               m_bAppTracking;
};

#endif