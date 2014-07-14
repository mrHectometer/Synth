#include "Seq.h"

AudioSynthSequencer::AudioSynthSequencer()
{
    notes[0] = 12;
    notes[1] = 0;
    notes[2] = 3;
    notes[3] = 12;
    
    notes[4] = 7;
    notes[5] = 0;
    notes[6] = 12;
    notes[7] = 7;
    
    notes[8] = 0;
    notes[9] = 12;
    notes[10] = 3;
    notes[11] = 0;
    
    notes[12] = 12;
    notes[13] = 3;
    notes[14] = 12;
    notes[15] = 7;   
    internalStep = 0;
}

int8_t AudioSynthSequencer::getInterval()
{
    int8_t ret = notes[internalStep];
    internalStep+=1;
    internalStep%=16;
    return ret;
}
int8_t AudioSynthSequencer::getInterval(uint16_t _step)
{
    return notes[_step];
}
void AudioSynthSequencer::setInterval(uint16_t _step, int8_t _interval)
{
    notes[_step] = _interval;
}
