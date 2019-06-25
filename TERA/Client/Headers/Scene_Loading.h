#pragma once
#include "Defines.h"
#include "Scene.h"

_BEGIN(Client)

class CLoading;
class CScene_Loading final : public CScene
{
public:
	explicit CScene_Loading(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CScene_Loading() = default;

public:
	virtual HRESULT Ready_Scene();
	virtual _int	Update_Scene(const _float& fTimeDelta);
	virtual _int	LateUpdate_Scene(const _float& fTimeDelta);
	virtual HRESULT Render_Scene();

private:
	CLoading*		m_pLoading = nullptr;

private:
	HRESULT Ready_Component_Prototype();
	HRESULT Ready_GameObject_Prototype();
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);

public:
	static CScene_Loading*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void			Free();

};

_END