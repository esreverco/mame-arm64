// license:BSD-3-Clause
// copyright-holders:F. Ulivi
//
// *****************************************
// Emulator for HP "hybrid" processor series
// *****************************************
//
// The HP hybrid processor series is composed of a few different models with different
// capabilities. The series was derived from HP's own 2116 processor by translating a
// discrete implementation of the 1960s into a multi-chip module (hence the "hybrid" name).
// This emulator currently supports both the 5061-3001 & the 5061-3011 versions.
//
// For this emulator I mainly relied on these sources:
// - http://www.hp9845.net/ website
// - HP manual "Assembly development ROM manual for the HP9845": this is the most precious
//   and "enabling" resource of all
// - US Patent 4,180,854 describing the HP9845 system
// - Study of disassembly of firmware of HP64000 & HP9845 systems
// - hp9800e emulator for inspiration on implementing EMC instructions
// - A lot of "educated" guessing

#ifndef _HPHYBRID_H_
#define _HPHYBRID_H_

// Input lines
#define HPHYBRID_IRH    0       // High-level interrupt
#define HPHYBRID_IRL    1       // Low-level interrupt
#define HPHYBRID_INT_LVLS   2   // Levels of interrupt

// I/O addressing space (16-bit wide)
// Addresses into this space are composed as follows:
// b[5..2] = Peripheral address 0..15
// b[1..0] = Register address (IC) 0..3
#define HP_IOADDR_PA_SHIFT      2
#define HP_IOADDR_IC_SHIFT      0

// Compose an I/O address from PA & IC
#define HP_MAKE_IOADDR(pa , ic)    (((pa) << HP_IOADDR_PA_SHIFT) | ((ic) << HP_IOADDR_IC_SHIFT))

// Addresses of memory mapped registers
#define HP_REG_A_ADDR   0x0000
#define HP_REG_B_ADDR   0x0001
#define HP_REG_P_ADDR   0x0002
#define HP_REG_R_ADDR   0x0003
#define HP_REG_R4_ADDR  0x0004
#define HP_REG_R5_ADDR  0x0005
#define HP_REG_R6_ADDR  0x0006
#define HP_REG_R7_ADDR  0x0007
#define HP_REG_IV_ADDR  0x0008
#define HP_REG_PA_ADDR  0x0009
#define HP_REG_W_ADDR   0x000A
#define HP_REG_DMAPA_ADDR       0x000B
#define HP_REG_DMAMA_ADDR       0x000C
#define HP_REG_DMAC_ADDR        0x000D
#define HP_REG_C_ADDR   0x000e
#define HP_REG_D_ADDR   0x000f
#define HP_REG_AR2_ADDR 0x0010
#define HP_REG_SE_ADDR  0x0014
#define HP_REG_R25_ADDR 0x0015
#define HP_REG_R26_ADDR 0x0016
#define HP_REG_R27_ADDR 0x0017
#define HP_REG_R32_ADDR 0x001a
#define HP_REG_R33_ADDR 0x001b
#define HP_REG_R34_ADDR 0x001c
#define HP_REG_R35_ADDR 0x001d
#define HP_REG_R36_ADDR 0x001e
#define HP_REG_R37_ADDR 0x001f
#define HP_REG_LAST_ADDR        0x001f
#define HP_REG_AR1_ADDR 0xfff8

#define HP_REG_IV_MASK  0xfff0
#define HP_REG_PA_MASK  0x000f

// Set boot mode of 5061-3001: either normal (false) or as in HP9845 system (true)
#define MCFG_HPHYBRID_SET_9845_BOOT(_mode) \
	hp_5061_3001_cpu_device::set_boot_mode_static(*device, _mode);

