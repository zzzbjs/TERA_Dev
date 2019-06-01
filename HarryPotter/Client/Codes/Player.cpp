#include "stdafx.h"
#include "..\Headers\Player.h"
#include "Object_Manager.h"
#include "Light_Manager.h"

_USING(Client)

#define PLAYER_SPEED	5.f

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
	, m_pKeyManager(CKeyManager::GetInstance())
{
}

CPlayer::CPlayer(const CPlayer & rhs)
	: CGameObject(rhs)
	, m_isMove(rhs.m_isMove)
{
}

// 원본객체에 필요한 데이터를 셋팅한다.
HRESULT CPlayer::Ready_GameObject_Prototype()
{
	// 파일입출력을 통해 객체의 정보를 셋팅한다.
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

// 실제 씬에서 사용할 객체가 호출하는 함수.
// 원본객체 복제외에도 추가적인 셋팅이필요하면 여기서 셋팅해라.
HRESULT CPlayer::Ready_GameObject(void* pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scaling(0.01f, 0.01f, 0.01f);
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(0.2, 0.f, 0.2));
	m_pMeshCom->SetUp_AnimationSet(STATE_IDLE);

	m_pMeshCom->ChangePivot(_vec3(1.f, 0.f, 0.f), -90);
	m_pMeshCom->ChangePivot(_vec3(0.f, 1.f, 0.f), 180);
	
	return NOERROR;
}

_int CPlayer::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	m_fTimeDelta = fTimeDelta;

	ViewChanage();
	KeyInput();

	m_fAniSpeed = 1.f;
	m_pMeshCom->Play_Animation(fTimeDelta, m_fAniSpeed);

	return _int();
}

_int CPlayer::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONEALPHA, this)))
		return -1;

	return _int();
}

HRESULT CPlayer::Render_GameObject()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pTransformCom ||
		nullptr == m_pMeshCom ||
		nullptr == m_pTextureCom)
		return E_FAIL;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	if (FAILED(SetUp_ConstantTable(pEffect)))
		return E_FAIL;

	pEffect->Begin(nullptr, 0);
	
	for (size_t i = 0; i < m_pMeshCom->Get_NumMeshContainer(); ++i)
	{
		if (FAILED(m_pMeshCom->Update_SkinnedMesh(i)))
			break;

		for (size_t j = 0; j < m_pMeshCom->Get_NumSubSet(i); ++j)
		{
			if (FAILED(m_pMeshCom->SetTexture_OnShader(pEffect, i, j, "g_BaseTexture", MESHTEXTURE::TYPE_DIFFUSE)))
				return E_FAIL;

			pEffect->CommitChanges();

			pEffect->BeginPass(0);

			m_pMeshCom->Render_Mesh(i, j);

			pEffect->EndPass();
		}
	}
	
	pEffect->End();

	Safe_Release(pEffect);

	return NOERROR;
}

HRESULT CPlayer::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	// For.Com_Mesh
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Mesh_Player", L"Com_Mesh", (CComponent**)&m_pMeshCom)))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_Default", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Shader_Mesh", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CPlayer::SetUp_HeightOnTerrain()
{
	CObject_Manager*	pObject_Manager = CObject_Manager::GetInstance();

	if (nullptr == pObject_Manager)
		return E_FAIL;
	pObject_Manager->AddRef();

	CBuffer_Terrain* pBufferCom = (CBuffer_Terrain*)pObject_Manager->Get_Component(SCENE_STAGE, L"Layer_BackGround", L"Com_Buffer", 0);
	if (nullptr == pBufferCom)
		return E_FAIL;

	// 플레이어의 Y값과 이 지형의 Y값을 비교해서, 점프를 구현하면 된다.
	_float	fY = pBufferCom->Compute_HeightOnBuffer(m_pTransformCom);

	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION)->x, fY + 0.5f, m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION)->z));

	Safe_Release(pObject_Manager);

	return NOERROR;
}

HRESULT CPlayer::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	pEffect->SetMatrix("g_matWorld", m_pTransformCom->Get_WorldMatrixPointer());
	pEffect->SetMatrix("g_matView", &CGameObject::Get_Transform(D3DTS_VIEW));
	pEffect->SetMatrix("g_matProj", &CGameObject::Get_Transform(D3DTS_PROJECTION));

	CLight_Manager*	pLight_Manager = CLight_Manager::GetInstance();
	if (nullptr == pLight_Manager)
		return E_FAIL;

	pLight_Manager->AddRef();

	const D3DLIGHT9* pLightInfo = pLight_Manager->Get_LightInfo(0);
	if (nullptr == pLightInfo)
		return E_FAIL;

	pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLightInfo->Diffuse);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLightInfo->Specular);

	Safe_Release(pLight_Manager);

	_matrix		matView = CGameObject::Get_Transform(D3DTS_VIEW);
	D3DXMatrixInverse(&matView, nullptr, &matView);

	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView.m[3][0]);

	Safe_Release(pEffect);

	return NOERROR;
}

