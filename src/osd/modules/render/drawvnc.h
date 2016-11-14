// license:BSD-3-Clause
// copyright-holders:Miodrag Milanovic
//============================================================
//
//  drawvnc.h - VNC drawing
//
//============================================================

#pragma once

#ifndef __DRAWVNC__
#define __DRAWVNC__

#include "emu.h"
#include "window.h"


//============================================================
//  TYPE DEFINITIONS
//============================================================

class renderer_vnc : public osd_renderer
{
public:
	renderer_vnc(std::shared_ptr<osd_window> window)
		: osd_renderer(window, FLAG_NONE)
		//, m_bmdata(nullptr)
		//, m_bmsize(0)
	{
	}
	virtual ~renderer_vnc();

	static bool init(running_machine &machine) { return false; }
	static void exit() { }

	virtual int create() override;
	virtual render_primitive_list *get_primitives() override;
	virtual int draw(const int update) override;
	virtual void save() override {};
	virtual void record() override {};
	virtual void toggle_fsfx() override {};

private:
	/* gdi_info is the information for the current screen */
	//uint8_t *               m_bmdata;
	//size_t                  m_bmsize;
};

#endif // __DRAWVNC__
