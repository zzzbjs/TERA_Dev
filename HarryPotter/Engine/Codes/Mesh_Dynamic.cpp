#include "..\Headers\Mesh_Dynamic.h"
#include "HierarchyLoader.h"

CMesh_Dynamic::CMesh_Dynamic(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CMesh_Dynamic::CMesh_Dynamic(const CMesh_Dynamic & rhs)
	: CComponent(rhs)
	, m_pLoader(rhs.m_pLoader)
	, m_pRootFrame(rhs.m_pRootFrame)
	, m_MeshContainers(rhs.m_MeshContainers)
{

	Safe_AddRef(m_pLoader);

}

HRESULT CMesh_Dynamic::Ready_Mesh_Dynamic(const _tchar * pFilePath, const _tchar * pFileName)
{
	_tchar		szFullPath[MAX_PATH] = L"";

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	m_pLoader = CHierarchyLoader::Create(m_pGraphic_Device, pFilePath);
	if (nullptr == m_pLoader)
		return E_FAIL;

	if (FAILED(D3DXLoadMeshHierarchyFromX(szFullPath, 0, m_pGraphic_Device, m_pLoader, nullptr, &m_pRootFrame, nullptr)))
		return E_FAIL;

	_matrix		matRoot;

	if (FAILED(Update_CombinedTransformationMatrix(m_pRootFrame, D3DXMatrixIdentity(&matRoot))))
		return E_FAIL;

	if (FAILED(SetUp_CombinedMatrixPointer(m_pRootFrame)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CMesh_Dynamic::Render_Mesh(_uint iMeshContainerID, _uint iSubSetID)
{
	m_MeshContainers[iMeshContainerID]->MeshData.pMesh->DrawSubset(iSubSetID);

	return NOERROR;
}

HRESULT CMesh_Dynamic::Update_SkinnedMesh(_uint iMeshContainerID)
{
	if (m_MeshContainers.size() <= iMeshContainerID)
		return E_FAIL;

	D3DXMESHCONTAINER_DERIVED* pMeshContainer = m_MeshContainers[iMeshContainerID];

	for (size_t i = 0; i < pMeshContainer->dwNumBones; ++i)
		pMeshContainer->pRenderingMatrices[i] = pMeshContainer->pOffsetMatrices[i] * *pMeshContainer->ppCombinedTransformationMatrices[i];

	void*	pSour, *pDest;

	pMeshContainer->pOriginalMesh->LockVertexBuffer(0, &pSour);
	pMeshContainer->MeshData.pMesh->LockVertexBuffer(0, &pDest);

	pMeshContainer->pSkinInfo->UpdateSkinnedMesh(pMeshContainer->pRenderingMatrices, nullptr, pSour, pDest);

	pMeshContainer->pOriginalMesh->UnlockVertexBuffer();
	pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();

	return NOERROR;
}

HRESULT CMesh_Dynamic::SetTexture_OnShader(LPD3DXEFFECT pEffect, _uint iMeshContainerID, _uint iSubsetID, const char * pConstantName, MESHTEXTURE::TYPE eType)
{
	pEffect->SetTexture(pConstantName, m_MeshContainers[iMeshContainerID]->pMeshTexture[iSubsetID].pTextures[eType]);

	return NOERROR;
}


HRESULT CMesh_Dynamic::Update_CombinedTransformationMatrix(D3DXFRAME * pFrame, const D3DXMATRIX* pParentMatrix)
{
	D3DXFRAME_DERIVED*		pFrame_Derived = (D3DXFRAME_DERIVED*)pFrame;

	pFrame_Derived->CombinedTransformationMatrix = pFrame_Derived->TransformationMatrix * *pParentMatrix;

	if (nullptr != pFrame_Derived->pFrameFirstChild)
	{
		Update_CombinedTransformationMatrix(pFrame_Derived->pFrameFirstChild, &pFrame_Derived->CombinedTransformationMatrix);
	}

	if (nullptr != pFrame_Derived->pFrameSibling)
	{
		Update_CombinedTransformationMatrix(pFrame_Derived->pFrameSibling, pParentMatrix);
	}

	return NOERROR;
}

HRESULT CMesh_Dynamic::SetUp_CombinedMatrixPointer(D3DXFRAME* pFrame)
{
	if (nullptr != pFrame->pMeshContainer)
	{
		D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pFrame->pMeshContainer;

		m_MeshContainers.push_back(pMeshContainer);

		for (size_t i = 0; i < pMeshContainer->dwNumBones; ++i)
		{
			D3DXFRAME_DERIVED* pFindFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pMeshContainer->pSkinInfo->GetBoneName(i));

			pMeshContainer->ppCombinedTransformationMatrices[i] = &pFindFrame->CombinedTransformationMatrix;
		}
	}

	if (nullptr != pFrame->pFrameSibling)
		SetUp_CombinedMatrixPointer(pFrame->pFrameSibling);

	if (nullptr != pFrame->pFrameFirstChild)
		SetUp_CombinedMatrixPointer(pFrame->pFrameFirstChild);

	return NOERROR;
}

CMesh_Dynamic * CMesh_Dynamic::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pFilePath, const _tchar * pFileName)
{
	CMesh_Dynamic* pInstance = new CMesh_Dynamic(pGraphic_Device);

	if (FAILED(pInstance->Ready_Mesh_Dynamic(pFilePath, pFileName)))
	{
		_MSGBOX("CMesh_Dynamic Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CMesh_Dynamic::Clone()
{
	return new CMesh_Dynamic(*this);
}

void CMesh_Dynamic::Free()
{
	Safe_Release(m_pLoader);

	CComponent::Free();
}
