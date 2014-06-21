#include "lfo.h"
#define DEBBIE
#include "debugUtils.h"


const int TableSize = 257;
const int TableSizeBits = 8;

const int16_t lfoSinTable[257]  = {0     , 804   , 1607  , 2410  , 3211  , 4011  , 4807  , 5601  , 6392  , 7179  , 7961  , 8739  , 9511  , 10278 , 11038 , 11792 , 12539 , 13278 , 14009 , 14732 , 15446 , 16150 , 16845 , 17530 , 18204 , 18867 , 19519 , 20159 , 20787 , 21402 , 22004 , 22594 , 23169 , 23731 , 24278 , 24811 , 25329 , 25831 , 26318 , 26789 , 27244 , 27683 , 28105 , 28510 , 28897 , 29268 , 29621 , 29955 , 30272 , 30571 , 30851 , 31113 , 31356 , 31580 , 31785 , 31970 , 32137 , 32284 , 32412 , 32520 , 32609 , 32678 , 32727 , 32757 , 32767 , 32757 , 32727 , 32678 , 32609 , 32520 , 32412 , 32284 , 32137 , 31970 , 31785 , 31580 , 31356 , 31113 , 30851 , 30571 , 30272 , 29955 , 29621 , 29268 , 28897 , 28510 , 28105 , 27683 , 27244 , 26789 , 26318 , 25831 , 25329 , 24811 , 24278 , 23731 , 23169 , 22594 , 22004 , 21402 , 20787 , 20159 , 19519 , 18867 , 18204 , 17530 , 16845 , 16150 , 15446 , 14732 , 14009 , 13278 , 12539 , 11792 , 11038 , 10278 , 9511  , 8739  , 7961  , 7179  , 6392  , 5601  , 4807  , 4011  , 3211  , 2410  , 1607  , 804   , 0     , -805  , -1608 , -2411 , -3212 , -4012 , -4808 , -5602 , -6393 , -7180 , -7962 , -8740 , -9512 , -10279, -11039, -11793, -12540, -13279, -14010, -14733, -15447, -16151, -16846, -17531, -18205, -18868, -19520, -20160, -20788, -21403, -22005, -22595, -23170, -23732, -24279, -24812, -25330, -25832, -26319, -26790, -27245, -27684, -28106, -28511, -28898, -29269, -29622, -29956, -30273, -30572, -30852, -31114, -31357, -31581, -31786, -31971, -32138, -32285, -32413, -32521, -32610, -32679, -32728, -32758, -32767, -32758, -32728, -32679, -32610, -32521, -32413, -32285, -32138, -31971, -31786, -31581, -31357, -31114, -30852, -30572, -30273, -29956, -29622, -29269, -28898, -28511, -28106, -27684, -27245, -26790, -26319, -25832, -25330, -24812, -24279, -23732, -23170, -22595, -22005, -21403, -20788, -20160, -19520, -18868, -18205, -17531, -16846, -16151, -15447, -14733, -14010, -13279, -12540, -11793, -11039, -10279, -9512 , -8740 , -7962 , -7180 , -6393 , -5602 , -4808 , -4012 , -3212 , -2411 , -1608 , -805  , 0};
const int16_t lfoSqrTable[257] = {-32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, -32768, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, };
const int16_t lfosawTable[257] = {-32768, -32512, -32256, -32000, -31744, -31488, -31232, -30976, -30720, -30464, -30208, -29952, -29696, -29440, -29184, -28928, -28672, -28416, -28160, -27904, -27648, -27392, -27136, -26880, -26624, -26368, -26112, -25856, -25600, -25344, -25088, -24832, -24576, -24320, -24064, -23808, -23552, -23296, -23040, -22784, -22528, -22272, -22016, -21760, -21504, -21248, -20992, -20736, -20480, -20224, -19968, -19712, -19456, -19200, -18944, -18688, -18432, -18176, -17920, -17664, -17408, -17152, -16896, -16640, -16384, -16129, -15873, -15617, -15361, -15105, -14849, -14593, -14337, -14081, -13825, -13569, -13313, -13057, -12801, -12545, -12289, -12033, -11777, -11521, -11265, -11009, -10753, -10497, -10241, -9985, -9729, -9473, -9217, -8961, -8705, -8449, -8193, -7937, -7681, -7425, -7169, -6913, -6657, -6401, -6145, -5889, -5633, -5377, -5121, -4865, -4609, -4353, -4097, -3841, -3585, -3329, -3073, -2817, -2561, -2305, -2049, -1793, -1537, -1281, -1025, -769, -513, -257, -1, 255, 511, 767, 1023, 1279, 1535, 1791, 2047, 2303, 2559, 2815, 3071, 3327, 3583, 3839, 4095, 4351, 4607, 4863, 5119, 5375, 5631, 5887, 6143, 6399, 6655, 6911, 7167, 7423, 7679, 7935, 8191, 8447, 8703, 8959, 9215, 9471, 9727, 9983, 10239, 10495, 10751, 11007, 11263, 11519, 11775, 12031, 12287, 12543, 12799, 13055, 13311, 13567, 13823, 14079, 14335, 14591, 14847, 15103, 15359, 15615, 15871, 16127, 16383, 16638, 16894, 17150, 17406, 17662, 17918, 18174, 18430, 18686, 18942, 19198, 19454, 19710, 19966, 20222, 20478, 20734, 20990, 21246, 21502, 21758, 22014, 22270, 22526, 22782, 23038, 23294, 23550, 23806, 24062, 24318, 24574, 24830, 25086, 25342, 25598, 25854, 26110, 26366, 26622, 26878, 27134, 27390, 27646, 27902, 28158, 28414, 28670, 28926, 29182, 29438, 29694, 29950, 30206, 30462, 30718, 30974, 31230, 31486, 31742, 31998, 32254, 32510, };
const int16_t lfoTriTable[257] = {-32768, -32256, -31744, -31232, -30720, -30208, -29696, -29184, -28672, -28160, -27648, -27136, -26624, -26112, -25600, -25088, -24576, -24064, -23552, -23040, -22528, -22016, -21504, -20992, -20480, -19968, -19456, -18944, -18432, -17920, -17408, -16896, -16384, -15873, -15361, -14849, -14337, -13825, -13313, -12801, -12289, -11777, -11265, -10753, -10241, -9729, -9217, -8705, -8193, -7681, -7169, -6657, -6145, -5633, -5121, -4609, -4097, -3585, -3073, -2561, -2049, -1537, -1025, -513, -1, 511, 1023, 1535, 2047, 2559, 3071, 3583, 4095, 4607, 5119, 5631, 6143, 6655, 7167, 7679, 8191, 8703, 9215, 9727, 10239, 10751, 11263, 11775, 12287, 12799, 13311, 13823, 14335, 14847, 15359, 15871, 16383, 16894, 17406, 17918, 18430, 18942, 19454, 19966, 20478, 20990, 21502, 22014, 22526, 23038, 23550, 24062, 24574, 25086, 25598, 26110, 26622, 27134, 27646, 28158, 28670, 29182, 29694, 30206, 30718, 31230, 31742, 32254, 32766, 32254, 31742, 31230, 30718, 30206, 29694, 29182, 28670, 28158, 27646, 27134, 26622, 26110, 25598, 25086, 24574, 24062, 23550, 23038, 22526, 22014, 21502, 20990, 20478, 19966, 19454, 18942, 18430, 17918, 17406, 16894, 16383, 15871, 15359, 14847, 14335, 13823, 13311, 12799, 12287, 11775, 11263, 10751, 10239, 9727, 9215, 8703, 8191, 7679, 7167, 6655, 6143, 5631, 5119, 4607, 4095, 3583, 3071, 2559, 2047, 1535, 1023, 511, -1, -513, -1025, -1537, -2049, -2561, -3073, -3585, -4097, -4609, -5121, -5633, -6145, -6657, -7169, -7681, -8193, -8705, -9217, -9729, -10241, -10753, -11265, -11777, -12289, -12801, -13313, -13825, -14337, -14849, -15361, -15873, -16384, -16896, -17408, -17920, -18432, -18944, -19456, -19968, -20480, -20992, -21504, -22016, -22528, -23040, -23552, -24064, -24576, -25088, -25600, -26112, -26624, -27136, -27648, -28160, -28672, -29184, -29696, -30208, -30720, -31232, -31744, -32256, };

