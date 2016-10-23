// license:BSD-3-Clause
// copyright-holders:Paul Leaman
/***************************************************************************

   Capcom CPS1/2 hardware

***************************************************************************/

#ifndef _CPS1_H_
#define _CPS1_H_

#include "sound/msm5205.h"
#include "sound/qsound.h"
#include "sound/okim6295.h"
#include "machine/gen_latch.h"
#include "machine/timekpr.h"
#include "cpu/m68000/m68000.h"

// Video raw params
// measured clocks:
// CPS2(Guru): V = 59.6376Hz, H = 15,4445kHz *H is probably measured too low!
// CPS1 GNG: V = 59.61Hz
/* CPS1(Charles MacDonald):
    Pixel clock: 8.00 MHz
    Total pixel clocks per scanline: 512 clocks
    Horizontal sync pulse width : 36 clocks
    Horizontal display and blanking period: 476 clocks
    Frame size: 262 scanlines
    Refresh rate: 59.63 MHz.
*/
#define CPS_PIXEL_CLOCK  (XTAL_16MHz/2)

#define CPS_HTOTAL       (512)
#define CPS_HBEND        (64)
#define CPS_HBSTART      (448)

#define CPS_VTOTAL       (262)
#define CPS_VBEND        (16)
#define CPS_VBSTART      (240)


struct gfx_range
{
	// start and end are as passed by the game (shift adjusted to be all
	// in the same scale a 8x8 tiles): they don't necessarily match the
	// position in ROM.
	int type;
	int start;
	int end;
	int bank;
};

struct CPS1config
{
	const char *name;             /* game driver name */

	/* Some games interrogate a couple of registers on bootup. */
	/* These are CPS1 board B self test checks. They wander from game to */
	/* game. */
	int cpsb_addr;        /* CPS board B test register address */
	int cpsb_value;       /* CPS board B test register expected value */

	/* some games use as a protection check the ability to do 16-bit multiplies */
	/* with a 32-bit result, by writing the factors to two ports and reading the */
	/* result from two other ports. */
	/* It looks like this feature was introduced with 3wonders (CPSB ID = 08xx) */
	int mult_factor1;
	int mult_factor2;
	int mult_result_lo;
	int mult_result_hi;

	/* unknown registers which might be related to the multiply protection */
	int unknown1;
	int unknown2;
	int unknown3;

	int layer_control;
	int priority[4];
	int palette_control;

	/* ideally, the layer enable masks should consist of only one bit, */
	/* but in many cases it is unknown which bit is which. */
	int layer_enable_mask[5];

	/* these depend on the B-board model and PAL */
	int bank_sizes[4];
	const struct gfx_range *bank_mapper;

	/* some C-boards have additional I/O for extra buttons/extra players */
	int in2_addr;
	int in3_addr;
	int out2_addr;

	int bootleg_kludge;
};


