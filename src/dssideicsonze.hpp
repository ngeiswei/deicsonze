/****************************************************************************
    
    DSSI wrapper for Deicsonze

    dssideicsonze.hpp

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

#ifndef __DEICSONZE_DSSIDEICSONZE_HPP
#define __DEICSONZE_DSSIDEICSONZE_HPP

#include "dssi/dssiplugin.hpp"
#include "deicsonze/deicsonze.hpp"

namespace deicsonze {

static enum {
	// Play mode
	PLAY_MODE,

	// Tone
	TONE_TIME,
	TONE_DETUNE,
	TONE_TRANSPOSE,

	// Noise
	NOISE_TIME,
	NOISE_PERIOD,

	// Amplitude envelope
	AMP_ENV_ATTACK_LEVEL,
	AMP_ENV_TIME1,
	AMP_ENV_LEVEL1,
	AMP_ENV_TIME2,
	AMP_ENV_LEVEL2,
	AMP_ENV_TIME3,
	AMP_ENV_SUSTAIN_LEVEL,
	AMP_ENV_RELEASE,

	// Pitch envelope
	PITCH_ENV_ATTACK_PITCH,
	PITCH_ENV_TIME,

	// Arpegio
	ARP_PITCH1,
	ARP_PITCH2,
	ARP_PITCH3,
	ARP_FREQ,
	ARP_REPEAT,

	// Ring modulation
	RING_MOD_WAVEFORM_LEVEL1,
	RING_MOD_WAVEFORM_LEVEL2,
	RING_MOD_WAVEFORM_LEVEL3,
	RING_MOD_WAVEFORM_LEVEL4,
	RING_MOD_WAVEFORM_LEVEL5,
	RING_MOD_WAVEFORM_LEVEL6,
	RING_MOD_WAVEFORM_LEVEL7,
	RING_MOD_WAVEFORM_LEVEL8,
	RING_MOD_MINOR,
	RING_MOD_SYNC,
	RING_MOD_DETUNE,
	RING_MOD_TRANSPOSE,

	// Pitch LFO
	LFO_FREQ,
	LFO_DELAY,
	LFO_DEPTH,

	// Portamento
	PORTAMENTO,

	// Pan
	PAN_CHANNEL0,
	PAN_CHANNEL1,
	PAN_CHANNEL2,

	// Audio outputs
	LEFT_OUTPUT,
	RIGHT_OUTPUT
} enum_ports;

class DSSIDeicsonze : public DSSIPlugin {
public:

	Deicsonze deicsonze;

	DSSIDeicsonze(unsigned long frame_rate);

	void run_synth(unsigned long sample_count, snd_seq_event_t* events,
	               unsigned long event_count);
	int get_midi_controller_for_port(unsigned long port);

	const DSSI_Program_Descriptor* get_program(unsigned long index);
	void select_program(unsigned long bank, unsigned long program);

	char* configure(const char* key, const char* value);

protected:

	unsigned long m_frame_rate;
	unsigned long m_last_frame;

	LADSPA_Data m_velocity;
	LADSPA_Data m_pitch;
	unsigned char m_note;
	bool m_note_is_on;

	DSSI_Program_Descriptor m_prog;
	std::vector<DSSI_Program_Descriptor> m_progs;

	LADSPA_Data m_pitchbend;

	// Read the control ports and update the patch accordingly
	void update_patch();
};

} // ~namespace deicsonze

#endif
