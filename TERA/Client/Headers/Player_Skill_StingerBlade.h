#pragma once
#include "PlayerState.h"

_BEGIN(Client)

class CPlayerState;
class CPlayer_Skill_StingerBlade : public CPlayerState
{
protected:
	explicit CPlayer_Skill_StingerBlade(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CPlayer_Skill_StingerBlade() = default;

public:
	virtual HRESULT						Initialize_State(CPlayer& Player);
	virtual CPlayerState*				Input_Keyboard(CPlayer& Player, const float& fTimeDelta, BYTE KeyID, void* pArg);
	virtual void						Update_State(CPlayer& Player, const float& fTimeDelta);

private:
	void								MovePlayerPosition(CPlayer & Player, _float fPlayerSpeed, const float& fTimeDelta, void * pArg, _int iMoveDir);
	void								SoundPlay(CPlayer& Player, _uint iSoundNum);

public:
	static CPlayer_Skill_StingerBlade*	Create(LPDIRECT3DDEVICE9 pGraphicDevice, CPlayer& Player, void* pArg = nullptr);
	virtual void						Free();

};

_END