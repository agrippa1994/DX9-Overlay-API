//-----------------------------------------------------------------------------
// File: D3DFont.cpp
//
// Desc: Texture-based font class
//
// Copyright (c)  Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#define STRICT
#include <stdio.h>
#include <tchar.h>
#include <d3dx9.h>
#include "D3DFont.h"

#include <vector>

#define SAFE_RELEASE( p ) if( p ){ p->Release(); p = NULL; }


//-----------------------------------------------------------------------------
// Custom vertex types for rendering text
//-----------------------------------------------------------------------------
#define MAX_NUM_VERTICES 50*6

struct FONT2DVERTEX { D3DXVECTOR4 p;   DWORD color;     FLOAT tu, tv; };
struct FONT3DVERTEX { D3DXVECTOR3 p;   D3DXVECTOR3 n;   FLOAT tu, tv; };

#define D3DFVF_FONT2DVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define D3DFVF_FONT3DVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

inline FONT2DVERTEX InitFont2DVertex(const D3DXVECTOR4& p, D3DCOLOR color,
	FLOAT tu, FLOAT tv)
{
	FONT2DVERTEX v;   v.p = p;   v.color = color;   v.tu = tu;   v.tv = tv;
	return v;
}

inline FONT3DVERTEX InitFont3DVertex(const D3DXVECTOR3& p, const D3DXVECTOR3& n,
	FLOAT tu, FLOAT tv)
{
	FONT3DVERTEX v;   v.p = p;   v.n = n;   v.tu = tu;   v.tv = tv;
	return v;
}




//-----------------------------------------------------------------------------
// Name: CD3DFont()
// Desc: Font class constructor
//-----------------------------------------------------------------------------
CD3DFont::CD3DFont(const std::wstring &fontName, DWORD dwHeight, DWORD dwFlags)
{
	m_fontName = fontName;
	m_dwFontHeight = dwHeight;
	m_dwFontFlags = dwFlags;
	m_dwSpacing = 0;

	m_pd3dDevice = nullptr;
	m_font = nullptr;
	m_pVB = NULL;

	m_pStateBlockSaved = NULL;
	m_pStateBlockDrawText = NULL;

	InitializeFont();

	for (size_t i = 0; i < ARRAYSIZE(m_pTexture); i++)
		m_pTexture[i] = nullptr;

	for (size_t i = 0; i < ARRAYSIZE(m_textureSize); i++)
		m_textureSize[i] = { 0, 0 };
}

//-----------------------------------------------------------------------------
// Name: ~CD3DFont()
// Desc: Font class destructor
//-----------------------------------------------------------------------------
CD3DFont::~CD3DFont()
{
	DestroyFont();
	InvalidateDeviceObjects();
	DeleteDeviceObjects();
}

void CD3DFont::InitializeFont()
{
	if (m_font == nullptr)
	{
		m_fTextScale = 1.0f;

		HDC hDC = CreateCompatibleDC(NULL);

		// Create a font.  By specifying ANTIALIASED_QUALITY, we might get an
		// antialiased font, but this is not guaranteed.
		INT nHeight = -MulDiv(m_dwFontHeight,
			(INT)(GetDeviceCaps(hDC, LOGPIXELSY) * m_fTextScale), 72);
		DWORD dwBold = (m_dwFontFlags&D3DFONT_BOLD) ? FW_BOLD : FW_NORMAL;
		DWORD dwItalic = (m_dwFontFlags&D3DFONT_ITALIC) ? TRUE : FALSE;
		m_font = CreateFontW(nHeight, 0, 0, 0, dwBold, dwItalic,
			FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
			VARIABLE_PITCH, m_fontName.c_str());

		if (m_font == NULL)
			m_font = nullptr;

		DeleteDC(hDC);
	}
}

void CD3DFont::DestroyFont()
{
	if (m_font != nullptr)
	{
		DeleteObject(m_font);
		m_font = nullptr;
	}
}

