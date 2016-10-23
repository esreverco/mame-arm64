// license:BSD-3-Clause
// copyright-holders:Aaron Giles
/*************************************************************************

    BattleToads

    Common definitions

*************************************************************************/

#include "cpu/tms34010/tms34010.h"
#include "cpu/z80/z80.h"
#include "video/tlc34076.h"
#include "sound/bsmt2000.h"
#include "machine/nvram.h"

class btoads_state : public driver_device
{
public:
	btoads_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag),
			m_audiocpu(*this, "audiocpu"),
			m_bsmt(*this, "bsmt"),
			m_tlc34076(*this, "tlc34076"),
			m_vram_fg0(*this, "vram_fg0", 16),
			m_vram_fg1(*this, "vram_fg1", 16),
			m_vram_fg_data(*this, "vram_fg_data"),
			m_vram_bg0(*this, "vram_bg0"),
			m_vram_bg1(*this, "vram_bg1"),
			m_sprite_scale(*this, "sprite_scale"),
			m_sprite_control(*this, "sprite_control") ,
		m_maincpu(*this, "maincpu"),
		m_screen(*this, "screen") { }

	// in drivers/btoads
	void main_sound_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	uint16_t main_sound_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	DECLARE_CUSTOM_INPUT_MEMBER( main_to_sound_r );
	DECLARE_CUSTOM_INPUT_MEMBER( sound_to_main_r );
	void sound_data_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t sound_data_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	uint8_t sound_ready_to_send_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	uint8_t sound_data_ready_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void sound_int_state_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t bsmt_ready_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void bsmt2000_port_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);

	// in video/btoads
	void misc_control_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void display_control_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void scroll0_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void scroll1_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void paletteram_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	uint16_t paletteram_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	void vram_bg0_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void vram_bg1_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	uint16_t vram_bg0_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	uint16_t vram_bg1_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	void vram_fg_display_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void vram_fg_draw_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	uint16_t vram_fg_display_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	uint16_t vram_fg_draw_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	void render_sprite_row(uint16_t *sprite_source, uint32_t address);
	TMS340X0_TO_SHIFTREG_CB_MEMBER(to_shiftreg);
	TMS340X0_FROM_SHIFTREG_CB_MEMBER(from_shiftreg);
	TMS340X0_SCANLINE_RGB32_CB_MEMBER(scanline_update);

protected:
	// device overrides
	virtual void device_timer(emu_timer &timer, device_timer_id id, int param, void *ptr) override;

	// driver_device overrides
	virtual void machine_start() override;
	virtual void video_start() override;

	// timer IDs
	enum
	{
		TIMER_ID_NOP,
		TIMER_ID_DELAYED_SOUND
	};

	// devices
	required_device<z80_device> m_audiocpu;
	required_device<bsmt2000_device> m_bsmt;
	required_device<tlc34076_device> m_tlc34076;

	// shared pointers
	required_shared_ptr<uint8_t> m_vram_fg0;
	required_shared_ptr<uint8_t> m_vram_fg1;
	required_shared_ptr<uint16_t> m_vram_fg_data;
	required_shared_ptr<uint16_t> m_vram_bg0;
	required_shared_ptr<uint16_t> m_vram_bg1;
	required_shared_ptr<uint16_t> m_sprite_scale;
	required_shared_ptr<uint16_t> m_sprite_control;

	// state
	uint8_t m_main_to_sound_data;
	uint8_t m_main_to_sound_ready;
	uint8_t m_sound_to_main_data;
	uint8_t m_sound_to_main_ready;
	uint8_t m_sound_int_state;
	uint8_t *m_vram_fg_draw;
	uint8_t *m_vram_fg_display;
	int32_t m_xscroll0;
	int32_t m_yscroll0;
	int32_t m_xscroll1;
	int32_t m_yscroll1;
	uint8_t m_screen_control;
	uint16_t m_sprite_source_offs;
	uint8_t *m_sprite_dest_base;
	uint16_t m_sprite_dest_offs;
	uint16_t m_misc_control;
	int m_xcount;
	required_device<cpu_device> m_maincpu;
	required_device<screen_device> m_screen;
};
