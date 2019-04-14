#pragma once

#include "VIBuffer.h"

_BEGIN(Engine)

class _DLL_EXPORTS CBuffer_TriCol final : public CVIBuffer
{
private:
	explicit CBuffer_TriCol(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBuffer_TriCol(const CBuffer_TriCol& rhs);
	virtual ~CBuffer_TriCol() = default;
public:
	virtual HRESULT Ready_VIBuffer();
	virtual void	Render_Buffer();


public:
	static CBuffer_TriCol*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent*		Clone();
	virtual void			Free();
};

_END