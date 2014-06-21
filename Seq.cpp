#include "Seq.h"

AudioSynthSequencer::AudioSynthSequencer()
{
    notes[0] = 12;
    notes[1] = 0;
    notes[2] = 0;
    notes[3] = 12;
    
    notes[4] = 0;
    notes[5] = 0;
    notes[6] = 10;
    notes[7] = 0;
    
    notes[8] = 12;
    notes[9] = 0;
    notes[10] = 0;
    notes[11] = 10;
    
    notes[12] = 0;
    notes[13] = 0;
    notes[14] = 8;
    notes[15] = 0;   
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
