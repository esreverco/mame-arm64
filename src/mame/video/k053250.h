// license:BSD-3-Clause
// copyright-holders:Olivier Galibert
#ifndef __K053250_H__
#define __K053250_H__

//
//  Konami 053250 road generator
//

#include "emu.h"

#define MCFG_K053250_ADD(_tag, _palette_tag, _screen_tag, offx, offy)  \
	MCFG_DEVICE_ADD(_tag, K053250, 0) \
	MCFG_GFX_PALETTE(_palette_tag) \
	MCFG_VIDEO_SET_SCREEN(_screen_tag) \
	k053250_device::static_set_offsets(*device, offx, offy);

class k053250_device :  public device_t,
						public device_gfx_interface,
						public device_video_interface
{
public:
	k053250_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	static void static_set_offsets(device_t &device, int offx, int offy);

	uint16_t reg_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	void reg_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	uint16_t ram_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	void ram_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	uint16_t rom_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);

	void draw( bitmap_rgb32 &bitmap, const rectangle &cliprect, int colorbase, int flags, bitmap_ind8 &priority_bitmap, int priority );

protected:
	// device-level overrides
	virtual void device_start() override;
	virtual void device_reset() override;

private:
	// configuration
	int m_offx, m_offy;

	// internal state
	required_region_ptr<uint8_t> m_rom;
	std::vector<uint8_t> m_unpacked_rom;
	std::vector<uint16_t> m_ram;
	uint16_t *m_buffer[2];
	uint8_t m_regs[8];
	uint8_t m_page;
	int32_t m_frame;

	// internal helpers
	void unpack_nibbles();
	void dma(int limiter);
	static void pdraw_scanline32(bitmap_rgb32 &bitmap, const pen_t *pal_base, uint8_t *source,
									const rectangle &cliprect, int linepos, int scroll, int zoom,
									uint32_t clipmask, uint32_t wrapmask, uint32_t orientation, bitmap_ind8 &priority, uint8_t pri);
};

extern const device_type K053250;

#endif
