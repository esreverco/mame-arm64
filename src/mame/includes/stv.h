// license:LGPL-2.1+
// copyright-holders:David Haywood, Angelo Salese, Olivier Galibert, Mariusz Wojcieszek, R. Belmont

#include "includes/saturn.h"
#include "machine/gen_latch.h"

class stv_state : public saturn_state
{
public:
	stv_state(const machine_config &mconfig, device_type type, const char *tag)
		: saturn_state(mconfig, type, tag),
		m_adsp(*this, "adsp"),
		m_adsp_pram(*this, "adsp_pram"),
		m_cryptdevice(*this, "315_5881"),
		m_5838crypt(*this, "315_5838"),
		m_soundlatch(*this, "soundlatch")
	{
	}

	DECLARE_DRIVER_INIT(astrass);
	DECLARE_DRIVER_INIT(batmanfr);
	DECLARE_DRIVER_INIT(finlarch);
	DECLARE_DRIVER_INIT(decathlt);
	DECLARE_DRIVER_INIT(sanjeon);
	DECLARE_DRIVER_INIT(puyosun);
	DECLARE_DRIVER_INIT(winterht);
	DECLARE_DRIVER_INIT(gaxeduel);
	DECLARE_DRIVER_INIT(rsgun);
	DECLARE_DRIVER_INIT(groovef);
	DECLARE_DRIVER_INIT(sandor);
	DECLARE_DRIVER_INIT(cottonbm);
	DECLARE_DRIVER_INIT(smleague);
	DECLARE_DRIVER_INIT(nameclv3);
	DECLARE_DRIVER_INIT(danchiq);
	DECLARE_DRIVER_INIT(hanagumi);
	DECLARE_DRIVER_INIT(cotton2);
	DECLARE_DRIVER_INIT(seabass);
	DECLARE_DRIVER_INIT(stv);
	DECLARE_DRIVER_INIT(thunt);
	DECLARE_DRIVER_INIT(critcrsh);
	DECLARE_DRIVER_INIT(stvmp);
	DECLARE_DRIVER_INIT(sasissu);
	DECLARE_DRIVER_INIT(dnmtdeka);
	DECLARE_DRIVER_INIT(ffreveng);
	DECLARE_DRIVER_INIT(fhboxers);
	DECLARE_DRIVER_INIT(pblbeach);
	DECLARE_DRIVER_INIT(sss);
	DECLARE_DRIVER_INIT(diehard);
	DECLARE_DRIVER_INIT(danchih);
	DECLARE_DRIVER_INIT(shienryu);
	DECLARE_DRIVER_INIT(elandore);
	DECLARE_DRIVER_INIT(prikura);
	DECLARE_DRIVER_INIT(maruchan);
	DECLARE_DRIVER_INIT(colmns97);
	DECLARE_DRIVER_INIT(grdforce);
	DECLARE_DRIVER_INIT(suikoenb);
	DECLARE_DRIVER_INIT(magzun);
	DECLARE_DRIVER_INIT(shanhigw);
	DECLARE_DRIVER_INIT(sokyugrt);
	DECLARE_DRIVER_INIT(vfremix);
	DECLARE_DRIVER_INIT(twcup98);
	DECLARE_DRIVER_INIT(znpwfv);
	DECLARE_DRIVER_INIT(othellos);
	DECLARE_DRIVER_INIT(mausuke);

	DECLARE_READ8_MEMBER(stv_ioga_r);
	DECLARE_WRITE8_MEMBER(stv_ioga_w);
	DECLARE_READ8_MEMBER(critcrsh_ioga_r);
	DECLARE_READ8_MEMBER(magzun_ioga_r);
	DECLARE_WRITE8_MEMBER(magzun_ioga_w);
	DECLARE_READ8_MEMBER(stvmp_ioga_r);
	DECLARE_WRITE8_MEMBER(stvmp_ioga_w);
	DECLARE_READ32_MEMBER(stv_ioga_r32);
	DECLARE_WRITE32_MEMBER(stv_ioga_w32);
	DECLARE_READ32_MEMBER(critcrsh_ioga_r32);
	DECLARE_READ32_MEMBER(stvmp_ioga_r32);
	DECLARE_WRITE32_MEMBER(stvmp_ioga_w32);
	DECLARE_READ32_MEMBER(magzun_ioga_r32);
	DECLARE_WRITE32_MEMBER(magzun_ioga_w32);
	DECLARE_READ32_MEMBER(magzun_hef_hack_r);
	DECLARE_READ32_MEMBER(magzun_rx_hack_r);

