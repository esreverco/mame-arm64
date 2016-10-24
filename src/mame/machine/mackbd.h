// license:BSD-3-Clause
// copyright-holders:R. Belmont
#pragma once

#ifndef __MACKBD_H__
#define __MACKBD_H__

#include "emu.h"



//**************************************************************************
//  INTERFACE CONFIGURATION MACROS
//**************************************************************************

#define MCFG_MACKBD_ADD(_tag) \
	MCFG_DEVICE_ADD(_tag, MACKBD, 0)

#define MCFG_MACKBD_REPLACE(_tag) \
	MCFG_DEVICE_REPLACE(_tag, MACKBD, 0)

#define MCFG_MACKBD_REMOVE(_tag) \
	MCFG_DEVICE_REMOVE(_tag)

#define MCFG_MACKBD_CLKOUT_HANDLER(_devcb) \
	devcb = &mackbd_device::set_clkout_handler(*device, DEVCB_##_devcb);

#define MCFG_MACKBD_DATAOUT_HANDLER(_devcb) \
	devcb = &mackbd_device::set_dataout_handler(*device, DEVCB_##_devcb);

//**************************************************************************
//  TYPE DEFINITIONS
//**************************************************************************

// ======================> mackbd_device

class mackbd_device :  public device_t
{
public:
	// static config helper
	template<class _Object> static devcb_base &set_clkout_handler(device_t &device, _Object object) { return downcast<mackbd_device &>(device).m_clkout_handler.set_callback(object); }
	template<class _Object> static devcb_base &set_dataout_handler(device_t &device, _Object object) { return downcast<mackbd_device &>(device).m_dataout_handler.set_callback(object); }

	// construction/destruction
	mackbd_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	uint8_t p0_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void p0_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t p1_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void p1_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t p2_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void p2_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t t1_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);

	void data_w(int state);

protected:
	// device-level overrides
	virtual void device_start() override;
	virtual void device_reset() override;
	virtual machine_config_constructor device_mconfig_additions() const override;
	virtual const tiny_rom_entry *device_rom_region() const override;
	virtual ioport_constructor device_input_ports() const override;

	required_device<cpu_device> m_maincpu;

private:
	uint8_t p0, p1, p2, data_from_mac, data_to_mac;

	devcb_write_line m_clkout_handler;
	devcb_write_line m_dataout_handler;

	void scan_kbd_col(int col);
};

// device type definition
extern const device_type MACKBD;

#endif
