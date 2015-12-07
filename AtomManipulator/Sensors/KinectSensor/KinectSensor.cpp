#include "KinectSensor.h"

#include <iostream>
#include <fstream>

#include "cv.h"
#include "cxcore.h"
#include "highgui.h"

using namespace std;

IplImage                 *image, *depthimg, *depthcol;

static const CvScalar g_JointColorTable[NUI_SKELETON_POSITION_COUNT] = 
{
	CV_RGB(169, 176, 155), // NUI_SKELETON_POSITION_HIP_CENTER
	CV_RGB(169, 176, 155), // NUI_SKELETON_POSITION_SPINE
	CV_RGB(168, 230, 29),  // NUI_SKELETON_POSITION_SHOULDER_CENTER
	CV_RGB(200, 0,   0),   // NUI_SKELETON_POSITION_HEAD
	CV_RGB(79,  84,  33),  // NUI_SKELETON_POSITION_SHOULDER_LEFT
	CV_RGB(84,  33,  42),  // NUI_SKELETON_POSITION_ELBOW_LEFT
	CV_RGB(255, 126, 0),   // NUI_SKELETON_POSITION_WRIST_LEFT
	CV_RGB(215,  86, 0),   // NUI_SKELETON_POSITION_HAND_LEFT
	CV_RGB(33,  79,  84),  // NUI_SKELETON_POSITION_SHOULDER_RIGHT
	CV_RGB(33,  33,  84),  // NUI_SKELETON_POSITION_ELBOW_RIGHT
	CV_RGB(77,  109, 243), // NUI_SKELETON_POSITION_WRIST_RIGHT
	CV_RGB(37,   69, 243), // NUI_SKELETON_POSITION_HAND_RIGHT
	CV_RGB(77,  109, 243), // NUI_SKELETON_POSITION_HIP_LEFT
	CV_RGB(69,  33,  84),  // NUI_SKELETON_POSITION_KNEE_LEFT
	CV_RGB(229, 170, 122), // NUI_SKELETON_POSITION_ANKLE_LEFT
	CV_RGB(255, 126, 0),   // NUI_SKELETON_POSITION_FOOT_LEFT
	CV_RGB(181, 165, 213), // NUI_SKELETON_POSITION_HIP_RIGHT
	CV_RGB(71, 222,  76),  // NUI_SKELETON_POSITION_KNEE_RIGHT
	CV_RGB(245, 228, 156), // NUI_SKELETON_POSITION_ANKLE_RIGHT
	CV_RGB(77,  109, 243)  // NUI_SKELETON_POSITION_FOOT_RIGHT
};


//lookups for color tinting based on player index
static const int g_IntensityShiftByPlayerR[] = { 1, 2, 0, 2, 0, 0, 2, 0 };
static const int g_IntensityShiftByPlayerG[] = { 1, 2, 2, 0, 2, 0, 0, 1 };
static const int g_IntensityShiftByPlayerB[] = { 1, 0, 2, 2, 0, 2, 0, 2 };

static const COLORREF g_SkeletonColors[NUI_SKELETON_COUNT] =
{
    RGB( 255, 0, 0),
    RGB( 0, 255, 0 ),
    RGB( 64, 255, 255 ),
    RGB( 255, 255, 64 ),
    RGB( 255, 64, 255 ),
    RGB( 128, 128, 255 )
};



KinectSensor::KinectSensor()
{

	//cnt = 0;
}

KinectSensor::~KinectSensor()
{

	//Nui_Zero();
}

