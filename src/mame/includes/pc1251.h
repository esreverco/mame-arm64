// license:GPL-2.0+
// copyright-holders:Peter Trauner
/*****************************************************************************
 *
 * includes/pc1251.h
 *
 * Pocket Computer 1251
 *
 ****************************************************************************/

#ifndef PC1251_H_
#define PC1251_H_

#include "pocketc.h"
#include "cpu/sc61860/sc61860.h"
#include "machine/nvram.h"

#define PC1251_CONTRAST (ioport("DSW0")->read() & 0x07)


class pc1251_state : public pocketc_state
{
public:
	enum
	{
		TIMER_POWER_UP
	};

	pc1251_state(const machine_config &mconfig, device_type type, const char *tag)
		: pocketc_state(mconfig, type, tag),
		m_maincpu(*this, "maincpu"),
		m_gfxdecode(*this, "gfxdecode"),
		m_palette(*this, "palette")  { }

	uint8_t m_outa;
	uint8_t m_outb;
	int m_power;
	uint8_t m_reg[0x100];

	void init_pc1251();
	uint32_t screen_update_pc1251(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	void pc1251_outa(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void pc1251_outb(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void pc1251_outc(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);

	int pc1251_reset();
	int pc1251_brk();
	uint8_t pc1251_ina(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	uint8_t pc1251_inb(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	uint8_t pc1251_lcd_read(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void pc1251_lcd_write(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	virtual void machine_start() override;
	void machine_start_pc1260();
	required_device<sc61860_device> m_maincpu;
	required_device<gfxdecode_device> m_gfxdecode;
	required_device<palette_device> m_palette;

protected:
	virtual void device_timer(emu_timer &timer, device_timer_id id, int param, void *ptr) override;
};

#endif /* PC1251_H_ */
