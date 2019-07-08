#pragma once
#include "NPC.h"
#include "Defines.h"
#include "GameObject.h"

_BEGIN(Client)

class CUI_Dialog;
class CCamera_Static;
class CContentsNPC : public CNPC
{
private:
	explicit CContentsNPC(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CContentsNPC(const CContentsNPC& rhs);
	virtual ~CContentsNPC() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Render_GameObject();

public:
	virtual HRESULT Add_Component();
	virtual HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	void			ScriptInfo();

public:
	static CContentsNPC*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg = nullptr);
	virtual void			Free();

};

_END