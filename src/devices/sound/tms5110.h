// license:BSD-3-Clause
// copyright-holders:Frank Palazzolo, Jarek Burczynski, Aaron Giles, Jonathan Gevaryahu, Couriersud
#pragma once

#ifndef __TMS5110_H__
#define __TMS5110_H__

#include "emu.h"

/* HACK: if defined, uses impossibly perfect 'straight line' interpolation */
#undef PERFECT_INTERPOLATION_HACK

/* TMS5110 commands */
										/* CTL8  CTL4  CTL2  CTL1  |   PDC's  */
										/* (MSB)             (LSB) | required */
#define TMS5110_CMD_RESET        (0) /*    0     0     0     x  |     1    */
#define TMS5110_CMD_LOAD_ADDRESS (2) /*    0     0     1     x  |     2    */
#define TMS5110_CMD_OUTPUT       (4) /*    0     1     0     x  |     3    */
#define TMS5110_CMD_SPKSLOW      (6) /*    0     1     1     x  |     1    | Note: this command is undocumented on the datasheets, it only appears on the patents. It might not actually work properly on some of the real chips as manufactured. Acts the same as CMD_SPEAK, but makes the interpolator take three A cycles whereever it would normally only take one, effectively making speech of any given word take twice as long as normal. */
#define TMS5110_CMD_READ_BIT     (8) /*    1     0     0     x  |     1    */
#define TMS5110_CMD_SPEAK       (10) /*    1     0     1     x  |     1    */
#define TMS5110_CMD_READ_BRANCH (12) /*    1     1     0     x  |     1    */
#define TMS5110_CMD_TEST_TALK   (14) /*    1     1     1     x  |     3    */

/* clock rate = 80 * output sample rate,     */
/* usually 640000 for 8000 Hz sample rate or */
/* usually 800000 for 10000 Hz sample rate.  */

