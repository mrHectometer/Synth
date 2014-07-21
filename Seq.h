///////////////////////////////////////////////////////////////////////////////////////////
//very simple read-only step sequencer, needed for development purposes.
///////////////////////////////////////////////////////////////////////////////////////////
#ifndef seq_h
#define seq_h
#include "audiostream.h"
#include "arm_math.h"
#include "utility/dspinst.h"
///////////////////////////////////////////////////////////////////////////////////////////

class AudioSynthSequencer
{
public:
    AudioSynthSequencer();
    void setInterval(uint16_t _step, int8_t _interval);
    int8_t getInterval(uint16_t _step);
    int8_t getInterval();
private:
    int8_t notes[16];
    uint16_t internalStep;
};

#endif
