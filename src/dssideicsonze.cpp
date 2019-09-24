/****************************************************************************
    
    DSSI wrapper for Deicsonze

    dssideicsonze.cpp

    Copyleft (c) 2016 Nil Geisweiller
    
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 01222-1307  USA

****************************************************************************/

#include <cmath>

#include "dssideicsonze.hpp"

using namespace deicsonze;
using namespace std;

DSSIDeicsonze::DSSIDeicsonze(unsigned long frame_rate) {
}

void DSSIDeicsonze::run_synth(unsigned long sample_count,
                              snd_seq_event_t* events,
                              unsigned long event_count) {
	update_patch();

	LADSPA_Data* left_out = m_ports[LEFT_OUTPUT];
	LADSPA_Data* right_out = m_ports[RIGHT_OUTPUT];

	unsigned long e = 0; //current event in the loop
	unsigned long t = 0; //current tick
	unsigned long net = 0; //tick of the next event

	// handle any MIDI events that occur in this frame
	while(e < event_count) {
		if(events[e].time.tick == t) {
			// printf("events[e].type = %d\n", events[e].type);
			switch(events[e].type) {
				//Note On, Off
			case SND_SEQ_EVENT_NOTEON :
				deicsonze.noteOn_process(0, events[e].data.note.note,
				                         events[e].data.note.velocity);
				break;
			case SND_SEQ_EVENT_NOTEOFF :
				deicsonze.noteOff_process(0, events[e].data.note.note);
				break;
				//Sysex
			case SND_SEQ_EVENT_SYSEX :
				deicsonze.sysex_process(events[e].data.ext.len,
				                        (unsigned char*) events[e].data.ext.ptr);
				break;
			default:
				break;
			}
			e++;
		}
		else {
			net = events[e].time.tick;
			deicsonze.audio_process(&left_out[t], &right_out[t], net - t);
			t = net;
		}
	}
	deicsonze.audio_process(&left_out[net], &right_out[net], sample_count - net);
}

int DSSIDeicsonze::get_midi_controller_for_port(unsigned long port) {
	return DSSI_NONE;
}

const DSSI_Program_Descriptor* DSSIDeicsonze::get_program(unsigned long index) {
	if (index < m_progs.size()) {
		return &m_progs[index];
	}
	return NULL;
}

void DSSIDeicsonze::select_program(unsigned long bank, unsigned long program) {
	// For now bank is ignored and program corresponds to the index
}

char* DSSIDeicsonze::configure(const char* key, const char* value) {
	return NULL;
}

