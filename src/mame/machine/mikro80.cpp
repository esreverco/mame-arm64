// license:BSD-3-Clause
// copyright-holders:Miodrag Milanovic
/***************************************************************************

        Mikro-80 machine driver by Miodrag Milanovic

        10/03/2008 Preliminary driver.

****************************************************************************/


#include "emu.h"
#include "cpu/i8085/i8085.h"
#include "includes/mikro80.h"

/* Driver initialization */
void mikro80_state::init_mikro80()
{
	/* set initialy ROM to be visible on first bank */
	uint8_t *RAM = m_region_maincpu->base();
	memset(RAM,0x0000,0x0800); // make frist page empty by default
	m_bank1->configure_entries(1, 2, RAM, 0x0000);
	m_bank1->configure_entries(0, 2, RAM, 0xf800);
	m_key_mask = 0x7f;
}

void mikro80_state::init_radio99()
{
	init_mikro80();
	m_key_mask = 0xff;
}

uint8_t mikro80_state::mikro80_8255_portb_r(address_space &space, offs_t offset, uint8_t mem_mask)
{
	uint8_t key = 0xff;
	if ((m_keyboard_mask & 0x01)!=0) { key &= m_io_line0->read(); }
	if ((m_keyboard_mask & 0x02)!=0) { key &= m_io_line1->read(); }
	if ((m_keyboard_mask & 0x04)!=0) { key &= m_io_line2->read(); }
	if ((m_keyboard_mask & 0x08)!=0) { key &= m_io_line3->read(); }
	if ((m_keyboard_mask & 0x10)!=0) { key &= m_io_line4->read(); }
	if ((m_keyboard_mask & 0x20)!=0) { key &= m_io_line5->read(); }
	if ((m_keyboard_mask & 0x40)!=0) { key &= m_io_line6->read(); }
	if ((m_keyboard_mask & 0x80)!=0) { key &= m_io_line7->read(); }
	return key & m_key_mask;
}

uint8_t mikro80_state::mikro80_8255_portc_r(address_space &space, offs_t offset, uint8_t mem_mask)
{
	return m_io_line8->read();
}

void mikro80_state::mikro80_8255_porta_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask)
{
	m_keyboard_mask = data ^ 0xff;
}

void mikro80_state::mikro80_8255_portc_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask)
{
}

void mikro80_state::device_timer(emu_timer &timer, device_timer_id id, int param, void *ptr)
{
	switch (id)
	{
	case TIMER_RESET:
		m_bank1->set_entry(0);
		break;
	default:
		assert_always(false, "Unknown id in mikro80_state::device_timer");
	}
}

void mikro80_state::machine_reset()
{
	timer_set(attotime::from_usec(10), TIMER_RESET);
	m_bank1->set_entry(1);
	m_keyboard_mask = 0;
}


uint8_t mikro80_state::mikro80_keyboard_r(address_space &space, offs_t offset, uint8_t mem_mask)
{
	return m_ppi8255->read(space, offset^0x03);
}

void mikro80_state::mikro80_keyboard_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask)
{
	m_ppi8255->write(space, offset^0x03, data);
}


void mikro80_state::mikro80_tape_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask)
{
	m_cassette->output(data & 0x01 ? 1 : -1);
}


uint8_t mikro80_state::mikro80_tape_r(address_space &space, offs_t offset, uint8_t mem_mask)
{
	double level = m_cassette->input();
	if (level <  0) {
			return 0x00;
	}
	return 0xff;
}

void mikro80_state::radio99_sound_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask)
{
	m_dac->write(BIT(data, 1));
}
