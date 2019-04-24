#pragma once

namespace Engine
{
	typedef struct tagVertex_Cube_Texture
	{
		D3DXVECTOR3			vPosition; // D3DFVF_XYZ
		D3DXVECTOR3			vTexUV; // D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0)
	}VTXCUBETEX;

	typedef struct tagVertex_ViewPort
	{
		D3DXVECTOR4			vPosition; // D3DFVF_XYZRHW
		D3DXVECTOR2			vTexUV; // D3DFVF_TEX1
	}VTXVIEWPORT;

	typedef struct tagVertex_Texture
	{
		D3DXVECTOR3			vPosition; // D3DFVF_XYZ
		D3DXVECTOR2			vTexUV; // D3DFVF_TEX1

	}VTXTEX;

	typedef struct tagVertex_Color
	{
		D3DXVECTOR3			vPosition; // D3DFVF_XYZ
		unsigned long		dwColor; // D3DFVF_DIFFUSE	
	}VTXCOL;

	typedef struct tagIndex16
	{
		unsigned short	_1, _2, _3;
	}INDEX16;

	typedef struct tagIndex32
	{
		unsigned long	_1, _2, _3;
	}INDEX32;
}

