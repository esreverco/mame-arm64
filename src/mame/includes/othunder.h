// license:BSD-3-Clause
// copyright-holders:David Graves
/*************************************************************************

    Operation Thunderbolt

*************************************************************************/

#include "audio/taitosnd.h"
#include "machine/eepromser.h"
#include "machine/taitoio.h"
#include "sound/flt_vol.h"
#include "video/tc0100scn.h"
#include "video/tc0110pcr.h"


struct othunder_tempsprite
{
	int gfx;
	int code,color;
	int flipx,flipy;
	int x,y;
	int zoomx,zoomy;
	int primask;
};


class othunder_state : public driver_device
{
public:
	enum
	{
		TIMER_AD_INTERRUPT
	};

	othunder_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag),
		m_spriteram(*this,"spriteram"),
		m_maincpu(*this, "maincpu"),
		m_audiocpu(*this, "audiocpu"),
		m_eeprom(*this, "eeprom"),
		m_tc0220ioc(*this, "tc0220ioc"),
		m_tc0100scn(*this, "tc0100scn"),
		m_tc0110pcr(*this, "tc0110pcr"),
		m_tc0140syt(*this, "tc0140syt"),
		m_2610_0l(*this, "2610.0l"),
		m_2610_0r(*this, "2610.0r"),
		m_2610_1l(*this, "2610.1l"),
		m_2610_1r(*this, "2610.1r"),
		m_2610_2l(*this, "2610.2l"),
		m_2610_2r(*this, "2610.2r"),
		m_gfxdecode(*this, "gfxdecode"),
		m_palette(*this, "palette") { }

	/* memory pointers */
	required_shared_ptr<uint16_t> m_spriteram;

	/* video-related */
	std::unique_ptr<othunder_tempsprite[]> m_spritelist;

	/* misc */
	int        m_vblank_irq;
	int        m_ad_irq;
	int        m_pan[4];

	/* devices */
	required_device<cpu_device> m_maincpu;
	required_device<cpu_device> m_audiocpu;
	optional_device<eeprom_serial_93cxx_device> m_eeprom;
	required_device<tc0220ioc_device> m_tc0220ioc;
	required_device<tc0100scn_device> m_tc0100scn;
	required_device<tc0110pcr_device> m_tc0110pcr;
	required_device<tc0140syt_device> m_tc0140syt;
	required_device<filter_volume_device> m_2610_0l;
	required_device<filter_volume_device> m_2610_0r;
	required_device<filter_volume_device> m_2610_1l;
	required_device<filter_volume_device> m_2610_1r;
	required_device<filter_volume_device> m_2610_2l;
	required_device<filter_volume_device> m_2610_2r;
	required_device<gfxdecode_device> m_gfxdecode;
	required_device<palette_device> m_palette;

	void irq_ack_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void othunder_tc0220ioc_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	uint16_t othunder_tc0220ioc_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	uint16_t othunder_lightgun_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	void othunder_lightgun_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void sound_bankswitch_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void othunder_sound_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	uint16_t othunder_sound_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	void othunder_TC0310FAM_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	virtual void machine_start() override;
	virtual void machine_reset() override;
	virtual void video_start() override;
	uint32_t screen_update_othunder(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	INTERRUPT_GEN_MEMBER(vblank_interrupt);
	void draw_sprites( screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect, const int *primasks, int y_offs );
	void update_irq();

protected:
	virtual void device_timer(emu_timer &timer, device_timer_id id, int param, void *ptr) override;
};
