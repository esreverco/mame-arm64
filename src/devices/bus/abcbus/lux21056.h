// license:BSD-3-Clause
// copyright-holders:Curt Coder
/**********************************************************************

    Luxor 55 21056-00 Xebec Interface Host Adapter emulation

*********************************************************************/

#pragma once

#ifndef __LUXOR_55_21056__
#define __LUXOR_55_21056__

#include "emu.h"
#include "abcbus.h"
#include "bus/scsi/scsi.h"
#include "cpu/z80/z80.h"
#include "cpu/z80/z80daisy.h"
#include "machine/z80dma.h"



//**************************************************************************
//  TYPE DEFINITIONS
//**************************************************************************

// ======================> luxor_55_21056_device

class luxor_55_21056_device :  public device_t,
								public device_abcbus_card_interface
{
public:
	// construction/destruction
	luxor_55_21056_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	// optional information overrides
	virtual const tiny_rom_entry *device_rom_region() const override;
	virtual machine_config_constructor device_mconfig_additions() const override;
	virtual ioport_constructor device_input_ports() const override;

	uint8_t sasi_status_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void stat_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t out_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void inp_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t sasi_data_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void sasi_data_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t rdy_reset_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void rdy_reset_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t sasi_sel_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void sasi_sel_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t sasi_rst_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void sasi_rst_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);

	uint8_t memory_read_byte(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void memory_write_byte(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t io_read_byte(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void io_write_byte(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);

	DECLARE_WRITE_LINE_MEMBER( write_sasi_req );
	DECLARE_WRITE_LINE_MEMBER( write_sasi_io );
	DECLARE_WRITE_LINE_MEMBER( write_sasi_cd );
	DECLARE_WRITE_LINE_MEMBER( write_sasi_msg );
	DECLARE_WRITE_LINE_MEMBER( write_sasi_bsy );

protected:
	// device-level overrides
	virtual void device_start() override;
	virtual void device_reset() override;

	// device_abcbus_interface overrides
	virtual void abcbus_cs(uint8_t data) override;
	virtual uint8_t abcbus_inp() override;
	virtual void abcbus_out(uint8_t data) override;
	virtual uint8_t abcbus_stat() override;
	virtual void abcbus_c1(uint8_t data) override;
	virtual void abcbus_c3(uint8_t data) override;

private:
	void set_rdy(int state);

	required_device<cpu_device> m_maincpu;
	required_device<z80dma_device> m_dma;
	required_device<SCSI_PORT_DEVICE> m_sasibus;
	required_device<output_latch_device> m_sasi_data_out;
	required_device<input_buffer_device> m_sasi_data_in;
	required_ioport m_s1;

	int m_cs;
	int m_rdy;
	int m_sasi_req;
	int m_sasi_io;
	int m_sasi_cd;
	int m_sasi_msg;
	int m_sasi_bsy;

	uint8_t m_inp;
	uint8_t m_out;
	uint8_t m_stat;
	uint8_t m_sasi_data;
};


// device type definition
extern const device_type LUXOR_55_21056;



#endif
