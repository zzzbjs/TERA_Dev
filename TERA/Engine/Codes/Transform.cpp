#include "..\Headers\Transform.h"

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
	, m_matWorld(rhs.m_matWorld)
{
}

HRESULT CTransform::SetUp_OnGraphicDev()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	// D3DTS_WORLD
	// D3DTS_VIEW
	// D3DTS_PROJECTION

	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_matWorld);

	return NOERROR;
}

HRESULT CTransform::Set_Scaling(const _float & fX, const _float & fY, const _float & fZ)
{
	// [이해를 돕기 위한 방법] //
	//_vec3	 vRight;

	//// 월드행렬 1열인 Right행의 첫번째 주소값인 &m_matWorld.m[0][0]에서,
	//// sizeof(_vec3)) 크기만큼의, 그러니까 float 3개 만큼의 크기를 카피 한다는 거니까,
	//// [0][0], [0][1], [0][2]를 다 카피한다 이말이 되버린다.
	//memcpy(&vRight, &m_matWorld.m[0][0], sizeof(_vec3));
	//
	//vRight = *D3DXVec3Normalize(&vRight, &vRight) * fX;

	//memcpy(&m_matWorld.m[0][0], &vRight, sizeof(_vec3));

	// [어렵지만 간편한 방법] //
	_vec3		vRight, vUp, vLook;

	vRight = *D3DXVec3Normalize(&vRight, (_vec3*)&m_matWorld.m[0][0]) * fX;
	memcpy(&m_matWorld.m[0][0], &vRight, sizeof(_vec3));

	vUp = *D3DXVec3Normalize(&vUp, (_vec3*)&m_matWorld.m[1][0]) * fY;
	memcpy(&m_matWorld.m[1][0], &vUp, sizeof(_vec3));

	vLook = *D3DXVec3Normalize(&vLook, (_vec3*)&m_matWorld.m[2][0]) * fZ;
	memcpy(&m_matWorld.m[2][0], &vLook, sizeof(_vec3));

	return NOERROR;
}

HRESULT CTransform::Set_Rotation_YawPitchRoll(_float fRadianY, _float fRadianX, _float fRadianZ)
{
	m_vRotRadValue.x = fRadianX;
	m_vRotRadValue.y = fRadianY;
	m_vRotRadValue.z = fRadianZ;

	_matrix		matRotate;

	D3DXMatrixRotationYawPitchRoll(&matRotate, fRadianY, fRadianX, fRadianZ);

	_vec3		vRight(1.f, 0.f, 0.f), vUp(0.f, 1.f, 0.f), vLook(0.f, 0.f, 1.f);

	vRight *= D3DXVec3Length((_vec3*)&m_matWorld.m[STATE_RIGHT][0]);
	D3DXVec3TransformNormal(&vRight, &vRight, &matRotate);

	vUp *= D3DXVec3Length((_vec3*)&m_matWorld.m[STATE_UP][0]);
	D3DXVec3TransformNormal(&vUp, &vUp, &matRotate);

	vLook *= D3DXVec3Length((_vec3*)&m_matWorld.m[STATE_LOOK][0]);
	D3DXVec3TransformNormal(&vLook, &vLook, &matRotate);

	memcpy(&m_matWorld.m[STATE_RIGHT][0], &vRight, sizeof(_vec3));
	memcpy(&m_matWorld.m[STATE_UP][0], &vUp, sizeof(_vec3));
	memcpy(&m_matWorld.m[STATE_LOOK][0], &vLook, sizeof(_vec3));

	return NOERROR;
}

HRESULT CTransform::Set_Angle_Axis(_vec3 vState, const _float & fRadian)
{
	// 이 함수는 무조건 원점을 기준으로 내가 설정한 fRadian 각만큼 돌린 상태로,
	// 초기화를 해주겠다 라는 함수이다.
	// 값이 지속적으로 바뀌지 않고 한번만 지정해 주기 때문에 fTimeDelta값이 필요 없다.

	_matrix		matRotate;
	D3DXMatrixRotationAxis(&matRotate, &vState, fRadian);

	// 그래서 이렇게 초기값을 1로 가지는 항등행렬 식의 벡터를 선언 해주는데,
	_vec3		vRight(1.f, 0.f, 0.f), vUp(0.f, 1.f, 0.f), vLook(0.f, 0.f, 1.f);

	// 이렇게 하면 각 벡터의 고유 크기정보는 사라져 버리기 때문에,
	// D3DXVec3Length함수를 사용하여, 각 벡터를 다시 초기화? 해준다.
	vRight *= D3DXVec3Length((_vec3*)&m_matWorld.m[STATE_RIGHT][0]);
	D3DXVec3TransformNormal(&vRight, &vRight, &matRotate);

	vUp *= D3DXVec3Length((_vec3*)&m_matWorld.m[STATE_UP][0]);
	D3DXVec3TransformNormal(&vUp, &vUp, &matRotate);

	vLook *= D3DXVec3Length((_vec3*)&m_matWorld.m[STATE_LOOK][0]);
	D3DXVec3TransformNormal(&vLook, &vLook, &matRotate);

	memcpy(&m_matWorld.m[STATE_RIGHT][0], &vRight, sizeof(_vec3));
	memcpy(&m_matWorld.m[STATE_UP][0], &vUp, sizeof(_vec3));
	memcpy(&m_matWorld.m[STATE_LOOK][0], &vLook, sizeof(_vec3));

	return NOERROR;
}

