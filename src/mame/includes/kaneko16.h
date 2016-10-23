// license:BSD-3-Clause
// copyright-holders:Luca Elia, David Haywood
/***************************************************************************

                            -= Kaneko 16 Bit Games =-

***************************************************************************/

#ifndef __KANEKO16_H__
#define __KANEKO16_H__

#include "machine/gen_latch.h"
#include "machine/nvram.h"
#include "video/kan_pand.h"
#include "video/kaneko_tmap.h"
#include "video/kaneko_spr.h"
#include "machine/eepromser.h"
#include "machine/kaneko_calc3.h"
#include "machine/kaneko_toybox.h"
#include "sound/2203intf.h"
#include "sound/okim6295.h"


class kaneko16_state : public driver_device
{
public:
	kaneko16_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag),
		m_maincpu(*this, "maincpu"),
		m_audiocpu(*this, "audiocpu"),
		m_oki(*this, "oki"),
		m_oki1(*this, "oki1"),
		m_oki2(*this, "oki2"),
		m_ym2149_1(*this, "ym2149_1"),
		m_ym2149_2(*this, "ym2149_2"),
		m_view2_0(*this, "view2_0"),
		m_view2_1(*this, "view2_1"),
		m_kaneko_spr(*this, "kan_spr"),
		m_pandora(*this, "pandora"),
		m_palette(*this, "palette"),
		m_eeprom(*this, "eeprom"),
		m_soundlatch(*this, "soundlatch"),
		m_spriteram(*this, "spriteram"),
		m_mainram(*this, "mainram")
		{ }

	required_device<cpu_device> m_maincpu;
	optional_device<cpu_device> m_audiocpu;
	optional_device<okim6295_device> m_oki;
	optional_device<okim6295_device> m_oki1;
	optional_device<okim6295_device> m_oki2;
	optional_device<ym2149_device> m_ym2149_1;
	optional_device<ym2149_device> m_ym2149_2;
	optional_device<kaneko_view2_tilemap_device> m_view2_0;
	optional_device<kaneko_view2_tilemap_device> m_view2_1;
	optional_device<kaneko16_sprite_device> m_kaneko_spr;
	optional_device<kaneko_pandora_device> m_pandora;
	required_device<palette_device> m_palette;
	optional_device<eeprom_serial_93cxx_device> m_eeprom;
	optional_device<generic_latch_8_device> m_soundlatch;

	optional_shared_ptr<uint16_t> m_spriteram;
	optional_shared_ptr<uint16_t> m_mainram;

	uint16_t m_disp_enable;

	int m_VIEW2_2_pri;


	void kaneko16_coin_lockout_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void kaneko16_soundlatch_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void kaneko16_eeprom_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);

	void kaneko16_display_enable(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);

	uint16_t kaneko16_ay1_YM2149_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	void kaneko16_ay1_YM2149_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	uint16_t kaneko16_ay2_YM2149_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	void kaneko16_ay2_YM2149_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void bakubrkr_oki_bank_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void wingforc_oki_bank_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);

	uint8_t eeprom_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void eeprom_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);

	void init_kaneko16();
	void init_samplebank();


	void machine_reset_gtmr();
	void video_start_kaneko16();
	void machine_reset_mgcrystl();
	uint32_t screen_update_kaneko16(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);

	template<class _BitmapClass>
	uint32_t screen_update_common(screen_device &screen, _BitmapClass &bitmap, const rectangle &cliprect);

	TIMER_DEVICE_CALLBACK_MEMBER(kaneko16_interrupt);
	TIMER_DEVICE_CALLBACK_MEMBER(shogwarr_interrupt);

	template<class _BitmapClass>
	void kaneko16_fill_bitmap(_BitmapClass &bitmap, const rectangle &cliprect);

	void kaneko16_common_oki_bank_w(  const char *bankname, const char* tag, int bank, size_t fixedsize, size_t bankedsize );
	void kaneko16_unscramble_tiles(const char *region);
	void kaneko16_expand_sample_banks(const char *region);
};

class kaneko16_gtmr_state : public kaneko16_state
{
public:
	kaneko16_gtmr_state(const machine_config &mconfig, device_type type, const char *tag)
		: kaneko16_state(mconfig, type, tag)
	{
	}

	void bloodwar_oki_0_bank_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void bloodwar_oki_1_bank_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void bonkadv_oki_0_bank_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void bonkadv_oki_1_bank_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void gtmr_oki_0_bank_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void gtmr_oki_1_bank_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void bloodwar_coin_lockout_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	uint16_t gtmr_wheel_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	uint16_t gtmr2_wheel_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	uint16_t gtmr2_IN1_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	void init_gtmr();

};



class kaneko16_berlwall_state : public kaneko16_state
{
public:
	kaneko16_berlwall_state(const machine_config &mconfig, device_type type, const char *tag)
		: kaneko16_state(mconfig, type, tag),
		m_bg15_select(*this, "bg15_select"),
		m_bg15_scroll(*this, "bg15_scroll"),
		m_bg15_bright(*this, "bg15_bright"),
		m_bgpalette(*this, "bgpalette")

	{
	}

	optional_shared_ptr<uint16_t> m_bg15_select;
	optional_shared_ptr<uint16_t> m_bg15_scroll;
	optional_shared_ptr<uint16_t> m_bg15_bright;
	required_device<palette_device> m_bgpalette;

	bitmap_ind16 m_bg15_bitmap[32];

	uint16_t kaneko16_bg15_select_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	void kaneko16_bg15_select_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	uint16_t kaneko16_bg15_bright_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	void kaneko16_bg15_bright_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);

	uint16_t berlwall_oki_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	void berlwall_oki_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);

	uint16_t berlwall_spriteram_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	void berlwall_spriteram_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	uint16_t berlwall_spriteregs_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	void berlwall_spriteregs_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);

	void init_berlwall();
	DECLARE_PALETTE_INIT(berlwall);
	void video_start_berlwall();
	uint32_t screen_update_berlwall(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect);
	void kaneko16_render_15bpp_bitmap(bitmap_rgb32 &bitmap, const rectangle &cliprect);
};

class kaneko16_shogwarr_state : public kaneko16_state
{
public:
	kaneko16_shogwarr_state(const machine_config &mconfig, device_type type, const char *tag)
		: kaneko16_state(mconfig, type, tag),
		m_calc3_prot(*this, "calc3_prot")
	{
	}

	optional_device<kaneko_calc3_device> m_calc3_prot;

	void shogwarr_oki_bank_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void brapboys_oki_bank_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);

	void init_shogwarr();
	void init_brapboys();
};

#endif
