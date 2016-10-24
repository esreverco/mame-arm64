// license:BSD-3-Clause
// copyright-holders:Bryan McPhail
/*************************************************************************

    The Main Event / Devastators

*************************************************************************/
#include "sound/upd7759.h"
#include "sound/k007232.h"
#include "video/k052109.h"
#include "video/k051960.h"
#include "video/k051733.h"
#include "video/konami_helper.h"

class mainevt_state : public driver_device
{
public:
	mainevt_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag),
		m_maincpu(*this, "maincpu"),
		m_audiocpu(*this, "audiocpu"),
		m_upd7759(*this, "upd"),
		m_k007232(*this, "k007232"),
		m_k052109(*this, "k052109"),
		m_k051960(*this, "k051960"),
		m_rombank(*this, "rombank") { }

	/* misc */
	int        m_nmi_enable;
	uint8_t      m_sound_irq_mask;

	/* devices */
	required_device<cpu_device> m_maincpu;
	required_device<cpu_device> m_audiocpu;
	optional_device<upd7759_device> m_upd7759;
	required_device<k007232_device> m_k007232;
	required_device<k052109_device> m_k052109;
	required_device<k051960_device> m_k051960;

	required_memory_bank m_rombank;

	void dv_nmienable_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void mainevt_bankswitch_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void mainevt_coin_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void mainevt_sh_irqtrigger_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void mainevt_sh_irqcontrol_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void devstor_sh_irqcontrol_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void mainevt_sh_bankswitch_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t k052109_051960_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void k052109_051960_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t mainevt_sh_busy_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void dv_sh_bankswitch_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	virtual void machine_start() override;
	virtual void machine_reset() override;
	uint32_t screen_update_mainevt(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	uint32_t screen_update_dv(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	void mainevt_interrupt(device_t &device);
	void dv_interrupt(device_t &device);
	void mainevt_sound_timer_irq(device_t &device);
	void devstors_sound_timer_irq(device_t &device);
	void volume_callback(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	K052109_CB_MEMBER(mainevt_tile_callback);
	K052109_CB_MEMBER(dv_tile_callback);
	K051960_CB_MEMBER(mainevt_sprite_callback);
	K051960_CB_MEMBER(dv_sprite_callback);
};
