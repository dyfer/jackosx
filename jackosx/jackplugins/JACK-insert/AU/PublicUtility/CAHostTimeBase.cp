/*	Copyright: 	� Copyright 2002 Apple Computer, Inc. All rights reserved.

	Disclaimer:	IMPORTANT:  This Apple software is supplied to you by Apple Computer, Inc.
			("Apple") in consideration of your agreement to the following terms, and your
			use, installation, modification or redistribution of this Apple software
			constitutes acceptance of these terms.  If you do not agree with these terms,
			please do not use, install, modify or redistribute this Apple software.

			In consideration of your agreement to abide by the following terms, and subject
			to these terms, Apple grants you a personal, non-exclusive license, under Apple�s
			copyrights in this original Apple software (the "Apple Software"), to use,
			reproduce, modify and redistribute the Apple Software, with or without
			modifications, in source and/or binary forms; provided that if you redistribute
			the Apple Software in its entirety and without modifications, you must retain
			this notice and the following text and disclaimers in all such redistributions of
			the Apple Software.  Neither the name, trademarks, service marks or logos of
			Apple Computer, Inc. may be used to endorse or promote products derived from the
			Apple Software without specific prior written permission from Apple.  Except as
			expressly stated in this notice, no other rights or licenses, express or implied,
			are granted by Apple herein, including but not limited to any patent rights that
			may be infringed by your derivative works or by other works in which the Apple
			Software may be incorporated.

			The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO
			WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED
			WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
			PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR IN
			COMBINATION WITH YOUR PRODUCTS.

			IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR
			CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
			GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
			ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION AND/OR DISTRIBUTION
			OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF CONTRACT, TORT
			(INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN
			ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/*=============================================================================
	CAHostTimeBase.cp

=============================================================================*/

//=============================================================================
//	Includes
//=============================================================================

#include "CAHostTimeBase.h"

Float64	CAHostTimeBase::sFrequency = 0;
UInt32	CAHostTimeBase::sMinDelta = 0;
UInt32	CAHostTimeBase::sToNanosNumerator = 0;
UInt32	CAHostTimeBase::sToNanosDenominator = 0;
UInt32	CAHostTimeBase::sFromNanosNumerator = 0;
UInt32	CAHostTimeBase::sFromNanosDenominator = 0;
bool	CAHostTimeBase::sUseMicroseconds = false;
bool	CAHostTimeBase::sIsInited = false;
#if !TARGET_API_MAC_OSX || Track_Host_TimeBase
UInt64	CAHostTimeBase::sLastTime = 0;
#endif

//=============================================================================
//	CAHostTimeBase
//
//	This class provides platform independant access to the host's time base.
//=============================================================================

void	CAHostTimeBase::Initialize()
{
	//	get the info about Absolute time
	#if !TARGET_API_MAC_OSX
		#if	TARGET_OS_MAC
			//	first check to see if UpTime is around
			if(UpTime != NULL)
			{
				GetTimeBaseInfo(&sMinDelta, &sToNanosNumerator, &sToNanosDenominator, &sFromNanosNumerator, &sFromNanosDenominator);
			}
			else
			{
				//	no UpTime, so fall back to Microseconds
				sUseMicroseconds = true;
				sMinDelta = 1;
				sToNanosNumerator = 1000;
				sToNanosDenominator = 1;
				sFromNanosNumerator = sToNanosDenominator;
				sFromNanosDenominator = sToNanosDenominator;
			}
		#else
			sUseMicroseconds = true;
			sMinDelta = 1;
			sToNanosNumerator = 1000;
			sToNanosDenominator = 1;
			sFromNanosNumerator = sToNanosDenominator;
			sFromNanosDenominator = sToNanosDenominator;
		#endif
	#else
		struct mach_timebase_info	theTimeBaseInfo;
		mach_timebase_info(&theTimeBaseInfo);
		sMinDelta = 1;
		sToNanosNumerator = theTimeBaseInfo.numer;
		sToNanosDenominator = theTimeBaseInfo.denom;
		sFromNanosNumerator = sToNanosDenominator;
		sFromNanosDenominator = sToNanosNumerator;
	#endif

	//	the frequency of that clock is: (sToNanosDenominator / sToNanosNumerator) * 10^9
	sFrequency = static_cast<Float64>(sToNanosDenominator) / static_cast<Float64>(sToNanosNumerator);
	sFrequency *= 1000000000.0;
	
	#if	Log_Host_Time_Base_Parameters
		DebugMessage(  "Host Time Base Parameters");
		DebugMessageN1(" Minimum Delta:          %lu", sMinDelta);
		DebugMessageN1(" Fequency:               %f", sFrequency);
		DebugMessageN1(" To Nanos Numerator:     %lu", sToNanosNumerator);
		DebugMessageN1(" To Nanos Denominator:   %lu", sToNanosDenominator);
		DebugMessageN1(" From Nanos Numerator:   %lu", sFromNanosNumerator);
		DebugMessageN1(" From Nanos Denominator: %lu", sFromNanosDenominator);
	#endif

	sIsInited = true;
}
