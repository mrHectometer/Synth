#ifndef connections_h
#define connections_h

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
AudioMultiplier2 VCA;
AudioSynthDigiFilter VCF;
AudioMixer4 OscMixer;

AudioMixer4 LfoFilterMixer;
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
AudioConnection c40(OscMixer,0,VCA,0);
AudioConnection c41(aEnv,0,VCA,1);
AudioConnection c60(VCA ,dac);

AudioConnection c70(fEnv,0,VCF,0);//filter envelope
AudioConnection c71(Lfo1,0,VCF,1);//filter envelope

///////////////////////////////////////////////////////////////////////////////////////////
#endif //connections_h

