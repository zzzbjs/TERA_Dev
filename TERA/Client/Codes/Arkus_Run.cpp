#include "stdafx.h"
#include "..\Headers\Arkus_Run.h"
#include "Arkus.h"
#include "Input_Device.h"
#include "Time.h"

#include "Arkus_Idle.h"
#include "Arkus_Attack.h"
#include "Arkus_Hit.h"
#include "Arkus_Groggy.h"
#include "Arkus_AlmostDead.h"
#include "Arkus_Death.h"

_USING(Client)

CArkus_Run::CArkus_Run(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CArkusState(pGraphic_Device)
{
}

HRESULT CArkus_Run::Initialize_State(CArkus & Arkus)
{
	Arkus.Set_AniIndex(CArkus::ARKUS_ANI::Run_Battle);

	return NOERROR;
}

CArkusState * CArkus_Run::Input_State(CArkus & Arkus, const float & fTimeDelta, BYTE KeyID, void * pArg)
{
	if (Arkus.Get_Mesh()->Get_NowPlayAniIndex() == CArkus::ARKUS_ANI::Run_Battle)
	{
		MoveArkusPosition(Arkus, 40.f, fTimeDelta, pArg, 0);

		srand((unsigned)time(NULL));

		if (Arkus.Get_Mesh()->IsAnimationEnded(0.85f))
		{
			if (Arkus.Get_CollisionPartCheck(Arkus.COLL_ATTACK_AREA) == true)
			{
				m_iAniState = rand() % 5 + 4;
				return CArkus_Attack::Create(m_pGraphic_Device, Arkus, &m_iAniState);
			//	return CArkus_Idle::Create(m_pGraphic_Device, Arkus, &m_iAniState);
			}
		}
		else
			return nullptr;
	}

	return nullptr;
}

void CArkus_Run::Update_State(CArkus & Arkus, const float & fTimeDelta)
{
}

void CArkus_Run::MoveArkusPosition(CArkus & Arkus, const _float fArkusSpeed, const _float & fTimeDelta, void * pArg, _int iMoveDir)
{
	_uint		iCellIndx = 0;
	if (true == ((CNavigation*)(pArg))->Move_OnNavigation(Arkus.Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION), Arkus.Get_Transform()->Get_StateInfo(CTransform::STATE_LOOK), 30.0f * fTimeDelta, &iCellIndx))
	{
		_vec3 vPlayerPos, vDir;
		vPlayerPos = *dynamic_cast<CPlayer*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Player"))->Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION);
		vDir = vPlayerPos - *Arkus.Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION);

		Arkus.Get_Transform()->Move(&vDir, fArkusSpeed, fTimeDelta);

		/* ※※※※※※※진짜 이동하면 꼭 호출해야합니다※※※※※※.*/
		((CNavigation*)(pArg))->SetUp_CurrentIndex(iCellIndx);
	}
}

CArkus_Run * CArkus_Run::Create(LPDIRECT3DDEVICE9 pGraphicDevice, CArkus & Arkus, void * pArg)
{
	CArkus_Run* pInstance = new CArkus_Run(pGraphicDevice);

	if (FAILED(pInstance->Initialize_State(Arkus)))
		Safe_Release(pInstance);

	return pInstance;
}

void CArkus_Run::Free()
{
	CArkusState::Free();
}
