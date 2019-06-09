#pragma once
#include "Player.h"
#include "Base.h"

_BEGIN(Client)

class CPlayer;
class CPlayerState : public CBase
{
protected:
	explicit CPlayerState(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CPlayerState() = default;

public:
	virtual HRESULT			Initialize_State(CPlayer& Player);
	virtual CPlayerState*	Input_Keyboard(CPlayer& Player, const float& fTimeDelta, BYTE KeyID);
	virtual void			Update_State(CPlayer& Player, const float& fTimeDelta);

	// Initialize_State�� State�� ���� ������ ���� �����ϰ�,
	// Enter_State�� �� ���ķ� �ش� State�� �ٲ� ������ �ѹ��� ���ͼ� �����ϴ� ������ ���ش�.
	virtual void			Enter_State(CPlayer& Player);

protected:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;

public:
	virtual void			Free();

};

_END