void KinectSensor::Nui_Zero()
{
  	if (m_pNuiSensor)
    {
        m_pNuiSensor->Release();
        m_pNuiSensor = NULL;
    }

    m_hNextDepthFrameEvent = NULL;
    m_hNextColorFrameEvent = NULL;
    m_hNextSkeletonEvent = NULL;
    m_pDepthStreamHandle = NULL;
    m_pVideoStreamHandle = NULL;
    m_hThNuiProcess = NULL;
    m_hEvNuiProcessStop = NULL;
    m_SkeletonDC = NULL;
    ZeroMemory(m_Points,sizeof(m_Points));
    m_LastSkeletonFoundTime = 0;
    m_bScreenBlanked = false;
    m_DepthFramesTotal = 0;
    m_LastDepthFPStime = 0;
    m_LastDepthFramesTotal = 0;
    ZeroMemory(m_SkeletonIds,sizeof(m_SkeletonIds));

	cvNamedWindow( "image", CV_WINDOW_AUTOSIZE);
	cvNamedWindow( "depthimg", CV_WINDOW_AUTOSIZE);
	//cvNamedWindow( "depthcol", CV_WINDOW_AUTOSIZE);
	image = cvCreateImage( cvSize(640,480), IPL_DEPTH_8U, 4 );
	depthimg = cvCreateImage( cvSize(640,480), IPL_DEPTH_8U, 3 );
	//depthcol = cvCreateImage( cvSize(640,480), IPL_DEPTH_8U, 3 );

	bSavePos = false;
	bDepth = false;

	for (int i=0; i<2048; i++) {
		float v = i/2048.0;
		v = powf(v, 3)* 6;
		t_gamma[i] = v*6*256;
	}
}

void KinectSensor::Nui_UnInit( )
{
    DeleteDC( m_SkeletonDC );

    // Stop the Nui processing thread
    if ( NULL != m_hEvNuiProcessStop )
    {
        // Signal the thread
        SetEvent(m_hEvNuiProcessStop);

        // Wait for thread to stop
        if ( NULL != m_hThNuiProcess )
        {
            WaitForSingleObject( m_hThNuiProcess, INFINITE );
            CloseHandle( m_hThNuiProcess );
        }
        CloseHandle( m_hEvNuiProcessStop );
    }

    if ( m_pNuiSensor )
    {
        m_pNuiSensor->NuiShutdown( );
    }
    if ( m_hNextSkeletonEvent && ( m_hNextSkeletonEvent != INVALID_HANDLE_VALUE ) )
    {
        CloseHandle( m_hNextSkeletonEvent );
        m_hNextSkeletonEvent = NULL;
    }
    if ( m_hNextDepthFrameEvent && ( m_hNextDepthFrameEvent != INVALID_HANDLE_VALUE ) )
    {
        CloseHandle( m_hNextDepthFrameEvent );
        m_hNextDepthFrameEvent = NULL;
    }
    if ( m_hNextColorFrameEvent && ( m_hNextColorFrameEvent != INVALID_HANDLE_VALUE ) )
    {
        CloseHandle( m_hNextColorFrameEvent );
        m_hNextColorFrameEvent = NULL;
    }

    if ( m_pNuiSensor )
    {
        m_pNuiSensor->Release();
        m_pNuiSensor = NULL;
    }

	//if (bSavePos)
	//{
	//	fstream mfile, mfile1, mfile2;
	//	mfile.open("SHOULDER_LEFT.txt", ios::out);
	//	mfile1.open("ELBOW_LEFT.txt", ios::out);
	//	mfile2.open("WRIST_LEFT.txt", ios::out);
	//	for( int j = 0; j< cnt; j++)
	//		mfile<<SHOULDER_LEFT[3*j]<<" "<<SHOULDER_LEFT[3*j+1]<<" "<<SHOULDER_LEFT[3*j+2]<<endl;
	//	mfile.close();
	//	for( int j = 0; j< cnt; j++)
	//		mfile1<<ELBOW_LEFT[3*j]<<" "<<ELBOW_LEFT[3*j+1]<<" "<<ELBOW_LEFT[3*j+2]<<endl;
	//	mfile1.close();
	//	for( int j = 0; j< cnt; j++)
	//		mfile2<<WRIST_LEFT[3*j]<<" "<<WRIST_LEFT[3*j+1]<<" "<<WRIST_LEFT[3*j+2]<<endl;
	//	mfile2.close();
	//}

	//cvDestroyWindow( "image");
	//cvReleaseImage( &image);
	printf("Delete KinectThread...\n");
}

