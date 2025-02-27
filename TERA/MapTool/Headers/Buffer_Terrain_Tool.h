#pragma once
#include "VIBuffer.h"

class CBuffer_Terrain_Tool final : public CVIBuffer
{
private:
	explicit CBuffer_Terrain_Tool(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CBuffer_Terrain_Tool() = default;
public:
	virtual HRESULT Ready_VIBuffer();
	virtual HRESULT Ready_VIBuffer(const _tchar* pHeighitMapPath);
	virtual void	Render_Buffer(const CTransform* pTransform = nullptr);
	virtual _float	Compute_HeightOnBuffer(const CTransform* pTransform);
//public:	// Getter
//	D3DFORMAT		GetD3DFormat() { return m_Format; }
//	_uint			GetNumPolygons() { return m_iNumPolygons; }
//	_vec3*			GetPositionPointer() { return m_pPositions; }
//	void*			GetIndicesPointer() { return m_pIndices; }
public:
	HRESULT			Reset_Terrain(_uint _iNumVtxX, _uint _iNumVtxZ, _float _fInterval, _float _fDetail);
private:
	void			ComputeNormal(_vec3* pVtx0, _vec3* pVtx1, _vec3* pVtx2, _vec3* pOut);
private:
	_uint			m_iNumVerticesX = 100;
	_uint			m_iNumVerticesZ = 100;
	_float			m_fInterval = 1.f;
	_float			m_fDetail = 1.f;
public:
	static CBuffer_Terrain_Tool* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	static CBuffer_Terrain_Tool* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pHeighitMapPath);

	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void		Free();
};

