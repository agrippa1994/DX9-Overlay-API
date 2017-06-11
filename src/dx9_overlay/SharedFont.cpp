#include "SharedFont.h"

SharedFont::SharedFont(const std::wstring &fontName, DWORD height, DWORD flags)
{
	m_fontName = fontName;
	m_height = height;
	m_flags = flags;

	Initialize();
}

SharedFont::~SharedFont()
{
	Cleanup();
}

void SharedFont::Initialize()
{
	m_referenceCount = 0;

	HDC hDC = CreateCompatibleDC(NULL);

	// Create a font.  By specifying ANTIALIASED_QUALITY, we might get an
	// antialiased font, but this is not guaranteed.
	INT nHeight = -MulDiv(m_height,
		(INT)(GetDeviceCaps(hDC, LOGPIXELSY)), 72);
	DWORD dwBold = (m_flags & 0x0001) ? FW_BOLD : FW_NORMAL;
	DWORD dwItalic = (m_flags & 0x0002) ? TRUE : FALSE;
	m_font = CreateFontW(nHeight, 0, 0, 0, dwBold, dwItalic,
		FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
		VARIABLE_PITCH, m_fontName.c_str());

	DeleteDC(hDC);

	for (size_t i = 0; i < ARRAYSIZE(m_textures); i++)
		m_textures[i] = nullptr;

	for (size_t i = 0; i < ARRAYSIZE(m_textureSize); i++)
		m_textureSize[i] = { 0, 0 };
}

void SharedFont::Cleanup()
{
	for (size_t i = 0; i < ARRAYSIZE(m_textures); i++)
	{
		if (m_textures[i] != nullptr)
		{
			m_textures[i]->Release();
			m_textures[i] = nullptr;
		}
	}

	if (m_font == NULL)
	{
		DeleteObject(m_font);
		m_font = NULL;
	}
}

void SharedFont::AddReference()
{
	m_referenceCount++;
}

bool SharedFont::RemoveReference()
{
	m_referenceCount--;
	if (m_referenceCount <= 0)
		return true;
	
	return false;
}

bool SharedFont::Compare(const std::wstring &fontName, DWORD height, DWORD flags)
{
	return m_fontName == fontName && m_height == height && m_flags == flags;
}

LPDIRECT3DTEXTURE9 SharedFont::GetCharacterTexture(LPDIRECT3DDEVICE9 device, USHORT index)
{
	if (device == nullptr || m_font == NULL)
		return nullptr;

	if (m_textures[index] == nullptr)
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
		HRESULT hr = device->CreateTexture(size.cx, size.cy, 1,
			0, D3DFMT_A4R4G4B4,
			D3DPOOL_MANAGED, &m_textures[index], NULL);

		if (FAILED(hr))
		{
			DeleteObject(hbmBitmap);
			DeleteDC(hDC);
			return nullptr;
		}

		// Lock the surface and write the alpha values for the set pixels
		D3DLOCKED_RECT d3dlr;
		m_textures[index]->LockRect(0, &d3dlr, 0, 0);
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

		m_textures[index]->UnlockRect(0);

		// Done updating texture, so clean up used objects
		DeleteObject(hbmBitmap);
		DeleteDC(hDC);
	}

	return m_textures[index];
}

SIZE SharedFont::GetCharacterSize(LPDIRECT3DDEVICE9 device, USHORT index)
{
	// Get texture to initialize also the size
	GetCharacterTexture(device, index);

	return m_textureSize[index];
}