LPDIRECT3DTEXTURE9 CD3DFont::GetCharacterTexture(USHORT index)
{
	if (m_pd3dDevice == nullptr)
		return nullptr;

	if (m_pTexture[index] == nullptr)
	{
		// Prepare to create a bitmap
		DWORD*      pBitmapBits;
		BITMAPINFO bmi;
		ZeroMemory(&bmi.bmiHeader, sizeof(BITMAPINFOHEADER));
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = 256;
		bmi.bmiHeader.biHeight = -256;
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biCompression = BI_RGB;
		bmi.bmiHeader.biBitCount = 32;
		
		HDC hDC = CreateCompatibleDC(NULL);
		SetMapMode(hDC, MM_TEXT);

		SelectObject(hDC, m_font);

		// Set text properties
		SetTextColor(hDC, RGB(255, 255, 255));
		SetBkColor(hDC, 0x00000000);
		SetTextAlign(hDC, TA_TOP);

		// Initialize for GetTextExtentPoint32 and getting dimension of character
		SIZE size;
		WCHAR str[2] = { (WCHAR)index, L'\0' };

		// Get minimum spacing (Maybe useless?)
		//GetTextExtentPoint32(hDC, TEXT(" "), 1, &size);
		//m_dwSpacing = (DWORD)ceil(size.cy * 0.3f);

		// Caculate real character
		GetTextExtentPoint32W(hDC, str, 1, &size);
		m_textureSize[index] = size;

		bmi.bmiHeader.biWidth = size.cx;
		bmi.bmiHeader.biHeight = -size.cy;

		// Create a bitmap for the font
		HBITMAP hbmBitmap = CreateDIBSection(hDC, &bmi, DIB_RGB_COLORS,
			(void**)&pBitmapBits, NULL, 0);

		SelectObject(hDC, hbmBitmap);

		// Output character to bitmap
		ExtTextOutW(hDC, 0, 0, ETO_OPAQUE, NULL, str, 1, NULL);

		// Create texture
		HRESULT hr = m_pd3dDevice->CreateTexture(size.cx, size.cy, 1,
			0, D3DFMT_A4R4G4B4,
			D3DPOOL_MANAGED, &m_pTexture[index], NULL);

		if (FAILED(hr))
		{
			DeleteObject(hbmBitmap);
			DeleteDC(hDC);
			return nullptr;
		}

		// Lock the surface and write the alpha values for the set pixels
		D3DLOCKED_RECT d3dlr;
		m_pTexture[index]->LockRect(0, &d3dlr, 0, 0);
		BYTE* pDstRow = (BYTE*)d3dlr.pBits;
		WORD* pDst16;
		BYTE bAlpha; // 4-bit measure of pixel intensity

		for (LONG y = 0; y < size.cy; y++)
		{
			pDst16 = (WORD*)pDstRow;
			for (LONG x = 0; x < size.cx; x++)
			{
				bAlpha = (BYTE)((pBitmapBits[size.cx*y + x] & 0xff) >> 4);
				if (bAlpha > 0)
				{
					*pDst16++ = (WORD)((bAlpha << 12) | 0x0fff);
				}
				else
				{
					*pDst16++ = 0x0000;
				}
			}
			pDstRow += d3dlr.Pitch;
		}

		m_pTexture[index]->UnlockRect(0);

		std::string s = "C:\\Users\\media\\Documents\\GitHub\\DX9-Overlay-API\\bin\\" + std::to_string(index) + ".png";
		D3DXSaveTextureToFile(s.c_str(), D3DXIMAGE_FILEFORMAT::D3DXIFF_PNG, m_pTexture[index], NULL);

		// Done updating texture, so clean up used objects
		DeleteObject(hbmBitmap);
		DeleteDC(hDC);
	}

	return m_pTexture[index];
}


