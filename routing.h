#ifndef routing_h
#define routing_h

#include "audiostream.h"


///////////////////////////////////////////////////////////////////////////////////////////
//audio streams and connections
//1 lfo. helemaal vooraan, omdat hij moet kunnen fm-en
AudioSynthWaveformLfo Lfo1;
AudioMixer4 LfoOsc1Mixer;
AudioSynthWaveformOsc Osc1;
AudioSynthWaveformOsc Osc2;
AudioSynthWaveformOsc Osc3;
AudioEffectEnvelope aEnv;
AudioEffectEnvelope fEnv;

int ToneFilter[]={0,0,0,0,0,0,0,0x80000000,0,0,0,0,0,0,0,0}; // defines 2 sets of coefficients, not sure max possible in

int updateFilter[5];
AudioFilterBiquad    mainFilter (ToneFilter);

AudioMixer4 OscMixer;

AudioMixer4 LfoFilterMixer;
//AudioEffectChorus Chorus;

//op het laatst
AudioOutputAnalog dac;
//lfo1 naar osc1
AudioConnection c110(Lfo1,Osc1);
//osc1 en lfo naar osc2
AudioConnection c111(Lfo1,0,LfoOsc1Mixer,0);
AudioConnection c112(Osc1,0,LfoOsc1Mixer,1);//Osc2 is modulated by Osc1
AudioConnection c113(LfoOsc1Mixer,0,Osc2,0);//Osc2 is modulated by Osc1

AudioConnection c1(Osc1,0,OscMixer,0);
AudioConnection c2(Osc2,0,OscMixer,1);
AudioConnection c3(Osc3,0,OscMixer,2);
AudioConnection c40(OscMixer,aEnv);
AudioConnection c60(aEnv ,dac);


//lfo to filter
AudioConnection c100(Lfo1,0,LfoFilterMixer,0);
AudioConnection c101(fEnv,0,LfoFilterMixer,1);


///////////////////////////////////////////////////////////////////////////////////////////
#endif

