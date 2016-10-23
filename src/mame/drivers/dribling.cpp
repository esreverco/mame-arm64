// license:BSD-3-Clause
// copyright-holders:Aaron Giles
/***************************************************************************

    Model Racing Dribbling hardware

    driver by Aaron Giles

    Games supported:
        * Dribbling

****************************************************************************

    Memory map

****************************************************************************

    ========================================================================
    CPU #1
    ========================================================================
    tbd
    ========================================================================
    Interrupts:
        NMI not connected
        IRQ generated by VBLANK
    ========================================================================

***************************************************************************/

#include "emu.h"
#include "cpu/z80/z80.h"
#include "machine/i8255.h"
#include "includes/dribling.h"



/*************************************
 *
 *  Interrupt generation
 *
 *************************************/

INTERRUPT_GEN_MEMBER(dribling_state::dribling_irq_gen)
{
	if (m_di)
		device.execute().set_input_line(0, ASSERT_LINE);
}



/*************************************
 *
 *  PPI inputs
 *
 *************************************/

uint8_t dribling_state::dsr_r(address_space &space, offs_t offset, uint8_t mem_mask)
{
	/* return DSR0-7 */
	return (m_ds << m_sh) | (m_dr >> (8 - m_sh));
}


uint8_t dribling_state::input_mux0_r(address_space &space, offs_t offset, uint8_t mem_mask)
{
	/* low value in the given bit selects */
	if (!(m_input_mux & 0x01))
		return ioport("MUX0")->read();
	else if (!(m_input_mux & 0x02))
		return ioport("MUX1")->read();
	else if (!(m_input_mux & 0x04))
		return ioport("MUX2")->read();
	return 0xff;
}



/*************************************
 *
 *  PPI outputs
 *
 *************************************/

void dribling_state::misc_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask)
{
	/* bit 7 = di */
	m_di = (data >> 7) & 1;
	if (!m_di)
		m_maincpu->set_input_line(0, CLEAR_LINE);

	/* bit 6 = parata */

	/* bit 5 = ab. campo */
	m_abca = (data >> 5) & 1;

	/* bit 4 = ab. a.b.f. */
	/* bit 3 = n/c */

	/* bit 2 = (9) = PC2 */
	/* bit 1 = (10) = PC1 */
	/* bit 0 = (32) = PC0 */
	m_input_mux = data & 7;
	logerror("%s:misc_w(%02X)\n", machine().describe_context(), data);
}


void dribling_state::sound_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask)
{
	/* bit 7 = stop palla */
	/* bit 6 = contrasto */
	/* bit 5 = calcio a */
	/* bit 4 = fischio */
	/* bit 3 = calcio b */
	/* bit 2 = folla a */
	/* bit 1 = folla m */
	/* bit 0 = folla b */
	logerror("%s:sound_w(%02X)\n", machine().describe_context(), data);
}


void dribling_state::pb_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask)
{
	/* write PB0-7 */
	logerror("%s:pb_w(%02X)\n", machine().describe_context(), data);
}


void dribling_state::shr_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask)
{
	/* bit 3 = watchdog */
	if (data & 0x08)
		m_watchdog->watchdog_reset();

	/* bit 2-0 = SH0-2 */
	m_sh = data & 0x07;
}



/*************************************
 *
 *  PPI accessors
 *
 *************************************/

uint8_t dribling_state::ioread(address_space &space, offs_t offset, uint8_t mem_mask)
{
	if (offset & 0x08)
		return m_ppi8255_0->read(space, offset & 3);
	else if (offset & 0x10)
		return m_ppi8255_1->read(space, offset & 3);
	return 0xff;
}


void dribling_state::iowrite(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask)
{
	if (offset & 0x08)
		m_ppi8255_0->write(space, offset & 3, data);
	else if (offset & 0x10)
		m_ppi8255_1->write(space, offset & 3, data);
	else if (offset & 0x40)
	{
		m_dr = m_ds;
		m_ds = data;
	}
}


/*************************************
 *
 *  Main CPU memory handlers
 *
 *************************************/

