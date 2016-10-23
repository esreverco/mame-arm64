// license:BSD-3-Clause
// copyright-holders:Nicola Salmoria

#include "machine/gen_latch.h"

class cclimber_state : public driver_device
{
public:
	cclimber_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag),
		m_maincpu(*this, "maincpu"),
		m_audiocpu(*this, "audiocpu"),
		m_gfxdecode(*this, "gfxdecode"),
		m_palette(*this, "palette"),
		m_soundlatch(*this, "soundlatch"),
		m_bigsprite_videoram(*this, "bigspriteram"),
		m_videoram(*this, "videoram"),
		m_column_scroll(*this, "column_scroll"),
		m_spriteram(*this, "spriteram"),
		m_bigsprite_control(*this, "bigspritectrl"),
		m_colorram(*this, "colorram"),
		m_flip_screen(*this, "flip_screen"),
		m_swimmer_side_background_enabled(*this, "sidebg_enable"),
		m_swimmer_palettebank(*this, "palettebank"),
		m_swimmer_background_color(*this, "bgcolor"),
		m_toprollr_bg_videoram(*this, "bg_videoram"),
		m_toprollr_bg_coloram(*this, "bg_coloram"),
		m_decrypted_opcodes(*this, "decrypted_opcodes") { }

	required_device<cpu_device> m_maincpu;
	optional_device<cpu_device> m_audiocpu;
	required_device<gfxdecode_device> m_gfxdecode;
	required_device<palette_device> m_palette;
	optional_device<generic_latch_8_device> m_soundlatch;

	required_shared_ptr<uint8_t> m_bigsprite_videoram;
	required_shared_ptr<uint8_t> m_videoram;
	optional_shared_ptr<uint8_t> m_column_scroll;
	required_shared_ptr<uint8_t> m_spriteram;
	required_shared_ptr<uint8_t> m_bigsprite_control;
	required_shared_ptr<uint8_t> m_colorram;
	required_shared_ptr<uint8_t> m_flip_screen;
	optional_shared_ptr<uint8_t> m_swimmer_side_background_enabled;
	optional_shared_ptr<uint8_t> m_swimmer_palettebank;
	optional_shared_ptr<uint8_t> m_swimmer_background_color;
	optional_shared_ptr<uint8_t> m_toprollr_bg_videoram;
	optional_shared_ptr<uint8_t> m_toprollr_bg_coloram;
	optional_shared_ptr<uint8_t> m_decrypted_opcodes;

	uint8_t m_yamato_p0;
	uint8_t m_yamato_p1;
	uint8_t m_toprollr_rombank;
	uint8_t m_nmi_mask;
	tilemap_t *m_pf_tilemap;
	tilemap_t *m_bs_tilemap;
	tilemap_t *m_toproller_bg_tilemap;
	std::unique_ptr<uint8_t[]> m_opcodes;

	DECLARE_WRITE8_MEMBER(swimmer_sh_soundlatch_w);
	DECLARE_WRITE8_MEMBER(yamato_p0_w);
	DECLARE_WRITE8_MEMBER(yamato_p1_w);
	DECLARE_READ8_MEMBER(yamato_p0_r);
	DECLARE_READ8_MEMBER(yamato_p1_r);
	DECLARE_WRITE8_MEMBER(toprollr_rombank_w);
	DECLARE_WRITE8_MEMBER(nmi_mask_w);
	DECLARE_WRITE8_MEMBER(cclimber_colorram_w);
	DECLARE_WRITE8_MEMBER(cannonb_flip_screen_w);

	virtual void machine_start() override;
	void init_cclimber();
	void init_yamato();
	void init_ckongb();
	void init_toprollr();
	void init_cclimberj();
	void init_cannonb2();
	void init_cannonb();
	void init_dking();
	void init_rpatrol();
	void machine_reset_cclimber();
	void video_start_cclimber();
	DECLARE_PALETTE_INIT(cclimber);
	void video_start_swimmer();
	DECLARE_PALETTE_INIT(swimmer);
	DECLARE_PALETTE_INIT(yamato);
	void video_start_toprollr();
	DECLARE_PALETTE_INIT(toprollr);

	TILE_GET_INFO_MEMBER(cclimber_get_pf_tile_info);
	TILE_GET_INFO_MEMBER(swimmer_get_pf_tile_info);
	TILE_GET_INFO_MEMBER(toprollr_get_pf_tile_info);
	TILE_GET_INFO_MEMBER(cclimber_get_bs_tile_info);
	TILE_GET_INFO_MEMBER(toprollr_get_bs_tile_info);
	TILE_GET_INFO_MEMBER(toproller_get_bg_tile_info);

	uint32_t screen_update_cclimber(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	uint32_t screen_update_swimmer(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	uint32_t screen_update_yamato(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	uint32_t screen_update_toprollr(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	void swimmer_set_background_pen();
	void draw_playfield(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	void cclimber_draw_bigsprite(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	void toprollr_draw_bigsprite(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	void cclimber_draw_sprites(bitmap_ind16 &bitmap, const rectangle &cliprect, gfx_element *gfx);
	void toprollr_draw_sprites(bitmap_ind16 &bitmap, const rectangle &cliprect, gfx_element *gfx);
	void swimmer_draw_sprites(bitmap_ind16 &bitmap, const rectangle &cliprect, gfx_element *gfx);
	void cclimber_decode(const uint8_t convtable[8][16]);

	INTERRUPT_GEN_MEMBER(vblank_irq);
	INTERRUPT_GEN_MEMBER(bagmanf_vblank_irq);
};
