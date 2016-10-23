// license:GPL-2.0+
// copyright-holders:Jarek Burczynski

#include "machine/buggychl.h"
#include "machine/gen_latch.h"
#include "sound/msm5232.h"

/* Disabled because the mcu dump is currently unavailable. -AS */
//#define USE_MCU

class msisaac_state : public driver_device
{
public:
	msisaac_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag),
		m_spriteram(*this, "spriteram"),
		m_videoram(*this, "videoram"),
		m_videoram3(*this, "videoram3"),
		m_videoram2(*this, "videoram2"),
		m_audiocpu(*this, "audiocpu"),
		m_maincpu(*this, "maincpu"),
		m_bmcu(*this, "bmcu"),
		m_msm(*this, "msm"),
		m_gfxdecode(*this, "gfxdecode"),
		m_palette(*this, "palette"),
		m_soundlatch(*this, "soundlatch") { }

	/* memory pointers */
	required_shared_ptr<uint8_t> m_spriteram;
	required_shared_ptr<uint8_t> m_videoram;
	required_shared_ptr<uint8_t> m_videoram3;
	required_shared_ptr<uint8_t> m_videoram2;

	/* video-related */
	bitmap_ind16    *m_tmp_bitmap1;
	bitmap_ind16    *m_tmp_bitmap2;
	tilemap_t     *m_bg_tilemap;
	tilemap_t     *m_fg_tilemap;
	tilemap_t     *m_bg2_tilemap;
	int         m_bg2_textbank;

	/* sound-related */
	int         m_sound_nmi_enable;
	int         m_pending_nmi;

	/* fake mcu (in msisaac.c) */
#ifndef USE_MCU
	uint8_t       m_mcu_val;
	uint8_t       m_direction;
#endif

	int         m_vol_ctrl[16];
	uint8_t       m_snd_ctrl0;
	uint8_t       m_snd_ctrl1;
	uint8_t       m_snd_ctrl2;
	uint8_t       m_snd_ctrl3;

	/* devices */
	required_device<cpu_device> m_audiocpu;
	required_device<cpu_device> m_maincpu;
	optional_device<buggychl_mcu_device> m_bmcu;
	required_device<msm5232_device> m_msm;
	required_device<gfxdecode_device> m_gfxdecode;
	required_device<palette_device> m_palette;
	required_device<generic_latch_8_device> m_soundlatch;

	void sound_command_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void nmi_disable_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void nmi_enable_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void ms_unknown_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t msisaac_mcu_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	uint8_t msisaac_mcu_status_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void msisaac_mcu_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void sound_control_1_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void msisaac_fg_scrolly_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void msisaac_fg_scrollx_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void msisaac_bg2_scrolly_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void msisaac_bg2_scrollx_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void msisaac_bg_scrolly_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void msisaac_bg_scrollx_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void msisaac_bg2_textbank_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void msisaac_bg_videoram_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void msisaac_bg2_videoram_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void msisaac_fg_videoram_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void sound_control_0_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	TILE_GET_INFO_MEMBER(get_fg_tile_info);
	TILE_GET_INFO_MEMBER(get_bg_tile_info);
	TILE_GET_INFO_MEMBER(get_bg2_tile_info);
	virtual void machine_start() override;
	virtual void machine_reset() override;
	virtual void video_start() override;
	void machine_reset_ta7630();
	uint32_t screen_update_msisaac(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	TIMER_CALLBACK_MEMBER(nmi_callback);
	void draw_sprites( bitmap_ind16 &bitmap, const rectangle &cliprect );
};