///////////////////////////////////////////////////////////////////////////////////////////
//frequentie omzetten naar fase increments
//in:  frequency
//out: phase increment
void AudioSynthWaveformLfo::frequency(float f)
{
	if (f > AUDIO_SAMPLE_RATE_EXACT / 2 || f < 0.0) return;
	phaseInc =  (f / AUDIO_SAMPLE_RATE_EXACT) * 4294967296.0f;
}
///////////////////////////////////////////////////////////////////////////////////////////
//Selecting a wavetable
//in:  table
//out: void
//mod: pWavetable (array pointer)
void AudioSynthWaveformLfo::switchTable(uint8_t table)
{
    switch(table)
    {
        case selectSinTable:
            pWaveTable = lfoSinTable;
            break;
        case selectTriTable:
            pWaveTable = lfoTriTable;
            break;
        case selectSqrTable:
            pWaveTable = lfoSqrTable;
            break;
        case selectSawTable:
            pWaveTable = lfoSawTable;
            break;
    }
}
///////////////////////////////////////////////////////////////////////////////////////////
void AudioSynthWaveformLfo::toggleWaveTable()
{
    selectTable+=1;
    selectTable%=4;
    switchTable(selectTable);
}
void AudioSynthWaveformLfo::setWaveTable(int newTable)
{
    selectTable=newTable;
    switchTable(selectTable);
}
///////////////////////////////////////////////////////////////////////////////////////////
//lfo destination
void setDest(int newDest)
{
    dest = newDest;
}
void toggleDest(int newDest)
{
    dest++;
    if(dest > lfoDest_Max)
        dest = lfoDest_Min;
}
int getDest()
{
    return dest;
}
///////////////////////////////////////////////////////////////////////////////////////////
void AudioSynthWaveformLfo::update(void)
{
    audio_block_t *block;
    uint32_t i, ph, inc, index, scale;
    int32_t val1, val2;
    int16_t mod;
    
    block = allocate();
    if (block) 
    {
    	ph = phase;
    	inc = phaseInc;
        for (i=0; i < AUDIO_BLOCK_SAMPLES; i++) 
        {          
            index = ph >> (32-TableSizeBits);
            //lineaire interpolatie tussen het huidige en het volgende sampeltje
            val1 = pWaveTable[index];
            val2 = pWaveTable[index+1];
            scale = (ph >> TableSizeBits) & 0xFFFF;
            val2 *= scale;
            val1 *= 0xFFFF - scale;
            block->data[i] = multiply_32x32_rshift32(val1 + val2, magnitude);
	    ph += inc;
	}

        phase = ph;
        phaseInc = inc;
	transmit(block);
	release(block);
	return;
    }
    phase += phaseInc * AUDIO_BLOCK_SAMPLES;
}





