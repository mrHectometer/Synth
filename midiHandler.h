#ifndef midihandler_h
#define midihandler_h

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

    if(currentNote == 0 
    || aEnv.getState() == ENVSTATE_SILENT)//er speelt al een noot, en hij is hoorbaar: legato!
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
void mix_oscs(AudioMixer4 Mixer, float osc12Mix, float subOscMix)
{
    DEBUG_PRINT1("osc12Mix",osc12Mix);
    DEBUG_PRINT1("subOscMix",subOscMix);
    Mixer.gain(0,(1-osc12Mix)*(1-subOscMix));
    Mixer.gain(1,osc12Mix*(1-subOscMix));
    Mixer.gain(2,subOscMix);
}
uint8_t filterType = FILTER_LOPASS;
float   filterFreq = 255;
float   filterRes = 5;
uint8_t seqToggle = 0;
void OnControlChange(byte channel, byte control, byte value)
{
    if(control == 4)
    {
        Osc1.setGlideTime(fPow100((float)value/127.0, 5000));
        Osc2.setGlideTime(fPow100((float)value/127.0, 5000));
        Osc3.setGlideTime(fPow100((float)value/127.0, 5000));
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
        mix_oscs(OscMixer, control_oscs_mix, control_subosc_mix);
    }
    if(control == 101)
    {
        control_subosc_mix = (float)value / 127.0;
        mix_oscs(OscMixer, control_oscs_mix, control_subosc_mix);
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

void OnPitchChange(byte channel, int pitch)
{
    
}

void onMidiInit()
{
    usbMIDI.setHandleNoteOff(OnNoteOff);
    usbMIDI.setHandleNoteOn(OnNoteOn);
    usbMIDI.setHandleControlChange(OnControlChange);
//    usbMIDI.setHandlePitchChange(OnPitchChange);
}
#endif
