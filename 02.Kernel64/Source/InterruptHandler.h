#ifndef __INTERRUPTHANDLER_H__
#define __INTERRUPTHANDLER_H__

#include "Types.h"

////////////////////////////////////////////////////////////////////////////////
//
// ÇÔ¼ö
//
////////////////////////////////////////////////////////////////////////////////
void kCommonExceptionHandler( int iVectorNumber, QWORD qwErrorCode );
void kCommonInterruptHandler( int iVectorNumber );
void kKeyboardHandler( int iVectorNumber );
void kTimerHandler( int iVectorNumber );

#endif /*__INTERRUPTHANDLER_H__*/
