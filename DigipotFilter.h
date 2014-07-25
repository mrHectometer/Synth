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
    int setResonance(int value);//0 to infinity (?)
private:
    audio_block_t *inputQueueArray[2];//2 blocks: one for the envelope, the other for the modulation
    int filterType;
    int freq;
    int envAmt;
    int modAmt;
    int freqPot1ChipSelect = 10;
    int freqPot2ChipSelect = 11;
    int resPotChipSelect = 12;
};