#define MCFG_TMS5110_M0_CB(_devcb) \
	devcb = &tms5110_device::set_m0_callback(*device, DEVCB_##_devcb);

#define MCFG_TMS5110_M1_CB(_devcb) \
	devcb = &tms5110_device::set_m1_callback(*device, DEVCB_##_devcb);

#define MCFG_TMS5110_ADDR_CB(_devcb) \
	devcb = &tms5110_device::set_addr_callback(*device, DEVCB_##_devcb);

#define MCFG_TMS5110_DATA_CB(_devcb) \
	devcb = &tms5110_device::set_data_callback(*device, DEVCB_##_devcb);

#define MCFG_TMS5110_ROMCLK_CB(_devcb) \
	devcb = &tms5110_device::set_romclk_callback(*device, DEVCB_##_devcb);


class tms5110_device : public device_t,
						public device_sound_interface
{
public:
	tms5110_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);
	tms5110_device(const machine_config &mconfig, device_type type, const char *name, const char *tag, device_t *owner, uint32_t clock, const char *shortname, const char *source);

	template<class _Object> static devcb_base &set_m0_callback(device_t &device, _Object object) { return downcast<tms5110_device &>(device).m_m0_cb.set_callback(object); }
	template<class _Object> static devcb_base &set_m1_callback(device_t &device, _Object object) { return downcast<tms5110_device &>(device).m_m1_cb.set_callback(object); }
	template<class _Object> static devcb_base &set_addr_callback(device_t &device, _Object object) { return downcast<tms5110_device &>(device).m_addr_cb.set_callback(object); }
	template<class _Object> static devcb_base &set_data_callback(device_t &device, _Object object) { return downcast<tms5110_device &>(device).m_data_cb.set_callback(object); }
	template<class _Object> static devcb_base &set_romclk_callback(device_t &device, _Object object) { return downcast<tms5110_device &>(device).m_romclk_cb.set_callback(object); }

	void ctl_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t ctl_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void pdc_w(int state);

	/* this is only used by cvs.c
	 * it is not related at all to the speech generation
	 * and conflicts with the new rom controller interface.
	 */
	uint8_t romclk_hack_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);

	void set_frequency(int frequency);
protected:
	// device-level overrides
	virtual void device_start() override;
	virtual void device_reset() override;

	virtual void device_timer(emu_timer &timer, device_timer_id id, int param, void *ptr) override;

	// sound stream update overrides
	virtual void sound_stream_update(sound_stream &stream, stream_sample_t **inputs, stream_sample_t **outputs, int samples) override;

	void set_variant(int variant);

	uint8_t m_SPEN;             /* set on speak command, cleared on stop command or reset command */
	uint8_t m_TALK;             /* set on SPEN & RESETL4(pc12->pc0 transition), cleared on stop command or reset command */
#define TALK_STATUS (m_SPEN|m_TALKD)
	uint8_t m_TALKD;            /* TALK(TCON) value, latched every RESETL4 */
	sound_stream *m_stream;

private:
	void new_int_write(uint8_t rc, uint8_t m0, uint8_t m1, uint8_t addr);
	void new_int_write_addr(uint8_t addr);
	uint8_t new_int_read();
	void register_for_save_states();
	int extract_bits(int count);
	void perform_dummy_read();
	int32_t lattice_filter();
	void process(int16_t *buffer, unsigned int size);
	void PDC_set(int data);
	void parse_frame();

	// internal state
	/* table */
	optional_region_ptr<uint8_t> m_table;

	/* coefficient tables */
	int m_variant;                /* Variant of the 5110 - see tms5110.h */

	/* coefficient tables */
	const struct tms5100_coeffs *m_coeff;

	/* these contain global status bits */
	uint8_t m_PDC;
	uint8_t m_CTL_pins;
	uint8_t m_state;

	/* Rom interface */
	uint32_t m_address;
	uint8_t  m_next_is_address;
	uint8_t  m_schedule_dummy_read;
	uint8_t  m_addr_bit;
	/* read byte */
	uint8_t  m_CTL_buffer;

	/* callbacks */
	devcb_write_line   m_m0_cb;      // the M0 line
	devcb_write_line   m_m1_cb;      // the M1 line
	devcb_write8       m_addr_cb;    // Write to ADD1,2,4,8 - 4 address bits
	devcb_read_line    m_data_cb;    // Read one bit from ADD8/Data - voice data
	// On a real chip rom_clk is running all the time
	// Here, we only use it to properly emulate the protocol.
	// Do not rely on it to be a timed signal.
	devcb_write_line   m_romclk_cb;  // rom clock - Only used to drive the data lines

	/* these contain data describing the current and previous voice frames */
#define OLD_FRAME_SILENCE_FLAG m_OLDE // 1 if E=0, 0 otherwise.
#define OLD_FRAME_UNVOICED_FLAG m_OLDP // 1 if P=0 (unvoiced), 0 if voiced
	uint8_t m_OLDE;
	uint8_t m_OLDP;

#define NEW_FRAME_STOP_FLAG (m_new_frame_energy_idx == 0xF) // 1 if this is a stop (Energy = 0xF) frame
#define NEW_FRAME_SILENCE_FLAG (m_new_frame_energy_idx == 0) // ditto as above
#define NEW_FRAME_UNVOICED_FLAG (m_new_frame_pitch_idx == 0) // ditto as above
	uint8_t m_new_frame_energy_idx;
	uint8_t m_new_frame_pitch_idx;
	uint8_t m_new_frame_k_idx[10];


	/* these are all used to contain the current state of the sound generation */
#ifndef PERFECT_INTERPOLATION_HACK
	int16_t m_current_energy;
	int16_t m_current_pitch;
	int16_t m_current_k[10];
#else
	uint8_t m_old_frame_energy_idx;
	uint8_t m_old_frame_pitch_idx;
	uint8_t m_old_frame_k_idx[10];
	uint8_t m_old_zpar;
	uint8_t m_old_uv_zpar;

	int32_t m_current_energy;
	int32_t m_current_pitch;
	int32_t m_current_k[10];
#endif

	uint16_t m_previous_energy; /* needed for lattice filter to match patent */

	uint8_t m_subcycle;         /* contains the current subcycle for a given PC: 0 is A' (only used on SPKSLOW mode on 51xx), 1 is A, 2 is B */
	uint8_t m_subc_reload;      /* contains 1 for normal speech, 0 when SPKSLOW is active */
	uint8_t m_PC;               /* current parameter counter (what param is being interpolated), ranges from 0 to 12 */
	/* NOTE: the interpolation period counts 1,2,3,4,5,6,7,0 for divide by 8,8,8,4,4,2,2,1 */
	uint8_t m_IP;               /* the current interpolation period */
	uint8_t m_inhibit;          /* If 1, interpolation is inhibited until the DIV1 period */
	uint8_t m_uv_zpar;          /* If 1, zero k5 thru k10 coefficients */
	uint8_t m_zpar;             /* If 1, zero ALL parameters. */
	uint8_t m_pitch_zero;       /* circuit 412; pitch is forced to zero under certain circumstances */
	uint16_t m_pitch_count;     /* pitch counter; provides chirp rom address */

	int32_t m_u[11];
	int32_t m_x[10];

	uint16_t m_RNG;             /* the random noise generator configuration is: 1 + x + x^3 + x^4 + x^13 TODO: no it isn't */
	int16_t m_excitation_data;

	/* The TMS51xx has two different ways of providing output data: the
	   analog speaker pins (which were usually used) and the Digital I/O pin.
	   The internal DAC used to feed the analog pins is only 8 bits, and has the
	   funny clipping/clamping logic, while the digital pin gives full 10 bit
	   resolution of the output data.
	   TODO: add a way to set/reset this other than the FORCE_DIGITAL define
	 */
	uint8_t m_digital_select;

	int32_t m_speech_rom_bitnum;

	uint8_t m_romclk_hack_timer_started;
	uint8_t m_romclk_hack_state;

	emu_timer *m_romclk_hack_timer;
};

extern const device_type TMS5110;

class tms5100_device : public tms5110_device
{
public:
	tms5100_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);
protected:
	// device-level overrides
	virtual void device_start() override;
};

extern const device_type TMS5100;

class tmc0281_device : public tms5110_device
{
public:
	tmc0281_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);
protected:
	// device-level overrides
	virtual void device_start() override;
};

extern const device_type TMC0281;

class tms5100a_device : public tms5110_device
{
public:
	tms5100a_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);
protected:
	// device-level overrides
	virtual void device_start() override;
};

