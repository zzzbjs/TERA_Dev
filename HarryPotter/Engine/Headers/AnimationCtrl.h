#pragma once
#include "Base.h"

_BEGIN(Engine)

class CAnimationCtrl final : public CBase
{
private:
	explicit CAnimationCtrl(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	explicit CAnimationCtrl(const CAnimationCtrl& rhs);
	virtual ~CAnimationCtrl() = default;
public: // Getter
	_double	Get_CurretTrackPos();
public:
	HRESULT Ready_AnimationCtrl();
	HRESULT SetUp_AnimationSet(_uint iIndex, const _float fAniSpeed = 1.f);
	HRESULT SetUp_AnimationSet(const char* pName);
	HRESULT Play_Animation(const _float& fTimeDelta);
public:
	_bool	IsAnimationEnded(_double fCtrlEndTime = 0.f);
private:
	LPD3DXANIMATIONSET			m_pAnimationSet = nullptr;
	LPD3DXANIMATIONCONTROLLER	m_pAniCtrl = nullptr;
	_uint						m_iCurrentTrack;
	_uint						m_iNewTrack;
	_uint						m_iOldIndex = 0;
	_double						m_TimeAcc = 0.0;
	_float						m_fAniSpeed = 1.f;
public:
	static CAnimationCtrl*	Create(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	static CAnimationCtrl*	Create(const CAnimationCtrl& rhs);
	virtual void			Free();
};

_END