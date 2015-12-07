/*=============================================================================
                   National Instruments / NI SoftMotion                             
  ----------------------------------------------------------------------------  
      Copyright (c) National Instruments 2012.  All Rights Reserved.            
  ----------------------------------------------------------------------------  
                                                                                
   Title:       nismCInterface.h                                                
   Purpose:     Include file for NI SoftMotion C Interface Support
                                                                                
=============================================================================*/

#ifndef ___nismCInterface_h___
#define ___nismCInterface_h___
#ifdef __cplusplus
	extern "C" {
#endif
/*-----------------------------------------------------------------------------
   File Management Defines
-----------------------------------------------------------------------------*/
#ifdef NISM_C_INTERFACE_EXPORT
   #ifdef __GNUC__
      #define __nismExport __attribute__((section(".export"))) long
      #define __nismExportVoid __attribute__((section(".export"))) void
   #else
      #define __nismExport long __declspec(dllexport) __stdcall 
      #define __nismExportVoid void __declspec(dllexport) __stdcall 
   #endif
#else
   #ifdef __GNUC__
      #define __nismExport long
      #define __nismExportVoid void
   #else
      #define __nismExport long __stdcall 
      #define __nismExportVoid void __stdcall 
   #endif
#endif

/* Type definitions file */
#include "nismCInterfaceTypes.h"

/*-----------------------------------------------------------------------------
   Functions
-----------------------------------------------------------------------------*/

__nismExport nismInitialize();

__nismExport nismCreateOperation(nism_Handle* handle, nism_OperationType operationType);
__nismExport nismDestroyOperation(nism_Handle* handle);

__nismExport nismOpenResource(nism_Handle* handle, const char* resourceName);
__nismExport nismCloseResource(nism_Handle* handle);

__nismExport nismSetResource(nism_Handle handle, const char* resourceName);

__nismExport nismExecute(nism_Handle handle);
__nismExport nismSolveProfile(nism_Handle moveHandle, nism_Handle profileHandle);
__nismExport nismStop(nism_Handle handle);
__nismExport nismCommit(nism_Handle handle);

__nismExport nismCreateBatch(nism_Handle handle, unsigned long batchSize, nism_Handle* batchHandle);
__nismExport nismExecuteBatch(nism_Handle handle, nism_Handle batchHandle);
__nismExport nismDestroyBatch(nism_Handle handle, nism_Handle* batchHandle);

__nismExport nismSetPropString(nism_Handle handle, nism_PropString property, const char* string);
__nismExport nismSetPropDouble(nism_Handle handle, nism_PropDouble property, double data);
__nismExport nismSetPropLong(nism_Handle handle, nism_PropLong property, long data);
__nismExport nismSetPropBool(nism_Handle handle, nism_PropBool property, unsigned char data); 
__nismExport nismSetPropDoubleArray(nism_Handle handle, nism_PropDoubleArray property, const double* dataArray, unsigned long sizeOfDataArray);
__nismExport nismSetPropDoubleArray2D(nism_Handle handle, nism_PropDoubleArray2D property, const double* dataArray2D, unsigned long sizeOfDataArray1, unsigned long sizeOfDataArray2);

__nismExport nismGetPropString(nism_Handle handle, nism_PropString property, char* string, unsigned long sizeOfString, unsigned long* fetched);
__nismExport nismGetPropDouble(nism_Handle handle, nism_PropDouble property, double* data);
__nismExport nismGetPropLong(nism_Handle handle, nism_PropLong property, long* data);
__nismExport nismGetPropBool(nism_Handle handle, nism_PropBool property, unsigned char* data);
__nismExport nismGetPropDoubleArray(nism_Handle handle, nism_PropDoubleArray property, double* dataArray, unsigned long sizeOfDataArray, unsigned long* fetched);
__nismExport nismGetPropDoubleArray2D(nism_Handle handle, nism_PropDoubleArray2D property, double* dataArray2D, unsigned long sizeOfDataArray1, unsigned long sizeOfDataArray2, unsigned long* fetched1, unsigned long* fetched2);

__nismExport nismWriteDataDouble(nism_Handle handle, nism_WriteDataDouble property, double data);
__nismExport nismWriteDataBool(nism_Handle handle, nism_WriteDataBool property, unsigned char data);
__nismExport nismWriteDataDoubleArray(nism_Handle handle, nism_WriteDataDoubleArray property, const double* dataArray, unsigned long sizeOfDataArray);
__nismExport nismWriteDataDoubleArray2D(nism_Handle handle, nism_WriteDataDoubleArray2D property, const double* dataArray2D, unsigned long sizeOfDataArray1, unsigned long sizeOfDataArray2);

__nismExport nismReadDataDouble(nism_Handle handle, nism_ReadDataDouble property, double* data);
__nismExport nismReadDataLong(nism_Handle handle, nism_ReadDataLong property, long* data);
__nismExport nismReadDataBool(nism_Handle handle, nism_ReadDataBool property, unsigned char* data); 
__nismExport nismReadDataDoubleArray(nism_Handle handle, nism_ReadDataDoubleArray property, double* dataArray, unsigned long sizeOfDataArray, unsigned long* fetched);
__nismExport nismReadDataLongArray(nism_Handle handle, nism_ReadDataLongArray property, long* dataArray, unsigned long sizeOfDataArray, unsigned long* fetched);

__nismExport nismAxisClearFaults(nism_Handle handle);
__nismExport nismAxisPower(nism_Handle handle, unsigned char powerEnableAxis, unsigned char powerEnableDrive);
__nismExport nismAxisResetPosition(nism_Handle handle, double position);
__nismExport nismCoordinateResetPosition(nism_Handle handle, const double* positionArray, unsigned long sizeOfPositionArray);
__nismExport nismAxisReadDigitalLine(nism_Handle handle, long line, unsigned char* lineData);
__nismExport nismAxisWriteDigitalLine(nism_Handle handle, long line, unsigned char lineData);
__nismExport nismAxisSwitchGainSet(nism_Handle handle, long activeGainSet);

__nismExport nismReadStatus(nism_Handle handle, nism_Status property, unsigned char* data);
__nismExport nismGetErrorDescription(long errorCode, char* errorDescription, int sizeOfErrorDescription);
__nismExport nismGetLastError(nism_Handle handle,  long* errorCode, char* errorDescription, int sizeOfErrorDescription, char* location, int sizeOfLocation);

#ifdef __cplusplus
   }
#endif
 
#endif /* ___nismCInterface_h___*/