extern const device_type TMS5100A;

class tmc0281d_device : public tms5110_device
{
public:
	tmc0281d_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);
protected:
	// device-level overrides
	virtual void device_start() override;
};

extern const device_type TMC0281D;

class cd2801_device : public tms5110_device
{
public:
	cd2801_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);
protected:
	// device-level overrides
	virtual void device_start() override;
};

extern const device_type CD2801;

class cd2802_device : public tms5110_device
{
public:
	cd2802_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);
protected:
	// device-level overrides
	virtual void device_start() override;
};

extern const device_type CD2802;

class tms5110a_device : public tms5110_device
{
public:
	tms5110a_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);
protected:
	// device-level overrides
	virtual void device_start() override;
};

extern const device_type TMS5110A;

class m58817_device : public tms5110_device
{
public:
	m58817_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	uint8_t status_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);

protected:
	// device-level overrides
	virtual void device_start() override;
};

extern const device_type M58817;



/* PROM controlled TMS5110 interface */

class tmsprom_device : public device_t
{
public:
	tmsprom_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	static void set_region(device_t &device, const char *region) { downcast<tmsprom_device &>(device).m_prom.set_tag(region); }
	static void set_rom_size(device_t &device, uint32_t rom_size) { downcast<tmsprom_device &>(device).m_rom_size = rom_size; }
	static void set_pdc_bit(device_t &device, uint8_t pdc_bit) { downcast<tmsprom_device &>(device).m_pdc_bit = pdc_bit; }
	static void set_ctl1_bit(device_t &device, uint8_t ctl1_bit) { downcast<tmsprom_device &>(device).m_ctl1_bit = ctl1_bit; }
	static void set_ctl2_bit(device_t &device, uint8_t ctl2_bit) { downcast<tmsprom_device &>(device).m_ctl2_bit = ctl2_bit; }
	static void set_ctl4_bit(device_t &device, uint8_t ctl4_bit) { downcast<tmsprom_device &>(device).m_ctl4_bit = ctl4_bit; }
	static void set_ctl8_bit(device_t &device, uint8_t ctl8_bit) { downcast<tmsprom_device &>(device).m_ctl8_bit = ctl8_bit; }
	static void set_reset_bit(device_t &device, uint8_t reset_bit) { downcast<tmsprom_device &>(device).m_reset_bit = reset_bit; }
	static void set_stop_bit(device_t &device, uint8_t stop_bit) { downcast<tmsprom_device &>(device).m_stop_bit = stop_bit; }
	template<class _Object> static devcb_base &set_pdc_callback(device_t &device, _Object object) { return downcast<tmsprom_device &>(device).m_pdc_cb.set_callback(object); }
	template<class _Object> static devcb_base &set_ctl_callback(device_t &device, _Object object) { return downcast<tmsprom_device &>(device).m_ctl_cb.set_callback(object); }

