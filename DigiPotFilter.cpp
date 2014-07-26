#include "DigipotFilter.h"
static int antilog(uint8_t uc_linear)
{
    uint8_t uc_index;
    uc_index = uc_linear >> 5;//Divide down to 8 segments
    switch (uc_index)
    {
        case 0://0-31 -> 0-124
	     return uc_linear * 4;
	     break;
        case 1://32-63 -> 128-252
	    return uc_linear*4;
	    break;
	case 2://64-95 -> 254-316 
	    uc_linear = uc_linear - 63;
	    return 252 + uc_linear*2;
	    break;
	case 3://96-127 ->316-380
	    uc_linear = uc_linear - 95;
	    return 316 + uc_linear*2;
	    break;
	case 4://128-159 ->380-412
	    uc_linear = 255 - uc_linear;
	    return 511 - uc_linear;
	    break;
	case 5://160-191 ->413-444
	    /*We want the highest region to be linear 9 bit*/
	    uc_linear = 255 - uc_linear;
	    return 511 - uc_linear;
	    break;
	case 6://191-222 ->445-476
	    /*We want the highest region to be linear 9 bit*/
	    uc_linear = 255 - uc_linear;
	    return 511 - uc_linear;
	    break;
	case 7://223-255 ->479-511
	        /*We want the highest region to be linear 9 bit*/
		uc_linear = 255 - uc_linear;
		return 511 - uc_linear;
		break;
        default:	
		return 511;
		break;	
	}
}
void AudioSynthDigiFilter::setFrequency(int value)
{
    freq = value;
}
void AudioSynthDigiFilter::setFilterType(int type)
{
    filterType = type;
}
void AudioSynthDigiFilter::setResonance(int value)
{
    res = value;
}
void AudioSynthDigiFilter::update(void)
{
    audio_block_t *block;
    uint32_t i;
    int16_t pot1value = 255;
    block = receiveReadOnly();
    if (block) 
    {
        //filterFreq = antilog(value*2)/2;
        digitalWriteFast(freqPot1ChipSelect,LOW);
        //  send in the address and value via spi:
        //last 2 bits: pot select
        //other 1: write command
        SPI.transfer(0b00010011);
        SPI.transfer(pot1value);
        // take the SS pin high to de-select the chip:
        digitalWriteFast(freqPot1ChipSelect,HIGH); 
        //output only one frame
	return;
    }
}

