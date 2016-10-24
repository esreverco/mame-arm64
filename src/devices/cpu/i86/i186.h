// license:BSD-3-Clause
// copyright-holders:Carl
#ifndef I186_H__
#define I186_H__

#include "emu.h"
#include "i86.h"

extern const device_type I80186;
extern const device_type I80188;

class i80186_cpu_device : public i8086_common_cpu_device
{
public:
	// construction/destruction
	i80186_cpu_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);
	i80186_cpu_device(const machine_config &mconfig, device_type type, const char *name, const char *tag, device_t *owner, uint32_t clock, const char *shortname, const char *source, int data_bus_size);

	template<class _Object> static devcb_base &static_set_read_slave_ack_callback(device_t &device, _Object object) { return downcast<i80186_cpu_device &>(device).m_read_slave_ack_func.set_callback(object); }
	template<class _Object> static devcb_base &static_set_chip_select_callback(device_t &device, _Object object) { return downcast<i80186_cpu_device &>(device).m_out_chip_select_func.set_callback(object); }
	template<class _Object> static devcb_base &static_set_tmrout0_handler(device_t &device, _Object object) { return downcast<i80186_cpu_device &>(device).m_out_tmrout0_func.set_callback(object); }
	template<class _Object> static devcb_base &static_set_tmrout1_handler(device_t &device, _Object object) { return downcast<i80186_cpu_device &>(device).m_out_tmrout1_func.set_callback(object); }

	int int_callback(device_t &device, int irqline);
	void drq0_w(int state) { if(state) drq_callback(0); m_dma[0].drq_state = state; }
	void drq1_w(int state) { if(state) drq_callback(1); m_dma[1].drq_state = state; }
	void tmrin0_w(int state) { if(state && (m_timer[0].control & 0x8004) == 0x8004) { inc_timer(0); } }
	void tmrin1_w(int state) { if(state && (m_timer[1].control & 0x8004) == 0x8004) { inc_timer(1); } }
	void int0_w(int state) { external_int(0, state); }
	void int1_w(int state) { external_int(1, state); }
	void int2_w(int state) { external_int(2, state); }
	void int3_w(int state) { external_int(3, state); }

	// device_memory_interface overrides
	virtual const address_space_config *memory_space_config(address_spacenum spacenum = AS_0) const override { return (spacenum == AS_PROGRAM) ? &m_program_config : ( (spacenum == AS_IO) ? &m_io_config : nullptr ); }

protected:
	// device_execute_interface overrides
	virtual uint64_t execute_clocks_to_cycles(uint64_t clocks) const override { return (clocks / 2); }
	virtual uint64_t execute_cycles_to_clocks(uint64_t cycles) const override { return (cycles * 2); }
	virtual void execute_run() override;
	virtual void device_start() override;
	virtual void device_reset() override;
	virtual void device_timer(emu_timer &timer, device_timer_id id, int param, void *ptr) override;
	virtual uint32_t execute_input_lines() const override { return 1; }
	virtual uint8_t fetch_op() override;
	virtual uint8_t fetch() override;
	uint32_t pc() { return m_pc = (m_sregs[CS] << 4) + m_ip; }

	virtual uint8_t read_port_byte(uint16_t port) override;
	virtual uint16_t read_port_word(uint16_t port) override;
	virtual void write_port_byte(uint16_t port, uint8_t data) override;
	virtual void write_port_word(uint16_t port, uint16_t data) override;

	static const uint8_t m_i80186_timing[200];

private:
	void update_interrupt_state();
	void handle_eoi(int data);
	void external_int(uint16_t intno, int state);
	void internal_timer_sync(int which);
	void internal_timer_update(int which, int new_count, int new_maxA, int new_maxB, int new_control);
	void update_dma_control(int which, int new_control);
	void drq_callback(int which);
	void inc_timer(int which);
	uint16_t internal_port_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	void internal_port_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);

	struct mem_state
	{
		uint16_t      lower;
		uint16_t      upper;
		uint16_t      middle;
		uint16_t      middle_size;
		uint16_t      peripheral;
	};

	struct timer_state
	{
		uint16_t      control;
		uint16_t      maxA;
		uint16_t      maxB;
		bool        active_count;
		uint16_t      count;
		emu_timer   *int_timer;
	};

	struct dma_state
	{
		bool        drq_state;
		uint32_t      source;
		uint32_t      dest;
		uint16_t      count;
		uint16_t      control;
	};

	struct intr_state
	{
		uint8_t       pending;
		uint16_t      ack_mask;
		uint16_t      priority_mask;
		uint16_t      in_service;
		uint16_t      request;
		uint16_t      status;
		uint16_t      poll_status;
		uint16_t      timer;
		uint16_t      dma[2];
		uint16_t      ext[4];
		uint8_t       ext_state;
	};

	timer_state     m_timer[3];
	dma_state       m_dma[2];
	intr_state      m_intr;
	mem_state       m_mem;

	static const device_timer_id TIMER_INT0 = 0;
	static const device_timer_id TIMER_INT1 = 1;
	static const device_timer_id TIMER_INT2 = 2;

	uint16_t m_reloc;

	address_space_config m_program_config;
	address_space_config m_io_config;

	devcb_read8 m_read_slave_ack_func;
	devcb_write16 m_out_chip_select_func;
	devcb_write_line m_out_tmrout0_func;
	devcb_write_line m_out_tmrout1_func;
};

class i80188_cpu_device : public i80186_cpu_device
{
public:
	// construction/destruction
	i80188_cpu_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);
};

#define MCFG_80186_IRQ_SLAVE_ACK(_devcb) \
		devcb = &i80186_cpu_device::static_set_read_slave_ack_callback(*device, DEVCB_##_devcb);

#define MCFG_80186_CHIP_SELECT_CB(_devcb) \
		devcb = &i80186_cpu_device::static_set_chip_select_callback(*device, DEVCB_##_devcb);

#define MCFG_80186_TMROUT0_HANDLER(_devcb) \
		devcb = &i80186_cpu_device::static_set_tmrout0_handler(*device, DEVCB_##_devcb);

#define MCFG_80186_TMROUT1_HANDLER(_devcb) \
		devcb = &i80186_cpu_device::static_set_tmrout1_handler(*device, DEVCB_##_devcb);

#endif
