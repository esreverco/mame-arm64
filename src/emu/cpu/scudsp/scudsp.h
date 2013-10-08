/*****************************************************************************
 *
 * SCUDSP CPU core
 *
 * skeleton for now ...
 *
 *****************************************************************************/

#pragma once

#ifndef __SCUDSP_H__
#define __SCUDSP_H__

enum
{
	#if 0
	SCUDSP_R0=1, SCUDSP_R1, SCUDSP_R2, SCUDSP_R3,
	SCUDSP_R4, SCUDSP_R5, SCUDSP_R6, SCUDSP_R7
	#endif
};

#define SCUDSP_RESET        INPUT_LINE_RESET    /* Non-Maskable */


class scudsp_cpu_device :  public cpu_device
{
public:
	// construction/destruction
	scudsp_cpu_device(const machine_config &mconfig, const char *_tag, device_t *_owner, UINT32 _clock);

protected:
	// device-level overrides
	virtual void device_start();
	virtual void device_reset();

	// device_execute_interface overrides
	virtual UINT32 execute_min_cycles() const { return 1; }
	virtual UINT32 execute_max_cycles() const { return 7; }
	virtual UINT32 execute_input_lines() const { return 0; }
	virtual void execute_run();
	virtual void execute_set_input(int inputnum, int state);

	// device_memory_interface overrides
	virtual const address_space_config *memory_space_config(address_spacenum spacenum = AS_0) const { return (spacenum == AS_PROGRAM) ? &m_program_config : ( (spacenum == AS_DATA) ? &m_data_config : NULL ); }

	// device_state_interface overrides
	void state_string_export(const device_state_entry &entry, astring &string);

	// device_disasm_interface overrides
	virtual UINT32 disasm_min_opcode_bytes() const { return 4; }
	virtual UINT32 disasm_max_opcode_bytes() const { return 4; }
	virtual offs_t disasm_disassemble(char *buffer, offs_t pc, const UINT8 *oprom, const UINT8 *opram, UINT32 options);

private:
	address_space_config m_program_config;
	address_space_config m_data_config;

	UINT8	m_pc;   /* registers */
	UINT8	m_flags;  /* flags */
	int     m_reset_state;
	address_space *m_program;
	address_space *m_data;
	int m_icount;

	void scudsp_illegal();

};


extern const device_type SCUDSP;


CPU_DISASSEMBLE( scudsp );

#endif /* __SCUDSP_H__ */
