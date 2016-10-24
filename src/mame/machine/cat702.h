// license:BSD-3-Clause
// copyright-holders:smf
/*  CAT702 security chip */

#pragma once

#ifndef __CAT702_H__
#define __CAT702_H__

#include "emu.h"

extern const device_type CAT702;

#define MCFG_CAT702_DATAOUT_HANDLER(_devcb) \
	devcb = &cat702_device::set_dataout_handler(*device, DEVCB_##_devcb);

class cat702_device : public device_t
{
public:
	cat702_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	// static configuration helpers
	template<class _Object> static devcb_base &set_dataout_handler(device_t &device, _Object object) { return downcast<cat702_device &>(device).m_dataout_handler.set_callback(object); }

	void write_select(int state);
	void write_datain(int state);
	void write_clock(int state);

protected:
	virtual void device_start() override;

private:
	uint8_t compute_sbox_coef(int sel, int bit);
	void apply_bit_sbox(int sel);
	void apply_sbox(const uint8_t *sbox);

	optional_memory_region m_region;
	uint8_t m_transform[8];

	int m_select;
	int m_clock;
	int m_datain;
	uint8_t m_state;
	uint8_t m_bit;

	devcb_write_line m_dataout_handler;
};

#endif
