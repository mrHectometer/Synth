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
#include "routing.h"
///////////////////////////////////////////////////////////////////////////////////////////
AudioSynthSequencer mainSeq;
///////////////////////////////////////////////////////////////////////////////////////////
//midi functions

uint8_t currentNote;//poor man's note administrator

float control_oscs_mix = 0.0;
float control_subosc_mix = 0.5;
void OnNoteOn(byte channel, byte note, byte velocity)
{
    const int noteMin = 16;
    const int noteMax = 93;
    
    DEBUG_PRINT1("midinote ", note);
    DEBUG_PRINT1("frequency", nMidiFrequency[note]);
    if(note > noteMax || note < noteMin) return; 

    if(currentNote == 0)//er speelt al een noot: legato!
    {
        Osc1.setNote(note,false);
        Osc2.setNote(note,false);
        Osc3.setNote(note,false);
        aEnv.noteOn();
    }
    else
    {
        Osc1.setNote(note,true);
        Osc2.setNote(note,true);
        Osc3.setNote(note,true);  
    }
    currentNote = note;
}
void OnNoteOff(byte channel, byte note, byte velocity)
{
    if(currentNote != note)
        return;
    currentNote = 0;
    aEnv.noteOff();
}
void mix_oscs(float a, float b)
{
    DEBUG_PRINT1("a",a);
    DEBUG_PRINT1("b",b);
    OscMixer.gain(0,(1-a)*(1-b));
    OscMixer.gain(1,a*(1-b));
    OscMixer.gain(2,b);
}
uint8_t filterType = FILTER_LOPASS;
float   filterFreq = 255;
float   filterRes = 5;
uint8_t seqToggle = 0;
void OnControlChange(byte channel, byte control, byte value)
{
     //if(control == 0) Osc1.toggleWaveTable();//sin tri sqr saw
    if(control == 4)
    {
        Osc1.setGlideStep(fPow100((float)value/127.0, 5000));
        Osc2.setGlideStep(fPow100((float)value/127.0, 5000));
        Osc3.setGlideStep(fPow100((float)value/127.0, 5000));
    } 
    if(control == 20) Osc1.toggleWaveTable();//sin tri sqr saw
    if(control == 21) Osc1.setcDetune(value-24);//-24 - 24
    if(control == 22) Osc1.setfDetune(value-65);//-65 - + 64 
    
    if(control == 40) Osc2.toggleWaveTable();//sin tri sqr saw
    if(control == 41) Osc2.setcDetune(value-24);//-24 - 24
    if(control == 42) Osc2.setfDetune(value-65);//-65 - + 64
    if(control == 43) Osc2.setDetunatorAmount((int)((float)value/1.27));
    if(control == 44) Osc2.setFMAmount((float)value/127.0);
    
    
    if(control == 60) Osc3.toggleWaveTable();//sin tri sqr saw
    if(control == 61) Osc3.setcDetune(value-24);//-24 - 24         -        hier moet nog iets speciaals komen: een octaaf functie ofzo
    
    //aEnv
    if(control == 80) aEnv.setAttack(fPow100((float)value/127.0, 5000));
    if(control == 81) aEnv.setDecay(fPow100((float)value/127.0, 5000));
    if(control == 82) aEnv.setSustain( (float)value/127.0);
    if(control == 83) aEnv.setRelease(fPow100((float)value/127.0, 5000));
    //mixer
    if(control == 100)
    {
        control_oscs_mix = (float)value / 127.0;
        mix_oscs(control_oscs_mix, control_subosc_mix);
    }
    if(control == 101)
    {
        control_subosc_mix = (float)value / 127.0;
        mix_oscs(control_oscs_mix, control_subosc_mix);
    }
    //120 filter
    if(control == 120) filterType = value >> 4;
    //if(control == 121) filterFreq = fPow100((float)value/127.0, 255.0);
    if(control == 121)
    {
        filterFreq = value*2;
    } 
    if(control == 122) filterRes = 0.1+value*(6.0/127.0);
    
    if(control == 127) seqToggle = value;
    //lfo
    //eerste: toggle door de modes
    //0 = frequentie osc1 
    //1 = frequentie osc1 + osc2
    //2 = amplitude Aenv
    //3 = pulsewidth?
    //4 = filter
    if(control == 141) Lfo1.frequency(fPow100((float)value/127.0, 20));
    if(control == 142) Lfo1.amplitude(fPow100((float)value/127.0, 1.0));
}
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
//    dac.analogReference(EXTERNAL);  qj  
    mix_oscs(control_oscs_mix, control_subosc_mix);
    aEnv.setAttack(2.0);
    aEnv.setDecay(40.0);
    aEnv.setSustain(0.6);
    aEnv.setRelease(20.0);

    Osc1.setGlideStep(50);
    Osc1.setfDetune(0);
    Osc1.setcDetune(0);
    Osc1.setDetunatorAmount(0);
    Osc3.setGlideStep(50);
    Osc2.setfDetune(0);
    Osc2.setcDetune(0);
    Osc2.setDetunatorAmount(0);
    
    Osc2.setGlideStep(50);
    Osc3.setcDetune(-12);
    
    Osc1.setWaveTable(selectSawTable);
    Osc2.setWaveTable(selectSawTable);
    Osc3.setWaveTable(selectSawTable);
    
    Lfo1.frequency(0.8);
    Lfo1.amplitude(.0);
    Osc2.setFMAmount(0.0);
    //initialize usbMIDI
    usbMIDI.setHandleNoteOff(OnNoteOff);
    usbMIDI.setHandleNoteOn(OnNoteOn);
    usbMIDI.setHandleControlChange(OnControlChange);
     // calcBiquad(FilterType   ,FrequencyC ,dBgain    ,Q      ,QuantizationUnit  ,SampleRate    ,int*);
    calcBiquad(FILTER_LOPASS    ,filterFreq        ,0         ,filterRes    ,2147483648        ,44100         ,updateFilter);
    mainFilter.updateCoefs(0,updateFilter); // default set updateCoefs(0,updateFilter);
    mainFilter.updateCoefs(1,updateFilter); // default set updateCoefs(0,updateFilter);
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
        //DEBUG_PRINT1("AudioProcessorUsageMax()", AudioProcessorUsageMax());
        //DEBUG_PRINT1("Osc1.processorUsageMax()", Osc1.processorUsageMax());
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
