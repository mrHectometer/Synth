#ifndef midihandler_h
#define midihandler_h
//loaned from rockit
//@ Created by Matt Heins, HackMe Electronics, 2011
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
///////////////////////////////////////////////////////////////////////////////////////////
//midi functions

uint8_t currentNote;//poor man's note administrator

float control_oscs_mix = 0.5;
float control_subosc_mix = 0.5;
void OnNoteOn(byte channel, byte note, byte velocity)
{
    const int noteMin = 16;
    const int noteMax = 93;
    
//    DEBUG_PRINT1("midinote ", note);
//    DEBUG_PRINT1("frequency", nMidiFrequency[note]);
    if(note > noteMax || note < noteMin) return; 

    if(currentNote == 0 
    || aEnv.getState() == ENVSTATE_SILENT)//er speelt al een noot, en hij is hoorbaar: legato!
    {
        Osc1.setNote(note,false);
        Osc2.setNote(note,false);
        Osc3.setNote(note,false);
        aEnv.noteOn();
        fEnv.noteOn();
    }
    else
    {
        Osc1.setNote(note,true);
        Osc2.setNote(note,true);
        Osc3.setNote(note,true);  
        //Osc1.setMagnitude(velocity<<16); 
        //Osc2.setMagnitude(velocity<<16); 
        //Osc3.setMagnitude(velocity<<16); 
    }
    currentNote = note;
}
void OnNoteOff(byte channel, byte note, byte velocity)
{
    if(currentNote != note)
        return;
    currentNote = 0;
    aEnv.noteOff();
    fEnv.noteOff();
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
    if(control == 23) Osc1.setPulseWidth(value<<25);//-65 - + 64 
    if(control == 24) Osc1.setSelfFMAmount(value);
    
    if(control == 40) Osc2.toggleWaveTable();//sin tri sqr saw
    if(control == 41) Osc2.setcDetune(value-24);//-24 - 24
    if(control == 42) Osc2.setfDetune(value-65);//-65 - + 64
    if(control == 43) Osc2.setDetunatorAmount((int)((float)value/1.27));
    if(control == 44) Osc2.setFMAmount(value);
    if(control == 45) Osc2.setSelfFMAmount(value);
    if(control == 46) Osc2.setPulseWidth(value<<25);//-65 - + 64 
    
    if(control == 60) Osc3.toggleWaveTable();//sin tri sqr saw
    if(control == 61) Osc3.setcDetune(value-24);//-24 - 24         -        hier moet nog iets speciaals komen: een octaaf functie ofzo
    
    //aEnv
    if(control == 80) aEnv.setAttack(fPow100((float)value/127.0, 5000));
    if(control == 81) aEnv.setDecay(fPow100((float)value/127.0, 5000));
    if(control == 82) aEnv.setSustain( (float)value/127.0);
    if(control == 83) aEnv.setRelease(fPow100((float)value/127.0, 5000));
    //fEnv
    if(control == 85) fEnv.setAttack(fPow100((float)value/127.0, 5000));
    if(control == 86) fEnv.setDecay(fPow100((float)value/127.0, 5000));
    if(control == 87) fEnv.setSustain( (float)value/127.0);
    if(control == 88) fEnv.setRelease(fPow100((float)value/127.0, 5000));
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
    //    filterFreq = antilog(value*2)/2;
        VCF.setFrequency(antilog(value*2)/2);
        DEBUG_PRINT1("antilog(value*2)/",antilog(value*2)*39);
    } 
    if(control == 122) 
    {
        digitalPotResWrite(antilog(value*2)/2);
    }
    if(control == 123) 
    {
        VCF.setEnvAmt(value<<9);
    }
    if(control == 127) seqToggle = value;
    //lfo
    //eerste: toggle door de modes
    //0 = frequentie osc1 
    //1 = frequentie osc1 + osc2
    //2 = amplitude Aenv
    //3 = pulsewidth?
    //4 = filter
    if(control == 90) Lfo1.frequency(fPow100((float)value/127.0, 20));
    if(control == 91) Lfo1.amplitude(fPow100((float)value/127.0, 1.0));
}
void onPitchBend(byte channel, int pitch)
{
    int newPitchBend = (pitch/82) -100;
    Osc1.setPitchBend(newPitchBend);
    Osc2.setPitchBend(newPitchBend);
    Osc3.setPitchBend(newPitchBend);
}
void onMidiInit()
{
    usbMIDI.setHandleNoteOff(OnNoteOff);
    usbMIDI.setHandleNoteOn(OnNoteOn);
    usbMIDI.setHandleControlChange(OnControlChange);
    usbMIDI.setHandlePitchChange(onPitchBend);
}
#endif
