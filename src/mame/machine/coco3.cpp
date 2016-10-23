// license:BSD-3-Clause
// copyright-holders:Nathan Woods
/***************************************************************************

    coco3.c

    TRS-80 Radio Shack Color Computer 3

  Functions to emulate general aspects of the machine (RAM, ROM, interrupts,
  I/O ports)

  The Dragon/CoCo2 have two PIAs.  These PIAs can trigger interrupts.  PIA0
  is set up to trigger IRQ on the CPU, and PIA1 can trigger FIRQ.  Each PIA
  has two output lines, and an interrupt will be triggered if either of these
  lines are asserted.

  -----  IRQ
  6809 |-<----------- PIA0
       |
       |
       |
       |
       |
       |-<----------- PIA1
  -----

  The CoCo 3 still supports these interrupts, but the GIME can chose whether
  "old school" interrupts are generated, or the new ones generated by the GIME

  -----  IRQ
  6809 |-<----------- PIA0
       |       |                ------
       |       -<-------<-------|    |
       |                        |GIME|
       |       -<-------<-------|    |
       | FIRQ  |                ------
       |-<----------- PIA1
  -----

  In an email discussion with JK, he informs me that when GIME interrupts are
  enabled, this actually does not prevent PIA interrupts.  Apparently JeffV's
  CoCo 3 emulator did not handle this properly.

***************************************************************************/

#include "includes/coco3.h"

//-------------------------------------------------
//  ff20_write
//-------------------------------------------------

void coco3_state::ff20_write(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask)
{
	coco_state::ff20_write(space, offset, data, mem_mask);

	if (offset == 0x02)
		m_gime->ff22_write(data);
}



//-------------------------------------------------
//  ff40_read
//-------------------------------------------------

uint8_t coco3_state::ff40_read(address_space &space, offs_t offset, uint8_t mem_mask)
{
	uint8_t result = 0x00;
	if (m_gime->spare_chip_select_enabled())
		result = coco_state::ff40_read(space, offset, mem_mask);
	return result;
}



//-------------------------------------------------
//  ff40_write
//-------------------------------------------------

void coco3_state::ff40_write(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask)
{
	if (m_gime->spare_chip_select_enabled())
		coco_state::ff40_write(space, offset, data, mem_mask);
}



//-------------------------------------------------
//  firq_get_line
//-------------------------------------------------

bool coco3_state::firq_get_line(void)
{
	return coco_state::firq_get_line() || m_gime->firq_r();
}



//-------------------------------------------------
//  irq_get_line
//-------------------------------------------------

bool coco3_state::irq_get_line(void)
{
	return coco_state::irq_get_line() || m_gime->irq_r();
}



//-------------------------------------------------
//  update_keyboard_input
//-------------------------------------------------

void coco3_state::update_keyboard_input(uint8_t value, uint8_t z)
{
	coco_state::update_keyboard_input(value, z);
	m_gime->set_il1(value == 0xFF);
}



//-------------------------------------------------
//  cart_w
//-------------------------------------------------

void coco3_state::cart_w(bool state)
{
	coco_state::cart_w(state);
	m_gime->set_il0(state);
}



//-------------------------------------------------
//  update_cart_base
//-------------------------------------------------

void coco3_state::update_cart_base(uint8_t *cart_base)
{
	m_gime->update_cart_rom();
}



//-------------------------------------------------
//  screen_update
//-------------------------------------------------

uint32_t coco3_state::screen_update(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect)
{
	bool result;
	if (!strcmp(screen.tag(), ":" COMPOSITE_SCREEN_TAG))
	{
		/* composite screen */
		result = m_gime->update_composite(bitmap, cliprect);
	}
	else if (!strcmp(screen.tag(), ":" RGB_SCREEN_TAG))
	{
		/* rgb screen */
		result = m_gime->update_rgb(bitmap, cliprect);
	}
	else
	{
		fatalerror("Called screen_update() with invalid tag '%s'\n", screen.tag());
	}
	return result;
}
