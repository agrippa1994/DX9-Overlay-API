//-----------------------------------------------------------------------------
// File: D3DFont.h
//
// Desc: Texture-based font class
//
// Copyright (c)  Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#ifndef D3DFONT_H
#define D3DFONT_H
#include <tchar.h>
#include <D3D9.h>

#include <string>
#include <vector>
#include <memory>

#include "SharedFont.h"

// Font creation flags
#define D3DFONT_BOLD        0x0001
#define D3DFONT_ITALIC      0x0002
#define D3DFONT_ZENABLE     0x0004

// Font rendering flags
#define D3DFONT_CENTERED_X  0x0001
#define D3DFONT_CENTERED_Y  0x0002
#define D3DFONT_TWOSIDED    0x0004
#define D3DFONT_FILTERED    0x0008
#define D3DFONT_BORDER		0x0010
#define D3DFONT_COLORTABLE	0x0020


//-----------------------------------------------------------------------------
// Name: class CD3DFont
// Desc: Texture-based font class for doing text in a 3D scene.
//-----------------------------------------------------------------------------
class CD3DFont
{
	static std::vector<std::shared_ptr<SharedFont>> sharedFonts;

	static std::shared_ptr<SharedFont> GetFont(const std::wstring &fontName, DWORD dwHeight, DWORD dwFlags);
	static void ReleaseFont(std::shared_ptr<SharedFont> font);

	LPDIRECT3DDEVICE9       m_pd3dDevice; // A D3DDevice used for rendering
	LPDIRECT3DVERTEXBUFFER9 m_pVB;        // VertexBuffer for rendering text

	// Stateblocks for setting and restoring render states
	LPDIRECT3DSTATEBLOCK9 m_pStateBlockSaved;
	LPDIRECT3DSTATEBLOCK9 m_pStateBlockDrawText;

	std::shared_ptr<SharedFont> m_font;
	DWORD m_dwFlags;
public:
	// 2D and 3D text drawing functions
	HRESULT DrawText(FLOAT x, FLOAT y, DWORD dwColor, const WCHAR* strText, DWORD dwFlags = 0L);

	// Function to get extent of text
	HRESULT GetTextExtent(const WCHAR* strText, SIZE* pSize);

	// Initializing and destroying device-dependent objects
	HRESULT InitDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice);
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();

	LPDIRECT3DDEVICE9 getDevice() const { return m_pd3dDevice; }
	// Constructor / destructor
	CD3DFont(const std::wstring &fontName, DWORD dwHeight, DWORD dwFlags = 0L);
	~CD3DFont();
};

#endif