//wat is nieuw?
//11-5-2014
//glide werkt nu goed
//fm werkt.

//29-5-2014
//lfo added in.
//it can adjust the pitch of the first oscillator
///////////////////////////////////////////////////////////////////////////////////////////
#include <SD.h>
#include <Wire.h>
#include <Audio.h>
#include <SPI.h>
#define DEBBIE
#include "debugUtils.h"
#include "Seq.h"
#include "lfo.h"
#include "osc.h"
#include "env.h"
#include "pow.h"
#include "connections.h"
#include "midiHandler.h"
///////////////////////////////////////////////////////////////////////////////////////////
AudioSynthSequencer mainSeq;

int settingDetune;
int settingOldDetune;
uint32_t filterTimecalc;
uint32_t filterTimeset;
//////////////////////////////////////////////////////////////////////////////////////////
//digipot1
const int slaveSelectPin = 10;
void digitalPot1Write(int value) {
  // take the SS pin low to select the chip:
  digitalWriteFast(slaveSelectPin,LOW);
  //  send in the address and value via spi:
  //last 2 bits: pot select
  //other 1: write command
  SPI.transfer(0b00010011);
  SPI.transfer(value);
  // take the SS pin high to de-select the chip:
  digitalWriteFast(slaveSelectPin,HIGH); 
}
//////////////////////////////////////////////////////////////////////////////////////////
void setup()
{
    pinMode (slaveSelectPin, OUTPUT);
    SPI.begin(); 
    SPI.setClockDivider(SPI_CLOCK_DIV32);
    AudioMemory(18);

    Serial.begin(9600);
    dac.analogReference(INTERNAL);
    mix_oscs(OscMixer, control_oscs_mix, control_subosc_mix);
    aEnv.setAttack(2.0);
    aEnv.setDecay(40.0);
    aEnv.setSustain(0.6);
    aEnv.setRelease(20.0);

    Osc1.setGlideTime(500);
    Osc1.setfDetune(0);
    Osc1.setcDetune(0);
    Osc1.setDetunatorAmount(0);
    Osc3.setGlideTime(500);
    Osc2.setfDetune(0);
    Osc2.setcDetune(0);
    Osc2.setDetunatorAmount(0);
    
    Osc2.setGlideTime(500);
    Osc3.setcDetune(-12);
    
    Osc1.setWaveTable(selectSawTable);
    Osc2.setWaveTable(selectSawTable);
    Osc3.setWaveTable(selectSawTable);
    
    Lfo1.frequency(0.8);
    Lfo1.amplitude(.0);
    Osc2.setFMAmount(0.0);
    //initialize usbMIDI
    onMidiInit();
}
elapsedMillis e;
elapsedMillis t;
int8_t seqNote = 0;
void loop()
{ 
    digitalPot1Write(filterFreq);
    if(e > 500)
    {
      DEBUG_PRINT1("filterFreq", filterFreq);
        e-=500;
    }
    if(seqToggle > 0)
    {
        if(t > 140)
        {
            t-=140;
            OnNoteOff(0, seqNote, 127);
            seqNote = 60+mainSeq.getInterval();
            OnNoteOn(0, seqNote, 127);
        }
    }
    usbMIDI.read();
}