HRESULT KinectSensor::invokeKinectSensor()
{
	HRESULT  hr;
    RECT     rc;
    bool     result;

    if ( !m_pNuiSensor )
    {
        HRESULT hr = NuiCreateSensorByIndex(0, &m_pNuiSensor);

        if ( FAILED(hr) )
        {
            return hr;
        }

        SysFreeString(m_instanceId);

        m_instanceId = m_pNuiSensor->NuiDeviceConnectionId();
    }

    m_hNextDepthFrameEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
    m_hNextColorFrameEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
    m_hNextSkeletonEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
    
    //int width = rc.right - rc.left;
    //int height = rc.bottom - rc.top;
    
    DWORD nuiFlags = NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX | NUI_INITIALIZE_FLAG_USES_SKELETON |  NUI_INITIALIZE_FLAG_USES_COLOR;
    hr = m_pNuiSensor->NuiInitialize( nuiFlags );
    if ( E_NUI_SKELETAL_ENGINE_BUSY == hr )
    {
        nuiFlags = NUI_INITIALIZE_FLAG_USES_DEPTH |  NUI_INITIALIZE_FLAG_USES_COLOR;
        hr = m_pNuiSensor->NuiInitialize( nuiFlags) ;
    }
  
    if ( FAILED( hr ) )
        return hr;

    if ( HasSkeletalEngine( m_pNuiSensor ) )
    {
        hr = m_pNuiSensor->NuiSkeletonTrackingEnable( m_hNextSkeletonEvent, 0 );
        if( FAILED( hr ) )
            return hr;
    }

    hr = m_pNuiSensor->NuiImageStreamOpen(
        NUI_IMAGE_TYPE_COLOR,
        NUI_IMAGE_RESOLUTION_640x480,
        0,
        2,
        m_hNextColorFrameEvent,
        &m_pVideoStreamHandle );

    if ( FAILED( hr ) )
        return hr;

    hr = m_pNuiSensor->NuiImageStreamOpen(
        HasSkeletalEngine(m_pNuiSensor) ? NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX : NUI_IMAGE_TYPE_DEPTH,
        NUI_IMAGE_RESOLUTION_640x480,
        0,
        2,
        m_hNextDepthFrameEvent,
        &m_pDepthStreamHandle );

    if ( FAILED( hr ) )
        return hr;

    // Start the Nui processing thread
    m_hEvNuiProcessStop = CreateEvent( NULL, FALSE, FALSE, NULL );
    m_hThNuiProcess = CreateThread( NULL, 0, Nui_ProcessThread, this, 0, NULL );

    return hr;
}

DWORD WINAPI KinectSensor::Nui_ProcessThread(LPVOID pParam)
{
    KinectSensor *pthis = (KinectSensor *) pParam;
    return pthis->Nui_ProcessThread();
}

//-------------------------------------------------------------------
// Nui_ProcessThread
//
// Thread to handle Kinect processing
//-------------------------------------------------------------------
DWORD WINAPI KinectSensor::Nui_ProcessThread()
{
    const int numEvents = 4;
    HANDLE hEvents[numEvents] = { m_hEvNuiProcessStop, m_hNextDepthFrameEvent, m_hNextColorFrameEvent, m_hNextSkeletonEvent };
    int    nEventIdx;
    //DWORD  t;

    //m_LastDepthFPStime = timeGetTime( );

    //blank the skeleton display on startup
    //m_LastSkeletonFoundTime = 0;
	printf("CreateKinectThread...\n");
    // Main thread loop
    bool continueProcessing = true;
    while ( continueProcessing )
    {
		//tic();
		//m_LastDepthFPStime = timeGetTime( );
        // Wait for any of the events to be signalled
        nEventIdx = WaitForMultipleObjects( numEvents, hEvents, FALSE, INFINITE );

        // Process signal events
        switch ( nEventIdx )
        {
            case WAIT_TIMEOUT:
                continue;

            // If the stop event, stop looping and exit
            case WAIT_OBJECT_0:
                continueProcessing = false;
                continue;

            case WAIT_OBJECT_0 + 1:
                Nui_GotDepthAlert();
                 ++m_DepthFramesTotal;
                break;

            case WAIT_OBJECT_0 + 2:
                Nui_GotVideoAlert();
                break;

            case WAIT_OBJECT_0 + 3:
                Nui_GotSkeletonAlert( );
                break;
        }

		//toc();
        // Once per second, display the depth FPS
        //t = timeGetTime( );
        //if ( (t - m_LastDepthFPStime) > 1000 )
        //{
   //         int fps = ((m_DepthFramesTotal - m_LastDepthFramesTotal) * 1000 + 500) / (t - m_LastDepthFPStime);
   //         m_LastDepthFramesTotal = m_DepthFramesTotal;
   //         m_LastDepthFPStime = t;
			//printf("fps: %d\n",fps);
        //}

    }

    return 0;
}