// PA changed callback
#define MCFG_HPHYBRID_PA_CHANGED(_devcb) \
		hp_hybrid_cpu_device::set_pa_changed_func(*device , DEVCB_##_devcb);

class hp_hybrid_cpu_device : public cpu_device
{
public:
				void dmar_w(int state);
				void halt_w(int state);
				void status_w(int state);
				void flag_w(int state);

		uint8_t pa_r(void) const;

		template<class _Object> static devcb_base &set_pa_changed_func(device_t &device, _Object object) { return downcast<hp_hybrid_cpu_device &>(device).m_pa_changed_func.set_callback(object); }

protected:
		hp_hybrid_cpu_device(const machine_config &mconfig, device_type type, const char *name, const char *tag, device_t *owner, uint32_t clock, const char *shortname , uint8_t addrwidth);

				// device-level overrides
				virtual void device_start() override;
				virtual void device_reset() override;

				// device_execute_interface overrides
				virtual uint32_t execute_min_cycles() const override { return 6; }
				virtual uint32_t execute_input_lines() const override { return 2; }
				virtual uint32_t execute_default_irq_vector() const  override { return 0xffff; }
				virtual void execute_run() override;
				virtual void execute_set_input(int inputnum, int state) override;

				uint16_t execute_one(uint16_t opcode);
				uint16_t execute_one_sub(uint16_t opcode);
		// Execute an instruction that doesn't belong to either BPC or IOC
		virtual uint16_t execute_no_bpc_ioc(uint16_t opcode) = 0;

				// device_memory_interface overrides
				virtual const address_space_config *memory_space_config(address_spacenum spacenum = AS_0) const override { return (spacenum == AS_PROGRAM) ? &m_program_config : ( (spacenum == AS_IO) ? &m_io_config : nullptr ); }

				// device_state_interface overrides
				void state_string_export(const device_state_entry &entry, std::string &str) const override;

				// device_disasm_interface overrides
				virtual uint32_t disasm_min_opcode_bytes() const override { return 2; }
				virtual uint32_t disasm_max_opcode_bytes() const override { return 2; }
				virtual offs_t disasm_disassemble(char *buffer, offs_t pc, const uint8_t *oprom, const uint8_t *opram, uint32_t options) override;

		// Different cases of memory access
		// See patent @ pg 361
		typedef enum {
				AEC_CASE_A,     // Instr. fetches, non-base page fetches of link pointers, BPC direct non-base page accesses
				AEC_CASE_B,     // Base page fetches of link pointers, BPC direct base page accesses
				AEC_CASE_C,     // IOC, EMC & BPC indirect final destination accesses
				AEC_CASE_D      // DMA accesses
		} aec_cases_t;

		// do memory address extension
		virtual uint32_t add_mae(aec_cases_t aec_case , uint16_t addr) = 0;

		uint16_t remove_mae(uint32_t addr);

		uint16_t RM(aec_cases_t aec_case , uint16_t addr);
		uint16_t RM(uint32_t addr);
		virtual uint16_t read_non_common_reg(uint16_t addr) = 0;

		void   WM(aec_cases_t aec_case , uint16_t addr , uint16_t v);
		void   WM(uint32_t addr , uint16_t v);
		virtual void write_non_common_reg(uint16_t addr , uint16_t v) = 0;

		uint16_t fetch(void);

		uint16_t get_skip_addr(uint16_t opcode , bool condition) const;

		devcb_write8 m_pa_changed_func;

				int m_icount;
		bool m_forced_bsc_25;

				// State of processor
				uint16_t m_reg_A;     // Register A
				uint16_t m_reg_B;     // Register B
				uint16_t m_reg_P;     // Register P
				uint16_t m_reg_R;     // Register R
				uint16_t m_reg_C;     // Register C
				uint16_t m_reg_D;     // Register D
				uint16_t m_reg_IV;    // Register IV
		uint16_t m_reg_W; // Register W
				uint8_t  m_reg_PA[ HPHYBRID_INT_LVLS + 1 ];   // Stack of register PA (4 bit-long)
				uint32_t m_flags;     // Flags
				uint8_t  m_dmapa;     // DMA peripheral address (4 bits)
				uint16_t m_dmama;     // DMA address
				uint16_t m_dmac;      // DMA counter
				uint16_t m_reg_I;     // Instruction register
		uint32_t m_genpc; // Full PC

private:
				address_space_config m_program_config;
				address_space_config m_io_config;

				address_space *m_program;
				direct_read_data *m_direct;
				address_space *m_io;

				uint32_t get_ea(uint16_t opcode);
				void do_add(uint16_t& addend1 , uint16_t addend2);
				uint16_t get_skip_addr_sc(uint16_t opcode , uint16_t& v , unsigned n);
				uint16_t get_skip_addr_sc(uint16_t opcode , uint32_t& v , unsigned n);
				void do_pw(uint16_t opcode);
				void check_for_interrupts(void);
		virtual void enter_isr(void);
								void handle_dma(void);

				uint16_t RIO(uint8_t pa , uint8_t ic);
				void   WIO(uint8_t pa , uint8_t ic , uint16_t v);
};

class hp_5061_3001_cpu_device : public hp_hybrid_cpu_device
{
public:
		hp_5061_3001_cpu_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	static void set_boot_mode_static(device_t &device, bool mode) { downcast<hp_5061_3001_cpu_device &>(device).m_boot_mode = mode; }

protected:
		virtual void device_start() override;
		virtual void device_reset() override;
		virtual uint32_t execute_max_cycles() const override { return 237; }       // FMP 15

		static uint8_t do_dec_shift_r(uint8_t d1 , uint64_t& mantissa);
		static uint8_t do_dec_shift_l(uint8_t d12 , uint64_t& mantissa);
		uint64_t get_ar1(void);
		void set_ar1(uint64_t v);
		uint64_t get_ar2(void) const;
		void set_ar2(uint64_t v);
		uint64_t do_mrxy(uint64_t ar);
		bool do_dec_add(bool carry_in , uint64_t& a , uint64_t b);
		void do_mpy(void);

		virtual uint16_t execute_no_bpc_ioc(uint16_t opcode) override;
		virtual offs_t disasm_disassemble(char *buffer, offs_t pc, const uint8_t *oprom, const uint8_t *opram, uint32_t options) override;
		virtual uint32_t add_mae(aec_cases_t aec_case , uint16_t addr) override;
		virtual uint16_t read_non_common_reg(uint16_t addr) override;
		virtual void write_non_common_reg(uint16_t addr , uint16_t v) override;

private:
		bool m_boot_mode;

		// Additional state of processor
		uint16_t m_reg_ar2[ 4 ];  // AR2 register
		uint16_t m_reg_se;        // SE register (4 bits)
		uint16_t m_reg_r25;       // R25 register
		uint16_t m_reg_r26;       // R26 register
		uint16_t m_reg_r27;       // R27 register
		uint16_t m_reg_aec[ HP_REG_R37_ADDR - HP_REG_R32_ADDR + 1 ];      // AEC registers R32-R37

		virtual void enter_isr(void) override;
};

class hp_5061_3011_cpu_device : public hp_hybrid_cpu_device
{
public:
		hp_5061_3011_cpu_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

protected:
		virtual uint32_t execute_max_cycles() const override { return 25; }
		virtual uint16_t execute_no_bpc_ioc(uint16_t opcode) override;
		virtual uint32_t add_mae(aec_cases_t aec_case , uint16_t addr) override;
		virtual uint16_t read_non_common_reg(uint16_t addr) override;
		virtual void write_non_common_reg(uint16_t addr , uint16_t v) override;

};

extern const device_type HP_5061_3001;
extern const device_type HP_5061_3011;

#endif /* _HPHYBRID_H_ */
