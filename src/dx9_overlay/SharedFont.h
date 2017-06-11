#pragma once

#include <D3D9.h>
#include <string>

class SharedFont
{
public:
	SharedFont(const std::wstring &fontName, DWORD height, DWORD flags);
	~SharedFont();

	void AddReference();
	bool RemoveReference();

	bool Compare(const std::wstring &fontName, DWORD height, DWORD flags);

	LPDIRECT3DTEXTURE9 GetCharacterTexture(LPDIRECT3DDEVICE9 device, USHORT index);
	SIZE GetCharacterSize(LPDIRECT3DDEVICE9 device, USHORT index);
private:
	int m_referenceCount;

	std::wstring m_fontName;
	DWORD m_height;
	DWORD m_flags;

	HFONT m_font;
	LPDIRECT3DTEXTURE9 m_textures[USHRT_MAX];
	SIZE m_textureSize[USHRT_MAX];

	void Initialize();
	void Cleanup();
};

