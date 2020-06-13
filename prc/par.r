/*
 * GWXLib.r
 *
 * Author: 	Tomi KankaanpŠŠ (Tomi.Kankaanpaa@handwise.com)
 * Date:	10.3.2000
 */

#ifndef _GWXLIB_R_
#define	_GWXLIB_R_

#include "Types.r"


// Version resource

#define kMajorRev			0x0
#define kMinorRev			0x5
#define kStage				development
#define kStageLevel			0x0
#define kShortStr			"00.05"
#define kLongOneStr			"00.05"
#define kLongTwoStr			""


resource 'vers' (1) 
{
	kMajorRev,
	kMinorRev,
	kStage,
	kStageLevel,
	verUS,
	kShortStr,
	kLongOneStr
};

resource 'vers' (2) 
{
	kMajorRev,
	kMinorRev,
	kStage,
	kStageLevel,
	verUS,
	kShortStr,
	kLongTwoStr
};


#endif // _GWXLIB_R_
