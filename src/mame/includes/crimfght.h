// license:BSD-3-Clause
// copyright-holders:Manuel Abadia
/*************************************************************************

    Crime Fighters

*************************************************************************/

#include "machine/bankdev.h"
#include "machine/gen_latch.h"
#include "sound/k007232.h"
#include "video/k052109.h"
#include "video/k051960.h"

class crimfght_state : public driver_device
{
public:
	crimfght_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag),
		m_maincpu(*this, "maincpu"),
		m_audiocpu(*this, "audiocpu"),
		m_bank0000(*this, "bank0000"),
		m_k007232(*this, "k007232"),
		m_k052109(*this, "k052109"),
		m_k051960(*this, "k051960"),
		m_palette(*this, "palette"),
		m_soundlatch(*this, "soundlatch"),
		m_rombank(*this, "rombank") { }

	/* devices */
	required_device<cpu_device> m_maincpu;
	required_device<cpu_device> m_audiocpu;
	required_device<address_map_bank_device> m_bank0000;
	required_device<k007232_device> m_k007232;
	required_device<k052109_device> m_k052109;
	required_device<k051960_device> m_k051960;
	required_device<palette_device> m_palette;
	required_device<generic_latch_8_device> m_soundlatch;
	required_memory_bank m_rombank;

	void crimfght_coin_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void sound_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t k052109_051960_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void k052109_051960_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	int audiocpu_irq_ack(device_t &device, int irqline);
	void ym2151_ct_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	virtual void machine_start() override;
	uint32_t screen_update_crimfght(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	void volume_callback(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	K052109_CB_MEMBER(tile_callback);
	K051960_CB_MEMBER(sprite_callback);
	void banking_callback(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	ioport_value system_r(ioport_field &field, void *param);

private:
	int m_woco;
	int m_rmrd;
	int m_init;
};
