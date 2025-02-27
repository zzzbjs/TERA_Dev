#include "stdafx.h"
#include "..\Headers\Arkus_Idle.h"
#include "Arkus.h"
#include "Input_Device.h"
#include "Time.h"

#include "Arkus_Run.h"
#include "Arkus_Attack.h"
#include "Arkus_Hit.h"
#include "Arkus_Groggy.h"
#include "Arkus_AlmostDead.h"
#include "Arkus_Death.h"

_USING(Client)

CArkus_Idle::CArkus_Idle(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CArkusState(pGraphic_Device)
{
}

HRESULT CArkus_Idle::Initialize_State(CArkus & Arkus)
{
	Arkus.Set_AniIndex(CArkus::ARKUS_ANI::Idle);

	return NOERROR;
}

CArkusState * CArkus_Idle::Input_State(CArkus & Arkus, const float & fTimeDelta, BYTE KeyID, void * pAgr)
{
	if (Arkus.Get_Mesh()->Get_NowPlayAniIndex() == CArkus::ARKUS_ANI::Idle)
	{
		//if (Arkus.Get_PlayerFrontInfo() == false)
		//{
		//	if (Arkus.Get_TurnRightInfo() == true)
		//	{
		//		m_iAniState = 1;
		//		return CArkus_Attack::Create(m_pGraphic_Device, Arkus, &m_iAniState);
		//	}
		//	else if (Arkus.Get_TurnRightInfo() == false)
		//	{
		//		m_iAniState = 2;
		//		return CArkus_Attack::Create(m_pGraphic_Device, Arkus, &m_iAniState);
		//	}
		//}

		// m_bIsDamageAvailable �ʱ�ȭ //
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Player"));
		AttackEventFree(pPlayer, &Arkus);

//		if (Arkus.Get_PlayerFrontInfo() == true)
//		{
			if (Arkus.Get_HP() <= 3000.f)
			{
				if (Arkus.Get_HP() <= 0.f)
					return CArkus_Death::Create(m_pGraphic_Device, Arkus, &m_iAniState);
				else
					return CArkus_AlmostDead::Create(m_pGraphic_Device, Arkus, &m_iAniState);
			}
			if (Arkus.CollisionCheck() == true)
			{
				//if (Arkus.Get_Mesh()->IsAnimationEnded(0.15f))
				//	return CArkus_Hit::Create(m_pGraphic_Device, Arkus, &m_iAniState);
			}

			if (Arkus.Get_CollisionPartCheck(Arkus.COLL_ATTACK_AREA) == false)
			{
				if (Arkus.Get_Mesh()->IsAnimationEnded(0.6f))
					return CArkus_Run::Create(m_pGraphic_Device, Arkus, &m_iAniState);
			}
			else if (Arkus.Get_CollisionPartCheck(Arkus.COLL_ATTACK_AREA) == true)
			{
				if (Arkus.Get_Mesh()->IsAnimationEnded(0.6f))
				{
					srand((unsigned)time(NULL));

					m_iAniState = rand() % 5 + 4;

					//if (m_iAniState == 8)
					//{
					//	if (Arkus.Get_PlayerFrontInfo() == false)
					//		m_iAniState = rand() % 5 + 4;
					//}
					m_iAniState = 1;
					return CArkus_Idle::Create(m_pGraphic_Device, Arkus, &m_iAniState);
				//	return CArkus_Attack::Create(m_pGraphic_Device, Arkus, &m_iAniState);
				}
			}
			else
				return nullptr;
//		}		
	}

	return nullptr;
}

void CArkus_Idle::Update_State(CArkus & Arkus, const float & fTimeDelta)
{
	//if (Arkus.Get_PlayerFrontInfo() == true)
	//	Arkus.ViewChanage();
	
//	Arkus.ViewChanage();
	Arkus.EnemyPositionCheck();

}

CArkus_Idle * CArkus_Idle::Create(LPDIRECT3DDEVICE9 pGraphicDevice, CArkus & Arkus, void * pArg)
{
	CArkus_Idle* pInstance = new CArkus_Idle(pGraphicDevice);

	if (FAILED(pInstance->Initialize_State(Arkus)))
		Safe_Release(pInstance);

	return pInstance;
}

void CArkus_Idle::Free()
{
	CArkusState::Free();
}
