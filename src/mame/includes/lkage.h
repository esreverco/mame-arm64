// license:BSD-3-Clause
// copyright-holders:Phil Stroffolino

#include "machine/gen_latch.h"

class lkage_state : public driver_device
{
public:
	lkage_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag),
		m_vreg(*this, "vreg"),
		m_scroll(*this, "scroll"),
		m_spriteram(*this, "spriteram"),
		m_videoram(*this, "videoram"),
		m_maincpu(*this, "maincpu"),
		m_audiocpu(*this, "audiocpu"),
		m_mcu(*this, "mcu"),
		m_gfxdecode(*this, "gfxdecode"),
		m_palette(*this, "palette"),
		m_soundlatch(*this, "soundlatch") { }

	required_shared_ptr<uint8_t> m_vreg;
	required_shared_ptr<uint8_t> m_scroll;
	required_shared_ptr<uint8_t> m_spriteram;
	required_shared_ptr<uint8_t> m_videoram;

	/* video-related */
	tilemap_t *m_bg_tilemap;
	tilemap_t *m_fg_tilemap;
	tilemap_t *m_tx_tilemap;
	uint8_t m_bg_tile_bank;
	uint8_t m_fg_tile_bank;
	uint8_t m_tx_tile_bank;

	int m_sprite_dx;

	/* misc */
	int m_sound_nmi_enable;
	int m_pending_nmi;

	/* mcu */
	uint8_t m_from_main;
	uint8_t m_from_mcu;
	int m_mcu_sent;
	int m_main_sent;
	uint8_t m_port_a_in;
	uint8_t m_port_a_out;
	uint8_t m_ddr_a;
	uint8_t m_port_b_in;
	uint8_t m_port_b_out;
	uint8_t m_ddr_b;
	uint8_t m_port_c_in;
	uint8_t m_port_c_out;
	uint8_t m_ddr_c;

	/* lkageb fake mcu */
	uint8_t m_mcu_val;
	int m_mcu_ready;    /* cpu data/mcu ready status */

	/* devices */
	required_device<cpu_device> m_maincpu;
	required_device<cpu_device> m_audiocpu;
	optional_device<cpu_device> m_mcu;
	required_device<gfxdecode_device> m_gfxdecode;
	required_device<palette_device> m_palette;
	required_device<generic_latch_8_device> m_soundlatch;

	DECLARE_WRITE8_MEMBER(lkage_sound_command_w);
	DECLARE_WRITE8_MEMBER(lkage_sh_nmi_disable_w);
	DECLARE_WRITE8_MEMBER(lkage_sh_nmi_enable_w);
	DECLARE_READ8_MEMBER(sound_status_r);
	DECLARE_READ8_MEMBER(port_fetch_r);
	DECLARE_READ8_MEMBER(fake_mcu_r);
	DECLARE_WRITE8_MEMBER(fake_mcu_w);
	DECLARE_READ8_MEMBER(fake_status_r);
	DECLARE_READ8_MEMBER(lkage_68705_port_a_r);
	DECLARE_WRITE8_MEMBER(lkage_68705_port_a_w);
	DECLARE_WRITE8_MEMBER(lkage_68705_ddr_a_w);
	DECLARE_READ8_MEMBER(lkage_68705_port_b_r);
	DECLARE_WRITE8_MEMBER(lkage_68705_port_b_w);
	DECLARE_WRITE8_MEMBER(lkage_68705_ddr_b_w);
	DECLARE_READ8_MEMBER(lkage_68705_port_c_r);
	DECLARE_WRITE8_MEMBER(lkage_68705_port_c_w);
	DECLARE_WRITE8_MEMBER(lkage_68705_ddr_c_w);
	DECLARE_WRITE8_MEMBER(lkage_mcu_w);
	DECLARE_READ8_MEMBER(lkage_mcu_r);
	DECLARE_READ8_MEMBER(lkage_mcu_status_r);
	DECLARE_WRITE8_MEMBER(lkage_videoram_w);
	void init_bygone();
	void init_lkage();
	void init_lkageb();
	TILE_GET_INFO_MEMBER(get_bg_tile_info);
	TILE_GET_INFO_MEMBER(get_fg_tile_info);
	TILE_GET_INFO_MEMBER(get_tx_tile_info);
	virtual void machine_start() override;
	virtual void machine_reset() override;
	virtual void video_start() override;
	uint32_t screen_update_lkage(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	TIMER_CALLBACK_MEMBER(nmi_callback);
	void draw_sprites( screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect );
};
