#pragma once
#include "Component.h"

// �� ���� ǥ���ϴ� �ﰢ��(��)���� ������ �������ִ� ��ü.

_BEGIN(Engine)

class CCell;
class _DLL_EXPORTS CNavigation final : public CComponent
{
private:
	explicit CNavigation(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CNavigation(const CNavigation& rhs);
	virtual ~CNavigation() = default;
public:
	HRESULT SetUp_CurrentIndex(_uint iIndex) { m_iCurrentIndex = iIndex; return NOERROR; }
public:
	virtual HRESULT Ready_Component_Prototype(const _tchar* pFilePath);
	virtual HRESULT Ready_Component(void* pArg);
	virtual void	Render_Navigation();
public:
	_bool	Move_OnNavigation(const _vec3* pPosition, const _vec3* pDir, _float fTimeDelta, _uint* pOutIndex);
	HRESULT SetUp_Neighbor();
private:
	vector<CCell*>			m_vecCell;
	typedef vector<CCell*>	VECCELL;
private:
	_uint					m_iCurrentIndex;
public:
	static CNavigation* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pFilePath);
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void		Free();
};

_END