void KinectSensor::Nui_GotVideoAlert( )
{
    NUI_IMAGE_FRAME imageFrame;

    HRESULT hr = m_pNuiSensor->NuiImageStreamGetNextFrame(
        m_pVideoStreamHandle,
        0,
        &imageFrame );

    INuiFrameTexture * pTexture = imageFrame.pFrameTexture;
    NUI_LOCKED_RECT LockedRect;
    pTexture->LockRect( 0, &LockedRect, NULL, 0 );
    if( LockedRect.Pitch != 0 )
    {
        BYTE * pBuffer = (BYTE*) LockedRect.pBits;

		cvSetData( image, (BYTE*) pBuffer, image->widthStep);

		//if (bFoundSkeleton)
		//{
		//	cvLine( image, cvPoint(m_Points[NUI_SKELETON_POSITION_HEAD].x,m_Points[NUI_SKELETON_POSITION_HEAD].y), cvPoint(m_Points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x,m_Points[NUI_SKELETON_POSITION_SHOULDER_CENTER].y), CV_RGB(0,255,0), 3);
		//	cvLine( image, cvPoint(m_Points[NUI_SKELETON_POSITION_HIP_CENTER].x,m_Points[NUI_SKELETON_POSITION_HIP_CENTER].y), cvPoint(m_Points[NUI_SKELETON_POSITION_SPINE].x,m_Points[NUI_SKELETON_POSITION_SPINE].y), CV_RGB(0,255,0), 3);
		//	cvLine( image, cvPoint(m_Points[NUI_SKELETON_POSITION_SPINE].x,m_Points[NUI_SKELETON_POSITION_SPINE].y), cvPoint(m_Points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x,m_Points[NUI_SKELETON_POSITION_SHOULDER_CENTER].y), CV_RGB(0,255,0), 3);
		//	cvLine( image, cvPoint(m_Points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x,m_Points[NUI_SKELETON_POSITION_SHOULDER_CENTER].y), cvPoint(m_Points[NUI_SKELETON_POSITION_SHOULDER_LEFT].x,m_Points[NUI_SKELETON_POSITION_SHOULDER_LEFT].y), CV_RGB(0,255,0), 3);
		//	cvLine( image, cvPoint(m_Points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x,m_Points[NUI_SKELETON_POSITION_SHOULDER_CENTER].y), cvPoint(m_Points[NUI_SKELETON_POSITION_SHOULDER_RIGHT].x,m_Points[NUI_SKELETON_POSITION_SHOULDER_RIGHT].y), CV_RGB(0,255,0), 3);
		//	cvLine( image, cvPoint(m_Points[NUI_SKELETON_POSITION_SHOULDER_RIGHT].x,m_Points[NUI_SKELETON_POSITION_SHOULDER_RIGHT].y), cvPoint(m_Points[NUI_SKELETON_POSITION_ELBOW_RIGHT].x,m_Points[NUI_SKELETON_POSITION_ELBOW_RIGHT].y), CV_RGB(0,255,0), 3);
		//	cvLine( image, cvPoint(m_Points[NUI_SKELETON_POSITION_ELBOW_RIGHT].x,m_Points[NUI_SKELETON_POSITION_ELBOW_RIGHT].y), cvPoint(m_Points[NUI_SKELETON_POSITION_WRIST_RIGHT].x,m_Points[NUI_SKELETON_POSITION_WRIST_RIGHT].y), CV_RGB(0,255,0), 3);
		//	cvLine( image, cvPoint(m_Points[NUI_SKELETON_POSITION_WRIST_RIGHT].x,m_Points[NUI_SKELETON_POSITION_WRIST_RIGHT].y), cvPoint(m_Points[NUI_SKELETON_POSITION_HAND_RIGHT].x,m_Points[NUI_SKELETON_POSITION_HAND_RIGHT].y), CV_RGB(0,255,0), 3);
		//	cvLine( image, cvPoint(m_Points[NUI_SKELETON_POSITION_SHOULDER_LEFT].x,m_Points[NUI_SKELETON_POSITION_SHOULDER_LEFT].y), cvPoint(m_Points[NUI_SKELETON_POSITION_ELBOW_LEFT].x,m_Points[NUI_SKELETON_POSITION_ELBOW_LEFT].y), CV_RGB(0,255,0), 3);
		//	cvLine( image, cvPoint(m_Points[NUI_SKELETON_POSITION_ELBOW_LEFT].x,m_Points[NUI_SKELETON_POSITION_ELBOW_LEFT].y), cvPoint(m_Points[NUI_SKELETON_POSITION_WRIST_LEFT].x,m_Points[NUI_SKELETON_POSITION_WRIST_LEFT].y), CV_RGB(0,255,0), 3);
		//	cvLine( image, cvPoint(m_Points[NUI_SKELETON_POSITION_WRIST_LEFT].x,m_Points[NUI_SKELETON_POSITION_WRIST_LEFT].y), cvPoint(m_Points[NUI_SKELETON_POSITION_HAND_LEFT].x,m_Points[NUI_SKELETON_POSITION_HAND_LEFT].y), CV_RGB(0,255,0), 3);

		//	for (int i = 0; i < NUI_SKELETON_POSITION_HIP_LEFT; i++)
		//		cvCircle( image, cvPoint(m_Points[i].x,m_Points[i].y), 7, g_JointColorTable[i], -1);
		//}

		cvShowImage( "image", image);
		cvWaitKey(1);
    }

    hr= m_pNuiSensor->NuiImageStreamReleaseFrame( m_pVideoStreamHandle, &imageFrame );
}


