// license:BSD-3-Clause
// copyright-holders:Philip Bennett
/***************************************************************************

  ISA 8 bit IBM PC Music Feature Card

***************************************************************************/

#pragma once

#ifndef __ISA_IBM_MUSIC_FEATURE_CARD_H__
#define __ISA_IBM_MUSIC_FEATURE_CARD_H__


#include "emu.h"
#include "isa.h"
#include "machine/i8255.h"
#include "machine/i8251.h"
#include "machine/pit8253.h"
#include "sound/ym2151.h"

//**************************************************************************
//  TYPE DEFINITIONS
//**************************************************************************

// ======================> isa8_ibm_mfc_device

class isa8_ibm_mfc_device : public device_t,
	public device_isa8_card_interface
{
public:
	// Construction/destruction
	isa8_ibm_mfc_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	uint8_t ppi0_i_a(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void ppi0_o_b(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t ppi0_i_c(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void ppi0_o_c(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);

	void ppi1_o_a(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t ppi1_i_b(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);

	void ppi1_o_c(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);

	uint8_t ibm_mfc_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void ibm_mfc_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);

	DECLARE_WRITE_LINE_MEMBER( d8253_out0 );
	DECLARE_WRITE_LINE_MEMBER( d8253_out1 );

	DECLARE_WRITE_LINE_MEMBER( write_usart_clock );

	DECLARE_WRITE_LINE_MEMBER( ibm_mfc_ym_irq );

protected:
	// Device-level overrides
	virtual void                    device_start() override;
	virtual void                    device_reset() override;

	virtual machine_config_constructor  device_mconfig_additions() const override;
	virtual ioport_constructor          device_input_ports() const override;

	virtual const tiny_rom_entry*        device_rom_region() const override;

private:
	void                            set_z80_interrupt(int src, int state);
	void                            set_pc_interrupt(int src, int state);
	void                            update_pc_interrupts(void);
	void                            update_z80_interrupts(void);

	uint8_t                           m_tcr;
	uint8_t                           m_pc_ppi_c;
	uint8_t                           m_z80_ppi_c;

	uint8_t                           m_pc_irq_state;
	uint8_t                           m_z80_irq_state;

	required_device<cpu_device>     m_cpu;
	required_device<ym2151_device>  m_ym2151;
	required_device<pit8253_device> m_d8253;
	required_device<i8251_device>   m_d71051;
	required_device<i8255_device>   m_d71055c_0;
	required_device<i8255_device>   m_d71055c_1;
};


// Device type definition
extern const device_type ISA8_IBM_MFC;

#endif  /* __ISA_IBM_MUSIC_FEATURE_CARD_H__ */
