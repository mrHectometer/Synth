#include "audiostream.h"
#include "arm_math.h"
#include "utility/dspinst.h"
#include "debugutils.h"
#include "spi.h"

enum filtertype
{
    FLT_BYPASS = 0,
    FLT_LOWPASS_12 = 1,
    FLT_HIGHPASS_12 = 2,
    FLT_BANDPASS_12 = 4,
    FLT_LOWPASS_24 = 8,
    FLT_HIGHASS_24 = 16,
    FLT_BANDPASS_24 = 32
};
class AudioSynthDigiFilter : public AudioStream
{
public:
    AudioSynthDigiFilter() : AudioStream(2, inputQueueArray) 
    {
      
    }
    virtual void update(void);
    void setFrequency(uint16_t value);//0 to 255
    void setResonance(int value);//0 to infinity (?)
    void setFreqPot1ChipSelect(int pin)
    {
        freqPot1ChipSelect = pin;
        pinMode (pin, OUTPUT);
    }
    void setFreqPot2ChipSelect(int pin)
    {
        freqPot2ChipSelect = pin;
        pinMode (pin, OUTPUT);
    }
    void setResPotChipSelect(int pin)
    {
        resPotChipSelect = pin;
        pinMode (pin, OUTPUT);
    }
    void setFiltertype(filtertype newvalue);
    uint16_t outvalue;
    uint16_t freq;
    int res;
    void setEnvAmt(uint16_t value);
    uint16_t envAmt = 30000;
    uint16_t modAmt = 30000;
private:
    audio_block_t *inputQueueArray[2];//2 blocks: one for the envelope, the other for the modulation
    filtertype filterType;
    
    
    
    int freqPot1ChipSelect = 10;
    int freqPot2ChipSelect = 9;
    int resPotChipSelect = 8;
    int multiplex1PinA = 4;
    int multiplex1PinB = 5;
    int multiplex2PinA = 6;
    int multiplex2PinB = 7;
    int multiplexerValue;
    //C pins are unused, you can already control 2x 4 channels now!
};

