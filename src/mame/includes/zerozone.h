// license:BSD-3-Clause
// copyright-holders:Brad Oliver
/*************************************************************************

    Zero Zone

*************************************************************************/

#include "emu.h"
#include "machine/gen_latch.h"
#include "cpu/z80/z80.h"

class zerozone_state : public driver_device
{
public:
	zerozone_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag),
			m_maincpu(*this, "maincpu"),
			m_audiocpu(*this, "audiocpu"),
			m_soundlatch(*this, "soundlatch"),
			m_vram(*this, "videoram"),
			m_gfxdecode(*this, "gfxdecode")
	{ }

	// in drivers/zerozone.c
	void sound_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);

	// in video/zerozone.c
	void tilemap_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void tilebank_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);

	// devices
	required_device<cpu_device> m_maincpu;
	required_device<z80_device> m_audiocpu;
	required_device<generic_latch_8_device> m_soundlatch;

	// shared pointers
	required_shared_ptr<uint16_t> m_vram;
	// currently this driver uses generic palette handling

	required_device<gfxdecode_device> m_gfxdecode;
	// state
	// video-related
	uint16_t         m_tilebank;
	tilemap_t     *m_zz_tilemap;

	uint32_t screen_update(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);

protected:

	// driver_device overrides
	virtual void machine_start() override;
	virtual void machine_reset() override;

	virtual void video_start() override;
	void get_zerozone_tile_info(tilemap_t &tilemap, tile_data &tileinfo, tilemap_memory_index tile_index);
};
