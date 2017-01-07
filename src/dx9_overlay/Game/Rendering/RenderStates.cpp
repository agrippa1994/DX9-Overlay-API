#include "RenderStates.h"



RenderStates::RenderStates(IDirect3DDevice9 *pDevice)
{
	if (pDevice)
		Initialize(pDevice);
	else
	{
		m_pStateBlockSaved = nullptr;
		m_pStateBlockDraw = nullptr;
	}
}


RenderStates::~RenderStates()
{
	// Delete states when exists
	if (m_pStateBlockSaved)
	{
		m_pStateBlockDraw->Release();
		m_pStateBlockDraw = nullptr;
	}

	if (m_pStateBlockDraw)
	{
		m_pStateBlockDraw->Release();
		m_pStateBlockDraw = nullptr;
	}
}

void RenderStates::BeginDraw(IDirect3DDevice9 * pDevice)
{
	if (m_pStateBlockSaved && m_pStateBlockDraw)
	{
		m_pStateBlockSaved->Capture();
		m_pStateBlockDraw->Apply();
	}
}

void RenderStates::EndDraw(IDirect3DDevice9 * pDevice)
{
	if (m_pStateBlockSaved && m_pStateBlockDraw)
		m_pStateBlockSaved->Apply();
}

void RenderStates::Initialize(IDirect3DDevice9 * pDevice)
{
	// Save states for correct rendering
	for (size_t i = 0; i < 2; i++)
	{
		pDevice->BeginStateBlock();

		pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0x08);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_CLIPPING, TRUE);
		pDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
		pDevice->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_COLORWRITEENABLE,
			D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN |
			D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);

		if (i == 0)
			pDevice->EndStateBlock(&m_pStateBlockSaved);
		else
			pDevice->EndStateBlock(&m_pStateBlockDraw);
	}

}
