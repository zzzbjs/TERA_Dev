#include "stdafx.h"
#include "..\Headers\Arkus_Attack.h"
#include "Arkus.h"
#include "Input_Device.h"
#include "Camera_Static.h"

#include "Arkus_Idle.h"
#include "Arkus_Rush.h"

_USING(Client)

CArkus_Attack::CArkus_Attack(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CArkusState(pGraphic_Device)
{
}

HRESULT CArkus_Attack::Initialize_State(CArkus & Arkus)
{
	Arkus.Set_ActionID(CArkus::ACTION_ID::ACTION_ATTACK);

	return NOERROR;
}

CArkusState * CArkus_Attack::Input_State(CArkus & Arkus, const float & fTimeDelta, BYTE KeyID, void * pArg)
{
	CCamera_Static* pCamera_Static = dynamic_cast<CCamera_Static*>(CObject_Manager::GetInstance()->Get_Object(SCENE_DRAGON, L"Layer_Camera", 1));

	if (Arkus.Get_Mesh()->Get_NowPlayAniIndex() == CArkus::ARKUS_ANI::RoundAtk01)
	{
		if (Arkus.Get_Mesh()->IsAnimationEnded(0.85f))
			return CArkus_Idle::Create(m_pGraphic_Device, Arkus, &m_iAniState);
	}
	else if (Arkus.Get_Mesh()->Get_NowPlayAniIndex() == CArkus::ARKUS_ANI::RoundAtk02)
	{
		if (Arkus.Get_Mesh()->IsAnimationEnded(0.85f))
			return CArkus_Idle::Create(m_pGraphic_Device, Arkus, &m_iAniState);
	}
	if (Arkus.Get_Mesh()->Get_NowPlayAniIndex() == CArkus::ARKUS_ANI::Atk01)
	{
		if (Arkus.Get_Mesh()->IsAnimationEnded(0.85f))
			return CArkus_Idle::Create(m_pGraphic_Device, Arkus, &m_iAniState);
	}
	else if (Arkus.Get_Mesh()->Get_NowPlayAniIndex() == CArkus::ARKUS_ANI::HeavyAtk02)
	{
		if (Arkus.Get_Mesh()->IsAnimationEnded(0.85f))
			return CArkus_Idle::Create(m_pGraphic_Device, Arkus, &m_iAniState);
	}
	else if (Arkus.Get_Mesh()->Get_NowPlayAniIndex() == CArkus::ARKUS_ANI::FlyAtk01)
	{
		// ī�޶� ����.
		if (pCamera_Static->Get_CameraModInfo() == true)
		{
			if (pCamera_Static->Get_CemeraDistance() < 130.f &&
				pCamera_Static->Get_CameraHeightValue() < 45.f)
			{
				pCamera_Static->Set_CameraDistance(pCamera_Static->Get_CemeraDistance() + 30.f * fTimeDelta);
				pCamera_Static->Set_CameraHeightValue(pCamera_Static->Get_CameraHeightValue() + 10.f * fTimeDelta);
			}
			// ���� ���� ����� ������ ��� �Ѵ�.
			else if (pCamera_Static->Get_CemeraDistance() >= 125.f &&
				pCamera_Static->Get_CameraHeightValue() >= 35.f)
			{
				// ���ϴ� ��ġ���� ī�޶� ������ �Ϸ��ϸ�,
				// ���� ���� ī�޶� ��ġ�� ���ư��� ���� �� ������ �ش�.
				// ������� ���ư����� ��� ���� => m_bCameraModOn�� false��� ���̴�.
				pCamera_Static->Set_CameraModInfo(false);
			}
		}
		// m_bCameraModOn�� false�̹Ƿ� ���� ��ġ�� ī�޶� ���ư� �ð��� �Ǿ��ٴ� ���̴�.
		else if (pCamera_Static->Get_CameraModInfo() == false)
		{
			// ī�޶� �ָ� �� �ƿ� �ߴٰ� �ٷ� ���� �ع�����,
			// ������ �ִϸ��̼ǰ� ��ġ�� �ľ��� �� ���� ������,
			// �ؿ��� IsAnimationEnded(0.8f)�� ���� �ð��� �帥 �ڿ�,
			// ī�޶� ���� ��ġ�� ������ �;� �� ������ �� �ϳ� �ش�.
			if(pCamera_Static->Get_CameraBackTimeInfo() == true)
			{
				// ���ư� �غ� ��� �������ٸ�, �ʱ� �� ��ŭ�� ���ư��� �Ѵ�.
				if (pCamera_Static->Get_CemeraDistance() >= 30.f &&
					pCamera_Static->Get_CameraHeightValue() >= 10.f)
				{
					pCamera_Static->Set_CameraDistance(pCamera_Static->Get_CemeraDistance() - 30.f * fTimeDelta);
					pCamera_Static->Set_CameraHeightValue(pCamera_Static->Get_CameraHeightValue() - 10.f * fTimeDelta);
				}
			}
		}
		// �ִϸ��̼��� �� ���� �ð� ��ô�� �Ǿ��ٸ�, 
		// ���ư� �غ� ��� �Ϸ� �Ǿ���.
		if (Arkus.Get_Mesh()->IsAnimationEnded(0.7f))
			pCamera_Static->Set_CameraBackTimeInfo(true);

		if (Arkus.Get_Mesh()->IsAnimationEnded(0.9f))
		{
			// �ִϸ��̼��� ����� ��
			if (pCamera_Static->Get_CameraModInfo() == false)
			{
				// ��� �� ���� ���� �ʱ� ������ �ǵ��� �ش�.
				pCamera_Static->Set_CameraModInfo(true);
				pCamera_Static->Set_CameraBackTimeInfo(false);

				pCamera_Static->Set_CameraDistance(30.f);
				pCamera_Static->Set_CameraHeightValue(10.f);

				return CArkus_Idle::Create(m_pGraphic_Device, Arkus, &m_iAniState);
			}
		}
	}
	else if (Arkus.Get_Mesh()->Get_NowPlayAniIndex() == CArkus::ARKUS_ANI::FlyAtk02Start)
	{
		if (Arkus.Get_Mesh()->IsAnimationEnded(0.85f))
		{
			m_iAniState = 7;
			return CArkus_Attack::Create(m_pGraphic_Device, Arkus, &m_iAniState);
		}
	}
	else if (Arkus.Get_Mesh()->Get_NowPlayAniIndex() == CArkus::ARKUS_ANI::FlyAtk02End)
	{
		MovePlayerPosition(Arkus, 40.f, fTimeDelta, pArg, 0);

		if (Arkus.Get_Mesh()->IsAnimationEnded(0.85f))
		{
			return CArkus_Idle::Create(m_pGraphic_Device, Arkus, &m_iAniState);
		}
	}
	else if (Arkus.Get_Mesh()->Get_NowPlayAniIndex() == CArkus::ARKUS_ANI::MoveAtkStart)
	{
		m_iAniState = 1;
		return CArkus_Rush::Create(m_pGraphic_Device, Arkus, &m_iAniState);
	}

	return nullptr;
}

void CArkus_Attack::Update_State(CArkus & Arkus, const float & fTimeDelta)
{
}

void CArkus_Attack::MovePlayerPosition(CArkus & Arkus, const _float fArkusSpeed, const _float & fTimeDelta, void * pArg, _int iMoveDir)
{
	_uint		iCellIndx = 0;
	if (true == ((CNavigation*)(pArg))->Move_OnNavigation(Arkus.Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION), Arkus.Get_Transform()->Get_StateInfo(CTransform::STATE_LOOK), 30.0f * fTimeDelta, &iCellIndx))
	{
		_vec3 vPlayerPos, vDir;
		vPlayerPos = *dynamic_cast<CPlayer*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Player"))->Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION);
		vDir = vPlayerPos - *Arkus.Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION);

		Arkus.Get_Transform()->Move(&vDir, fArkusSpeed, fTimeDelta);

		/* �ءءءءءء���¥ �̵��ϸ� �� ȣ���ؾ��մϴ١ءءءءء�.*/
		((CNavigation*)(pArg))->SetUp_CurrentIndex(iCellIndx);
	}
}

