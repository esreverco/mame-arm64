// license:BSD-3-Clause
// copyright-holders:Aaron Giles
/*************************************************************************

    Sega G-80 raster hardware

*************************************************************************/
#include "sound/samples.h"
#include "machine/segag80.h"
#include "sound/sn76496.h"
#include "audio/segasnd.h"
#include "machine/gen_latch.h"


class sega005_sound_device;

class segag80r_state : public driver_device
{
public:
	enum
	{
		TIMER_VBLANK_LATCH_CLEAR
	};

	segag80r_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag),
		m_mainram(*this, "mainram"),
		m_videoram(*this, "videoram"),
		m_sn1(*this, "sn1"),
		m_sn2(*this, "sn2"),
		m_maincpu(*this, "maincpu"),
		m_audiocpu(*this, "audiocpu"),
		m_samples(*this, "samples"),
		m_speech(*this, "segaspeech"),
		m_usbsnd(*this, "usbsnd"),
		m_005snd(*this, "005"),
		m_gfxdecode(*this, "gfxdecode"),
		m_screen(*this, "screen"),
		m_palette(*this, "palette"),
		m_soundlatch(*this, "soundlatch"),
		m_decrypted_opcodes(*this, "decrypted_opcodes") { }

	required_shared_ptr<uint8_t> m_mainram;
	required_shared_ptr<uint8_t> m_videoram;

	optional_device<sn76496_device> m_sn1;
	optional_device<sn76496_device> m_sn2;
	required_device<cpu_device> m_maincpu;
	optional_device<cpu_device> m_audiocpu;
	optional_device<samples_device> m_samples;
	optional_device<speech_sound_device> m_speech;
	optional_device<usb_sound_device> m_usbsnd;
	optional_device<sega005_sound_device> m_005snd;
	required_device<gfxdecode_device> m_gfxdecode;
	required_device<screen_device> m_screen;
	required_device<palette_device> m_palette;
	optional_device<generic_latch_8_device> m_soundlatch;
	optional_shared_ptr<uint8_t> m_decrypted_opcodes;

	std::vector<uint8_t> m_paletteram;

	uint8_t m_sound_state[2];
	uint8_t m_sound_rate;
	uint16_t m_sound_addr;
	uint8_t m_sound_data;
	uint8_t m_square_state;
	uint8_t m_square_count;
	uint8_t m_n7751_command;
	uint8_t m_n7751_busy;
	segag80_decrypt_func m_decrypt;
	uint8_t m_background_pcb;
	double m_rweights[3];
	double m_gweights[3];
	double m_bweights[2];
	uint8_t m_video_control;
	uint8_t m_video_flip;
	uint8_t m_vblank_latch;
	tilemap_t *m_spaceod_bg_htilemap;
	tilemap_t *m_spaceod_bg_vtilemap;
	uint16_t m_spaceod_hcounter;
	uint16_t m_spaceod_vcounter;
	uint8_t m_spaceod_fixed_color;
	uint8_t m_spaceod_bg_control;
	uint8_t m_spaceod_bg_detect;
	tilemap_t *m_bg_tilemap;
	uint8_t m_bg_enable;
	uint8_t m_bg_char_bank;
	uint16_t m_bg_scrollx;
	uint16_t m_bg_scrolly;
	uint8_t m_pignewt_bg_color_offset;
	void mainram_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void vidram_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void monsterb_vidram_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void pignewt_vidram_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void sindbadm_vidram_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t mangled_ports_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	uint8_t spaceod_mangled_ports_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	uint8_t spaceod_port_fc_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void coin_count_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void segag80r_videoram_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t segag80r_video_port_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void segag80r_video_port_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t spaceod_back_port_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void spaceod_back_port_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void monsterb_videoram_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void monsterb_back_port_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void pignewt_videoram_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void pignewt_back_color_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void pignewt_back_port_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void sindbadm_videoram_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void sindbadm_back_port_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void astrob_sound_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void spaceod_sound_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t n7751_rom_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	uint8_t n7751_command_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	uint8_t n7751_t1_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	DECLARE_INPUT_CHANGED_MEMBER(service_switch);
	void usb_ram_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void sindbadm_soundport_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void sindbadm_misc_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void sindbadm_sn1_SN76496_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void sindbadm_sn2_SN76496_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void init_spaceod();
	void init_sindbadm();
	void init_pignewt();
	void init_monsterb();
	void init_005();
	void init_monster2();
	void init_astrob();
	void spaceod_get_tile_info(tilemap_t &tilemap, tile_data &tileinfo, tilemap_memory_index tile_index);
	tilemap_memory_index spaceod_scan_rows(uint32_t col, uint32_t row, uint32_t num_cols, uint32_t num_rows);
	void bg_get_tile_info(tilemap_t &tilemap, tile_data &tileinfo, tilemap_memory_index tile_index);
	virtual void machine_start() override;
	virtual void video_start() override;
	uint32_t screen_update_segag80r(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	void segag80r_vblank_start(device_t &device);
	void sindbadm_vblank_start(device_t &device);
	void sega005_sound_a_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void sega005_sound_b_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	inline void sega005_update_sound_data();
	void monsterb_sound_a_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void monsterb_sound_b_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t n7751_status_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void n7751_command_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void n7751_rom_control_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void n7751_p2_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void vblank_latch_set();
	void g80_set_palette_entry(int entry, uint8_t data);
	void spaceod_bg_init_palette();
	void draw_videoram(bitmap_ind16 &bitmap, const rectangle &cliprect, const uint8_t *transparent_pens);
	void draw_background_spaceod(bitmap_ind16 &bitmap, const rectangle &cliprect);
	void draw_background_page_scroll(bitmap_ind16 &bitmap, const rectangle &cliprect);
	void draw_background_full_scroll(bitmap_ind16 &bitmap, const rectangle &cliprect);
	offs_t decrypt_offset(address_space &space, offs_t offset);
	inline uint8_t demangle(uint8_t d7d6, uint8_t d5d4, uint8_t d3d2, uint8_t d1d0);
	void monsterb_expand_gfx(const char *region);

protected:
	virtual void device_timer(emu_timer &timer, device_timer_id id, int param, void *ptr) override;
};


/*----------- defined in audio/segag80r.c -----------*/


class sega005_sound_device : public device_t,
									public device_sound_interface
{
public:
	sega005_sound_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	emu_timer *m_sega005_sound_timer;
	sound_stream *m_sega005_stream;

protected:
	// device-level overrides
	virtual void device_config_complete() override;
	virtual void device_start() override;

	// sound stream update overrides
	virtual void sound_stream_update(sound_stream &stream, stream_sample_t **inputs, stream_sample_t **outputs, int samples) override;

private:
	// internal state
	void sega005_auto_timer(void *ptr, int32_t param);
};

extern const device_type SEGA005;


MACHINE_CONFIG_EXTERN( astrob_sound_board );
MACHINE_CONFIG_EXTERN( 005_sound_board );
MACHINE_CONFIG_EXTERN( spaceod_sound_board );
MACHINE_CONFIG_EXTERN( monsterb_sound_board );

/*----------- defined in video/segag80r.c -----------*/

#define G80_BACKGROUND_NONE         0
#define G80_BACKGROUND_SPACEOD      1
#define G80_BACKGROUND_MONSTERB     2
#define G80_BACKGROUND_PIGNEWT      3
#define G80_BACKGROUND_SINDBADM     4
