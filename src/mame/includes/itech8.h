// license:BSD-3-Clause
// copyright-holders:Aaron Giles
/*************************************************************************

    Incredible Technologies/Strata system
    (8-bit blitter variant)

**************************************************************************/

#include "machine/ticket.h"
#include "video/tlc34076.h"
#include "video/tms34061.h"

#define YBUFFER_COUNT   15

class itech8_state : public driver_device
{
public:
	itech8_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag),
		m_maincpu(*this, "maincpu"),
		m_soundcpu(*this, "soundcpu"),
		m_subcpu(*this, "sub"),
		m_tms34061(*this, "tms34061"),
		m_tlc34076(*this, "tlc34076"),
		m_screen(*this, "screen"),
		m_ticket(*this, "ticket"),
		m_an(*this, { { "AN_C", "AN_D", "AN_E", "AN_F" } }),
		m_fakex(*this, "FAKEX"),
		m_fakey(*this, "FAKEY"),
		m_visarea(0, 0, 0, 0) { }

	enum
	{
		TIMER_IRQ_OFF,
		TIMER_BEHIND_BEAM_UPDATE,
		TIMER_DELAYED_SOUND_DATA,
		TIMER_BLITTER_DONE,
		TIMER_DELAYED_Z80_CONTROL
	};

	required_device<cpu_device> m_maincpu;
	required_device<cpu_device> m_soundcpu;
	optional_device<cpu_device> m_subcpu;
	required_device<tms34061_device> m_tms34061;
	required_device<tlc34076_device> m_tlc34076;
	required_device<screen_device> m_screen;
	required_device<ticket_dispenser_device> m_ticket;
	optional_ioport_array<4> m_an;
	optional_ioport m_fakex;
	optional_ioport m_fakey;

	rectangle m_visarea;

	uint8_t m_grom_bank;
	uint8_t m_blitter_int;
	uint8_t m_tms34061_int;
	uint8_t m_periodic_int;
	uint8_t m_sound_data;
	uint8_t m_pia_porta_data;
	uint8_t m_pia_portb_data;
	uint8_t m_z80_ctrl;
	uint8_t m_z80_port_val;
	uint8_t m_z80_clear_to_send;
	uint16_t m_sensor0;
	uint16_t m_sensor1;
	uint16_t m_sensor2;
	uint16_t m_sensor3;
	uint8_t m_curvx;
	uint8_t m_curvy;
	uint8_t m_curx;
	int8_t m_xbuffer[YBUFFER_COUNT];
	int8_t m_ybuffer[YBUFFER_COUNT];
	int m_ybuffer_next;
	int m_curxpos;
	int m_last_ytotal;
	uint8_t m_crosshair_vis;
	uint8_t m_blitter_data[16];
	uint8_t m_blit_in_progress;
	uint8_t m_page_select;
	offs_t m_fetch_offset;
	uint8_t m_fetch_rle_count;
	uint8_t m_fetch_rle_value;
	uint8_t m_fetch_rle_literal;
	uint8_t *m_grom_base;
	uint32_t m_grom_size;
	uint8_t m_grmatch_palcontrol;
	uint8_t m_grmatch_xscroll;
	rgb_t m_grmatch_palette[2][16];
	emu_timer *m_irq_off_timer;
	emu_timer *m_behind_beam_update_timer;
	emu_timer *m_delayed_sound_data_timer;
	emu_timer *m_blitter_done_timer;
	emu_timer *m_delayed_z80_control_timer;

	// common
	void generate_tms34061_interrupt(int state);
	void nmi_ack_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void blitter_bank_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void rimrockn_bank_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void pia_portb_out(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void sound_data_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void gtg2_sound_data_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t sound_data_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void grom_bank_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void grom_bank16_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void display_page16_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void palette16_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void palette_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void page_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t blitter_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void blitter_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void tms34061_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t tms34061_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void grmatch_palette_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void grmatch_xscroll_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void pia_porta_out(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void ym2203_portb_out(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);

	ioport_value special_r(ioport_field &field, void *param);
	ioport_value gtg_mux(ioport_field &field, void *param);

	void init_rimrockn();
	void init_grmatch();
	void init_peggle();
	void init_slikshot();
	void init_neckneck();
	void init_arligntn();
	void init_hstennis();
	void init_sstrike();
	virtual void machine_start() override;
	virtual void machine_reset() override;
	virtual void video_start() override;
	void video_start_slikshot();
	void machine_start_sstrike();

	uint32_t screen_update_2layer(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect);
	uint32_t screen_update_grmatch(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect);
	uint32_t screen_update_slikshot(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect);
	uint32_t screen_update_2page(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect);
	uint32_t screen_update_2page_large(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect);

	void generate_nmi(device_t &device);
	void irq_off(void *ptr, int32_t param);
	void behind_the_beam_update(void *ptr, int32_t param);
	void delayed_sound_data_w(void *ptr, int32_t param);
	void blitter_done(void *ptr, int32_t param);
	void grmatch_palette_update(timer_device &timer, void *ptr, int32_t param);

	inline uint8_t fetch_next_raw();
	inline void consume_raw(int count);
	inline uint8_t fetch_next_rle();
	inline void consume_rle(int count);
	void perform_blit(address_space &space);
	void update_interrupts(int periodic, int tms34061, int blitter);

	/*----------- defined in machine/slikshot.c -----------*/

	uint8_t slikz80_port_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void slikz80_port_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);

	uint8_t slikshot_z80_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	uint8_t slikshot_z80_control_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void slikshot_z80_control_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);

	void inters_to_vels(uint16_t inter1, uint16_t inter2, uint16_t inter3, uint8_t beams,
							uint8_t *xres, uint8_t *vxres, uint8_t *vyres);
	void vels_to_inters(uint8_t x, uint8_t vx, uint8_t vy,
							uint16_t *inter1, uint16_t *inter2, uint16_t *inter3, uint8_t *beams);
	void inters_to_words(uint16_t inter1, uint16_t inter2, uint16_t inter3, uint8_t *beams,
							uint16_t *word1, uint16_t *word2, uint16_t *word3);

	void words_to_sensors(uint16_t word1, uint16_t word2, uint16_t word3, uint8_t beams,
							uint16_t *sens0, uint16_t *sens1, uint16_t *sens2, uint16_t *sens3);
	void compute_sensors();
	void delayed_z80_control_w(void *ptr, int32_t param);

protected:
	virtual void device_timer(emu_timer &timer, device_timer_id id, int param, void *ptr) override;
};
