-- license:BSD-3-Clause
-- copyright-holders:MAMEdev Team

---------------------------------------------------------------------------
--
--   tiny.lua
--
--   Small driver-specific example makefile
--   Use make SUBTARGET=tiny to build
--
---------------------------------------------------------------------------


--------------------------------------------------
-- Specify all the CPU cores necessary for the
-- drivers referenced in tiny.lst.
--------------------------------------------------

CPUS["I386"] = true           
CPUS["I86"] = true            
CPUS["IE15"] = true           
CPUS["M6502"] = true          
CPUS["M6800"] = true          
CPUS["M6805"] = true          
CPUS["MCS48"] = true          
CPUS["MCS51"] = true          
CPUS["NEC"] = true            
CPUS["TMS32010"] = true       
CPUS["UPD7810"] = true        
CPUS["Z80"] = true            


--------------------------------------------------
-- Specify all the sound cores necessary for the
-- drivers referenced in tiny.lst.
--------------------------------------------------

SOUNDS["BEEP"] = true         
SOUNDS["CDDA"] = true         
SOUNDS["DAC"] = true          
SOUNDS["SAA1099"] = true      
SOUNDS["SID6581"] = true      
SOUNDS["SN76496"] = true      
SOUNDS["SPEAKER"] = true      
SOUNDS["VOLT_REG"] = true     
SOUNDS["YM2151"] = true       
SOUNDS["YM3812"] = true       
SOUNDS["YMF262"] = true       


--------------------------------------------------
-- specify available video cores
--------------------------------------------------

VIDEOS["CRTC_EGA"] = true     
VIDEOS["MC6845"] = true       
VIDEOS["PC_VGA"] = true       
VIDEOS["UPD7220"] = true      

--------------------------------------------------
-- specify available machine cores
--------------------------------------------------

MACHINES["ACIA6850"] = true   
MACHINES["AM9517A"] = true    
MACHINES["AT_KEYBC"] = true   
MACHINES["BANKDEV"] = true    
MACHINES["CS4031"] = true     
MACHINES["CS8221"] = true     
MACHINES["DP8390"] = true     
MACHINES["E05A03"] = true     
MACHINES["E05A30"] = true     
MACHINES["EEPROMDEV"] = true  
MACHINES["FDC_PLL"] = true    
MACHINES["GENPC"] = true      
MACHINES["I8251"] = true      
MACHINES["I8255"] = true      
MACHINES["IDE"] = true        
MACHINES["IE15"] = true       
MACHINES["INS8250"] = true    
MACHINES["MC146818"] = true   
MACHINES["MPU401"] = true     
MACHINES["PC_FDC"] = true     
MACHINES["PC_LPT"] = true     
MACHINES["PCKEYBRD"] = true   
MACHINES["PIC8259"] = true    
MACHINES["PIT8253"] = true    
MACHINES["RP5C01"] = true     
MACHINES["STEPPERS"] = true   
MACHINES["UPD765"] = true     
MACHINES["VT82C496"] = true   
MACHINES["WD11C00_17"] = true 
MACHINES["WD2010"] = true     
MACHINES["WD7600"] = true     


--------------------------------------------------
-- specify available bus cores
--------------------------------------------------

BUSES["CENTRONICS"] = true    
BUSES["GENERIC"] = true       
BUSES["ISA"] = true           
BUSES["LPCI"] = true          
BUSES["MIDI"] = true          
BUSES["PC_JOY"] = true        
BUSES["PC_KBD"] = true        
BUSES["RS232"] = true         
BUSES["SUNKBD"] = true        


FORMATS["ASST128_DSK"] = true 
FORMATS["BASICDSK"] = true    
FORMATS["NASLITE_DSK"] = true 
FORMATS["PC_DSK"] = true      
FORMATS["UPD765_DSK"] = true  


--------------------------------------------------
-- This is the list of files that are necessary
-- for building all of the drivers referenced
-- in tiny.lst
--------------------------------------------------

function createProjects_mame_pc(_target, _subtarget)
	project ("mame_pc")
	targetsubdir(_target .."_" .. _subtarget)
	kind (LIBTYPE)
	uuid (os.uuid("drv-mame-pc"))
	addprojectflags()
	precompiledheaders()

	includedirs {
		MAME_DIR .. "src/osd",
		MAME_DIR .. "src/emu",
		MAME_DIR .. "src/devices",
		MAME_DIR .. "src/mame",
		MAME_DIR .. "src/lib",
		MAME_DIR .. "src/lib/util",
		MAME_DIR .. "3rdparty",
		GEN_DIR  .. "mame/layout",
	}

files{
        MAME_DIR .. "src/mame/drivers/at.cpp",
        MAME_DIR .. "src/mame/machine/at.h",
        MAME_DIR .. "src/mame/machine/at.cpp",
        MAME_DIR .. "src/mame/drivers/atpci.cpp",
        MAME_DIR .. "src/mame/drivers/ps2.cpp",
        MAME_DIR .. "src/mame/drivers/ct486.cpp",
        MAME_DIR .. "src/mame/drivers/ibmpc.cpp",
        MAME_DIR .. "src/mame/drivers/ibmpcjr.cpp",
        MAME_DIR .. "src/mame/video/pc_t1t.h",
        MAME_DIR .. "src/mame/video/pc_t1t.cpp",
        MAME_DIR .. "src/mame/drivers/pc.cpp",
        MAME_DIR .. "src/mame/drivers/amstr_pc.cpp",
        MAME_DIR .. "src/mame/drivers/asst128.cpp",
        MAME_DIR .. "src/mame/drivers/europc.cpp",
        MAME_DIR .. "src/mame/drivers/genpc.cpp",
        MAME_DIR .. "src/mame/drivers/genpc.cpp",
        MAME_DIR .. "src/mame/drivers/tandy1t.cpp",
        MAME_DIR .. "src/mame/drivers/tosh1000.cpp",
        MAME_DIR .. "src/mame/video/pc_t1t.cpp",
		MAME_DIR .. "src/mame/drivers/pcxt.cpp",
}
end

function linkProjects_mame_pc(_target, _subtarget)
	links {
		"mame_pc",
	}
end
