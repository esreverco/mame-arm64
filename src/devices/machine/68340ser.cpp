// license:BSD-3-Clause
// copyright-holders:David Haywood
/* 68340 SERIAL module */

#include "emu.h"
#include "68340.h"


uint32_t m68340cpu_device::m68340_internal_serial_r(address_space &space, offs_t offset, uint32_t mem_mask)
{
	m68340cpu_device *m68k = this;
	m68340_serial* serial = m68k->m68340SERIAL;
	assert(serial != nullptr);

	if (serial)
	{
		int pc = space.device().safe_pc();
		logerror("%08x m68340_internal_serial_r %08x, (%08x)\n", pc, offset*4,mem_mask);
	}

	return 0x00000000;
}

void m68340cpu_device::m68340_internal_serial_w(address_space &space, offs_t offset, uint32_t data, uint32_t mem_mask)
{
	m68340cpu_device *m68k = this;
	m68340_serial* serial = m68k->m68340SERIAL;
	assert(serial != nullptr);

	if (serial)
	{
		int pc = space.device().safe_pc();
		logerror("%08x m68340_internal_serial_w %08x, %08x (%08x)\n", pc, offset*4,data,mem_mask);
	}

}

void m68340_serial::reset(void)
{
}
