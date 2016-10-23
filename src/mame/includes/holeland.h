// license:BSD-3-Clause
// copyright-holders:Mathis Rosenhauer
/*************************************************************************

    Hole Land

*************************************************************************/

class holeland_state : public driver_device
{
public:
	holeland_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag),
		m_maincpu(*this, "maincpu"),
		m_gfxdecode(*this, "gfxdecode"),
		m_palette(*this, "palette"),
		m_videoram(*this, "videoram"),
		m_colorram(*this, "colorram"),
		m_spriteram(*this, "spriteram") { }


	/* devices */
	required_device<cpu_device> m_maincpu;
	required_device<gfxdecode_device> m_gfxdecode;
	required_device<palette_device> m_palette;

	/* memory pointers */
	required_shared_ptr<uint8_t> m_videoram;
	required_shared_ptr<uint8_t> m_colorram;
	required_shared_ptr<uint8_t> m_spriteram;

	/* video-related */
	tilemap_t    *m_bg_tilemap;
	int        m_palette_offset;
	int        m_po[2];

	void videoram_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void colorram_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void pal_offs_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void scroll_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void flipscreen_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);

	TILE_GET_INFO_MEMBER(holeland_get_tile_info);
	TILE_GET_INFO_MEMBER(crzrally_get_tile_info);

	void video_start_holeland();
	void video_start_crzrally();

	uint32_t screen_update_holeland(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	uint32_t screen_update_crzrally(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	void holeland_draw_sprites( bitmap_ind16 &bitmap, const rectangle &cliprect );
	void crzrally_draw_sprites( bitmap_ind16 &bitmap,const rectangle &cliprect );
};