HRESULT CTransform::Move(_int iDirection, const _float & fSpeedPerSec, const _float & fTimeDelta)
{
	// 위치 = 위치 + 현재 바라보고 있는 방향벡터(=vLook) (이 때, 방향과 크기를 1로) * 속도PerSec * fTimeDelta
	// 월드행렬의 4행에 갱신된 위치정보를 적용시켜준다.(대입해준다)

	_vec3	vPosition, vLook, vRight;

	memcpy(&vPosition, &m_matWorld.m[STATE_POSITION][0], sizeof(_vec3));
	memcpy(&vLook, &m_matWorld.m[STATE_LOOK][0], sizeof(_vec3));
	memcpy(&vRight, &m_matWorld.m[STATE_RIGHT][0], sizeof(_vec3));

	if (iDirection == 0)
		vPosition += ((*D3DXVec3Normalize(&vLook, &vLook)) * fSpeedPerSec * fTimeDelta);
	else if (iDirection == 1)
		vPosition -= ((*D3DXVec3Normalize(&vLook, &vLook)) * fSpeedPerSec * fTimeDelta);
	else if (iDirection == 2)
		vPosition -= ((*D3DXVec3Normalize(&vRight, &vRight)) * fSpeedPerSec * fTimeDelta);
	else if (iDirection == 3)
		vPosition += ((*D3DXVec3Normalize(&vRight, &vRight)) * fSpeedPerSec * fTimeDelta);

	memcpy(&m_matWorld.m[STATE_POSITION][0], &vPosition, sizeof(_vec3));

	return NOERROR;
}

HRESULT CTransform::Move(const _vec3 * vPos, const _float & fSpeedPerSec, const _float & fTimeDelta)
{
	// Feat.형진이

	if (nullptr == vPos)
		return E_FAIL;

	_vec3 vTemp;
	vTemp = *D3DXVec3Normalize(&vTemp, vPos) * fSpeedPerSec * fTimeDelta;

	_vec3	vPosition;
	memcpy(&vPosition, &m_matWorld.m[STATE_POSITION][0], sizeof(_vec3));

	vPosition += vTemp;

	memcpy(&m_matWorld.m[STATE_POSITION][0], &vPosition, sizeof(_vec3));

	return NOERROR;
}

