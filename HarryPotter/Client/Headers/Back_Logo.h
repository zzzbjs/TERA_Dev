#pragma once

#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
class CBuffer_TriCol;	// �׸��⸦ ���� ���ҽ��� ���� ���۸� �߰��Ѵ�.
class CRenderer;		// ��ΰ��� �����׷쿡 �߰��ϱ� ����. && Render�Լ��� ȣ���� �� �ֵ��� 
_END

_BEGIN(Client)

class CBack_Logo final : public CGameObject
{
private:
	explicit CBack_Logo(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBack_Logo(const CBack_Logo& rhs);
	virtual ~CBack_Logo() = default;
public:
	virtual HRESULT	Ready_GameObject_Prototype();
	virtual HRESULT	Ready_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Render_GameObject();
private:
	// ��� �� ������Ʈ ��ü���� ��� ������ ������ �ִ�.
	CBuffer_TriCol*	m_pBufferCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
private:
	HRESULT			Add_Component();
public:
	static CBack_Logo*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone();
	virtual void			Free();
};

_END