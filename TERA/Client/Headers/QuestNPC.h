#pragma once
#include "Unit.h"
#include "Defines.h"
#include "GameObject.h"

_BEGIN(Client)

class CQuestNPC : public CUnit
{
private:
	explicit CQuestNPC(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CQuestNPC(const CQuestNPC& rhs);
	virtual ~CQuestNPC() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Render_GameObject();

public:
	virtual HRESULT Add_Component();
	virtual HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	void			CollisionCheck();
	void			PlayerInside(const CCollider* _PlayerCollider);
	void			TalkEvent(const CCollider* _PlayerCollider);
	void			LookAtPlayer();
	void			TalkWithPlayer();
	void			ScriptInfo();

private:
	CCollider*		m_pColliderEventCom = nullptr;

private:
	_bool			m_bIsPlayerInside = false;
	_uint			m_iEventArgValue = 0;
	_bool			m_bPlayerRenderingFirst = true;
	_bool			m_bIsEventCollisionFirst = true;
	_bool			m_bIsTalking = false;
	_tchar*			m_pMainScript[10];
	_tchar*			m_pReplyScript[10];
	_int			m_iScriptNumber = -1;

public:
	static CQuestNPC*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg = nullptr);
	virtual void			Free();
};

_END