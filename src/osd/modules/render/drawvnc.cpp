// license:BSD-3-Clause
// copyright-holders:Miodrag Milanovic
//============================================================
//
//  drawvnc.h - VNC drawing
//
//============================================================

#include "drawvnc.h"
#include "rendersw.hxx"
#include "vnc/vncd_connection.hpp"
#include <iostream>
#define XK_LATIN1
#include "vnc/keysymdef.h"
#include "vnc/vncd.hpp"

class SampleVncdConnection : public VncdConnection {

public:

	SampleVncdConnection(asio::ip::tcp::socket tcpConnection, VncdTimer timer) :
		VncdConnection(std::move(tcpConnection), std::move(timer))
	{
		memset(framebuffer, 0, sizeof(framebuffer));
		// noop
	}

	virtual ~SampleVncdConnection();
	uint8_t framebuffer[640 * 480 * 4];
	void fillFramebufferWith(uint8_t r, uint8_t g, uint8_t b);
protected:
	virtual void keyDownEventRecieved(uint32_t keysym);
	virtual void keyUpEventRecieved(uint32_t keysym);
	virtual void mouseEventRecieved(uint16_t xpos, uint16_t ypos, uint8_t buttonMask);
	virtual void connectionStarted();
	virtual uint8_t* getFramebufferRGBX32();
	virtual uint16_t getFrameWidth();
	virtual uint16_t getFrameHeight();
	virtual std::string getSessionTitle();
	virtual std::string requirePassword();
	virtual void setCurrentStatusMessage(std::string& msg);
	virtual void setCurrentStatusMessage(const char* msg);
};

void SampleVncdConnection::fillFramebufferWith(uint8_t r, uint8_t g, uint8_t b) {
	for (int y = 0; y < 480; ++y) {

		for (int x = 0; x < 640; ++x) {

			framebuffer[(x + (y * 640)) * 4 + 0] = r;
			framebuffer[(x + (y * 640)) * 4 + 1] = g;
			framebuffer[(x + (y * 640)) * 4 + 2] = b;

		}

	}
}

SampleVncdConnection::~SampleVncdConnection() {

}

void SampleVncdConnection::keyDownEventRecieved(uint32_t keysym) {

}

void SampleVncdConnection::keyUpEventRecieved(uint32_t keysym) {
	if (keysym == XK_r) {
		fillFramebufferWith(255, 0, 0);
		notifyClient_regionUpdated(0, 0, 640, 480);
	}

	else if (keysym == XK_g) {
		fillFramebufferWith(0, 255, 0);
		notifyClient_regionUpdated(0, 0, 640, 480);
	}

	else if (keysym == XK_b) {
		fillFramebufferWith(0, 0, 255);
		notifyClient_regionUpdated(0, 0, 640, 480);
	}

	else if (keysym == XK_z) {
		notifyClient_bell();

	}
	else if (keysym == XK_q) {
		notifyClient_sizeChanged();

	}
}

void SampleVncdConnection::mouseEventRecieved(uint16_t xpos, uint16_t ypos, uint8_t buttonMask) {

}

void SampleVncdConnection::connectionStarted() {
	// start timer

	fillFramebufferWith(255, 0, 0);
}

uint8_t* SampleVncdConnection::getFramebufferRGBX32() {
	return framebuffer;
}

uint16_t SampleVncdConnection::getFrameWidth() {
	return 640;
}

uint16_t SampleVncdConnection::getFrameHeight() {
	return 480;
}

std::string SampleVncdConnection::getSessionTitle() {
	return "SampleVncdConnection";
}

void SampleVncdConnection::setCurrentStatusMessage(std::string& status) {
	std::cout << status << "\n";
}

void SampleVncdConnection::setCurrentStatusMessage(const char* status) {
	std::cout << status << "\n";
}

std::string SampleVncdConnection::requirePassword() {

	// Blank: no authentication required
	// Non-zero length: VNC authentication (DES)
	return "test";
}


//============================================================
//  destructor
//============================================================

renderer_vnc::~renderer_vnc()
{
	// free the bitmap memory
	//if (m_bmdata != nullptr)
//		global_free_array(m_bmdata);
}

//============================================================
//  renderer_vnc::create
//============================================================

int renderer_vnc::create()
{
	vncd<SampleVncdConnection> v;
	v.acceptConnections("0.0.0.0", 5900); 
	return 0;
}

//============================================================
//  renderer_vnc::get_primitives
//============================================================

render_primitive_list *renderer_vnc::get_primitives()
{
	auto win = try_getwindow();
	if (win == nullptr)
		return nullptr;

	win->target()->set_bounds(1280, 1024, 1.0);
	return &win->target()->get_primitives();
}

//============================================================
//  renderer_vnc::draw
//============================================================

int renderer_vnc::draw(const int update)
{
/*	
	auto win = assert_window();

	// we don't have any special resize behaviors
	if (win->m_resize_state == RESIZE_STATE_PENDING)
		win->m_resize_state = RESIZE_STATE_NORMAL;

	// get the target bounds
	RECT bounds;
	GetClientRect(win->platform_window<HWND>(), &bounds);

	// compute width/height/pitch of target
	int width = rect_width(&bounds);
	int height = rect_height(&bounds);
	int pitch = (width + 3) & ~3;

	// make sure our temporary bitmap is big enough
	if (pitch * height * 4 > m_bmsize)
	{
		m_bmsize = pitch * height * 4 * 2;
		global_free_array(m_bmdata);
		m_bmdata = global_alloc_array(uint8_t, m_bmsize);
	}

	// draw the primitives to the bitmap
	win->m_primlist->acquire_lock();
	software_renderer<uint32_t, 0,0,0, 16,8,0>::draw_primitives(*win->m_primlist, m_bmdata, width, height, pitch);
	win->m_primlist->release_lock();

	// fill in bitmap-specific info
	m_bminfo.bmiHeader.biWidth = pitch;
	m_bminfo.bmiHeader.biHeight = -height;

	// blit to the screen
	StretchDIBits(win->m_dc, 0, 0, width, height,
				0, 0, width, height,
				m_bmdata, &m_bminfo, DIB_RGB_COLORS, SRCCOPY);*/
	return 0;
}
