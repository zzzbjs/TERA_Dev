#pragma once
#include "ArkusState.h"

_BEGIN(Client)

class CArkusState;
class CArkus_Run : public CArkusState
{
public:
	explicit CArkus_Run(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CArkus_Run() = default;

public:
	virtual HRESULT			Initialize_State(CArkus& Arkus);
	virtual CArkusState*	Input_State(CArkus& Arkus, const float& fTimeDelta, BYTE KeyID, void* pArg);
	virtual void			Update_State(CArkus& Arkus, const float& fTimeDelta);

private:
	void					MoveArkusPosition(CArkus & Arkus, const _float fArkusSpeed, const _float& fTimeDelta, void * pArg, _int iMoveDir);

public:
	static CArkus_Run*		Create(LPDIRECT3DDEVICE9 pGraphicDevice, CArkus& Arkus, void* pArg = nullptr);
	virtual void			Free();

};

_END