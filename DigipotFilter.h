#include "audiostream.h"
#include "arm_math.h"
#include "utility/dspinst.h"
#include "spi.h"
class AudioSynthDigiFilter : public AudioStream
{
public:
    AudioSynthDigiFilter() : AudioStream(2, inputQueueArray) 
    {
      
    }
    virtual void update(void);
    void setFrequency(int value);//0 to 20000
    void setFilterType(int type);//LP, HP, BP, 
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
    void setMultiplexChipSelect(int pin)
    {
        multiplexChipSelect = pin;
        pinMode (pin, OUTPUT);
    }
    
private:
    audio_block_t *inputQueueArray[2];//2 blocks: one for the envelope, the other for the modulation
    int filterType;
    int freq;
    int envAmt;
    int modAmt;
    int res;
    int freqPot1ChipSelect = 10;
    int freqPot2ChipSelect = 9;
    int resPotChipSelect = 8;
    int multiplexChipSelect = 7;
};