//HRESULT CTransform::Set_Angle_X(const _float & fRadian)
//{
//	_matrix		matRot;
//	D3DXMatrixRotationX(&matRot, fRadian);
//
//	_vec3		vRight(1.f, 0.f, 0.f), vUp(0.f, 1.f, 0.f), vLook(0.f, 0.f, 1.f);
//
//	vRight *= D3DXVec3Length((_vec3*)&m_matWorld.m[STATE_RIGHT][0]);
//	D3DXVec3TransformNormal(&vRight, &vRight, &matRot);
//
//	vUp *= D3DXVec3Length((_vec3*)&m_matWorld.m[STATE_UP][0]);
//	D3DXVec3TransformNormal(&vUp, &vUp, &matRot);
//
//	vLook *= D3DXVec3Length((_vec3*)&m_matWorld.m[STATE_LOOK][0]);
//	D3DXVec3TransformNormal(&vLook, &vLook, &matRot);
//
//	memcpy(&m_matWorld.m[STATE_RIGHT][0], &vRight, sizeof(_vec3));
//	memcpy(&m_matWorld.m[STATE_UP][0], &vUp, sizeof(_vec3));
//	memcpy(&m_matWorld.m[STATE_LOOK][0], &vLook, sizeof(_vec3));
//
//	return NOERROR;
//}
//
//HRESULT CTransform::Set_Angle_Y(const _float & fRadian)
//{
//	_matrix		matRot;
//
//	D3DXMatrixRotationY(&matRot, fRadian);
//
//	_vec3		vRight(1.f, 0.f, 0.f), vUp(0.f, 1.f, 0.f), vLook(0.f, 0.f, 1.f);
//
//	vRight *= D3DXVec3Length((_vec3*)&m_matWorld.m[STATE_RIGHT][0]);
//	D3DXVec3TransformNormal(&vRight, &vRight, &matRot);
//
//	vUp *= D3DXVec3Length((_vec3*)&m_matWorld.m[STATE_UP][0]);
//	D3DXVec3TransformNormal(&vUp, &vUp, &matRot);
//
//	vLook *= D3DXVec3Length((_vec3*)&m_matWorld.m[STATE_LOOK][0]);
//	D3DXVec3TransformNormal(&vLook, &vLook, &matRot);
//
//	memcpy(&m_matWorld.m[STATE_RIGHT][0], &vRight, sizeof(_vec3));
//	memcpy(&m_matWorld.m[STATE_UP][0], &vUp, sizeof(_vec3));
//	memcpy(&m_matWorld.m[STATE_LOOK][0], &vLook, sizeof(_vec3));
//
//	return NOERROR;
//}
//
//HRESULT CTransform::Set_Angle_Z(const _float & fRadian)
//{
//	_matrix		matRot;
//	D3DXMatrixRotationZ(&matRot, fRadian);
//
//	_vec3		vRight(1.f, 0.f, 0.f), vUp(0.f, 1.f, 0.f), vLook(0.f, 0.f, 1.f);
//
//	vRight *= D3DXVec3Length((_vec3*)&m_matWorld.m[STATE_RIGHT][0]);
//	D3DXVec3TransformNormal(&vRight, &vRight, &matRot);
//
//	vUp *= D3DXVec3Length((_vec3*)&m_matWorld.m[STATE_UP][0]);
//	D3DXVec3TransformNormal(&vUp, &vUp, &matRot);
//
//	vLook *= D3DXVec3Length((_vec3*)&m_matWorld.m[STATE_LOOK][0]);
//	D3DXVec3TransformNormal(&vLook, &vLook, &matRot);
//
//	memcpy(&m_matWorld.m[STATE_RIGHT][0], &vRight, sizeof(_vec3));
//	memcpy(&m_matWorld.m[STATE_UP][0], &vUp, sizeof(_vec3));
//	memcpy(&m_matWorld.m[STATE_LOOK][0], &vLook, sizeof(_vec3));
//
//	return NOERROR;
//}
//
//HRESULT CTransform::Go_Straight(const _float& fSpeed, const _float & fTimeDelta)
//{
//	// 위치 = 위치 + 현재 바라보고 있는 방향벡터(방향, 크기를 1로) * 속도PerSec * fTimeDelta
//	// 월드행렬의 4행에 갱신된 위치정보를 적용시켜준다.(대입해준다)
//
//
//	_vec3			vPosition, vLook;
//
//	memcpy(&vPosition, &m_matWorld.m[STATE_POSITION][0], sizeof(_vec3));
//	memcpy(&vLook, &m_matWorld.m[STATE_LOOK][0], sizeof(_vec3));
//
//	vPosition += *D3DXVec3Normalize(&vLook, &vLook) * fSpeed * fTimeDelta;
//
//	memcpy(&m_matWorld.m[STATE_POSITION][0], &vPosition, sizeof(_vec3));
//
//	return NOERROR;
//}
//
//HRESULT CTransform::Back_Straight(const _float & fSpeed, const _float & fTimeDelta)
//{
//	_vec3			vPosition, vLook;
//
//	memcpy(&vPosition, &m_matWorld.m[STATE_POSITION][0], sizeof(_vec3));
//	memcpy(&vLook, &m_matWorld.m[STATE_LOOK][0], sizeof(_vec3));
//
//	vPosition -= *D3DXVec3Normalize(&vLook, &vLook) * fSpeed * fTimeDelta;
//
//	memcpy(&m_matWorld.m[STATE_POSITION][0], &vPosition, sizeof(_vec3));
//
//	return NOERROR;
//}
//
//HRESULT CTransform::Move_Right(const _float & fSpeedPerSec, const _float & fTimeDelta)
//{
//	_vec3			vPosition, vRight;
//
//	memcpy(&vPosition, &m_matWorld.m[STATE_POSITION][0], sizeof(_vec3));
//	memcpy(&vRight, &m_matWorld.m[STATE_RIGHT][0], sizeof(_vec3));
//
//	vPosition += *D3DXVec3Normalize(&vRight, &vRight) * fSpeedPerSec * fTimeDelta;
//
//	memcpy(&m_matWorld.m[STATE_POSITION][0], &vPosition, sizeof(_vec3));
//
//	return NOERROR;
//}
//
//HRESULT CTransform::Move_Left(const _float & fSpeedPerSec, const _float & fTimeDelta)
//{
//	_vec3			vPosition, vRight;
//
//	memcpy(&vPosition, &m_matWorld.m[STATE_POSITION][0], sizeof(_vec3));
//	memcpy(&vRight, &m_matWorld.m[STATE_RIGHT][0], sizeof(_vec3));
//
//	vPosition -= *D3DXVec3Normalize(&vRight, &vRight) * fSpeedPerSec * fTimeDelta;
//
//	memcpy(&m_matWorld.m[STATE_POSITION][0], &vPosition, sizeof(_vec3));
//
//	return NOERROR;
//}
//
//HRESULT CTransform::Rotation_X(const _float & fRadianPerSec, const _float & fTimeDelta)
//{
//	_matrix		matRot;
//	D3DXMatrixRotationX(&matRot, fRadianPerSec * fTimeDelta);
//
//	_vec3		vRight, vUp, vLook;
//
//	memcpy(&vRight, &m_matWorld.m[STATE_RIGHT][0], sizeof(_vec3));
//	D3DXVec3TransformNormal(&vRight, &vRight, &matRot);
//
//	memcpy(&vUp, &m_matWorld.m[STATE_UP][0], sizeof(_vec3));
//	D3DXVec3TransformNormal(&vUp, &vUp, &matRot);
//
//	memcpy(&vLook, &m_matWorld.m[STATE_LOOK][0], sizeof(_vec3));
//	D3DXVec3TransformNormal(&vLook, &vLook, &matRot);
//
//	memcpy(&m_matWorld.m[STATE_RIGHT][0], &vRight, sizeof(_vec3));
//	memcpy(&m_matWorld.m[STATE_UP][0], &vUp, sizeof(_vec3));
//	memcpy(&m_matWorld.m[STATE_LOOK][0], &vLook, sizeof(_vec3));
//
//	return NOERROR;
//}
//
//HRESULT CTransform::Rotation_Y(const _float & fRadianPerSec, const _float & fTimeDelta)
//{
//	_matrix		matRot;
//	D3DXMatrixRotationY(&matRot, fRadianPerSec * fTimeDelta);
//
//	_vec3		vRight, vUp, vLook;
//
//	memcpy(&vRight, &m_matWorld.m[STATE_RIGHT][0], sizeof(_vec3));
//	D3DXVec3TransformNormal(&vRight, &vRight, &matRot);
//
//	memcpy(&vUp, &m_matWorld.m[STATE_UP][0], sizeof(_vec3));
//	D3DXVec3TransformNormal(&vUp, &vUp, &matRot);
//
//	memcpy(&vLook, &m_matWorld.m[STATE_LOOK][0], sizeof(_vec3));
//	D3DXVec3TransformNormal(&vLook, &vLook, &matRot);
//
//	memcpy(&m_matWorld.m[STATE_RIGHT][0], &vRight, sizeof(_vec3));
//	memcpy(&m_matWorld.m[STATE_UP][0], &vUp, sizeof(_vec3));
//	memcpy(&m_matWorld.m[STATE_LOOK][0], &vLook, sizeof(_vec3));
//
//	return NOERROR;
//}
//
//HRESULT CTransform::Rotation_Z(const _float & fRadianPerSec, const _float & fTimeDelta)
//{
//	_matrix		matRot;
//	D3DXMatrixRotationZ(&matRot, fRadianPerSec * fTimeDelta);
//
//	_vec3		vRight, vUp, vLook;
//
//	memcpy(&vRight, &m_matWorld.m[STATE_RIGHT][0], sizeof(_vec3));
//	D3DXVec3TransformNormal(&vRight, &vRight, &matRot);
//
//	memcpy(&vUp, &m_matWorld.m[STATE_UP][0], sizeof(_vec3));
//	D3DXVec3TransformNormal(&vUp, &vUp, &matRot);
//
//	memcpy(&vLook, &m_matWorld.m[STATE_LOOK][0], sizeof(_vec3));
//	D3DXVec3TransformNormal(&vLook, &vLook, &matRot);
//
//	memcpy(&m_matWorld.m[STATE_RIGHT][0], &vRight, sizeof(_vec3));
//	memcpy(&m_matWorld.m[STATE_UP][0], &vUp, sizeof(_vec3));
//	memcpy(&m_matWorld.m[STATE_LOOK][0], &vLook, sizeof(_vec3));
//
//	return NOERROR;
//}

