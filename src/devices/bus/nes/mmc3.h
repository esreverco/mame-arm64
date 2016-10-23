// license:BSD-3-Clause
// copyright-holders:Fabio Priuli
#ifndef __NES_MMC3_H
#define __NES_MMC3_H

#include "nxrom.h"


// ======================> nes_txrom_device

class nes_txrom_device : public nes_nrom_device
{
public:
	// construction/destruction
	nes_txrom_device(const machine_config &mconfig, device_type type, const char *name, const char *tag, device_t *owner, uint32_t clock, const char *shortname, const char *source);
	nes_txrom_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	// device-level overrides
	virtual void device_start() override { mmc3_start(); }
	virtual uint8_t read_m(address_space &space, offs_t offset, uint8_t mem_mask = 0xff) override;
	virtual void write_m(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff) override;
	virtual void txrom_write(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	virtual void write_h(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff) override { txrom_write(space, offset, data, mem_mask); }
	virtual void prg_cb(int start, int bank);
	virtual void chr_cb(int start, int bank, int source);

	virtual void hblank_irq(int scanline, int vblank, int blanked) override;
	virtual void pcb_reset() override;

protected:
	virtual void set_prg(int prg_base, int prg_mask);
	virtual void set_chr(uint8_t chr, int chr_base, int chr_mask);
	void mmc3_start();
	void mmc3_common_initialize(int prg_mask, int chr_mask, int IRQ_type);

	// are there MMC3 clones which need more regs?
	uint16_t m_mmc_prg_bank[4];
	uint16_t m_mmc_vrom_bank[8];  // a few clones need more than the 6 banks used by base MMC3 (e.g. waixing_g)

	int m_prg_base, m_prg_mask; // MMC3 based multigame carts select a block of banks by using these (and then act like normal MMC3),
	int m_chr_base, m_chr_mask; // while MMC3 and clones (mapper 118 & 119) simply set them as 0 and 0xff resp.

	int m_latch;
	int m_wram_protect;
	int m_alt_irq;

	uint16_t m_irq_count, m_irq_count_latch;
	uint8_t m_irq_clear;
	int m_irq_enable;
};


// ======================> nes_hkrom_device

class nes_hkrom_device : public nes_txrom_device
{
public:
	// construction/destruction
	nes_hkrom_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	// device-level overrides
	virtual void device_start() override;
	virtual uint8_t read_m(address_space &space, offs_t offset, uint8_t mem_mask = 0xff) override;
	virtual void write_m(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff) override;
	virtual void write_h(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff) override;

	virtual void pcb_reset() override;

protected:
	int m_wram_enable;
	uint8_t m_mmc6_reg;

	// MMC-6 contains 1K of internal ram, battery backed up
	uint8_t m_mmc6_ram[0x400];
};


// ======================> nes_txsrom_device

class nes_txsrom_device : public nes_txrom_device
{
public:
	// construction/destruction
	nes_txsrom_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	// device-level overrides
	virtual void write_h(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff) override;
	virtual void chr_cb(int start, int bank, int source) override;

protected:
	void set_mirror();
};


// ======================> nes_tqrom_device

class nes_tqrom_device : public nes_txrom_device
{
public:
	// construction/destruction
	nes_tqrom_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

protected:
	virtual void set_chr( uint8_t chr, int chr_base, int chr_mask ) override;
};


// ======================> nes_qj_device

class nes_qj_device : public nes_txrom_device
{
public:
	// construction/destruction
	nes_qj_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	// device-level overrides
	virtual void write_m(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff) override;
	virtual void pcb_reset() override;
};


// ======================> nes_zz_device

class nes_zz_device : public nes_txrom_device
{
public:
	// construction/destruction
	nes_zz_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	// device-level overrides
	virtual void write_m(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff) override;
	virtual void pcb_reset() override;
};




// device type definition
extern const device_type NES_TXROM;
extern const device_type NES_HKROM;
extern const device_type NES_TXSROM;
extern const device_type NES_TQROM;
extern const device_type NES_QJ_PCB;
extern const device_type NES_ZZ_PCB;

#endif
