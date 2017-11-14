// TonePlayer.h
//

#ifndef __TONEPLAYER_H__
#define __TONEPLAYER_H__

#include <Arduino.h>
#include <ToneGenerator.h>


////////////////////////////////////////////////////////////////////////////////////
//

#define PLAY_NONE				(0)
#define PLAY_MUTE				(1)
#define PLAY_BEEP				(2)
#define PLAY_MELODY				(3)

#define SOUND_MUTE				(1)
#define SOUND_BEEP				(2)
#define SOUND_MELODY			(3)


#define GAP_BETWEEN_TONE		(1)		// 1ms

#define MIN_VOLUME				(0)
#define MAX_VOLUME				(98)
#define MID_VOLUME				(8)
#define KEY_VOLUME				(6)

#define PLAY_COOPERATIVE		(0)
#define PLAY_PREEMPTIVE			(1)


////////////////////////////////////////////////////////////////////////////////////
//

struct Tone 
{
	int			freq;
	int			length;
};

struct PlayContext
{
	//
	Tone *			tonePtr;
	int				toneCount;
	
	//
	int				playVolume;
	int				playType;
	
	//
	int				repeatCount;
};


////////////////////////////////////////////////////////////////////////////////////
//

class TonePlayer 
{
public:
	TonePlayer(ToneGenerator & gen);
	
public:	
	void			setMelody(Tone * tonePtr, int toneCount, int repeat = 1, int preemptive = 1, int volume = -1);
	void			setMelodyEx(Tone * tonePtr, int toneCount, int volume = -1);
	void			setBeep(int freq, int period, int duty, int repeat = 0, int volume = -1); // duty --> ms, <= period
	void			setMute(int preemptive = 1);
	
	void			setTone(int freq = 0, int volume = -1);
	void			setVolume(int value);
	
	int				getVolume();
	
	void			update();
	
	// synchronous function
	void			beep(uint32_t freq, uint32_t duration, int count = 1);

private:
	int				playCheck();
	void			playNext();

public:
	//
	PlayContext		playTone;
	PlayContext		nextTone;
	
	/*
	//
	Sound *			soundPlayPtr;
	int				soundPlayCount;
	int				soundPlayType;

	//
	Sound *			soundNextPtr;
	int				soundNextCount;
	int				soundNextType;
	*/

	//
	int				toneIndex;
	unsigned long	toneStartTick;
	//
	int				playCount;
	
	//
	int				playVolume;
	
	//
	ToneGenerator &	toneGen;
};

// inline members
inline int TonePlayer::getVolume()
	{ return playVolume; }

#endif // __TONEPLAYER_H__
