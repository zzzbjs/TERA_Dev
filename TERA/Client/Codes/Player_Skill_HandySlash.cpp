#include "stdafx.h"
#include "..\Headers\Player_Skill_HandySlash.h"
#include "Player.h"
#include "Input_Device.h"

#include "Player_Hit.h"
#include "Player_Idle.h"
#include "Player_KnockDown.h"

_USING(Client)

CPlayer_Skill_HandySlash::CPlayer_Skill_HandySlash(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CPlayerState(pGraphic_Device)
{
}

HRESULT CPlayer_Skill_HandySlash::Initialize_State(CPlayer & Player)
{
	Player.Set_SoundCheckInfo(true);
	Player.Set_AniIndex(CPlayer::PLAYER_ANI::HandySlash);

	return NOERROR;
}

CPlayerState * CPlayer_Skill_HandySlash::Input_Keyboard(CPlayer & Player, const float & fTimeDelta, BYTE KeyID, void * pArg)
{
	if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::HandySlash)
	{
		CArkus*	pArkus = dynamic_cast<CArkus*>(CObject_Manager::GetInstance()->Get_Object(SCENE_DRAGON, L"Layer_Monster"));
		AttackAvailableCheck(pArkus, &Player);

		if (Player.Get_Mesh_Bone()->IsAnimationEnded(0.1f))
			SoundPlay(Player);

		if (Player.Get_Mesh_Bone()->IsAnimationEnded(0.2f))
			MovePlayerPosition(Player, 40.f, fTimeDelta, pArg, 0);

		if (Player.Get_Mesh_Bone()->IsAnimationEnded(0.5f))
			MovePlayerPosition(Player, -38.f, fTimeDelta, pArg, 0);

		if (Player.Get_Mesh_Bone()->IsAnimationEnded(0.4f))
			AttackEvent(pArkus, &Player, 1);

		if (Player.Get_Mesh_Bone()->IsAnimationEnded(0.9f))
		{
			if (Player.CollisionCheck() == true)
			{
				CArkus*	pArkus = dynamic_cast<CArkus*>(CObject_Manager::GetInstance()->Get_Object(SCENE_DRAGON, L"Layer_Monster"));

				if (pArkus->Get_AniIndex() == CArkus::ARKUS_ANI::RoundAtk01 ||
					pArkus->Get_AniIndex() == CArkus::ARKUS_ANI::RoundAtk02 ||
					pArkus->Get_AniIndex() == CArkus::ARKUS_ANI::FlyAtk02End ||
					pArkus->Get_AniIndex() == CArkus::ARKUS_ANI::MoveAtkEnd)
				{
					return CPlayer_KnockDown::Create(m_pGraphic_Device, Player, &m_iAniState);
				}
				else
					return CPlayer_Hit::Create(m_pGraphic_Device, Player, &m_iAniState);
			}
			else if (Player.CollisionCheck() == false)
			{
				AttackEventFree(&Player, CPlayer::PLAYER_ANI::HandySlash);
				m_iAniState = 2;
				return CPlayer_Idle::Create(m_pGraphic_Device, Player, &m_iAniState);
			}
		}
	}

	return nullptr;
}

void CPlayer_Skill_HandySlash::Update_State(CPlayer & Player, const float & fTimeDelta)
{

}

void CPlayer_Skill_HandySlash::MovePlayerPosition(CPlayer & Player, _float fPlayerSpeed, const float & fTimeDelta, void * pArg, _int iMoveDir)
{
	_uint		iCellIndx = 0;
	if (true == ((CNavigation*)(pArg))->Move_OnNavigation(Player.Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION), Player.Get_Transform()->Get_StateInfo(CTransform::STATE_LOOK), 30.0f * fTimeDelta, &iCellIndx))
	{
		Player.Get_TransformMove()->Move(Player.Get_Transform()->Get_StateInfo(CTransform::STATE_LOOK), fPlayerSpeed, fTimeDelta);

		/* ※※※※※※※진짜 이동하면 꼭 호출해야합니다※※※※※※.*/
		((CNavigation*)(pArg))->SetUp_CurrentIndex(iCellIndx);
	}
}

void CPlayer_Skill_HandySlash::SoundPlay(CPlayer & Player)
{
	if (Player.Get_SoundCheckInfo() == true)
	{
		CSoundManager::GetInstance()->Stop_Sound(CSoundManager::Channel_ID::CH_SKILL);
		CSoundManager::GetInstance()->Play_SoundChannel("Slayer_TypoonSlash_Short_01.ogg", CSoundManager::Channel_ID::CH_SKILL, false);

		Player.Set_SoundCheckInfo(false);
	}
}

CPlayer_Skill_HandySlash * CPlayer_Skill_HandySlash::Create(LPDIRECT3DDEVICE9 pGraphicDevice, CPlayer & Player, void * pArg)
{
	CPlayer_Skill_HandySlash* pInstance = new CPlayer_Skill_HandySlash(pGraphicDevice);

	if (FAILED(pInstance->Initialize_State(Player)))
		Safe_Release(pInstance);

	return pInstance;
}

void CPlayer_Skill_HandySlash::Free()
{
	CPlayerState::Free();
}
