#include "..\Headers\Object_Manager.h"
#include "GameObject.h"
#include "Layer.h"

_IMPLEMENT_SINGLETON(CObject_Manager)

CObject_Manager::CObject_Manager()
{
}

const CComponent * CObject_Manager::Get_Component(const _uint & iSceneIdx, const _tchar * pLayerTag, const _tchar * pComponentTag, const _uint & iIndex)
{
	if (m_iMaxNumScene <= iSceneIdx ||
		nullptr == m_pmapObject)
		return nullptr;

	CLayer*		pLayer = Find_Layer(iSceneIdx, pLayerTag);

	if (nullptr == pLayer)
		return nullptr;

	return pLayer->Get_Component(pComponentTag, iIndex);
}

HRESULT CObject_Manager::Reserve_Object_Manager(const _uint & iMaxNumScene)
{
	// ���� �����ϸ� Ready_Object_Manager ������ �ƴұ� �����ȴ�.

	if (nullptr != m_pmapPrototype ||
		nullptr != m_pmapObject)
		return E_FAIL;

	// ����� Engine�̰� Client���� �̰͵��� ��� �� ���ε�..
	// Ŭ��� �������̰� ������ ���� �� �� ���
	// �� ������ �����ϰ�, ������ ������Ʈ���� ���� �����ϱ����� �䷸�� �����Ѵ�.
	m_pmapPrototype = new MAPPROTOTYPE[iMaxNumScene];
	m_pmapObject = new MAPOBJECT[iMaxNumScene];
	m_iMaxNumScene = iMaxNumScene;

	return NOERROR;
}

HRESULT CObject_Manager::Add_Object_Prototype(const _uint & iSceneIdx, const _tchar * pProtoTag, CGameObject * pInGameObject)
{
	if (m_iMaxNumScene <= iSceneIdx ||
		nullptr == m_pmapPrototype ||
		nullptr == pInGameObject)
		return E_FAIL;

	// ���� ��ü�� �߰��ϱ� ���� �ش� ��ü�� �̹� �����ϴ��� ���� Ȯ���ϴ� �Լ��� �����Ѵ�.
	CGameObject*	pGameObject = Find_Object_Prototype(iSceneIdx, pProtoTag);

	// �ش� ��ü�� �����ϸ�... ���ȰŶ��..?
	// �׳� �̹� �����ϴϱ� ���з� �������ִ� �� ������..
	// �޽��� �ڽ��� ����ִ°� ���� ������?
	if (nullptr != pGameObject)
		return E_FAIL;

	// �̰� �̷��� ���ִ� ������,
	// �޾ƿ� _tchar �������� pProtoTag�� ���ͷ� ����� �ϸ� (L"" �̷���)�� ������ ���µ�,
	// �� ������ �����ھ� ������ ������ �����. ���� �ȿ� �ִ� �����Ͱ� ����Ű�� ���� ���Ҷ����� ���ǹ�����. �̸��� ��������� ����� ��Ȳ.
	// �׷��� �̷��� �����迭�� �������༭ �־��ش�.
	_tchar* dynamicArray = new _tchar[lstrlen(pProtoTag) + 1];
	lstrcpy(dynamicArray, pProtoTag);

	//���� ��ü���� �����ϴ� �����̳ʿ� ���ڷ� ���� ���� ������Ʈ�� �Ⱦ� �־� �ݴϴ�.
	m_pmapPrototype[iSceneIdx].insert(MAPPROTOTYPE::value_type(dynamicArray, pInGameObject));

	return NOERROR;
}

HRESULT CObject_Manager::Add_Object(const _uint & iProtoSceneID, const _tchar * pProtoTag, const _uint & iSceneID, const _tchar * pLayerTag, void* pArg)
{
	// ���� �ش� ������Ʈ�� ���� ��ü�� �����ϴ��� �˻��ϰ�
	CGameObject*	pPrototype = Find_Object_Prototype(iProtoSceneID, pProtoTag);

	// ���ٸ�.. ���Ȱ���..?
	if (nullptr == pPrototype)
		return E_FAIL;

	// �ִٸ� �ش� ������ü�� ���� ����������. �̶� �����ϴ°� Clone �ǽðڴ�.
	CGameObject*	pGameObject = pPrototype->Clone(pArg);
	if (nullptr == pGameObject)
		return E_FAIL;

	// ���ڷ� ���� ���� �ǻ�� ��ü�� �����ϴ��� Ȯ���ϰ�
	CLayer*		pLayer = Find_Layer(iSceneID, pLayerTag);

	// ���̾ �����ٸ�,
	// ���̾ ���� ��ü�� �߰� ��, ���̾ �߰��Ѵ�.
	if (nullptr == pLayer) 
	{
		pLayer = CLayer::Create();
		if (nullptr == pLayer)
			return E_FAIL;

		// CLayer Class �� m_ObjectList ����Ʈ�� push_back �Ѵ�.
		if (FAILED(pLayer->Add_ObjectToLayer(pGameObject)))
			return E_FAIL;

		_tchar* dynamicArray = new _tchar[lstrlen(pLayerTag) + 1];
		lstrcpy(dynamicArray, pLayerTag);

		m_pmapObject[iSceneID].insert(MAPOBJECT::value_type(dynamicArray, pLayer));
	}
	else // �־��ٸ� �׳� �߰��Ѵ�.
	{
		if (FAILED(pLayer->Add_ObjectToLayer(pGameObject)))
			return E_FAIL;
	}

	return NOERROR;
}

