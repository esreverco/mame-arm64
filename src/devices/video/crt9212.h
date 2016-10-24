// license:BSD-3-Clause
// copyright-holders:Curt Coder
/**********************************************************************

    SMC CRT9212 Double Row Buffer (DRB) emulation

**********************************************************************
                            _____   _____
                  DIN2   1 |*    \_/     | 28  DIN3
                  DIN1   2 |             | 27  _WCLK
                  DIN0   3 |             | 26  _OE
                 DOUT7   4 |             | 25  WEN2
                 DOUT6   5 |             | 24  WEN1
                 DOUT5   6 |             | 23  GND
                 DOUT4   7 |   CRT9212   | 22  ROF
                   Vcc   8 |             | 21  WOF
                 DOUT3   9 |             | 20  REN
                 DOUT2  10 |             | 19  _CLRCNT
                 DOUT1  11 |             | 18  _TOG
                 DOUT0  12 |             | 17  _RCLK
                  DIN7  13 |             | 16  DIN4
                  DIN6  14 |_____________| 15  DIN5

**********************************************************************/

#pragma once

#ifndef __CRT9212__
#define __CRT9212__

#include "emu.h"



//**************************************************************************
//  MACROS / CONSTANTS
//**************************************************************************

const int CRT9212_RAM_SIZE  = 135;



//**************************************************************************
//  INTERFACE CONFIGURATION MACROS
//**************************************************************************

#define MCFG_CRT9212_WEN2_VCC() \
	crt9212_t::static_set_wen2(*device, 1);

#define MCFG_CRT9212_DOUT_CALLBACK(_write) \
	devcb = &crt9212_t::set_dout_wr_callback(*device, DEVCB_##_write);

#define MCFG_CRT9212_ROF_CALLBACK(_write) \
	devcb = &crt9212_t::set_rof_wr_callback(*device, DEVCB_##_write);

#define MCFG_CRT9212_WOF_CALLBACK(_write) \
	devcb = &crt9212_t::set_wof_wr_callback(*device, DEVCB_##_write);



//**************************************************************************
//  TYPE DEFINITIONS
//**************************************************************************

// ======================> crt9212_t

class crt9212_t :  public device_t
{
public:
	// construction/destruction
	crt9212_t(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	static void static_set_wen2(device_t &device, int state) { downcast<crt9212_t &>(device).m_wen2 = state; }

	template<class _Object> static devcb_base &set_dout_wr_callback(device_t &device, _Object object) { return downcast<crt9212_t &>(device).m_write_dout.set_callback(object); }
	template<class _Object> static devcb_base &set_rof_wr_callback(device_t &device, _Object object) { return downcast<crt9212_t &>(device).m_write_rof.set_callback(object); }
	template<class _Object> static devcb_base &set_wof_wr_callback(device_t &device, _Object object) { return downcast<crt9212_t &>(device).m_write_wof.set_callback(object); }

	void write(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff) { m_data = data; }
	void clrcnt_w(int state);
	void tog_w(int state) { m_tog = state; }
	void ren_w(int state) { m_ren = state; }
	void wen1_w(int state) { m_wen1 = state; }
	void wen2_w(int state) { m_wen2 = state; }
	void oe_w(int state) { m_oe = state; }
	void rclk_w(int state);
	void wclk_w(int state);

protected:
	// device-level overrides
	virtual void device_start() override;

private:
	devcb_write8           m_write_dout;
	devcb_write_line       m_write_rof;
	devcb_write_line       m_write_wof;

	// inputs
	uint8_t m_data;
	int m_clrcnt;
	int m_tog;
	int m_ren;
	int m_wen1;
	int m_wen2;
	int m_oe;
	int m_rclk;
	int m_wclk;

	// internal state
	bool m_clrcnt_edge;
	uint8_t m_data_latch;
	int m_ren_int;
	int m_wen_int;
	uint8_t m_ram[CRT9212_RAM_SIZE][2];
	int m_buffer;
	int m_rac;
	int m_wac;
};


// device type definition
extern const device_type CRT9212;



#endif
