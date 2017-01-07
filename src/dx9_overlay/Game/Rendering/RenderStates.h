#pragma once
#include <d3dx9.h>

class RenderStates
{
public:
	RenderStates(IDirect3DDevice9 *pDevice);
	~RenderStates();

	void BeginDraw(IDirect3DDevice9 *pDevice);
	void EndDraw(IDirect3DDevice9 *pDevice);
private:
	LPDIRECT3DSTATEBLOCK9 m_pStateBlockSaved;
	LPDIRECT3DSTATEBLOCK9 m_pStateBlockDraw;

	void Initialize(IDirect3DDevice9 *pDevice);
};
