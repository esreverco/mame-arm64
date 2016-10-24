// license:BSD-3-Clause
// copyright-holders:David Haywood, Sylvain Glaize, Paul Priest, Olivier Galibert

#include "cpu/sh2/sh2.h"


struct hit_t
{
	uint16_t x1p, y1p, z1p, x1s, y1s, z1s;
	uint16_t x2p, y2p, z2p, x2s, y2s, z2s;
	uint16_t org;

	uint16_t x1_p1, x1_p2, y1_p1, y1_p2, z1_p1, z1_p2;
	uint16_t x2_p1, x2_p2, y2_p1, y2_p2, z2_p1, z2_p2;
	uint16_t x1tox2, y1toy2, z1toz2;
	int16_t x_in, y_in, z_in;
	uint16_t flag;

	uint8_t disconnect;
};


class skns_state : public driver_device
{
public:
	skns_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag),
		m_maincpu(*this,"maincpu"),
		m_spritegen(*this, "spritegen"),
		m_gfxdecode(*this, "gfxdecode"),
		m_palette(*this, "palette"),
		m_spriteram(*this,"spriteram"),
		m_spc_regs(*this, "spc_regs"),
		m_v3_regs(*this, "v3_regs"),
		m_tilemapA_ram(*this, "tilemapa_ram"),
		m_tilemapB_ram(*this, "tilemapb_ram"),
		m_v3slc_ram(*this, "v3slc_ram"),
		m_pal_regs(*this, "pal_regs"),
		m_palette_ram(*this, "palette_ram"),
		m_v3t_ram(*this, "v3t_ram"),
		m_main_ram(*this, "main_ram"),
		m_cache_ram(*this, "cache_ram") { }

	required_device<sh2_device> m_maincpu;
	required_device<sknsspr_device> m_spritegen;
	required_device<gfxdecode_device> m_gfxdecode;
	required_device<palette_device> m_palette;

	required_shared_ptr<uint32_t> m_spriteram;
	required_shared_ptr<uint32_t> m_spc_regs;
	required_shared_ptr<uint32_t> m_v3_regs;
	required_shared_ptr<uint32_t> m_tilemapA_ram;
	required_shared_ptr<uint32_t> m_tilemapB_ram;
	required_shared_ptr<uint32_t> m_v3slc_ram;
	required_shared_ptr<uint32_t> m_pal_regs;
	required_shared_ptr<uint32_t> m_palette_ram;
	required_shared_ptr<uint32_t> m_v3t_ram;
	required_shared_ptr<uint32_t> m_main_ram;
	required_shared_ptr<uint32_t> m_cache_ram;

	hit_t m_hit;
	bitmap_ind16 m_sprite_bitmap;
	bitmap_ind16 m_tilemap_bitmap_lower;
	bitmap_ind8 m_tilemap_bitmapflags_lower;
	bitmap_ind16 m_tilemap_bitmap_higher;
	bitmap_ind8 m_tilemap_bitmapflags_higher;
	int m_depthA;
	int m_depthB;
	int m_use_spc_bright;
	int m_use_v3_bright;
	uint8_t m_bright_spc_b;
	uint8_t m_bright_spc_g;
	uint8_t m_bright_spc_r;
	uint8_t m_bright_spc_b_trans;
	uint8_t m_bright_spc_g_trans;
	uint8_t m_bright_spc_r_trans;
	uint8_t m_bright_v3_b;
	uint8_t m_bright_v3_g;
	uint8_t m_bright_v3_r;
	uint8_t m_bright_v3_b_trans;
	uint8_t m_bright_v3_g_trans;
	uint8_t m_bright_v3_r_trans;
	int m_spc_changed;
	int m_v3_changed;
	int m_palette_updated;
	int m_alt_enable_background;
	int m_alt_enable_sprites;
	tilemap_t *m_tilemap_A;
	tilemap_t *m_tilemap_B;
	uint8_t *m_btiles;
	uint8_t m_region;

	void hit_w(address_space &space, offs_t offset, uint32_t data, uint32_t mem_mask = 0xffffffff);
	void hit2_w(address_space &space, offs_t offset, uint32_t data, uint32_t mem_mask = 0xffffffff);
	uint32_t hit_r(address_space &space, offs_t offset, uint32_t mem_mask = 0xffffffff);
	void io_w(address_space &space, offs_t offset, uint32_t data, uint32_t mem_mask = 0xffffffff);
	void v3t_w(address_space &space, offs_t offset, uint32_t data, uint32_t mem_mask = 0xffffffff);
	void pal_regs_w(address_space &space, offs_t offset, uint32_t data, uint32_t mem_mask = 0xffffffff);
	void palette_ram_w(address_space &space, offs_t offset, uint32_t data, uint32_t mem_mask = 0xffffffff);
	void tilemapA_w(address_space &space, offs_t offset, uint32_t data, uint32_t mem_mask = 0xffffffff);
	void tilemapB_w(address_space &space, offs_t offset, uint32_t data, uint32_t mem_mask = 0xffffffff);
	void v3_regs_w(address_space &space, offs_t offset, uint32_t data, uint32_t mem_mask = 0xffffffff);

	uint32_t gutsn_speedup_r(address_space &space, offs_t offset, uint32_t mem_mask = 0xffffffff);
	uint32_t cyvern_speedup_r(address_space &space, offs_t offset, uint32_t mem_mask = 0xffffffff);
	uint32_t puzzloopj_speedup_r(address_space &space, offs_t offset, uint32_t mem_mask = 0xffffffff);
	uint32_t puzzloopa_speedup_r(address_space &space, offs_t offset, uint32_t mem_mask = 0xffffffff);
	uint32_t puzzloopu_speedup_r(address_space &space, offs_t offset, uint32_t mem_mask = 0xffffffff);
	uint32_t puzzloope_speedup_r(address_space &space, offs_t offset, uint32_t mem_mask = 0xffffffff);
	uint32_t senknow_speedup_r(address_space &space, offs_t offset, uint32_t mem_mask = 0xffffffff);
	uint32_t teljan_speedup_r(address_space &space, offs_t offset, uint32_t mem_mask = 0xffffffff);
	uint32_t jjparads_speedup_r(address_space &space, offs_t offset, uint32_t mem_mask = 0xffffffff);
	uint32_t jjparad2_speedup_r(address_space &space, offs_t offset, uint32_t mem_mask = 0xffffffff);
	uint32_t ryouran_speedup_r(address_space &space, offs_t offset, uint32_t mem_mask = 0xffffffff);
	uint32_t galpans2_speedup_r(address_space &space, offs_t offset, uint32_t mem_mask = 0xffffffff);
	uint32_t panicstr_speedup_r(address_space &space, offs_t offset, uint32_t mem_mask = 0xffffffff);
	uint32_t sengekis_speedup_r(address_space &space, offs_t offset, uint32_t mem_mask = 0xffffffff);
	uint32_t sengekij_speedup_r(address_space &space, offs_t offset, uint32_t mem_mask = 0xffffffff);

	DECLARE_CUSTOM_INPUT_MEMBER(paddle_r);

	void init_sengekis();
	void init_cyvern();
	void init_puzzloopa();
	void init_teljan();
	void init_panicstr();
	void init_puzzloope();
	void init_sengekij();
	void init_puzzloopj();
	void init_sarukani();
	void init_gutsn();
	void init_jjparad2();
	void init_galpans3();
	void init_jjparads();
	void init_galpans2();
	void init_galpanis();
	void init_puzzloopu();
	void init_senknow();
	void init_galpani4();
	void init_ryouran();
	virtual void machine_start() override;
	virtual void machine_reset() override;
	virtual void video_start() override;
	virtual void video_reset() override;
	void machine_reset_sknsa();
	void machine_reset_sknsj();
	void machine_reset_sknsu();
	void machine_reset_sknse();
	void machine_reset_sknsk();

	void get_tilemap_A_tile_info(tilemap_t &tilemap, tile_data &tileinfo, tilemap_memory_index tile_index);
	void get_tilemap_B_tile_info(tilemap_t &tilemap, tile_data &tileinfo, tilemap_memory_index tile_index);
	uint32_t screen_update(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect);
	void interrupt_callback(timer_device &timer, void *ptr, int32_t param);
	void irq(timer_device &timer, void *ptr, int32_t param);
	void draw_roz(bitmap_ind16 &bitmap, bitmap_ind8& bitmapflags, const rectangle &cliprect, tilemap_t *tmap, uint32_t startx, uint32_t starty, int incxx, int incxy, int incyx, int incyy, int wraparound, int columnscroll, uint32_t* scrollram);
	void palette_set_rgb_brightness (int offset, uint8_t brightness_r, uint8_t brightness_g, uint8_t brightness_b);
	void palette_update();
	void draw_a( bitmap_ind16 &bitmap, bitmap_ind8 &bitmap_flags, const rectangle &cliprect, int tran );
	void draw_b( bitmap_ind16 &bitmap, bitmap_ind8 &bitmap_flags, const rectangle &cliprect, int tran );
	void hit_recalc();
	void init_drc();
	void set_drc_pcflush(uint32_t addr);
};
