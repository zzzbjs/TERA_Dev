#pragma once
#include "PlayerState.h"

_BEGIN(Client)

class CPlayerState;
class CPlayer_Move : public CPlayerState
{
protected:
	explicit CPlayer_Move(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CPlayer_Move() = default;

public:
	virtual HRESULT			Initialize_State(CPlayer& Player);
	virtual CPlayerState*	Input_Keyboard(CPlayer& Player, const float& fTimeDelta, BYTE KeyID, void* pArg);
	virtual void			Update_State(CPlayer& Player, const float& fTimeDelta);

public:
	static CPlayer_Move*	Create(LPDIRECT3DDEVICE9 pGraphicDevice, CPlayer& Player, void* pArg = nullptr);
	virtual void			Free();

};

_END