static ADDRESS_MAP_START( dribling_map, AS_PROGRAM, 8, dribling_state )
	AM_RANGE(0x0000, 0x1fff) AM_ROM
	AM_RANGE(0x2000, 0x3fff) AM_RAM AM_SHARE("videoram")
	AM_RANGE(0x4000, 0x7fff) AM_ROM
	AM_RANGE(0xc000, 0xdfff) AM_RAM_WRITE(dribling_colorram_w) AM_SHARE("colorram")
ADDRESS_MAP_END


static ADDRESS_MAP_START( io_map, AS_IO, 8, dribling_state )
	ADDRESS_MAP_GLOBAL_MASK(0xff)
	AM_RANGE(0x00, 0xff) AM_READWRITE(ioread, iowrite)
ADDRESS_MAP_END



/*************************************
 *
 *  Port definitions
 *
 *************************************/

static INPUT_PORTS_START( dribling )
	PORT_START("MUX0")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_JOYSTICKRIGHT_LEFT ) PORT_PLAYER(1)
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_JOYSTICKRIGHT_RIGHT ) PORT_PLAYER(1)
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_JOYSTICKRIGHT_DOWN ) PORT_PLAYER(1)
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_JOYSTICKRIGHT_UP ) PORT_PLAYER(1)
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_JOYSTICKLEFT_LEFT ) PORT_PLAYER(1)
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_JOYSTICKLEFT_RIGHT ) PORT_PLAYER(1)
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_JOYSTICKLEFT_DOWN ) PORT_PLAYER(1)
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_JOYSTICKLEFT_UP ) PORT_PLAYER(1)

	PORT_START("MUX1")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_JOYSTICKRIGHT_LEFT ) PORT_PLAYER(2)
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_JOYSTICKRIGHT_RIGHT ) PORT_PLAYER(2)
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_JOYSTICKRIGHT_DOWN ) PORT_PLAYER(2)
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_JOYSTICKRIGHT_UP ) PORT_PLAYER(2)
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_JOYSTICKLEFT_LEFT ) PORT_PLAYER(2)
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_JOYSTICKLEFT_RIGHT ) PORT_PLAYER(2)
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_JOYSTICKLEFT_DOWN ) PORT_PLAYER(2)
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_JOYSTICKLEFT_UP ) PORT_PLAYER(2)

	PORT_START("MUX2")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_PLAYER(1)
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_PLAYER(1)
	PORT_BIT( 0x0c, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_PLAYER(2)
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_PLAYER(2)
	PORT_BIT( 0xc0, IP_ACTIVE_LOW, IPT_UNUSED )

	PORT_START("IN0")
	PORT_BIT( 0x0f, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_COIN1 )
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_START1 )
	PORT_DIPNAME( 0x40, 0x40, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x40, DEF_STR( On ) )
	PORT_DIPNAME( 0x80, 0x80, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x80, DEF_STR( On ) )
INPUT_PORTS_END



/*************************************
 *
 *  Machine driver
 *
 *************************************/

void dribling_state::machine_start()
{
	save_item(NAME(m_abca));
	save_item(NAME(m_di));
	save_item(NAME(m_dr));
	save_item(NAME(m_ds));
	save_item(NAME(m_sh));
	save_item(NAME(m_input_mux));
}

void dribling_state::machine_reset()
{
	m_abca = 0;
	m_di = 0;
	m_dr = 0;
	m_ds = 0;
	m_sh = 0;
	m_input_mux = 0;
}


