// license:BSD-3-Clause
// copyright-holders:Barry Rodewald
/*

    VIA VT82C496G "Green PC" system chipset

*/

#ifndef __VT82C496_H__
#define __VT82C496_H__

#include "emu.h"
#include "ram.h"

#define MCFG_VT82C496_ADD(_tag) \
	MCFG_DEVICE_ADD(_tag, VT82C496, 0)

#define MCFG_VT82C496_CPU( _tag ) \
	vt82c496_device::static_set_cpu(*device, _tag);

#define MCFG_VT82C496_REGION( _tag ) \
	vt82c496_device::static_set_region(*device, _tag);

class vt82c496_device :  public device_t
{
public:
	// construction/destruction
	vt82c496_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	static void static_set_cpu(device_t &device, const char *tag) { dynamic_cast<vt82c496_device &>(device).m_cpu_tag = tag; }
	static void static_set_region(device_t &device, const char *tag) { dynamic_cast<vt82c496_device &>(device).m_region_tag = tag; }

	uint8_t read(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void write(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
protected:
	// device-level overrides
	virtual void device_start() override;
	virtual void device_reset() override;

private:
	const char* m_cpu_tag;
	const char* m_region_tag;
//  cpu_device* m_maincpu;
	address_space* m_space;
	ram_device* m_ram;
	uint8_t* m_rom;

	uint8_t m_reg[0x100];
	uint8_t m_reg_select;

	void update_mem_c0(uint8_t data);
	void update_mem_d0(uint8_t data);
	void update_mem_e0(uint8_t data);
};

// device type definition
extern const device_type VT82C496;


#endif /* __VT82C496_H__ */