void DSSIDeicsonze::update_patch()
{
	// Play mode
	deicsonze.patch.playmode = (PlayMode)std::round(*m_ports[PLAY_MODE]);

	// Tone
	deicsonze.patch.tone.time = *m_ports[TONE_TIME];
	deicsonze.patch.tone.detune = *m_ports[TONE_DETUNE] + *m_ports[TONE_TRANSPOSE];

	// Noise
	deicsonze.patch.noise.time = *m_ports[NOISE_TIME];
	deicsonze.patch.noise.period = (int)std::round(*m_ports[NOISE_PERIOD]);

	// Amplitude envelope
	deicsonze.patch.ampenv.attack_level = *m_ports[AMP_ENV_ATTACK_LEVEL];
	deicsonze.patch.ampenv.time1 = *m_ports[AMP_ENV_TIME1];
	deicsonze.patch.ampenv.level1 = *m_ports[AMP_ENV_LEVEL1];
	deicsonze.patch.ampenv.time2 = *m_ports[AMP_ENV_TIME2];
	deicsonze.patch.ampenv.level2 = *m_ports[AMP_ENV_LEVEL2];
	deicsonze.patch.ampenv.time3 = *m_ports[AMP_ENV_TIME3];
	deicsonze.patch.ampenv.sustain_level = *m_ports[AMP_ENV_SUSTAIN_LEVEL];
	deicsonze.patch.ampenv.release = *m_ports[AMP_ENV_RELEASE];

	// Pitch envelope
	deicsonze.patch.pitchenv.attack_pitch = *m_ports[PITCH_ENV_ATTACK_PITCH];
	deicsonze.patch.pitchenv.time = *m_ports[PITCH_ENV_TIME];

	// Arpegio
	deicsonze.patch.arp.pitch1 = *m_ports[ARP_PITCH1];
	deicsonze.patch.arp.pitch2 = *m_ports[ARP_PITCH2];
	deicsonze.patch.arp.pitch3 = *m_ports[ARP_PITCH3];
	deicsonze.patch.arp.freq = *m_ports[ARP_FREQ];
	deicsonze.patch.arp.repeat = (int)std::round(*m_ports[ARP_REPEAT]);

	// Ring modulation
	deicsonze.patch.ringmod.waveform[0] = *m_ports[RING_MOD_WAVEFORM_LEVEL1];
	deicsonze.patch.ringmod.waveform[1] = *m_ports[RING_MOD_WAVEFORM_LEVEL2];
	deicsonze.patch.ringmod.waveform[2] = *m_ports[RING_MOD_WAVEFORM_LEVEL3];
	deicsonze.patch.ringmod.waveform[3] = *m_ports[RING_MOD_WAVEFORM_LEVEL4];
	deicsonze.patch.ringmod.waveform[4] = *m_ports[RING_MOD_WAVEFORM_LEVEL5];
	deicsonze.patch.ringmod.waveform[5] = *m_ports[RING_MOD_WAVEFORM_LEVEL6];
	deicsonze.patch.ringmod.waveform[6] = *m_ports[RING_MOD_WAVEFORM_LEVEL7];
	deicsonze.patch.ringmod.waveform[7] = *m_ports[RING_MOD_WAVEFORM_LEVEL8];
	deicsonze.patch.ringmod.mirror = (bool)std::round(*m_ports[RING_MOD_MINOR]);
	deicsonze.patch.ringmod.sync = (bool)std::round(*m_ports[RING_MOD_SYNC]);
	deicsonze.patch.ringmod.detune =
		*m_ports[RING_MOD_DETUNE] + *m_ports[RING_MOD_TRANSPOSE];

	// Pitch LFO
	deicsonze.patch.lfo.freq = *m_ports[LFO_FREQ];
	deicsonze.patch.lfo.delay = *m_ports[LFO_DELAY];
	deicsonze.patch.lfo.depth = *m_ports[LFO_DEPTH];

	// Portamento
	deicsonze.patch.port = *m_ports[PORTAMENTO];

	// Pan
	deicsonze.patch.pan.channel[0] = *m_ports[PAN_CHANNEL0];
	deicsonze.patch.pan.channel[1] = *m_ports[PAN_CHANNEL1];
	deicsonze.patch.pan.channel[2] = *m_ports[PAN_CHANNEL2];
}

void initialise_2() __attribute__((constructor));