void KinectSensor::Nui_GotDepthAlert( )
{
	NUI_IMAGE_FRAME imageFrame;

	HRESULT hr = m_pNuiSensor->NuiImageStreamGetNextFrame(
		m_pDepthStreamHandle,
		0,
		&imageFrame );

	INuiFrameTexture * pTexture = imageFrame.pFrameTexture;
	NUI_LOCKED_RECT LockedRect;
	pTexture->LockRect( 0, &LockedRect, NULL, 0 );
	if( LockedRect.Pitch != 0 )
	{
		//printf("%d\n",imageFrame.eResolution);
		BYTE * pBuffer = (BYTE*) LockedRect.pBits;
		USHORT * pBuff = (USHORT*) LockedRect.pBits;
		//pBuff = (USHORT*) LockedRect.pBits;
		////bdep = true;
		int ind = 0;
		//POINT uv;
		for(int i=0;i<480;i++)
		{
			for(int j=0;j<640;j++)
			{
				ind = i*640+j;
				BYTE index = pBuff[ind]&0x07;
				USHORT realDepth = (pBuff[ind]&0xFFF8)>>3;

				//Vector4 aa = NuiTransformDepthImageToSkeleton( j, i, realDepth, NUI_IMAGE_RESOLUTION_640x480);
				//hr = NuiImageGetColorPixelCoordinatesFromDepthPixel( NUI_IMAGE_RESOLUTION_640x480, 0, j, i, realDepth, &uv.x, &uv.y);
				//uv.x *= 2;
				//uv.y *= 2;
				//PointCloudUV[i*640+j].u = uv.x;
				//PointCloudUV[i*640+j].v = uv.y;
				//PointCloudUV[i*640+j].x = aa.x;
				//PointCloudUV[i*640+j].y = aa.y;
				//PointCloudUV[i*640+j].z = aa.z;
				//cout << aa.x << endl;
				//printf("%f %f %f\n",aa.x,aa.y,aa.z);

				//http://msdn.microsoft.com/zh-tw/hh367958
				BYTE scale = 255 - (BYTE)(256*realDepth/0x0fff);
				buf[3*ind] = 0; 
				buf[3*ind+1] = 0;
				buf[3*ind+2] = 0;

				switch( index )
				{
				case 0:
					buf[3*ind]=scale/2;
					buf[3*ind+1]=scale/2;
					buf[3*ind+2]=scale/2;
					break;
				case 1:
					buf[3*ind]=scale;
					break;
				case 2:
					buf[3*ind+1]=scale;
					break;
				case 3:
					buf[3*ind+2]=scale;
					break;
				case 4:
					buf[3*ind]=scale;
					buf[3*ind+1]=scale;
					break;
				case 5:
					buf[3*ind]=scale;
					buf[3*ind+2]=scale;
					break;
				case 6:
					buf[3*ind+1]=scale;
					buf[3*ind+2]=scale;
					break;
				case 7:
					buf[3*ind]=255-scale/2;
					buf[3*ind+1]=255-scale/2;
					buf[3*ind+2]=255-scale/2;
					break;
				}
			}
		}
		cvSetData( depthimg, buf, 640*3);


		if (bFoundSkeleton)
		{
			cvLine( depthimg, cvPoint(m_Points_dep[NUI_SKELETON_POSITION_HEAD].x,m_Points_dep[NUI_SKELETON_POSITION_HEAD].y), cvPoint(m_Points_dep[NUI_SKELETON_POSITION_SHOULDER_CENTER].x,m_Points_dep[NUI_SKELETON_POSITION_SHOULDER_CENTER].y), CV_RGB(0,255,0), 3);
			cvLine( depthimg, cvPoint(m_Points_dep[NUI_SKELETON_POSITION_HIP_CENTER].x,m_Points_dep[NUI_SKELETON_POSITION_HIP_CENTER].y), cvPoint(m_Points_dep[NUI_SKELETON_POSITION_SPINE].x,m_Points_dep[NUI_SKELETON_POSITION_SPINE].y), CV_RGB(0,255,0), 3);
			cvLine( depthimg, cvPoint(m_Points_dep[NUI_SKELETON_POSITION_SPINE].x,m_Points_dep[NUI_SKELETON_POSITION_SPINE].y), cvPoint(m_Points_dep[NUI_SKELETON_POSITION_SHOULDER_CENTER].x,m_Points_dep[NUI_SKELETON_POSITION_SHOULDER_CENTER].y), CV_RGB(0,255,0), 3);
			cvLine( depthimg, cvPoint(m_Points_dep[NUI_SKELETON_POSITION_SHOULDER_CENTER].x,m_Points_dep[NUI_SKELETON_POSITION_SHOULDER_CENTER].y), cvPoint(m_Points_dep[NUI_SKELETON_POSITION_SHOULDER_LEFT].x,m_Points_dep[NUI_SKELETON_POSITION_SHOULDER_LEFT].y), CV_RGB(0,255,0), 3);
			cvLine( depthimg, cvPoint(m_Points_dep[NUI_SKELETON_POSITION_SHOULDER_CENTER].x,m_Points_dep[NUI_SKELETON_POSITION_SHOULDER_CENTER].y), cvPoint(m_Points_dep[NUI_SKELETON_POSITION_SHOULDER_RIGHT].x,m_Points_dep[NUI_SKELETON_POSITION_SHOULDER_RIGHT].y), CV_RGB(0,255,0), 3);
			cvLine( depthimg, cvPoint(m_Points_dep[NUI_SKELETON_POSITION_SHOULDER_RIGHT].x,m_Points_dep[NUI_SKELETON_POSITION_SHOULDER_RIGHT].y), cvPoint(m_Points_dep[NUI_SKELETON_POSITION_ELBOW_RIGHT].x,m_Points_dep[NUI_SKELETON_POSITION_ELBOW_RIGHT].y), CV_RGB(0,255,0), 3);
			cvLine( depthimg, cvPoint(m_Points_dep[NUI_SKELETON_POSITION_ELBOW_RIGHT].x,m_Points_dep[NUI_SKELETON_POSITION_ELBOW_RIGHT].y), cvPoint(m_Points_dep[NUI_SKELETON_POSITION_WRIST_RIGHT].x,m_Points_dep[NUI_SKELETON_POSITION_WRIST_RIGHT].y), CV_RGB(0,255,0), 3);
			cvLine( depthimg, cvPoint(m_Points_dep[NUI_SKELETON_POSITION_WRIST_RIGHT].x,m_Points_dep[NUI_SKELETON_POSITION_WRIST_RIGHT].y), cvPoint(m_Points_dep[NUI_SKELETON_POSITION_HAND_RIGHT].x,m_Points_dep[NUI_SKELETON_POSITION_HAND_RIGHT].y), CV_RGB(0,255,0), 3);
			cvLine( depthimg, cvPoint(m_Points_dep[NUI_SKELETON_POSITION_SHOULDER_LEFT].x,m_Points_dep[NUI_SKELETON_POSITION_SHOULDER_LEFT].y), cvPoint(m_Points_dep[NUI_SKELETON_POSITION_ELBOW_LEFT].x,m_Points_dep[NUI_SKELETON_POSITION_ELBOW_LEFT].y), CV_RGB(0,255,0), 3);
			cvLine( depthimg, cvPoint(m_Points_dep[NUI_SKELETON_POSITION_ELBOW_LEFT].x,m_Points_dep[NUI_SKELETON_POSITION_ELBOW_LEFT].y), cvPoint(m_Points_dep[NUI_SKELETON_POSITION_WRIST_LEFT].x,m_Points_dep[NUI_SKELETON_POSITION_WRIST_LEFT].y), CV_RGB(0,255,0), 3);
			cvLine( depthimg, cvPoint(m_Points_dep[NUI_SKELETON_POSITION_WRIST_LEFT].x,m_Points_dep[NUI_SKELETON_POSITION_WRIST_LEFT].y), cvPoint(m_Points_dep[NUI_SKELETON_POSITION_HAND_LEFT].x,m_Points_dep[NUI_SKELETON_POSITION_HAND_LEFT].y), CV_RGB(0,255,0), 3);

			for (int i = 0; i < NUI_SKELETON_POSITION_HIP_LEFT; i++)
				cvCircle( depthimg, cvPoint(m_Points_dep[i].x,m_Points_dep[i].y), 7, g_JointColorTable[i], -1);
		}


		// draw the bits to the bitmap
		//RGBQUAD * rgbrun = m_rgbWk;
		//USHORT * pBufferRun = (USHORT*) pBuffer;
		//for( int y = 0 ; y < 240 ; y++ )
		//{
		//    for( int x = 0 ; x < 320 ; x++ )
		//    {
		//        RGBQUAD quad = Nui_ShortToQuad_Depth( *pBufferRun );
		//        pBufferRun++;
		//        *rgbrun = quad;
		//        rgbrun++;
		//    }
		//}
		//cvSetData( depthimg, (BYTE*)rgbrun->rgbBlue, depthimg->widthStep);


		cvShowImage( "depthimg", depthimg);
		//cvShowImage( "depthcol", depthcol);
		cvWaitKey(1);

		//m_DrawDepth.DrawFrame( (BYTE*) m_rgbWk );
    }

    hr = m_pNuiSensor->NuiImageStreamReleaseFrame( m_pDepthStreamHandle, &imageFrame );
}


