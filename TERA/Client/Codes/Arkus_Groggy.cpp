#include "stdafx.h"
#include "..\Headers\Arkus_Groggy.h"
#include "Arkus.h"
#include "Input_Device.h"

#include "Arkus_Idle.h"
#include "Arkus_AlmostDead.h"
#include "Arkus_Death.h"

_USING(Client)

CArkus_Groggy::CArkus_Groggy(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CArkusState(pGraphic_Device)
{
}

HRESULT CArkus_Groggy::Initialize_State(CArkus & Arkus)
{
	Arkus.Set_AniIndex(CArkus::ARKUS_ANI::Groggy);

	return NOERROR;
}

CArkusState * CArkus_Groggy::Input_State(CArkus & Arkus, const float & fTimeDelta, BYTE KeyID, void * pArg)
{
	if (Arkus.Get_Mesh()->Get_NowPlayAniIndex() == CArkus::ARKUS_ANI::Groggy)
	{
		if (Arkus.Get_HP() <= 0.f)
			return CArkus_Death::Create(m_pGraphic_Device, Arkus, &m_iAniState);

		MoveArkusPosition(Arkus, 25.f, fTimeDelta, pArg, 0);

		if (Arkus.Get_Mesh()->IsAnimationEnded(0.75f))
		{
			if(Arkus.Get_HP() >= 0.f)
				return CArkus_AlmostDead::Create(m_pGraphic_Device, Arkus, &m_iAniState);
		}
	}
	return nullptr;
}

void CArkus_Groggy::Update_State(CArkus & Arkus, const float & fTimeDelta)
{
	Arkus.ViewChanage();
}

void CArkus_Groggy::MoveArkusPosition(CArkus & Arkus, const _float fArkusSpeed, const _float & fTimeDelta, void * pArg, _int iMoveDir)
{
	_uint		iCellIndx = 0;
	if (true == ((CNavigation*)(pArg))->Move_OnNavigation(Arkus.Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION), Arkus.Get_Transform()->Get_StateInfo(CTransform::STATE_LOOK), 30.0f * fTimeDelta, &iCellIndx))
	{
		_vec3 vPlayerPos, vDir;
		vPlayerPos = *dynamic_cast<CPlayer*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Player"))->Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION);
		vDir = (vPlayerPos - *Arkus.Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION)) * -1;

		Arkus.Get_Transform()->Move(&vDir, fArkusSpeed, fTimeDelta);

		/* ※※※※※※※진짜 이동하면 꼭 호출해야합니다※※※※※※.*/
		((CNavigation*)(pArg))->SetUp_CurrentIndex(iCellIndx);
	}
}

CArkus_Groggy * CArkus_Groggy::Create(LPDIRECT3DDEVICE9 pGraphicDevice, CArkus & Arkus, void * pArg)
{
	CArkus_Groggy* pInstance = new CArkus_Groggy(pGraphicDevice);

	if (FAILED(pInstance->Initialize_State(Arkus)))
		Safe_Release(pInstance);

	return pInstance;
}

void CArkus_Groggy::Free()
{
	CArkusState::Free();
}
