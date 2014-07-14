///////////////////////////////////////////////////////////////////////////////////////////
#ifndef lfo_h
#define lfo_h
#include "audiostream.h"
#include "arm_math.h"
#include "utility/dspinst.h"
///////////////////////////////////////////////////////////////////////////////////////////
extern const int16_t lfoSinTable[];
extern const int16_t lfoTriTable[257];
extern const int16_t lfoSqrTable[257];
extern const int16_t lfoSawTable[257];

#define selectSinTable 0
#define selectTriTable 1
#define selectSqrTable 2
#define selectSawTable 3

///////////////////////////////////////////////////////////////////////////////////////////
//lfo destination
//0 = frequentie osc1 
//1 = frequentie osc1 + osc2
//2 = amplitude aEnv
//3 = pulsewidth?
//4 = filter
extern const int lfoDest_Min;
extern const int lfoDest_Max;


extern const int lfoDest;
const int lfoDest_Osc1f = 0;
const int lfoDest_Osc12f = 1;
const int lfoDest_aEnv = 2;
const int lfoDest_PW = 3;
const int lfoDest_fFreq = 4;
///////////////////////////////////////////////////////////////////////////////////////////
class AudioSynthWaveformLfo : public AudioStream
{
public:
    AudioSynthWaveformLfo() : AudioStream(0, NULL), magnitude(16384) {}
    void frequency(float freq);
    void amplitude(float n) 
    {
	if (n < 0) n = 0;
	else if (n > 1.0) n = 1.0;
	magnitude = n * 65536.0;
    }
    virtual void update(void);
    void toggleWaveTable();
    void setWaveTable(int newTable);
    void setTarget(int target);
    void setDest(int newDest);
    void toggleDest();
    int getDest();
private:
    void switchTable(uint8_t table);  
    uint32_t phase;
    uint32_t phaseInc;
    int32_t magnitude;
    int selectTable;
    int dest;
    const int16_t *pWaveTable = lfoSinTable;
};

#endif
