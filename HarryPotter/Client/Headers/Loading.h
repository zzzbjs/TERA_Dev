#pragma once
#include "Defines.h"
#include "Base.h"

_BEGIN(Engine)
class CComponent_Manager;
_END

_BEGIN(Client)

class CLoading final : public CBase
{
private:
	explicit CLoading(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLoading() = default;
public:
	SCENEID Get_SceneID() const {
		return m_eSceneID;
	}
	_uint Get_Complete() const {
		return m_iComplete;
	}
public:
	HRESULT Ready_Loading(SCENEID eSceneID);
public:
	static _uint APIENTRY Thread_Main(void* pArg);
private:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;
	SCENEID					m_eSceneID;
	HANDLE					m_hThread;
	CRITICAL_SECTION		m_CS;
	CComponent_Manager*		m_pComponent_Manager = nullptr;
	_uint					m_iComplete;
private:
	HRESULT Ready_Stage_Component();

public:
	static CLoading* Create(LPDIRECT3DDEVICE9 pGraphic_Device, SCENEID eSceneID);
	virtual void Free();
};

_END