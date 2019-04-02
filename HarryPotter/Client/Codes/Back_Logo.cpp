#include "stdafx.h"
#include "..\Headers\Back_Logo.h"

_USING(Client)

CBack_Logo::CBack_Logo(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CBack_Logo::CBack_Logo(const CBack_Logo & rhs)
	: CGameObject(rhs)
{
}
// 원본 객체에 필요한 데이터를 셋팅한다.
HRESULT CBack_Logo::Ready_GameObject_Prototype()
{
	// 파일 입, 출력을 통해 객체의 정보를 셋팅한다.
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

//실제 씬에서 사용할 객체를 호출하는 함수.
// 원본객체 복제 외에도, 추가적인 셋팅이 필요하면 여기서 셋팅하면 된다.
HRESULT CBack_Logo::Ready_GameObject()
{
	return NOERROR;
}

_int CBack_Logo::Update_GameObject(const _float & fTimeDelta)
{
	return _int();
}

_int CBack_Logo::LateUpdate_GameObject(const _float & fTimeDelta)
{
	return _int();
}

HRESULT CBack_Logo::Render_GameObject()
{
	return NOERROR;
}

// 원본 객체를 생성한다.
CBack_Logo * CBack_Logo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBack_Logo* pInstance = new CBack_Logo(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSGBOX("CBack_Logo Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

// this? :
// 1. 멤버함수 안에 존재한다.
// 2. 멤버함수는 객체로 부터 호출(객체.멤버함수(), 객체주소->멤버함수())
// 3. 멤버함수안에 존재하는 this는 멤버함수의 호출을 가능하게 한 객체의 주소를 의미한다.
CGameObject * CBack_Logo::Clone()
{
	// *this 는 원본객체에 해당 함.
	// 복사 생성자를 호출하면 부모의 복사 생성자도 호출한다.
	// 장치 객체 때문에 얕은 복사를 하면 문제가 생긴다.
	// (하지만 포인터는 얕은복사해도 상관이 없다고 한다)
	// AddRefCnt 때문에 얕은 복사 하면 안된다.
	CBack_Logo* pInstance = new CBack_Logo(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSGBOX("CBack_Logo Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBack_Logo::Free()
{
	CGameObject::Free();
}