//-----------------------------------------------------------------------------
// Name: InitDeviceObjects()
// Desc: Initializes device-dependent objects, including the vertex buffer used
//       for rendering text and the texture map which stores the font image.
//-----------------------------------------------------------------------------
HRESULT CD3DFont::InitDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice)
{
	// Keep a local copy of the device
	m_pd3dDevice = pd3dDevice;
	if (GetCharacterTexture(L' ') == nullptr)
		return S_FALSE;

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: RestoreDeviceObjects()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CD3DFont::RestoreDeviceObjects()
{
	HRESULT hr;

	// Create vertex buffer for the letters
	int vertexSize = max(sizeof(FONT2DVERTEX), sizeof(FONT3DVERTEX));
	if (FAILED(hr = m_pd3dDevice->CreateVertexBuffer(MAX_NUM_VERTICES * vertexSize,
		D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 0,
		D3DPOOL_DEFAULT, &m_pVB, NULL)))
	{
		return hr;
	}

	// Create the state blocks for rendering text
	for (UINT which = 0; which < 2; which++)
	{
		m_pd3dDevice->BeginStateBlock();

		if (D3DFONT_ZENABLE & m_dwFontFlags)
			m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
		else
			m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

		m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pd3dDevice->SetRenderState(D3DRS_ALPHAREF, 0x08);
		m_pd3dDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
		m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		m_pd3dDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
		m_pd3dDevice->SetRenderState(D3DRS_CLIPPING, TRUE);
		m_pd3dDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, FALSE);
		m_pd3dDevice->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
		m_pd3dDevice->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
		m_pd3dDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
		m_pd3dDevice->SetRenderState(D3DRS_COLORWRITEENABLE,
			D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN |
			D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
		m_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
		m_pd3dDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
		m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
		m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		m_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

		if (which == 0)
			m_pd3dDevice->EndStateBlock(&m_pStateBlockSaved);
		else
			m_pd3dDevice->EndStateBlock(&m_pStateBlockDrawText);
	}

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: InvalidateDeviceObjects()
// Desc: Destroys all device-dependent objects
//-----------------------------------------------------------------------------
HRESULT CD3DFont::InvalidateDeviceObjects()
{
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pStateBlockSaved);
	SAFE_RELEASE(m_pStateBlockDrawText);

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: DeleteDeviceObjects()
// Desc: Destroys all device-dependent objects
//-----------------------------------------------------------------------------
HRESULT CD3DFont::DeleteDeviceObjects()
{
	for (size_t i = 0; i < ARRAYSIZE(m_pTexture); i++)
	{
		// Save Release
		if (m_pTexture[i] != nullptr)
		{
			m_pTexture[i]->Release();
			m_pTexture[i] = nullptr;
		}
	}

	m_pd3dDevice = nullptr;

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: GetTextExtent()
// Desc: Get the dimensions of a text string
//-----------------------------------------------------------------------------
HRESULT CD3DFont::GetTextExtent(const WCHAR* strText, SIZE* pSize)
{
	if (NULL == strText || NULL == pSize)
		return E_FAIL;

	float rowWidth = 0.0f;
	float rowHeight = (float)m_textureSize[L' '].cy;
	float width = 0.0f;
	float height = rowHeight;

	auto stringLength = lstrlenW(strText);

	for (int i = 0; i < stringLength; i++)
	{
		wchar_t c = strText[i];

		if (c == L'{')
		{
			int endIndex = 0;
			bool valid = true;
			std::wstring numericString = L"";
			for (int j = i + 1; j < stringLength; j++)
			{
				wchar_t ch = strText[j];
				ch = towupper(ch);

				if (ch == '}')
				{
					endIndex = j;
					break;
				}

				bool numeric = (ch >= L'0' && ch <= L'9') || (ch >= L'A' && ch <= L'F');
				if (!numeric)
				{
					valid = false;
					break;
				}
				else
				{
					numericString += ch;
					if (numericString.size() > 8)
					{
						valid = false;
						break;
					}
				}
			}

			if (endIndex > 0 && valid)
			{
				i = endIndex;
				continue;
			}
		}

		if (c == '\n')
		{
			rowWidth = 0.0f;
			height += rowHeight;
			continue;
		}

		if (c < 0 || c > USHRT_MAX)
			continue;

		if(GetCharacterTexture(c) == nullptr)
			return S_FALSE;

		rowWidth += m_textureSize[c].cx;

		if (rowWidth > width)
			width = rowWidth;
	}

	pSize->cx = (int)width;
	pSize->cy = (int)height;

	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: DrawText()
// Desc: Draws 2D text. Note that sx and sy are in pixels
//-----------------------------------------------------------------------------
HRESULT CD3DFont::DrawText(FLOAT sx, FLOAT sy, DWORD dwColor,
	const WCHAR* strText, DWORD dwFlags)
{
	if (m_pd3dDevice == NULL || strText == NULL)
		return E_FAIL;

	// Setup renderstate
	m_pStateBlockSaved->Capture();
	m_pStateBlockDrawText->Apply();
	m_pd3dDevice->SetFVF(D3DFVF_FONT2DVERTEX);
	m_pd3dDevice->SetPixelShader(NULL);
	m_pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(FONT2DVERTEX));

	if (dwFlags & D3DFONT_FILTERED)
	{
		m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	}

	// Adjust for character spacing
	//sx -= m_dwSpacing;
	float startX = sx;

	// Fill vertex buffer
	FONT2DVERTEX* vertices = NULL;
	int trianglesCount = 0;
	m_pVB->Lock(0, 0, (void**)&vertices, D3DLOCK_DISCARD);

	auto stringLength = lstrlenW(strText);
	DWORD customColor = dwColor;

	for (int i = 0; i < stringLength; i++)
	{
		wchar_t c = strText[i];

		if (c == L'{')
		{
			int endIndex = 0;
			bool valid = true;
			std::wstring numericString = L"";
			for (int j = i + 1; j < stringLength; j++)
			{
				wchar_t ch = strText[j];
				ch = towupper(ch);

				if (ch == '}')
				{
					endIndex = j;
					break;
				}

				bool numeric = (ch >= L'0' && ch <= L'9') || (ch >= L'A' && ch <= L'F');
				if (!numeric)
				{
					valid = false;
					break;
				}
				else
				{
					numericString += ch;
					if (numericString.size() > 8)
					{
						valid = false;
						break;
					}
				}
			}

			if (endIndex > 0 && valid)
			{
				swscanf_s(numericString.data(), L"%X", &customColor);
				customColor |= (dwColor >> 24) << 24;
				i = endIndex;
				continue;
			}
		}


		if (c == '\n')
		{
			sx = startX;
			sy += m_textureSize[L' '].cy;
			continue;
		}

		if (c < 0 || c > USHRT_MAX)
			continue;

		auto characterTexture = GetCharacterTexture(c);
		if (characterTexture == nullptr)
			return S_FALSE;

		float w = (float)m_textureSize[c].cx;
		float h = (float)m_textureSize[c].cy;

		if (c != L' ')
		{
			if (dwFlags & D3DFONT_COLORTABLE)
				dwColor = customColor;

			*vertices++ = InitFont2DVertex(D3DXVECTOR4(sx - 0.5f, sy - 0.5f, 0.9f, 1.0f), dwColor, 0, 0);
			*vertices++ = InitFont2DVertex(D3DXVECTOR4(sx - 0.5f + w, sy - 0.5f, 0.9f, 1.0f), dwColor, 1.0, 0.0);
			*vertices++ = InitFont2DVertex(D3DXVECTOR4(sx - 0.5f + w, sy - 0.5f + h, 0.9f, 1.0f), dwColor, 1.0, 1.0);
			*vertices++ = InitFont2DVertex(D3DXVECTOR4(sx  - 0.5f, sy - 0.5f + h, 0.9f, 1.0f), dwColor, 0.0, 1.0);
			trianglesCount += 2;

			// We need to draw the text everytime, because the texture changed
			// Unlock, render, and relock the vertex buffer
			m_pVB->Unlock();

			m_pd3dDevice->SetTexture(0, characterTexture);

			m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, trianglesCount);
			vertices = NULL;
			m_pVB->Lock(0, 0, (void**)&vertices, D3DLOCK_DISCARD);
			trianglesCount = 0L;

			m_pd3dDevice->SetTexture(0, NULL);
		}



		sx += w;
	}

	// Unlock and render the vertex buffer
	m_pVB->Unlock();
	if (trianglesCount > 0)
		m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, trianglesCount);

	// Restore the modified renderstates
	m_pStateBlockSaved->Apply();

	return S_OK;
}