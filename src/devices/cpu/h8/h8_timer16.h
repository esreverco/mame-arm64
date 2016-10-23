// license:BSD-3-Clause
// copyright-holders:Olivier Galibert
/***************************************************************************

    h8_timer16.h

    H8 16 bits timer


***************************************************************************/

#ifndef __H8_TIMER16_H__
#define __H8_TIMER16_H__

#include "h8.h"
#include "h8_intc.h"

#define MCFG_H8_TIMER16_ADD( _tag, _count, _tstr )  \
	MCFG_DEVICE_ADD( _tag, H8_TIMER16, 0 )          \
	downcast<h8_timer16_device *>(device)->set_info(_count, _tstr);

#define MCFG_H8_TIMER16_CHANNEL_ADD( _tag, tgr_count, tbr_count, intc, irq_base ) \
	MCFG_DEVICE_ADD( _tag, H8_TIMER16_CHANNEL, 0 )  \
	downcast<h8_timer16_channel_device *>(device)->set_info(tgr_count, tbr_count, intc, irq_base);

#define MCFG_H8H_TIMER16_CHANNEL_ADD( _tag, tgr_count, tbr_count, intc, irq_base ) \
	MCFG_DEVICE_ADD( _tag, H8H_TIMER16_CHANNEL, 0 ) \
	downcast<h8h_timer16_channel_device *>(device)->set_info(tgr_count, tbr_count, intc, irq_base);

#define MCFG_H8S_TIMER16_CHANNEL_ADD( _tag, tgr_count, tier_mask, intc, irq_base, t0, t1, t2, t3, t4, t5, t6, t7 ) \
	MCFG_DEVICE_ADD( _tag, H8S_TIMER16_CHANNEL, 0 ) \
	downcast<h8s_timer16_channel_device *>(device)->set_info(tgr_count, tier_mask, intc, irq_base, t0, t1, t2, t3, t4, t5, t6, t7);

#define MCFG_H8S_TIMER16_CHANNEL_SET_CHAIN( _tag )  \
	downcast<h8s_timer16_channel_device *>(device)->set_chain(_tag);

class h8_timer16_channel_device : public device_t {
public:
	enum {
		CHAIN,
		INPUT_A,
		INPUT_B,
		INPUT_C,
		INPUT_D,
		DIV_1,
		DIV_2,
		DIV_4,
		DIV_8,
		DIV_16,
		DIV_32,
		DIV_64,
		DIV_128,
		DIV_256,
		DIV_512,
		DIV_1024,
		DIV_2048,
		DIV_4096
	};

	enum {
		TGR_CLEAR_NONE = -1,
		TGR_CLEAR_EXT  = -2
	};

	enum {
		IRQ_A = 0x01,
		IRQ_B = 0x02,
		IRQ_C = 0x04,
		IRQ_D = 0x08,
		IRQ_V = 0x10,
		IRQ_U = 0x20,
		IRQ_TRIG = 0x40
	};


	h8_timer16_channel_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);
	h8_timer16_channel_device(const machine_config &mconfig, device_type type, const char *name, const char *tag, device_t *owner, uint32_t clock, const char *shortname, const char *source);

	void set_info(int tgr_count, int tbr_count, const char *intc, int irq_base);

	uint8_t tcr_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void tcr_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t tmdr_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void tmdr_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t tior_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void tior_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t tier_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void tier_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t tsr_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void tsr_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint16_t tcnt_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	void tcnt_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	uint16_t tgr_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	void tgr_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	uint16_t tbr_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	void tbr_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);

	uint64_t internal_update(uint64_t current_time);
	void set_ier(uint8_t value);
	void set_enable(bool enable);
	void tisr_w(int offset, uint8_t data);
	uint8_t tisr_r(int offset) const;

protected:
	required_device<h8_device> cpu;
	h8_timer16_channel_device *chained_timer;
	h8_intc_device *intc;
	const char *chain_tag, *intc_tag;
	int interrupt[6];
	uint8_t tier_mask;

	int tgr_count, tbr_count;
	int tgr_clearing;
	uint8_t tcr, tier, ier, isr;
	int clock_type, clock_divider;
	uint16_t tcnt, tgr[6];
	uint64_t last_clock_update, event_time;
	uint32_t phase, counter_cycle;
	bool counter_incrementing;
	bool channel_active;

	virtual void device_start() override;
	virtual void device_reset() override;

	void update_counter(uint64_t cur_time = 0);
	void recalc_event(uint64_t cur_time = 0);
	virtual void tcr_update();
	virtual void tier_update();
	virtual void isr_update(uint8_t value);
	virtual uint8_t isr_to_sr() const;
};

class h8h_timer16_channel_device : public h8_timer16_channel_device {
public:
	h8h_timer16_channel_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);
	virtual ~h8h_timer16_channel_device();

	void set_info(int tgr_count, int tbr_count, const char *intc, int irq_base);

protected:
	virtual void tcr_update() override;
	virtual void tier_update() override;
	virtual void isr_update(uint8_t value) override;
	virtual uint8_t isr_to_sr() const override;
};

class h8s_timer16_channel_device : public h8_timer16_channel_device {
public:
	h8s_timer16_channel_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);
	virtual ~h8s_timer16_channel_device();

	void set_info(int tgr_count, uint8_t _tier_mask, const char *intc, int irq_base,
					int t0, int t1, int t2, int t3, int t4, int t5, int t6, int t7);
	void set_chain(const char *chain_tag);

protected:
	int count_types[8];

	virtual void tcr_update() override;
	virtual void tier_update() override;
	virtual void isr_update(uint8_t value) override;
	virtual uint8_t isr_to_sr() const override;
};

class h8_timer16_device : public device_t {
public:
	h8_timer16_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	void set_info(int timer_count, uint8_t default_tstr);

	uint8_t tstr_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void tstr_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t tsyr_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void tsyr_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t tmdr_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void tmdr_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t tfcr_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void tfcr_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t toer_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void toer_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t tocr_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void tocr_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t tisr_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void tisr_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t tisrc_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void tisrc_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void tolr_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);

protected:
	required_device<h8_device> cpu;
	h8_timer16_channel_device *timer_channel[6];
	int timer_count;
	uint8_t default_tstr;
	uint8_t tstr;

	virtual void device_start() override;
	virtual void device_reset() override;
};

extern const device_type H8_TIMER16;
extern const device_type H8_TIMER16_CHANNEL;
extern const device_type H8H_TIMER16_CHANNEL;
extern const device_type H8S_TIMER16_CHANNEL;

#endif