void CPlayer::ViewChanage()
{
	// 마우스 커서를 기준으로 플레이어의 뷰를 변환한다 //
	// Feat.형진이
	CTransform*	vCameraTransformCom = ((CTransform*)(CObject_Manager::GetInstance()->Get_Component(SCENE_STAGE, L"Layer_Camera", L"Com_Transform", 1)));
	_vec3 vCameraLook = *vCameraTransformCom->Get_StateInfo(CTransform::STATE_LOOK);
	
	_vec3	vPlayerRight = *m_pTransformCom->Get_StateInfo(CTransform::STATE_RIGHT);
	_vec3	vPlayerUp = *m_pTransformCom->Get_StateInfo(CTransform::STATE_UP);
	_vec3	vPlayerLook = *m_pTransformCom->Get_StateInfo(CTransform::STATE_LOOK);
	_float	fPlayerRightSize = D3DXVec3Length(&vPlayerRight);
	_float	fPlayerLookSize = D3DXVec3Length(&vPlayerLook);

	vCameraLook.y = 0.f;
	D3DXVec3Normalize(&vCameraLook, &vCameraLook);

	m_pTransformCom->Set_StateInfo(CTransform::STATE_LOOK, &(vCameraLook * fPlayerLookSize));

	_vec3 vPlayerNewRight;
	D3DXVec3Cross(&vPlayerNewRight, &vPlayerUp, &vCameraLook);

	D3DXVec3Normalize(&vPlayerNewRight, &vPlayerNewRight);

	m_pTransformCom->Set_StateInfo(CTransform::STATE_RIGHT, &(vPlayerNewRight * fPlayerRightSize));

}

void CPlayer::KeyInput()
{
	// 걷고 뛰기 토글 플래그.
	{
		if (GetAsyncKeyState('R') && m_bRunflag == false)
		{
			m_bRunflag = true;
			m_bIsRun = !m_bIsRun;
		}
		if (!GetAsyncKeyState('R') && m_bRunflag == true)
			m_bRunflag = false;
	}

	if (GetAsyncKeyState('W') & 0x8000)
	{
		if (m_bIsRun == false)
		{
			m_pTransformCom->Move(0, PLAYER_SPEED - 2, m_fTimeDelta);
			m_pMeshCom->SetUp_AnimationSet(STATE_WALK);
		}
		else if (m_bIsRun == true)
		{
			m_pTransformCom->Move(0, PLAYER_SPEED, m_fTimeDelta);
			m_pMeshCom->SetUp_AnimationSet(STATE_RUN);
		}
	}
	else if (GetAsyncKeyState('S') & 0x8000)
	{
		m_pTransformCom->Move(1, PLAYER_SPEED, m_fTimeDelta);
		m_pMeshCom->SetUp_AnimationSet(STATE_RUN_BACK);
	}
	else if (GetAsyncKeyState('A') & 0x8000)
	{
		m_pTransformCom->Move(2, PLAYER_SPEED, m_fTimeDelta);
		m_pMeshCom->SetUp_AnimationSet(STATE_RUN_LEFT);
	}
	else if (GetAsyncKeyState('D') & 0x8000)
	{
		m_pTransformCom->Move(3, PLAYER_SPEED, m_fTimeDelta);
		m_pMeshCom->SetUp_AnimationSet(STATE_RUN_RIGHT);
	}
	else if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		m_pMeshCom->SetUp_AnimationSet(STATE_CASTAIM);
	}
	else if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		m_pMeshCom->SetUp_AnimationSet(6);
	}
	else
	{
		// 마법 태그랑 충돌 불 변수가 True 면
		// 여기서 애니메이션 제어를 한다.

		//if (스테이트 == 마법)
		//{
		//	애니메이션이 종료되었어?
		//	되었으면 밑에 구문 실행.
		//}
		m_pMeshCom->SetUp_AnimationSet(25);
	}
}

void CPlayer::ETC()
{
	// [Update_GameObject]
	//{
	//	if (true == m_isMove)
	//	{
	//		_bool isFinish = false;

	//		m_pTransformCom->Move_Target(&m_vTargetPos, 10.f, fTimeDelta, &isFinish);

	//		if (true == isFinish)
	//			m_isMove = false;
	//	}

	//	if (FAILED(SetUp_HeightOnTerrain()))
	//		return -1;
	//}
}

// 원본객체를 생성한다.
CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayer* pInstance = new CPlayer(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("CPlayer Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

// this? : 
// 1.멤버함수안에 존재. 
// 2.멤버함수는 객체로부터 호출(객체.멤버함수(), 객체주소->멤버함수())
// 3.멤버함수안에 존재하는 this는 멤버함수의 호출을 가능하게한 객체의 주소를 의미한다.
CGameObject * CPlayer::Clone(void* pArg)
{
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("CPlayer Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pShaderCom);

	CGameObject::Free();
}