static MACHINE_CONFIG_START( dribling, dribling_state )

	/* basic machine hardware */
	MCFG_CPU_ADD("maincpu", Z80, 5000000)
	MCFG_CPU_PROGRAM_MAP(dribling_map)
	MCFG_CPU_IO_MAP(io_map)
	MCFG_CPU_VBLANK_INT_DRIVER("screen", dribling_state,  dribling_irq_gen)

	MCFG_DEVICE_ADD("ppi8255_0", I8255A, 0)
	MCFG_I8255_IN_PORTA_CB(READ8(dribling_state, dsr_r))
	MCFG_I8255_IN_PORTB_CB(READ8(dribling_state, input_mux0_r))
	MCFG_I8255_OUT_PORTC_CB(WRITE8(dribling_state, misc_w))

	MCFG_DEVICE_ADD("ppi8255_1", I8255A, 0)
	MCFG_I8255_OUT_PORTA_CB(WRITE8(dribling_state, sound_w))
	MCFG_I8255_OUT_PORTB_CB(WRITE8(dribling_state, pb_w))
	MCFG_I8255_IN_PORTC_CB(IOPORT("IN0"))
	MCFG_I8255_OUT_PORTC_CB(WRITE8(dribling_state, shr_w))

	MCFG_WATCHDOG_ADD("watchdog")

	/* video hardware */
	MCFG_SCREEN_ADD("screen", RASTER)
	MCFG_SCREEN_VIDEO_ATTRIBUTES(VIDEO_UPDATE_BEFORE_VBLANK)
	MCFG_SCREEN_REFRESH_RATE(60)
	MCFG_SCREEN_VBLANK_TIME(ATTOSECONDS_IN_USEC(2500) /* not accurate */)
	MCFG_SCREEN_SIZE(256, 256)
	MCFG_SCREEN_VISIBLE_AREA(0, 255, 40, 255)
	MCFG_SCREEN_UPDATE_DRIVER(dribling_state, screen_update_dribling)
	MCFG_SCREEN_PALETTE("palette")

	MCFG_PALETTE_ADD("palette", 256)
	MCFG_PALETTE_INIT_OWNER(dribling_state, dribling)

	/* sound hardware */
MACHINE_CONFIG_END



/*************************************
 *
 *  ROM definitions
 *
 *************************************/

ROM_START( dribling )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "5p.bin",  0x0000, 0x1000, CRC(0e791947) SHA1(57bc4f4e9e1fe3fbac1017601c9c75029b2601a4) )
	ROM_LOAD( "5n.bin",  0x1000, 0x1000, CRC(bd0f223a) SHA1(f9fbc5670a8723c091d61012e545774d315eb18f) ) //
	ROM_LOAD( "5l.bin",  0x4000, 0x1000, CRC(1fccfc85) SHA1(c0365ad54144414218f52209173b858b927c9626) )
	ROM_LOAD( "5k.bin",  0x5000, 0x1000, CRC(737628c4) SHA1(301fda413388c26da5b5150aec2cefc971801749) ) //
	ROM_LOAD( "5h.bin",  0x6000, 0x1000, CRC(30d0957f) SHA1(52135e12094ee1c8828a48c355bdd565aa5895de) ) //

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "3p.bin",  0x0000, 0x1000, CRC(208971b8) SHA1(f91f3ea04d75beb58a61c844472b4dba53d84c0f) )
	ROM_LOAD( "3n.bin",  0x1000, 0x1000, CRC(356c9803) SHA1(8e2ce52f32b33886f4747dadf3aeb78148538173) )

	ROM_REGION( 0x600, "proms", 0 )
	ROM_LOAD( "93453-d9.3c",  0x0000, 0x0400, CRC(b045d005) SHA1(7e3ac10a99aa37f6348b3a57a747116b7025103e) )
	ROM_LOAD( "63s140-d8.3e", 0x0400, 0x0100, CRC(8f1a9908) SHA1(12c513c589757f1282e9979d3589f9b49d30ec0f) )
	ROM_LOAD( "tbp24s10.2d",  0x0500, 0x0100, CRC(a17d6956) SHA1(81724daf2e2d319f55cc34cc881b6a9a4e64e7ac) )
ROM_END


