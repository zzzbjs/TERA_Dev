#include "stdafx.h"
#include "..\Headers\SkyBox.h"
#include "Object_Manager.h"

_USING(Client)

CSkyBox::CSkyBox(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CSkyBox::CSkyBox(const CSkyBox & rhs)
	: CGameObject(rhs)
{
}

// 원본객체에 필요한 데이터를 셋팅한다.
HRESULT CSkyBox::Ready_GameObject_Prototype()
{
	// 파일입출력을 통해 객체의 정보를 셋팅한다.
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

// 실제 씬에서 사용할 객체가 호출하는 함수.
// 원본객체 복제외에도 추가적인 셋팅이필요하면 여기서 셋팅해라.
HRESULT CSkyBox::Ready_GameObject(void* pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	return NOERROR;
}

_int CSkyBox::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	return _int();
}

_int CSkyBox::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	CObject_Manager*	pObject_Manager = CObject_Manager::GetInstance();

	if (nullptr == pObject_Manager)
		return -1;

	pObject_Manager->AddRef();

	CTransform*	pCameraTransform = (CTransform*)pObject_Manager->Get_Component(SCENE_STAGE, L"Layer_Camera", L"Com_Transform", 1);
	if (nullptr == pCameraTransform)
		return -1;

	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, pCameraTransform->Get_StateInfo(CTransform::STATE_POSITION));

	Safe_Release(pObject_Manager);

	// 다른 픽셀들에게 항상 덮여야하기때문에. 
	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this)))
		return -1;

	return _int();
}

HRESULT CSkyBox::Render_GameObject()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pTransformCom ||
		nullptr == m_pBufferCom ||
		nullptr == m_pTextureCom)
		return E_FAIL;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	if (SetUp_ConstantTable(pEffect))
		return E_FAIL;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	// 행렬 = 행렬 * 행렬
	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);

	return NOERROR;
}

HRESULT CSkyBox::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	// For.Com_Buffer
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Buffer_CubeBox", L"Com_Buffer", (CComponent**)&m_pBufferCom)))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_SkyBox", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Shader_Sky", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CSkyBox::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	pEffect->SetMatrix("g_matWorld", m_pTransformCom->Get_WorldMatrixPointer());
	pEffect->SetMatrix("g_matView", &CGameObject::Get_Transform(D3DTS_VIEW));
	pEffect->SetMatrix("g_matProj", &CGameObject::Get_Transform(D3DTS_PROJECTION));

	m_pTextureCom->SetUp_OnShader(pEffect, "g_BaseTexture", 2);

	Safe_Release(pEffect);

	return NOERROR;
}

//HRESULT CSkyBox::SetUp_RenderState()
//{
//
//	CGameObject::Set_SamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
//	CGameObject::Set_SamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
//	CGameObject::Set_SamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
//
//	// 큐브버퍼의 각면은 바깥방향을 바라보고 있게끔 작업. 
//	// 카메라는 큐브안에 존재. 큐브 각면의 뒷면을 바라보는 효과.
//	CGameObject::Set_RenderState(D3DRS_CULLMODE, D3DCULL_CW);
//	CGameObject::Set_RenderState(D3DRS_LIGHTING, FALSE);
//
//	// 스카이박스의 픽셀 깊이를 깊이버퍼에 저장하지 않는다.
//	// 스카이박스 이후에 그려지는 픽세륻르은 스카이박스 픽셀깊이와 비교할 수 없다. 때문에 무조건 이후그려지는 애들이 덮고 그린다.
//	CGameObject::Set_RenderState(D3DRS_ZWRITEENABLE, FALSE);
//
//	return NOERROR;
//}
//
//HRESULT CSkyBox::Release_RenderState()
//{
//	CGameObject::Set_SamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
//	CGameObject::Set_SamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
//	CGameObject::Set_SamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
//
//	CGameObject::Set_RenderState(D3DRS_CULLMODE, D3DCULL_CCW);
//	CGameObject::Set_RenderState(D3DRS_ZWRITEENABLE, TRUE);
//	CGameObject::Set_RenderState(D3DRS_LIGHTING, TRUE);
//
//	return NOERROR;
//}

// 원본객체를 생성한다.
CSkyBox * CSkyBox::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSkyBox* pInstance = new CSkyBox(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("CSkyBox Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

// this? : 
// 1.멤버함수안에 존재. 
// 2.멤버함수는 객체로부터 호출(객체.멤버함수(), 객체주소->멤버함수())
// 3.멤버함수안에 존재하는 this는 멤버함수의 호출을 가능하게한 객체의 주소를 의미한다.
CGameObject * CSkyBox::Clone(void* pArg)
{
	CSkyBox* pInstance = new CSkyBox(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("CSkyBox Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkyBox::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pShaderCom);

	CGameObject::Free();
}
