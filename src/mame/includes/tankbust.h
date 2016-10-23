// license:GPL-2.0+
// copyright-holders:Jarek Burczynski
class tankbust_state : public driver_device
{
public:
	tankbust_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag),
		m_maincpu(*this, "maincpu"),
		m_subcpu(*this, "sub"),
		m_gfxdecode(*this, "gfxdecode"),
		m_palette(*this, "palette"),
		m_txtram(*this, "txtram"),
		m_videoram(*this, "videoram"),
		m_colorram(*this, "colorram"),
		m_spriteram(*this, "spriteram") { }

	required_device<cpu_device> m_maincpu;
	required_device<cpu_device> m_subcpu;
	required_device<gfxdecode_device> m_gfxdecode;
	required_device<palette_device> m_palette;

	required_shared_ptr<uint8_t> m_txtram;
	required_shared_ptr<uint8_t> m_videoram;
	required_shared_ptr<uint8_t> m_colorram;
	required_shared_ptr<uint8_t> m_spriteram;

	int m_latch;
	uint32_t m_timer1;
	int m_e0xx_data[8];
	uint8_t m_variable_data;
	tilemap_t *m_bg_tilemap;
	tilemap_t *m_txt_tilemap;
	uint8_t m_xscroll[2];
	uint8_t m_yscroll[2];
	uint8_t m_irq_mask;

	void soundlatch_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void e0xx_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t debug_output_area_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	uint8_t some_changing_input(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void background_videoram_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void background_colorram_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void txtram_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void xscroll_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void yscroll_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t soundlatch_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	uint8_t soundtimer_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);

	TILE_GET_INFO_MEMBER(get_bg_tile_info);
	TILE_GET_INFO_MEMBER(get_txt_tile_info);

	virtual void machine_start() override;
	virtual void machine_reset() override;
	virtual void video_start() override;
	DECLARE_PALETTE_INIT(tankbust);

	uint32_t screen_update(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	void draw_sprites(bitmap_ind16 &bitmap, const rectangle &cliprect);

	INTERRUPT_GEN_MEMBER(vblank_irq);
	TIMER_CALLBACK_MEMBER(soundlatch_callback);
	TIMER_CALLBACK_MEMBER(soundirqline_callback);
};