HRESULT CTransform::Rotation_Axis(const _vec3 & vAxis, const _float & fRadianPerSec, const _float & fTimeDelta)
{
	_matrix		matRot;
	D3DXMatrixRotationAxis(&matRot, &vAxis, fRadianPerSec * fTimeDelta);

	_vec3		vRight, vUp, vLook;

	memcpy(&vRight, &m_matWorld.m[STATE_RIGHT][0], sizeof(_vec3));
	D3DXVec3TransformNormal(&vRight, &vRight, &matRot);

	if (D3DXVec3Length(&vRight) <= 0)
		return NOERROR;

	memcpy(&vUp, &m_matWorld.m[STATE_UP][0], sizeof(_vec3));
	D3DXVec3TransformNormal(&vUp, &vUp, &matRot);

	if (D3DXVec3Length(&vUp) <= 0)
		return NOERROR;

	memcpy(&vLook, &m_matWorld.m[STATE_LOOK][0], sizeof(_vec3));
	D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

	if (D3DXVec3Length(&vLook) <= 0)
		return NOERROR;

	memcpy(&m_matWorld.m[STATE_RIGHT][0], &vRight, sizeof(_vec3));
	memcpy(&m_matWorld.m[STATE_UP][0], &vUp, sizeof(_vec3));
	memcpy(&m_matWorld.m[STATE_LOOK][0], &vLook, sizeof(_vec3));

	return NOERROR;
}