ROM_START( driblingo )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "5p.bin",       0x0000, 0x1000, CRC(0e791947) SHA1(57bc4f4e9e1fe3fbac1017601c9c75029b2601a4) )
	ROM_LOAD( "dribblng.5n",  0x1000, 0x1000, CRC(5271e620) SHA1(ebed8e31057bb8492840a6e3b8bc453f7cb67243) )
	ROM_LOAD( "5l.bin",       0x4000, 0x1000, CRC(1fccfc85) SHA1(c0365ad54144414218f52209173b858b927c9626) )
	ROM_LOAD( "dribblng.5j",  0x5000, 0x1000, CRC(e535ac5b) SHA1(ba13298378f1e5b2b40634874097ad29c402fdea) )
	ROM_LOAD( "dribblng.5h",  0x6000, 0x1000, CRC(e6af7264) SHA1(a015120d85461e599c4bb9626ebea296386a31bb) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "3p.bin",  0x0000, 0x1000, CRC(208971b8) SHA1(f91f3ea04d75beb58a61c844472b4dba53d84c0f) )
	ROM_LOAD( "3n.bin",  0x1000, 0x1000, CRC(356c9803) SHA1(8e2ce52f32b33886f4747dadf3aeb78148538173) )

	ROM_REGION( 0x600, "proms", 0 )
	ROM_LOAD( "93453-d9.3c",  0x0000, 0x0400, CRC(b045d005) SHA1(7e3ac10a99aa37f6348b3a57a747116b7025103e) )
	ROM_LOAD( "63s140-d8.3e", 0x0400, 0x0100, CRC(8f1a9908) SHA1(12c513c589757f1282e9979d3589f9b49d30ec0f) )
	ROM_LOAD( "tbp24s10.2d",  0x0500, 0x0100, CRC(a17d6956) SHA1(81724daf2e2d319f55cc34cc881b6a9a4e64e7ac) )
ROM_END

ROM_START( driblingbr )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "1",  0x0000, 0x1000, CRC(35d97f4f) SHA1(c82b1d2a91e25cf3e3f049e0127d300572f0f54c) )
	ROM_LOAD( "2",  0x1000, 0x1000, CRC(bd0f223a) SHA1(f9fbc5670a8723c091d61012e545774d315eb18f) )
	ROM_LOAD( "3",  0x4000, 0x1000, CRC(1fccfc85) SHA1(c0365ad54144414218f52209173b858b927c9626) )
	ROM_LOAD( "4",  0x5000, 0x1000, CRC(3ed4073a) SHA1(dec36e9dda07ea5f50163b98051955783131773d) )
	ROM_LOAD( "5",  0x6000, 0x1000, CRC(c21a1d32) SHA1(6e919f1416e6c4df133d3140f7331f65f65d4942) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "6",  0x0000, 0x1000, CRC(208971b8) SHA1(f91f3ea04d75beb58a61c844472b4dba53d84c0f) )
	ROM_LOAD( "7",  0x1000, 0x1000, CRC(356c9803) SHA1(8e2ce52f32b33886f4747dadf3aeb78148538173) )

	ROM_REGION( 0x600, "proms", 0 )
	ROM_LOAD( "93453-d9.3c",  0x0000, 0x0400, CRC(b045d005) SHA1(7e3ac10a99aa37f6348b3a57a747116b7025103e) )
	ROM_LOAD( "63s140-d8.3e", 0x0400, 0x0100, CRC(8f1a9908) SHA1(12c513c589757f1282e9979d3589f9b49d30ec0f) )
	ROM_LOAD( "tbp24s10.2d",  0x0500, 0x0100, CRC(a17d6956) SHA1(81724daf2e2d319f55cc34cc881b6a9a4e64e7ac) )
ROM_END


/*************************************
 *
 *  Game drivers
 *
 *************************************/

GAME( 1983, dribling,   0,        dribling, dribling, driver_device, 0, ROT0, "Model Racing", "Dribbling", MACHINE_NO_SOUND | MACHINE_SUPPORTS_SAVE )
GAME( 1983, driblingo,  dribling, dribling, dribling, driver_device, 0, ROT0, "Model Racing (Olympia license)", "Dribbling (Olympia)", MACHINE_NO_SOUND | MACHINE_SUPPORTS_SAVE )
GAME( 1983, driblingbr, dribling, dribling, dribling, driver_device, 0, ROT0, "bootleg (Videomac)", "Dribbling (bootleg, Brazil)", MACHINE_NO_SOUND | MACHINE_SUPPORTS_SAVE )