void initialise_2() {
	DSSIPortList ports;
  
	//////////////////////
	// Control ports    //
	//////////////////////
	const LADSPA_PortDescriptor c_desc = LADSPA_PORT_CONTROL | LADSPA_PORT_INPUT;
	const LADSPA_PortRangeHintDescriptor r_desc = LADSPA_HINT_BOUNDED_BELOW |
		LADSPA_HINT_BOUNDED_ABOVE;
	const LADSPA_PortRangeHintDescriptor i_desc = LADSPA_HINT_BOUNDED_BELOW |
		LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_INTEGER;
	const LADSPA_PortRangeHintDescriptor t_desc = LADSPA_HINT_TOGGLED;
	const LADSPA_PortRangeHintDescriptor d_min = LADSPA_HINT_DEFAULT_MINIMUM;
	const LADSPA_PortRangeHintDescriptor d_max = LADSPA_HINT_DEFAULT_MAXIMUM;
	const LADSPA_PortRangeHintDescriptor d_middle = LADSPA_HINT_DEFAULT_MIDDLE;
	const LADSPA_PortRangeHintDescriptor d_low = LADSPA_HINT_DEFAULT_LOW;
	const LADSPA_PortRangeHintDescriptor d_high = LADSPA_HINT_DEFAULT_HIGH;
	const LADSPA_PortRangeHintDescriptor d_0 = LADSPA_HINT_DEFAULT_0;
	const LADSPA_PortRangeHintDescriptor d_1 = LADSPA_HINT_DEFAULT_1;

	// Play mode
	ports.add_port(c_desc, "Play mode", i_desc | d_0, -0.1, 4.1);

	// Tone
	ports.add_port(c_desc, "Tone time", r_desc | d_min, -1.0, 10.0);
	ports.add_port(c_desc, "Tone detune", r_desc | d_0, -1.0, 1.0);
	ports.add_port(c_desc, "Tone transpose", i_desc | d_0, -24.1, 24.1);

	// Noise
	ports.add_port(c_desc, "Noise time", r_desc | d_0, -1.0, 10.0);
	ports.add_port(c_desc, "Noise period", i_desc | d_middle, -0.1, 31.1);

	// Amplitude envelope
	ports.add_port(c_desc, "AmpEnv attack_level", r_desc | d_1, 0.0, 1.0);
	ports.add_port(c_desc, "AmpEnv time1", r_desc | d_0, 0.0, 10.0);
	ports.add_port(c_desc, "AmpEnv level1", r_desc | d_1, 0.0, 1.0);
	ports.add_port(c_desc, "AmpEnv time2", r_desc | d_0, 0.0, 10.0);
	ports.add_port(c_desc, "AmpEnv level2", r_desc | d_1, 0.0, 1.0);
	ports.add_port(c_desc, "AmpEnv time3", r_desc | d_0, 0.0, 10.0);
	ports.add_port(c_desc, "AmpEnv sustain_level", r_desc | d_1, 0.0, 1.0);
	ports.add_port(c_desc, "AmpEnv release", r_desc | d_0, 0.0, 10.0);

	// Pitch envelope
	ports.add_port(c_desc, "PitchEnv attack_pitch", r_desc | d_0, -96.0, 96.0);
	ports.add_port(c_desc, "PitchEnv time", r_desc | d_0, 0.0, 10.0);

	// Arpegio
	ports.add_port(c_desc, "Arp pitch1", r_desc | d_0, -48.0, 48.0);
	ports.add_port(c_desc, "Arp pitch2", r_desc | d_0, -48.0, 48.0);
	ports.add_port(c_desc, "Arp pitch3", r_desc | d_0, -48.0, 48.0);
	ports.add_port(c_desc, "Arp freq", r_desc | d_1, 0.0, 100.0);
	ports.add_port(c_desc, "Arp repeat", i_desc | d_0, -0.1, 2.1);

	// Ring modulation
	ports.add_port(c_desc, "RingMod waveform level1", r_desc | d_1, 0.0, 1.0);
	ports.add_port(c_desc, "RingMod waveform level2", r_desc | d_1, 0.0, 1.0);
	ports.add_port(c_desc, "RingMod waveform level3", r_desc | d_1, 0.0, 1.0);
	ports.add_port(c_desc, "RingMod waveform level4", r_desc | d_1, 0.0, 1.0);
	ports.add_port(c_desc, "RingMod waveform level5", r_desc | d_1, 0.0, 1.0);
	ports.add_port(c_desc, "RingMod waveform level6", r_desc | d_1, 0.0, 1.0);
	ports.add_port(c_desc, "RingMod waveform level7", r_desc | d_1, 0.0, 1.0);
	ports.add_port(c_desc, "RingMod waveform level8", r_desc | d_1, 0.0, 1.0);
	ports.add_port(c_desc, "RingMod mirror", t_desc | d_1, -0.1, 1.1);
	ports.add_port(c_desc, "RingMod sync", t_desc | d_1, -0.1, 1.1);
	ports.add_port(c_desc, "RingMod detune", r_desc | d_0, -1.0, 1.0);
	ports.add_port(c_desc, "RingMod transpose", i_desc | d_0, -24.1, 24.1);

	// Pitch LFO
	ports.add_port(c_desc, "LFO freq", r_desc | d_1, 0.0, 20.0);
	ports.add_port(c_desc, "LFO delay", r_desc | d_0, 0.0, 10.0);
	ports.add_port(c_desc, "LFO depth", r_desc | d_0, 0.0, 12.0);

	// Portamento
	ports.add_port(c_desc, "Portamento", r_desc | d_0, 0.0, 2.0);

	// Pan
	ports.add_port(c_desc, "Pan channel0", r_desc | d_middle, 0.0, 1.0);
	ports.add_port(c_desc, "Pan channel1", r_desc | d_low, 0.0, 1.0);
	ports.add_port(c_desc, "Pan channel2", r_desc | d_high, 0.0, 1.0);

	//////////////////
	// Audio output //
	//////////////////
	ports.add_port(LADSPA_PORT_AUDIO | LADSPA_PORT_OUTPUT, "Left output");
	ports.add_port(LADSPA_PORT_AUDIO | LADSPA_PORT_OUTPUT, "Right output");
  
	register_dssi<DSSIDeicsonze>(1259, "Deicsonze-DSSI", 0, "Deicsonze DSSI plugin",
	                             "Nil Geisweiller", "GPL", ports);
}