HRESULT CTransform::Move_Target(const CTransform * pTransform, const _float & fSpeedPerSec, const _float & fTimeDelta)
{
	_vec3		vTargetPos = *pTransform->Get_StateInfo(STATE_POSITION);
	_vec3		vPosition = *Get_StateInfo(STATE_POSITION);
	_vec3		vDir = vTargetPos - vPosition;

	vPosition += *D3DXVec3Normalize(&vDir, &vDir) * fSpeedPerSec * fTimeDelta;

	vDir *= D3DXVec3Length(Get_StateInfo(CTransform::STATE_LOOK));

	_vec3	vUp = *Get_StateInfo(CTransform::STATE_UP);

	_vec3	vRight;
	D3DXVec3Normalize(&vRight, D3DXVec3Cross(&vRight, &vUp, &vDir));

	vRight *= D3DXVec3Length(Get_StateInfo(CTransform::STATE_RIGHT));

	Set_StateInfo(STATE_RIGHT, &vRight);

	Set_StateInfo(STATE_LOOK, &vDir);

	Set_StateInfo(STATE_POSITION, &vPosition);

	return NOERROR;
}

HRESULT CTransform::Move_Target(const _vec3 * pTargetPos, const _float & fSpeedPerSec, const _float & fTimeDelta, _bool* pFinish)
{
	_vec3		vTargetPos = *pTargetPos;

	_vec3		vPosition = *Get_StateInfo(STATE_POSITION);
	_vec3		vDir = vTargetPos - vPosition;

	_float		fLength = D3DXVec3Length(&vDir);

	if (0.5f > fLength)
	{
		*pFinish = true;
		return NOERROR;
	}

	vPosition += *D3DXVec3Normalize(&vDir, &vDir) * fSpeedPerSec * fTimeDelta;

	vDir *= D3DXVec3Length(Get_StateInfo(CTransform::STATE_LOOK));

	_vec3	vUp = *Get_StateInfo(CTransform::STATE_UP);

	_vec3	vRight;
	D3DXVec3Normalize(&vRight, D3DXVec3Cross(&vRight, &vUp, &vDir));

	vRight *= D3DXVec3Length(Get_StateInfo(CTransform::STATE_RIGHT));

	Set_StateInfo(STATE_RIGHT, &vRight);

	Set_StateInfo(STATE_LOOK, &vDir);

	Set_StateInfo(STATE_POSITION, &vPosition);

	*pFinish = false;

	return NOERROR;
}

HRESULT CTransform::Ready_Transform()
{
	D3DXMatrixIdentity(&m_matWorld);

	return NOERROR;
}

CTransform * CTransform::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTransform* pInstance = new CTransform(pGraphic_Device);

	if (FAILED(pInstance->Ready_Transform()))
	{
		_MSGBOX("CTransform Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CTransform::Clone(void* pArg)
{
	return new CTransform(*this);
}

void CTransform::Free()
{
	CComponent::Free();
}
