#include "..\Headers\Buffer_ScreenTex.h"
#include "Transform.h"

CBuffer_ScreenTex::CBuffer_ScreenTex(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

CBuffer_ScreenTex::CBuffer_ScreenTex(const CBuffer_ScreenTex & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CBuffer_ScreenTex::Ready_VIBuffer()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_iVtxSize = sizeof(VTXVIEWPORT);
	m_iNumVertices = 4;

	// 이미 모든 위치변환을 거쳤다. 셰이더로그린다.(버텍스셰이더 (x)), 행렬변환을 수행할 수 없다.
	m_dwVtxFVF = D3DFVF_XYZRHW | D3DFVF_TEX1;
	m_iNumPolygons = 2;

	m_iIndexSize = sizeof(INDEX16);
	m_Format = D3DFMT_INDEX16;

	// 정점버퍼를 할당한다 + 인덱스버퍼를 할당한다.
	if (FAILED(CVIBuffer::Ready_VIBuffer()))
		return E_FAIL;

	VTXVIEWPORT*		pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _vec4(-0.5f, -0.5f, 0.f, 1.f);
	memcpy(&m_pPositions[0], &pVertices[0].vPosition, sizeof(_vec3));
	pVertices[0].vTexUV = _vec2(0.0f, 0.f);

	pVertices[1].vPosition = _vec4(0.5f, -0.5f, 0.f, 1.f);
	memcpy(&m_pPositions[1], &pVertices[1].vPosition, sizeof(_vec3));
	pVertices[1].vTexUV = _vec2(1.0f, 0.f);

	pVertices[2].vPosition = _vec4(0.5f, 0.5f, 0.f, 1.f);
	memcpy(&m_pPositions[2], &pVertices[2].vPosition, sizeof(_vec3));
	pVertices[2].vTexUV = _vec2(1.0f, 1.f);

	pVertices[3].vPosition = _vec4(-0.5f, 0.5f, 0.f, 1.f);
	memcpy(&m_pPositions[3], &pVertices[3].vPosition, sizeof(_vec3));
	pVertices[3].vTexUV = _vec2(0.0f, 1.f);

	m_pVB->Unlock();

	INDEX16*	pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	pIndices[0]._1 = 0;
	pIndices[0]._2 = 1;
	pIndices[0]._3 = 2;

	pIndices[1]._1 = 0;
	pIndices[1]._2 = 2;
	pIndices[1]._3 = 3;

	m_pIB->Unlock();

	return NOERROR;
}

HRESULT CBuffer_ScreenTex::Ready_VIBuffer(_float fStartX, _float fStartY, _float fSizeX, _float fSizeY)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_iVtxSize = sizeof(VTXVIEWPORT);
	m_iNumVertices = 4;

	// 이미 모든 위치변환을 거쳤다. 셰이더로그린다.(버텍스셰이더 (x)), 행렬변환을 수행할 수 없다.
	m_dwVtxFVF = D3DFVF_XYZRHW | D3DFVF_TEX1;
	m_iNumPolygons = 2;

	m_iIndexSize = sizeof(INDEX16);
	m_Format = D3DFMT_INDEX16;

	// 정점버퍼를 할당한다 + 인덱스버퍼를 할당한다.
	if (FAILED(CVIBuffer::Ready_VIBuffer()))
		return E_FAIL;

	VTXVIEWPORT*	pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _vec4(fStartX, fStartY, 0.f, 1.f);
	memcpy(&m_pPositions[0], &pVertices[0].vPosition, sizeof(_vec3));
	pVertices[0].vTexUV = _vec2(0.0f, 0.f);

	pVertices[1].vPosition = _vec4(fStartX + fSizeX, fStartY, 0.f, 1.f);
	memcpy(&m_pPositions[1], &pVertices[1].vPosition, sizeof(_vec3));
	pVertices[1].vTexUV = _vec2(1.0f, 0.f);

	pVertices[2].vPosition = _vec4(fStartX + fSizeX, fStartY + fSizeY, 0.f, 1.f);
	memcpy(&m_pPositions[2], &pVertices[2].vPosition, sizeof(_vec3));
	pVertices[2].vTexUV = _vec2(1.0f, 1.f);

	pVertices[3].vPosition = _vec4(fStartX, fStartY + fSizeY, 0.f, 1.f);
	memcpy(&m_pPositions[3], &pVertices[3].vPosition, sizeof(_vec3));
	pVertices[3].vTexUV = _vec2(0.0f, 1.f);

	m_pVB->Unlock();


	INDEX16*	pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	pIndices[0]._1 = 0;
	pIndices[0]._2 = 1;
	pIndices[0]._3 = 2;

	pIndices[1]._1 = 0;
	pIndices[1]._2 = 2;
	pIndices[1]._3 = 3;

	m_pIB->Unlock();

	return NOERROR;
}

void CBuffer_ScreenTex::Render_Buffer(const CTransform* pTransform)
{
	if (nullptr == m_pGraphic_Device)
		return;

	if (nullptr != pTransform)
	{
		_matrix		matTransform = *pTransform->Get_WorldMatrixPointer();

		// 정점의 위치에 행렬을 곱해서 정점의 위치를 변환하자.
		VTXVIEWPORT*		pVertices = nullptr;

		m_pVB->Lock(0, 0, (void**)&pVertices, 0);

		for (size_t i = 0; i < m_iNumVertices; i++)
		{
			D3DXVec3TransformCoord((_vec3*)&pVertices[i].vPosition, &m_pPositions[i], &matTransform);
		}
		m_pVB->Unlock();
	}

	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iVtxSize);
	m_pGraphic_Device->SetFVF(m_dwVtxFVF);
	m_pGraphic_Device->SetIndices(m_pIB);
	m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iNumVertices, 0, m_iNumPolygons);
}

CBuffer_ScreenTex * CBuffer_ScreenTex::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBuffer_ScreenTex* pInstance = new CBuffer_ScreenTex(pGraphic_Device);

	if (FAILED(pInstance->Ready_VIBuffer()))
	{
		_MSGBOX("CBuffer_ScreenTex Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CBuffer_ScreenTex * CBuffer_ScreenTex::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _float fStartX, _float fStartY, _float fSizeX, _float fSizeY)
{
	CBuffer_ScreenTex* pInstance = new CBuffer_ScreenTex(pGraphic_Device);

	if (FAILED(pInstance->Ready_VIBuffer(fStartX, fStartY, fSizeX, fSizeY)))
	{
		_MSGBOX("CBuffer_ScreenTex Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CBuffer_ScreenTex::Clone(void* pArg)
{
	return new CBuffer_ScreenTex(*this);
}

void CBuffer_ScreenTex::Free()
{
	CVIBuffer::Free();
}
