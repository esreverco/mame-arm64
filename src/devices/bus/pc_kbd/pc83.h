// license:BSD-3-Clause
// copyright-holders:Curt Coder
/**********************************************************************

    IBM 5150 83-key keyboard emulation

*********************************************************************/

#pragma once

#ifndef __PC_KBD_IBM_PC_83__
#define __PC_KBD_IBM_PC_83__

#include "emu.h"
#include "cpu/mcs48/mcs48.h"
#include "pc_kbdc.h"
#include "machine/rescap.h"



//**************************************************************************
//  TYPE DEFINITIONS
//**************************************************************************

// ======================> ibm_pc_83_keyboard_device

class ibm_pc_83_keyboard_device :  public device_t,
									public device_pc_kbd_interface
{
public:
	// construction/destruction
	ibm_pc_83_keyboard_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	// optional information overrides
	virtual const tiny_rom_entry *device_rom_region() const override;
	virtual machine_config_constructor device_mconfig_additions() const override;
	virtual ioport_constructor device_input_ports() const override;

	void bus_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t p1_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void p2_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t t1_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);

protected:
	// device-level overrides
	virtual void device_start() override;
	virtual void device_reset() override;

	// device_pc_kbd_interface overrides
	virtual DECLARE_WRITE_LINE_MEMBER( clock_write ) override { };
	virtual DECLARE_WRITE_LINE_MEMBER( data_write ) override { };

private:
	required_device<cpu_device> m_maincpu;
	required_ioport_array<24> m_dr;

	uint8_t m_cnt;
};


// device type definition
extern const device_type PC_KBD_IBM_PC_83;



#endif