CArkus_Attack * CArkus_Attack::Create(LPDIRECT3DDEVICE9 pGraphicDevice, CArkus & Arkus, void * pArg)
{
	CArkus_Attack* pInstance = new CArkus_Attack(pGraphicDevice);

	if (*(_int*)(pArg) == 1)
		Arkus.Set_AniIndex(CArkus::ARKUS_ANI::RoundAtk01);

	else if ((*(_int*)(pArg) == 2))
		Arkus.Set_AniIndex(CArkus::ARKUS_ANI::RoundAtk02);

	else if ((*(_int*)(pArg) == 3))
		Arkus.Set_AniIndex(CArkus::ARKUS_ANI::Atk01);

	else if ((*(_int*)(pArg) == 4))
		Arkus.Set_AniIndex(CArkus::ARKUS_ANI::HeavyAtk02);

	else if ((*(_int*)(pArg) == 5))
		Arkus.Set_AniIndex(CArkus::ARKUS_ANI::FlyAtk01);

	else if ((*(_int*)(pArg) == 6))
		Arkus.Set_AniIndex(CArkus::ARKUS_ANI::FlyAtk02Start);

	else if ((*(_int*)(pArg) == 7))
		Arkus.Set_AniIndex(CArkus::ARKUS_ANI::FlyAtk02Start);

	else if ((*(_int*)(pArg) == 8))
		Arkus.Set_AniIndex(CArkus::ARKUS_ANI::MoveAtkStart);

	if (FAILED(pInstance->Initialize_State(Arkus)))
		Safe_Release(pInstance);

	return pInstance;
}

void CArkus_Attack::Free()
{
	CArkusState::Free();
}