RGBQUAD KinectSensor::Nui_ShortToQuad_Depth( USHORT s )
{
    USHORT RealDepth = NuiDepthPixelToDepth(s);
    USHORT Player    = NuiDepthPixelToPlayerIndex(s);

    // transform 13-bit depth information into an 8-bit intensity appropriate
    // for display (we disregard information in most significant bit)
    BYTE intensity = (BYTE)~(RealDepth >> 4);

    // tint the intensity by dividing by per-player values
    RGBQUAD color;
    color.rgbRed   = intensity >> g_IntensityShiftByPlayerR[Player];
    color.rgbGreen = intensity >> g_IntensityShiftByPlayerG[Player];
    color.rgbBlue  = intensity >> g_IntensityShiftByPlayerB[Player];

    return color;
}

void KinectSensor::Nui_GotSkeletonAlert( )
{
    //NUI_SKELETON_FRAME SkeletonFrame = {0};
	bool ready = false;
    bFoundSkeleton = false;

    if( SUCCEEDED(m_pNuiSensor->NuiSkeletonGetNextFrame( 0, &SkeletonFrame )) )
    {
		double minDist = DBL_MAX;
		for( int i = 0 ; i < NUI_SKELETON_COUNT ; i++ )
		{
			if( SkeletonFrame.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKED /*||
			(SkeletonFrame.SkeletonData[i].eTrackingState == NUI_SKELETON_POSITION_ONLY && m_bAppTracking)*/)
			{
				ready = true;
				double tmpDist = sqrt( SkeletonFrame.SkeletonData[i].Position.x*SkeletonFrame.SkeletonData[i].Position.x+
					                                 SkeletonFrame.SkeletonData[i].Position.y*SkeletonFrame.SkeletonData[i].Position.y+
													 SkeletonFrame.SkeletonData[i].Position.z*SkeletonFrame.SkeletonData[i].Position.z);
				if (tmpDist < minDist)
				{
					minDist = tmpDist;
					num_person = i;
				}
			}
		}
	}
	// no skeletons!
    if( !ready )
    {
        return;
    }

	// smooth out the skeleton data
    HRESULT hr = m_pNuiSensor->NuiTransformSmooth(&SkeletonFrame,NULL);
    if ( FAILED(hr) )
    {
        return;
    }

	bool bSkeletonIdsChanged = false;
    for ( int i = 0 ; i < NUI_SKELETON_COUNT; i++ )
    {
        if ( m_SkeletonIds[i] != SkeletonFrame.SkeletonData[i].dwTrackingID )
        {
            m_SkeletonIds[i] = SkeletonFrame.SkeletonData[i].dwTrackingID;
            bSkeletonIdsChanged = true;
        }
	}
	
	//memset( HumanBody, 0, sizeof(HumanBody));

	if (ready)
	{
		USHORT depthq;
		POINT depxy;
		for (int i = 0; i < NUI_SKELETON_POSITION_HIP_LEFT; i++)
		{
			NuiTransformSkeletonToDepthImage( SkeletonFrame.SkeletonData[num_person].SkeletonPositions[i], &m_Points_dep[i].x, &m_Points_dep[i].y, &depthq );

			hr = NuiImageGetColorPixelCoordinatesFromDepthPixel( NUI_IMAGE_RESOLUTION_640x480, 0, m_Points_dep[i].x, m_Points_dep[i].y, depthq, &m_Points[i].x, &m_Points[i].y);

			//m_Points_dep[i].x = (m_Points_dep[i].x * 640) / 320;
			//m_Points_dep[i].y = (m_Points_dep[i].y * 480) / 240;

			m_Points_dep[i].x *= 2;
			m_Points_dep[i].y *= 2;
		}


		memcpy( HumanBody, SkeletonFrame.SkeletonData[num_person].SkeletonPositions, sizeof(SkeletonFrame.SkeletonData[num_person].SkeletonPositions));
		bFoundSkeleton = true;
	}
}