HRESULT CObject_Manager::Clear_Object(const _uint & iSceneIdx)
{
	if (m_iMaxNumScene <= iSceneIdx ||
		nullptr == m_pmapObject)
		return E_FAIL;

	for (auto& ObjectLayerPair : m_pmapObject[iSceneIdx])
	{
		_tchar* pTempArray = const_cast<_tchar*>(ObjectLayerPair.first);
		Safe_Delete_Array(pTempArray);
		Safe_Release(ObjectLayerPair.second);
	}

	m_pmapObject[iSceneIdx].clear();

	return NOERROR;
}

HRESULT CObject_Manager::Clear_Prototype(const _uint & iSceneIdx)
{
	if (m_iMaxNumScene <= iSceneIdx ||
		nullptr == m_pmapPrototype)
		return E_FAIL;

	for (auto& ProtypePair : m_pmapPrototype[iSceneIdx])
	{
		_tchar* pTempArray = const_cast<_tchar*>(ProtypePair.first);
		Safe_Delete_Array(pTempArray);
		Safe_Release(ProtypePair.second);
	}

	m_pmapPrototype[iSceneIdx].clear();

	return NOERROR;
}

_int CObject_Manager::Update_Object_Manager(const _float & fTimeDelta)
{
	if (nullptr == m_pmapObject)
		return -1;
	_int		iExitCode = 0;

	for (size_t i = 0; i < m_iMaxNumScene; ++i)
	{
		for (auto& Pair : m_pmapObject[i])
		{
			// �� ��� ��ü���� ObjectList�� ����ִ� pGameObject���� Update�� �����ϴ� �����̴�.
			// ������Ʈ �Ŵ����� ���� �� �����̳ʿ� ����� �ִ� �� ��� ������Ʈ���� Update�� ���ش�.
			// �� ��, ������Ʈ���� List�� ������ �ִ� Layer Class�� �����Ͽ� Update_Layer �Լ��� �����Ѵ�.
			iExitCode = Pair.second->Update_Layer(fTimeDelta);
			if (iExitCode & 0x80000000)
				return iExitCode;
		}
	}

	return _int(iExitCode);
}

_int CObject_Manager::LateUpdate_Object_Manager(const _float & fTimeDelta)
{
	// ���� ������ ����ε� ������ �ѹ� ���θ��� ����?

	if (nullptr == m_pmapObject)
		return -1;

	_int		iExitCode = 0;

	for (size_t i = 0; i < m_iMaxNumScene; ++i)
	{
		for (auto& Pair : m_pmapObject[i])
		{
			iExitCode = Pair.second->LateUpdate_Layer(fTimeDelta);
			if (iExitCode & 0x80000000)
				return iExitCode;
		}
	}

	return _int(iExitCode);
}

CLayer * CObject_Manager::FindObjectLayer(const _uint & iSceneIdx, const _tchar * pLayerTag)
{
	if (m_iMaxNumScene <= iSceneIdx ||
		nullptr == m_pmapObject)
		return nullptr;

	auto	iter = find_if(m_pmapObject[iSceneIdx].begin(), m_pmapObject[iSceneIdx].end(),
		[&pLayerTag](auto& Pair)->bool {return !lstrcmp(pLayerTag, Pair.first) ? true : false; });

	if (iter == m_pmapObject[iSceneIdx].end())
		return nullptr;

	return iter->second;
}

CGameObject * CObject_Manager::Find_Object_Prototype(const _uint & iSceneIdx, const _tchar * pProtoTag)
{
	if (m_iMaxNumScene <= iSceneIdx ||
		nullptr == m_pmapPrototype)
		return nullptr;

	// ã�Ƽ� iter�� ������ �־��ּ���.
	auto	iter = find_if(m_pmapPrototype[iSceneIdx].begin(), m_pmapPrototype[iSceneIdx].end(), CFinder_Tag(pProtoTag));
	//auto	iter = find_if(m_pMapPrototype[iSceneIdx].begin(), m_pMapPrototype[iSceneIdx].end(), [&](MAPPROTOTYPE::value_type Pair)->bool {if (0 == lstrcmp(pProtoTag, Pair.first)) return true; return false; });

	// ������ nullpter�� return �����ϴ�.
	if (iter == m_pmapPrototype[iSceneIdx].end())
		return nullptr;

	return iter->second;
}

CLayer * CObject_Manager::Find_Layer(const _uint & iSceneIdx, const _tchar * pLayerTag)
{
	// �� ��� ��ü���� �����ϴ� �����̳ʸ� ������ ���̴�.

	if (m_iMaxNumScene <= iSceneIdx ||
		nullptr == m_pmapObject)
		return nullptr;

	auto	iter = find_if(m_pmapObject[iSceneIdx].begin(), m_pmapObject[iSceneIdx].end(), CFinder_Tag(pLayerTag));

	if (iter == m_pmapObject[iSceneIdx].end())
		return nullptr;

	return iter->second;
}

void CObject_Manager::Free()
{
	for (size_t i = 0; i < m_iMaxNumScene; i++)
	{
		for (auto& Pair : m_pmapPrototype[i])
		{
			_tchar* pTempArray = const_cast<_tchar*>(Pair.first);
			Safe_Delete_Array(pTempArray);
			Safe_Release(Pair.second);
		}

		m_pmapPrototype[i].clear();
	}
	Safe_Delete_Array(m_pmapPrototype);

	for (size_t i = 0; i < m_iMaxNumScene; i++)
	{
		for (auto& Pair : m_pmapObject[i])
		{
			_tchar* pTempArray = const_cast<_tchar*>(Pair.first);
			Safe_Delete_Array(pTempArray);
			Safe_Release(Pair.second);
		}

		m_pmapObject[i].clear();
	}
	Safe_Delete_Array(m_pmapObject);
}