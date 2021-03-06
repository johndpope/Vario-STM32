// NmeaParserEx.h
//

#ifndef __NMEAPARSEREX_H__
#define __NMEAPARSEREX_H__

#include <DefaultSettings.h>
#include <Arduino.h>
#include <Stream.h>

#include <time.h>


/////////////////////////////////////////////////////////////////////////////
// class NmeaParserEx

class NmeaParserEx
{
public:
	NmeaParserEx(Stream & stm);
	
public:
	void				update(/*float baroAlt*/);
	
	int					available();
	int					read();
	
	int					availableIGC();
	int					readIGC();
	
	void				reset();
	
	//
	#if 0
	uint32_t			getDate();  	// DDMMYY
	uint32_t			getTime();		// HHMMSS
	#endif
	time_t				getDateTime();
	float				getLatitude();
	float				getLongitude();
	float				getAltitude();
	uint32_t			getSpeed();
	uint32_t			getHeading();
	
private:
	int					isFull() 	{ return ((mWrite + 1) % MAX_NMEA_PARSER_BUFFER) == mTail; }
	int					isEmpty()	{ return mHead == mTail; }
	
	void				parseField(int fieldIndex, int startPos);
	
	float				strToFloat(int startPos);
	long				strToNum(int startPos);
	
	long				floatToCoordi(float value);
	
private:
	//
	Stream &			mStream;
	char				mBuffer[MAX_NMEA_PARSER_BUFFER];
	volatile int		mHead;
	volatile int		mTail;
	volatile int		mWrite;
	volatile int		mFieldStart;
	volatile int		mFieldIndex;
	
	//
	volatile int		mParseStep; 		// -1 or 0 ~ 12
	volatile uint8_t	mParseState;		// 
	volatile uint8_t	mParity;
	
	//
	//uint32_t			mDate;
	//uint32_t			mTime;
	
	struct tm 			mTmStruct;
	time_t				mDateTime;
	
	float				mLatitude;
	float				mLongitude;
	float				mAltitude;
	uint32_t			mSpeed;
	uint32_t			mHeading;
	
//	float				mBaroAlt;
	
	// IGC sentence
	char				mIGCSentence[MAX_IGC_SENTENCE+1];
	volatile int		mIGCNext;	// next = 0 ~ MAX_XXX -1 -> available
	volatile int		mIGCSize;	// size = 0 -> empty, size = MAX_xx -> valid
};


// inline functions
//

#if 0
inline uint32_t NmeaParserEx::getDate()
	{ return mDate; }
	
inline uint32_t	NmeaParserEx::getTime()
	{ return mTime; }
#endif
	
inline time_t	NmeaParserEx::getDateTime()
	{ return mDateTime; }
	
inline float NmeaParserEx::getLatitude()
	{ return mLatitude; }
	
inline float NmeaParserEx::getLongitude()
	{ return mLongitude; }
	
inline float NmeaParserEx::getAltitude()
	{ return mAltitude; }
	
inline uint32_t NmeaParserEx::getSpeed()
	{ return mSpeed; }
	
inline uint32_t NmeaParserEx::getHeading()
	{ return mHeading; }
	

#endif // __NMEAPARSEREX_H__
