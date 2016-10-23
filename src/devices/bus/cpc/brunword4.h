// license:BSD-3-Clause
// copyright-holders:Barry Rodewald
/*

    Brunword MK4 - Word processor ROM / expansion

*/

#include "emu.h"
#include "cpcexp.h"

class cpc_brunword4_device  : public device_t,
				public device_cpc_expansion_card_interface
{
public:
	// construction/destruction
	cpc_brunword4_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	// optional information overrides
	virtual const tiny_rom_entry *device_rom_region() const override;

	void rombank_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	virtual void set_mapping(uint8_t type) override;

protected:
	// device-level overrides
	virtual void device_start() override;
	virtual void device_reset() override;

private:
	cpc_expansion_slot_device *m_slot;

	bool m_rombank_active;
	uint8_t m_bank_sel;
};

// device type definition
extern const device_type CPC_BRUNWORD_MK4;
