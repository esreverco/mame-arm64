// license:BSD-3-Clause
// copyright-holders:Fabio Priuli
/***********************************************************************************************************

 SuperFX add-on chip emulation (for SNES/SFC)

 ***********************************************************************************************************/


#include "emu.h"
#include "sfx.h"
#include "cpu/g65816/g65816.h"

//-------------------------------------------------
//  sns_rom_superfx_device - constructor
//-------------------------------------------------

const device_type SNS_LOROM_SUPERFX = &device_creator<sns_rom_superfx_device>;


sns_rom_superfx_device::sns_rom_superfx_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
			: sns_rom_device(mconfig, SNS_LOROM_SUPERFX, "SNES Cart (LoROM) + SuperFX", tag, owner, clock, "sns_rom_superfx", __FILE__),
			m_superfx(*this, "superfx")
{
}

void sns_rom_superfx_device::device_start()
{
	save_item(NAME(sfx_ram));
}

void sns_rom_superfx_device::device_reset()
{
	memset(sfx_ram, 0x00, sizeof(sfx_ram));
}

/*-------------------------------------------------
 mapper specific handlers
 -------------------------------------------------*/

// LoROM + SuperFX (GSU-1,2)
// TODO: mask sfx_ram based on the actual RAM...

uint8_t sns_rom_superfx_device::superfx_r_bank1(address_space &space, offs_t offset, uint8_t mem_mask)
{
	return m_rom[rom_bank_map[offset / 0x10000] * 0x8000 + (offset & 0x7fff)];
}

uint8_t sns_rom_superfx_device::superfx_r_bank2(address_space &space, offs_t offset, uint8_t mem_mask)
{
	return m_rom[rom_bank_map[offset / 0x8000] * 0x8000 + (offset & 0x7fff)];
}

uint8_t sns_rom_superfx_device::superfx_r_bank3(address_space &space, offs_t offset, uint8_t mem_mask)
{
	return sfx_ram[offset & 0xfffff];
}

void sns_rom_superfx_device::superfx_w_bank1(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask)
{
}

void sns_rom_superfx_device::superfx_w_bank2(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask)
{
}

void sns_rom_superfx_device::superfx_w_bank3(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask)
{
	sfx_ram[offset & 0xfffff] = data;
}

static ADDRESS_MAP_START( sfx_map, AS_PROGRAM, 8, sns_rom_superfx_device )
	AM_RANGE(0x000000, 0x3fffff) AM_READWRITE(superfx_r_bank1, superfx_w_bank1)
	AM_RANGE(0x400000, 0x5fffff) AM_READWRITE(superfx_r_bank2, superfx_w_bank2)
	AM_RANGE(0x600000, 0x7dffff) AM_READWRITE(superfx_r_bank3, superfx_w_bank3)
	AM_RANGE(0x800000, 0xbfffff) AM_READWRITE(superfx_r_bank1, superfx_w_bank1)
	AM_RANGE(0xc00000, 0xdfffff) AM_READWRITE(superfx_r_bank2, superfx_w_bank2)
	AM_RANGE(0xe00000, 0xffffff) AM_READWRITE(superfx_r_bank3, superfx_w_bank3)
ADDRESS_MAP_END


void sns_rom_superfx_device::snes_extern_irq_w(int state)
{
	machine().device("maincpu")->execute().set_input_line(G65816_LINE_IRQ, state);
}


static MACHINE_CONFIG_FRAGMENT( snes_sfx )
	MCFG_CPU_ADD("superfx", SUPERFX, 21480000)  /* 21.48MHz */
	MCFG_CPU_PROGRAM_MAP(sfx_map)
	MCFG_SUPERFX_OUT_IRQ(WRITELINE(sns_rom_superfx_device, snes_extern_irq_w))  /* IRQ line from cart */
MACHINE_CONFIG_END

machine_config_constructor sns_rom_superfx_device::device_mconfig_additions() const
{
	return MACHINE_CONFIG_NAME( snes_sfx );
}

uint8_t sns_rom_superfx_device::chip_read(address_space &space, offs_t offset, uint8_t mem_mask)
{
	return m_superfx->mmio_read(offset);
}

void sns_rom_superfx_device::chip_write(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask)
{
	m_superfx->mmio_write(offset, data);
}


uint8_t sns_rom_superfx_device::read_l(address_space &space, offs_t offset, uint8_t mem_mask)
{
	return read_h(space, offset);
}

uint8_t sns_rom_superfx_device::read_h(address_space &space, offs_t offset, uint8_t mem_mask)
{
	if (offset < 0x400000)
		return m_rom[rom_bank_map[offset / 0x10000] * 0x8000 + (offset & 0x7fff)];
	else if (offset < 0x600000)
	{
		if (m_superfx->access_rom())
		{
			return m_rom[rom_bank_map[(offset - 0x400000) / 0x8000] * 0x8000 + (offset & 0x7fff)];
		}
		else
		{
			static const uint8_t sfx_data[16] = {
				0x00, 0x01, 0x00, 0x01, 0x04, 0x01, 0x00, 0x01,
				0x00, 0x01, 0x08, 0x01, 0x00, 0x01, 0x0c, 0x01,
			};
			return sfx_data[offset & 0x0f];
		}
	}
	return 0xff;    // this handler should never be called for [60-7f]/[e0-ff] ranges
}

uint8_t sns_rom_superfx_device::read_ram(address_space &space, offs_t offset, uint8_t mem_mask)
{
	if (m_superfx->access_ram())
		return sfx_ram[offset & 0xfffff];
	return 0xff;    // should be open bus...
}

void sns_rom_superfx_device::write_ram(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask)
{
	if (m_superfx->access_ram())
		sfx_ram[offset & 0xfffff] = data;
}
