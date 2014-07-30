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
void AudioSynthDigiFilter::setFrequency(uint16_t value)
{
    freq = value<<8;//scale to 0 to 65535
}
void AudioSynthDigiFilter::setFiltertype(filtertype newvalue)
{
    filterType = newvalue;
    //output naar de multiplexer.
    //neem een 2x4 multiplexer, voor 2 filters.
    //plaats die dan niet op het filter board.
    //of gewoon 2 van 8 kanalen, net zo goedkoop
}
void AudioSynthDigiFilter::setResonance(int value)
{
    res = value;
}
void digitalPotWrite(int value) {
  // take the SS pin low to select the chip:
  digitalWriteFast(10,LOW);
  //  send in the address and value via spi:
  //last 2 bits: pot select
  //other 1: write command
  SPI.transfer(0b00010011);
  SPI.transfer(value);
  // take the SS pin high to de-select the chip:
  digitalWriteFast(10,HIGH); 
}
void AudioSynthDigiFilter::update(void)
{
    audio_block_t *block, *modBlock;
    uint16_t envIn, baseFreq;
    int16_t modIn;
    uint8_t pot1value;
    block = receiveReadOnly(0);
    modBlock = receiveReadOnly(1);
    if (block)
    {
        if (modBlock)
        {
            //final frequency = baseFreq + envIn * envAmt + modIn * modAmt;
            //following implementation isn't correct yet: negative envIns aren't possible
            envIn = (block->data[64] * envAmt) >> 15;//32768 is the middle here, so we can have inverting and non in
            modIn = (modBlock->data[64] * modAmt) >> 15;
            baseFreq = freq;
            int32_t totalfreq = baseFreq + envIn + modIn;
            if(totalfreq > 65535) totalfreq = 65535;
            if(totalfreq < 0) totalfreq = 0;
            pot1value = totalfreq >> 8;
            outvalue = pot1value;
            digitalPotWrite(pot1value);
        }
        release(modBlock);
    }
    release(block);
}

