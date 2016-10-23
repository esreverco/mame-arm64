// license:GPL-2.0+
// copyright-holders:Dirk Best
/***************************************************************************

    SV-806 80 column card for SVI-318/328

***************************************************************************/

#pragma once

#ifndef __SVI3X8_SLOT_SV806_H__
#define __SVI3X8_SLOT_SV806_H__

#include "emu.h"
#include "slot.h"
#include "video/mc6845.h"


//**************************************************************************
//  TYPE DEFINITIONS
//**************************************************************************

// ======================> sv806_device

class sv806_device : public device_t, public device_svi_slot_interface
{
public:
	// construction/destruction
	sv806_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	virtual uint8_t mreq_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff) override;
	virtual void mreq_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff) override;
	virtual uint8_t iorq_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff) override;
	virtual void iorq_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff) override;

	MC6845_UPDATE_ROW(crtc_update_row);

protected:
	virtual const tiny_rom_entry *device_rom_region() const override;
	virtual machine_config_constructor device_mconfig_additions() const override;
	virtual void device_start() override;

private:
	required_device<hd6845_device> m_crtc;
	required_device<palette_device> m_palette;
	required_memory_region m_gfx;

	std::unique_ptr<uint8_t[]> m_ram;
	int m_ram_enabled;
};

// device type definition
extern const device_type SV806;

#endif // __SVI3X8_SLOT_SV806_H__
