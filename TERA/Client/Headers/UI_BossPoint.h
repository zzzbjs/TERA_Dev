#pragma once
#include "UI.h"

_BEGIN(Client)

class CUI_BossPoint final : public CUI
{
private:
	explicit CUI_BossPoint(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI_BossPoint(const CUI_BossPoint& rhs);
	virtual ~CUI_BossPoint() = default;

public: // Geter

public:	// Setter
	void	Set_CutSceneStart(_bool bButton) { m_bIsCutSceneStart = bButton; }

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Render_GameObject();

private:
	virtual HRESULT	Add_Component();
	virtual HRESULT	SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _uint iTargetTextureIdx = 1);

private:
	CTransform*			m_pTransformPointBoardCom = nullptr;
	CBuffer_RcTex*		m_pBufferPointBoardCom = nullptr;
	CTexture*			m_pTexturePointBoardCom = nullptr;

	CTransform*			m_pTransformHpCom = nullptr;
	CBuffer_RcTex*		m_pBufferHpCom = nullptr;
	CTexture*			m_pTextureHpCom = nullptr;

private:
	HRESULT				NullCheck();
	void				CutSceneEvent();
	void				PointCalculater(_float fCurrentValue);

private:
	_float				m_fTimeDelta = 1.f;

	_bool				m_bIsCutSceneStart = false;
	_bool				m_bIsHpRender = false;

	_float				m_fHP = 100000.f;
	_float				m_fCalculatedHP = 0.f;
	_float				m_fHPRatio = 0.f;

	_tchar				m_szHP[64];

public:
	static CUI_BossPoint*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*		Clone(void * pArg = nullptr) override;
	virtual void				Free();

};

_END