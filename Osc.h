///////////////////////////////////////////////////////////////////////////////////////////
#ifndef osc_h
#define osc_h
#include "audiostream.h"
#include "arm_math.h"
#include "utility/dspinst.h"
///////////////////////////////////////////////////////////////////////////////////////////
extern const float nMidiFrequency[];
extern const int16_t sinTable[];
extern const int16_t triTable[14][1025];
extern const int16_t sqrTable[14][1025];
extern const int16_t sawTable[14][1025];

#define selectSinTable 0
#define selectTriTable 1
#define selectSqrTable 2
#define selectSawTable 3
///////////////////////////////////////////////////////////////////////////////////////////
class AudioSynthWaveformOsc : public AudioStream
{
public:
    AudioSynthWaveformOsc() : AudioStream(1, inputQueueArray), magnitude(16384) {}
    uint32_t f2phaseInc(float freq);

    void setNote(int note, boolean glide);
    void amplitude(float n) 
    {
	if (n < 0) n = 0;
	else if (n > 1.0) n = 1.0;
	magnitude = n * 65536.0;
    }
    virtual void update(void);
    void toggleWaveTable();
    void setWaveTable(int newTable);
    void setPitchBend(int cents);
    void setfDetune(int cents);
    void setcDetune(int notes);
    void setGlideTime(float mSeconds);
    uint32_t phaseIncGlide(int num_samples);
    void setFMAmount(float value);
    void setRMAmount(float value);
    void setDetunatorAmount(int value);
private:
    void switchTable(uint8_t order, uint8_t table);
    
    uint32_t phase;
    uint32_t phaseInc;
    float fromNoteFrequency;//frequency from previous note
    float noteFrequency;//current frequency
    
    uint32_t noteFrequency_phaseInc;
    uint32_t fromNoteFrequency_phaseInc;
    
    int midiNote;
    int fromMidiNote;
    
    //glide
    boolean glide;
    uint32_t glideStep;
    int32_t glideAccu;
    int32_t phaseIncNote;
    int32_t phaseIncFromNote;
    int32_t magnitude;
    int16_t fmAmount;
    int16_t detunatorAmount;
    uint32_t detunatorMul[6];//multiply the inc with this value >>31
    uint32_t phaseD[6];
    int fDetune = 0;//fine (don't believe comments when they say they are fine)
    int cDetune = 0;//coarse
    int pitchBend = 0;
    int fModulation = 0;//works the same as fDetune
    int selectTable;
    int orderIndex;//which LUT to use
    const int16_t *pWaveTable = sinTable;
    audio_block_t *inputQueueArray[1];//for phase or ring modulation
};

#endif