	void m0_w(int state);
	int data_r();

	/* offset is rom # */
	void rom_csq_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void bit_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void enable_w(int state);

protected:
	// device-level overrides
	virtual void device_start() override;

	virtual void device_timer(emu_timer &timer, device_timer_id id, int param, void *ptr) override;

private:
	void register_for_save_states();
	void update_prom_cnt();

	// internal state
	/* Rom interface */
	uint32_t m_address;
	/* ctl lines */
	uint8_t  m_m0;
	uint8_t  m_enable;
	uint32_t  m_base_address;
	uint8_t  m_bit;

	int m_prom_cnt;

	required_region_ptr<uint8_t> m_rom;
	required_region_ptr<uint8_t> m_prom;
	uint32_t m_rom_size;                /* individual rom_size */
	uint8_t m_pdc_bit;                  /* bit # of pdc line */
	/* virtual bit 8: constant 0, virtual bit 9:constant 1 */
	uint8_t m_ctl1_bit;                 /* bit # of ctl1 line */
	uint8_t m_ctl2_bit;                 /* bit # of ctl2 line */
	uint8_t m_ctl4_bit;                 /* bit # of ctl4 line */
	uint8_t m_ctl8_bit;                 /* bit # of ctl8 line */
	uint8_t m_reset_bit;                /* bit # of rom reset */
	uint8_t m_stop_bit;                 /* bit # of stop */
	devcb_write_line m_pdc_cb;      /* tms pdc func */
	devcb_write8 m_ctl_cb;          /* tms ctl func */

	emu_timer *m_romclk_timer;
};

extern const device_type TMSPROM;

#define MCFG_TMSPROM_REGION(_region) \
	tmsprom_device::set_region(*device, "^" _region);

#define MCFG_TMSPROM_ROM_SIZE(_size) \
	tmsprom_device::set_rom_size(*device, _size);

#define MCFG_TMSPROM_PDC_BIT(_bit) \
	tmsprom_device::set_pdc_bit(*device, _bit);

#define MCFG_TMSPROM_CTL1_BIT(_bit) \
	tmsprom_device::set_ctl1_bit(*device, _bit);

#define MCFG_TMSPROM_CTL2_BIT(_bit) \
	tmsprom_device::set_ctl2_bit(*device, _bit);

#define MCFG_TMSPROM_CTL4_BIT(_bit) \
	tmsprom_device::set_ctl4_bit(*device, _bit);

#define MCFG_TMSPROM_CTL8_BIT(_bit) \
	tmsprom_device::set_ctl8_bit(*device, _bit);

#define MCFG_TMSPROM_RESET_BIT(_bit) \
	tmsprom_device::set_reset_bit(*device, _bit);

#define MCFG_TMSPROM_STOP_BIT(_bit) \
	tmsprom_device::set_stop_bit(*device, _bit);

#define MCFG_TMSPROM_PDC_CB(_devcb) \
	devcb = &tmsprom_device::set_pdc_callback(*device, DEVCB_##_devcb);

#define MCFG_TMSPROM_CTL_CB(_devcb) \
	devcb = &tmsprom_device::set_ctl_callback(*device, DEVCB_##_devcb);

#endif /* __TMS5110_H__ */
