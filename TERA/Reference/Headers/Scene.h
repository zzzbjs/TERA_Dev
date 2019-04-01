#pragma once

// Ŭ���̾�Ʈ���� ������ ������ �θ� �Ǵ� Ŭ����.
#include "Base.h"
#include "Object_Manager.h"

_BEGIN(Engine)

class CGameObject;
class _DLL_EXPORTS CScene : public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CScene() = default;
public:
	virtual HRESULT Ready_Scene();
	virtual _int	Update_Scene(const _float& fTimeDelta);
	// ���� ���� Updateȣ�� ���� ���������� �ѹ� �� ȣ��Ǵ� Update
	// ���ȿ��� ��ü���� ������Ʈ ȣ��. ��� ��ü���� ������Ʈ�� ȣ���� ��, �ٽ� �ѹ� ��ü���� ������Ʈ�� ȣ��.
	virtual _int	LateUpdate_Scene(const _float& fTimeDelta);
	virtual HRESULT Render_Scene();
protected:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;
	CObject_Manager*		m_pObject_Manager = nullptr;
protected:
	HRESULT Add_Object_Prototype(const _uint& iSceneIdx, const _tchar* pProtoTag, CGameObject* pGameObject);
public:
	virtual void Free();
};

_END