class cps_state : public driver_device
{
public:
	cps_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag),
		m_mainram(*this, "mainram"),
		m_gfxram(*this, "gfxram"),
		m_cps_a_regs(*this, "cps_a_regs"),
		m_cps_b_regs(*this, "cps_b_regs"),
		m_qsound_sharedram1(*this, "qsound_ram1"),
		m_qsound_sharedram2(*this, "qsound_ram2"),
		m_objram1(*this, "objram1"),
		m_objram2(*this, "objram2"),
		m_output(*this, "output"),
		m_io_in0(*this, "IN0"),
		m_io_in1(*this, "IN1"),
		m_cps2_dial_type(0),
		m_ecofghtr_dial_direction0(0),
		m_ecofghtr_dial_direction1(0),
		m_ecofghtr_dial_last0(0),
		m_ecofghtr_dial_last1(0),
		m_maincpu(*this, "maincpu"),
		m_audiocpu(*this, "audiocpu"),
		m_oki(*this, "oki"),
		m_m48t35(*this,"m48t35"),
		m_msm_1(*this, "msm1"),
		m_msm_2(*this, "msm2"),
		m_gfxdecode(*this, "gfxdecode"),
		m_screen(*this, "screen"),
		m_palette(*this, "palette"),
		m_soundlatch(*this, "soundlatch"),
		m_soundlatch2(*this, "soundlatch2"),
		m_decrypted_opcodes(*this, "decrypted_opcodes"),
		m_region_key(*this, "key"),
		m_region_stars(*this, "stars")
	{ }

	/* memory pointers */
	// cps1
	optional_shared_ptr<uint16_t> m_mainram;
	required_shared_ptr<uint16_t> m_gfxram;
	required_shared_ptr<uint16_t> m_cps_a_regs;
	required_shared_ptr<uint16_t> m_cps_b_regs;
	uint16_t *     m_scroll1;
	uint16_t *     m_scroll2;
	uint16_t *     m_scroll3;
	uint16_t *     m_obj;
	uint16_t *     m_other;
	std::unique_ptr<uint16_t[]>     m_buffered_obj;
	optional_shared_ptr<uint8_t> m_qsound_sharedram1;
	optional_shared_ptr<uint8_t> m_qsound_sharedram2;
	std::unique_ptr<uint8_t[]> m_decrypt_kabuki;
	// cps2
	optional_shared_ptr<uint16_t> m_objram1;
	optional_shared_ptr<uint16_t> m_objram2;
	optional_shared_ptr<uint16_t> m_output;

	optional_ioport m_io_in0;
	optional_ioport m_io_in1;
	std::unique_ptr<uint16_t[]>     m_cps2_buffered_obj;
	// game-specific
	std::unique_ptr<uint16_t[]>    m_gigaman2_dummyqsound_ram;
	uint16_t  sf2ceblp_prot;

	/* video-related */
	tilemap_t      *m_bg_tilemap[3];
	int          m_scanline1;
	int          m_scanline2;
	int          m_scancalls;

	int          m_scroll1x;
	int          m_scroll1y;
	int          m_scroll2x;
	int          m_scroll2y;
	int          m_scroll3x;
	int          m_scroll3y;

	int          m_stars_enabled[2];        /* Layer enabled [Y/N] */
	int          m_stars1x;
	int          m_stars1y;
	int          m_stars2x;
	int          m_stars2y;
	int          m_last_sprite_offset;      /* Offset of the last sprite */
	int          m_cps2_last_sprite_offset; /* Offset of the last sprite */
	int          m_pri_ctrl;                /* Sprite layer priorities */
	int          m_objram_bank;

	/* misc */
	int          m_dial[2];     // forgottn
	int          m_readpaddle;  // pzloop2
	int          m_cps2networkpresent;
	int          m_cps2digitalvolumelevel;
	int          m_cps2disabledigitalvolume;
	emu_timer    *m_digital_volume_timer;
	int          m_cps2_dial_type;
	int          m_ecofghtr_dial_direction0;
	int          m_ecofghtr_dial_direction1;
	int          m_ecofghtr_dial_last0;
	int          m_ecofghtr_dial_last1;


	/* fcrash sound hw */
	int          m_sample_buffer1;
	int          m_sample_buffer2;
	int          m_sample_select1;
	int          m_sample_select2;

	/* video config (never changed after video_start) */
	const struct CPS1config *m_game_config;
	int          m_scroll_size;
	int          m_obj_size;
	int          m_cps2_obj_size;
	int          m_other_size;
	int          m_palette_align;
	int          m_palette_size;
	int          m_stars_rom_size;
	uint8_t        m_empty_tile[32*32];
	int          m_cps_version;

	/* fcrash video config */
	uint8_t        m_layer_enable_reg;
	uint8_t        m_layer_mask_reg[4];
	int          m_layer_scroll1x_offset;
	int          m_layer_scroll2x_offset;
	int          m_layer_scroll3x_offset;
	int          m_sprite_base;
	int          m_sprite_list_end_marker;
	int          m_sprite_x_offset;
	std::unique_ptr<uint16_t[]> m_bootleg_sprite_ram;
	std::unique_ptr<uint16_t[]> m_bootleg_work_ram;

	/* devices */
	required_device<m68000_base_device> m_maincpu;
	optional_device<cpu_device> m_audiocpu;
	optional_device<okim6295_device> m_oki;
	optional_device<m48t35_device> m_m48t35;
	optional_device<msm5205_device> m_msm_1;    // fcrash
	optional_device<msm5205_device> m_msm_2;    // fcrash
	required_device<gfxdecode_device> m_gfxdecode;
	required_device<screen_device> m_screen;
	required_device<palette_device> m_palette;
	optional_device<generic_latch_8_device> m_soundlatch;
	optional_device<generic_latch_8_device> m_soundlatch2;
	optional_shared_ptr<uint16_t> m_decrypted_opcodes;
	optional_memory_region m_region_key;
	optional_memory_region m_region_stars;

	uint16_t cps1_hack_dsw_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	uint16_t cps1_in1_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	uint16_t cps1_in2_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	uint16_t cps1_in3_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	uint16_t forgottn_dial_0_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	uint16_t forgottn_dial_1_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	void forgottn_dial_0_reset_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void forgottn_dial_1_reset_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void cps1_snd_bankswitch_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void cps1_soundlatch_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void cps1_soundlatch2_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void cpsq_coinctrl2_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	uint16_t qsound_rom_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	uint16_t qsound_sharedram2_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	void qsound_sharedram2_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void qsound_banksw_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint16_t sf2rb_prot_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	uint16_t sf2rb2_prot_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	uint16_t sf2dongb_prot_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	uint16_t sf2ceblp_prot_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	void sf2ceblp_prot_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	uint16_t cps1_dsw_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	void cps1_coinctrl_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	uint16_t qsound_sharedram1_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	void qsound_sharedram1_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	uint16_t ganbare_ram_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	void ganbare_ram_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void cps1_cps_a_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	uint16_t cps1_cps_b_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	void cps1_cps_b_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void cps1_gfxram_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void cps2_objram_bank_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	uint16_t cps2_objram1_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	uint16_t cps2_objram2_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	void cps2_objram1_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void cps2_objram2_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void cps1_oki_pin7_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void sf2m1_layer_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void sf2m3_layer_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	uint16_t dinohunt_sound_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	void init_sf2rb();
	void init_sf2rb2();
	void init_sf2thndr();
	void init_dinohunt();
	void init_forgottn();
	void init_sf2hack();
	void init_slammast();
	void init_pang3b();
	void init_pang3();
	void init_sf2ee();
	void init_sf2m8();
	void init_cps1();
	void init_dino();
	void init_punisher();
	void init_wof();
	void init_ganbare();
	void init_cps2_video();
	void init_cps2();
	void init_cps2nc();
	void init_cps2crypt();
	void init_ssf2tb();
	void init_pzloop2();
	void init_singbrd();
	void init_gigaman2();
	void init_ecofghtr();
	void init_sf2dongb();
	void init_sf2ceblp();
	TILEMAP_MAPPER_MEMBER(tilemap0_scan);
	TILEMAP_MAPPER_MEMBER(tilemap1_scan);
	TILEMAP_MAPPER_MEMBER(tilemap2_scan);
	TILE_GET_INFO_MEMBER(get_tile0_info);
	TILE_GET_INFO_MEMBER(get_tile1_info);
	TILE_GET_INFO_MEMBER(get_tile2_info);
	void machine_start_cps1();
	void video_start_cps1();
	void machine_start_common();
	void machine_start_cps2();
	void video_start_cps2();
	void machine_start_qsound();
	void machine_start_ganbare();
	void machine_reset_cps();
	void video_start_cps();
	void machine_start_sf2m1();
	uint32_t screen_update_cps1(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	void screen_eof_cps1(screen_device &screen, bool state);
	INTERRUPT_GEN_MEMBER(cps1_interrupt);
	TIMER_DEVICE_CALLBACK_MEMBER(ganbare_interrupt);
	TIMER_DEVICE_CALLBACK_MEMBER(cps2_interrupt);
	TIMER_CALLBACK_MEMBER(cps2_update_digital_volume);

	void kabuki_setup(void (*decode)(uint8_t *src, uint8_t *dst));

	/* fcrash handlers */
	void init_kodb();
	void init_cawingbl();
	void init_dinopic();
	void init_knightsb();
	void init_punipic();
	void init_punipic3();
	void init_sf2m1();
	void init_sf2mdt();
	void init_sf2mdta();
	void init_sf2mdtb();
	void init_sf2b();
	void init_slampic();
	void machine_start_fcrash();
	void machine_reset_fcrash();
	void machine_start_cawingbl();
	void machine_start_dinopic();
	void machine_start_knightsb();
	void machine_start_kodb();
	void machine_start_punipic();
	void machine_start_sf2mdt();
	void machine_start_slampic();
	void machine_start_sgyxz();
	void cawingbl_soundlatch_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void dinopic_layer_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void dinopic_layer2_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void knightsb_layer_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void kodb_layer_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void punipic_layer_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void sf2mdt_layer_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void sf2mdta_layer_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void slampic_layer_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void fcrash_soundlatch_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void fcrash_snd_bankswitch_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void sf2mdt_snd_bankswitch_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void knightsb_snd_bankswitch_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void fcrash_msm5205_0_data_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void fcrash_msm5205_1_data_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void varthb_layer_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	uint32_t screen_update_fcrash(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	void fcrash_update_transmasks();
	void fcrash_render_sprites(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	void fcrash_render_layer(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect, int layer, int primask);
	void fcrash_render_high_layer(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect, int layer);
	void fcrash_build_palette();


	/* cps video */
	inline uint16_t *cps1_base( int offset, int boundary );
	void cps1_get_video_base();
	void unshuffle(uint64_t *buf, int len);
	void cps2_gfx_decode();
	int gfxrom_bank_mapper(int type, int code);
	void cps1_update_transmasks();
	void cps1_build_palette(const uint16_t* const palette_base);
	void cps1_find_last_sprite();
	void cps1_render_sprites(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	void cps2_find_last_sprite();
	void cps2_render_sprites(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect, int *primasks);
	void cps1_render_stars(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	void cps1_render_layer(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect, int layer, int primask);
	void cps1_render_high_layer(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect, int layer);
	void cps2_set_sprite_priorities();
	void cps2_objram_latch();
	uint16_t *cps2_objbase();


	/* cps2 driver */
	void init_digital_volume();
	uint16_t gigaman2_dummyqsound_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	void gigaman2_dummyqsound_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void gigaman2_gfx_reorder();
	void cps2_eeprom_port_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	uint16_t cps2_qsound_volume_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	uint16_t kludge_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	uint16_t joy_or_paddle_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	uint16_t joy_or_paddle_ecofghtr_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	DECLARE_WRITE_LINE_MEMBER(m5205_int1);
	DECLARE_WRITE_LINE_MEMBER(m5205_int2);
};

/*----------- defined in drivers/cps1.c -----------*/

MACHINE_CONFIG_EXTERN(cps1_12MHz);

ADDRESS_MAP_EXTERN( qsound_sub_map, 8 );

GFXDECODE_EXTERN( cps1 );

INPUT_PORTS_EXTERN( dino );
INPUT_PORTS_EXTERN( knights );
INPUT_PORTS_EXTERN( punisher );
INPUT_PORTS_EXTERN( sf2 );
INPUT_PORTS_EXTERN( slammast );
INPUT_PORTS_EXTERN( varth );


#endif