	image_init_result load_cart(device_image_interface &image, generic_slot_device *slot);
	DECLARE_DEVICE_IMAGE_LOAD_MEMBER( stv_cart1 ) { return load_cart(image, m_cart1); }
	DECLARE_DEVICE_IMAGE_LOAD_MEMBER( stv_cart2 ) { return load_cart(image, m_cart2); }
	DECLARE_DEVICE_IMAGE_LOAD_MEMBER( stv_cart3 ) { return load_cart(image, m_cart3); }
	DECLARE_DEVICE_IMAGE_LOAD_MEMBER( stv_cart4 ) { return load_cart(image, m_cart4); }

	void install_stvbios_speedups( void );

	void machine_start_stv();
	void machine_reset_stv();

	/* Batman Forever specifics */
	optional_device<adsp2181_device>    m_adsp;
	optional_shared_ptr<uint32_t> m_adsp_pram;

	struct
	{
		uint16_t bdma_internal_addr;
		uint16_t bdma_external_addr;
		uint16_t bdma_control;
		uint16_t bdma_word_count;
	} m_adsp_regs;

	void machine_reset_batmanfr();
	DECLARE_READ16_MEMBER( adsp_control_r );
	DECLARE_WRITE16_MEMBER( adsp_control_w );
	DECLARE_WRITE32_MEMBER(batmanfr_sound_comms_w);

	// protection specific variables and functions (see machine/stvprot.c)
	uint32_t m_abus_protenable;
	uint32_t m_abus_protkey;

	uint32_t m_a_bus[4];

	DECLARE_READ32_MEMBER( common_prot_r );
	DECLARE_WRITE32_MEMBER( common_prot_w );

	void install_common_protection();
	void stv_register_protection_savestates();



	optional_device<sega_315_5881_crypt_device> m_cryptdevice;
	optional_device<sega_315_5838_comp_device> m_5838crypt;
	optional_device<generic_latch_16_device> m_soundlatch; // batmanfr
	uint16_t crypt_read_callback(uint32_t addr);
	uint16_t crypt_read_callback_ch1(uint32_t addr);
	uint16_t crypt_read_callback_ch2(uint32_t addr);
};


#define MASTER_CLOCK_352 57272720
#define MASTER_CLOCK_320 53693174
#define CEF_1   m_vdp1_regs[0x010/2]|=0x0002
#define CEF_0   m_vdp1_regs[0x010/2]&=~0x0002
#define BEF_1   m_vdp1_regs[0x010/2]|=0x0001
#define BEF_0   m_vdp1_regs[0x010/2]&=~0x0001
#define STV_VDP1_TVMR ((m_vdp1_regs[0x000/2])&0xffff)
#define STV_VDP1_VBE  ((STV_VDP1_TVMR & 0x0008) >> 3)
#define STV_VDP1_TVM  ((STV_VDP1_TVMR & 0x0007) >> 0)

#define IRQ_VBLANK_IN  1 << 0
#define IRQ_VBLANK_OUT 1 << 1
#define IRQ_HBLANK_IN  1 << 2
#define IRQ_TIMER_0    1 << 3
#define IRQ_TIMER_1    1 << 4
#define IRQ_DSP_END    1 << 5
#define IRQ_SOUND_REQ  1 << 6
#define IRQ_SMPC       1 << 7
#define IRQ_PAD        1 << 8
#define IRQ_DMALV2     1 << 9
#define IRQ_DMALV1     1 << 10
#define IRQ_DMALV0     1 << 11
#define IRQ_DMAILL     1 << 12
#define IRQ_VDP1_END   1 << 13
#define IRQ_ABUS       1 << 15

GFXDECODE_EXTERN( stv );
