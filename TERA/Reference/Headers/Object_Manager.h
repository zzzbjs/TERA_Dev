#pragma once
#include "Base.h"

// ��ü���� ��Ƽ� �����Ѵ�.
// �ΰ����� ��ü���� ����.(����, �ǻ���� ��ü)

_BEGIN(Engine)

class CLayer;
class CComponent;
class CGameObject;
class _DLL_EXPORTS CObject_Manager final : public CBase
{
	_DECLARE_SINGLETON(CObject_Manager)
private:
	explicit CObject_Manager();
	virtual ~CObject_Manager() = default;
public:
	const CComponent* Get_Component(const _uint& iSceneIdx, const _tchar* pLayerTag, const _tchar* pComponentTag, const _uint& iIndex = 0);
public:
	//��ü���� �����ϴ� �ʵ��� ���� �迭�� ũ�⸦ �����ش�,
	HRESULT Reserve_Object_Manager(const _uint& iMaxNumScene);
	//���� ��ü�� �ʿ� ����
	HRESULT Add_Object_Prototype(const _uint& iSceneIdx, const _tchar* pProtoTag, CGameObject* pInGameObject);
	//Ŭ�� ��ü�� ����
	HRESULT Add_Object(const _uint& iProtoSceneID, const _tchar* pProtoTag, const _uint& iSceneID, const _tchar* pLayerTag, void* pArg = nullptr);

	// [������ Clear_Object_Prototype�Լ��� �� ���� �ɰ�]
	//Ŭ�� ��ü�� ����
	HRESULT Clear_Object(const _uint& iSceneIdx);
	//���� ��ü�� ����
	HRESULT Clear_Prototype(const _uint& iSceneIdx);

	_int	Update_Object_Manager(const _float& fTimeDelta);
	_int	LateUpdate_Object_Manager(const _float& fTimeDelta);
public:
	CGameObject*	Find_Object_Prototype(const _uint& iSceneIdx, const _tchar* pProtoTag);
	// ���纻�ʿ��� ������Ʈ ���̾ �˻�
	CLayer*			FindObjectLayer(const _uint& iSceneIdx, const _tchar* pLayerTag);
private:
	_uint	m_iMaxNumScene = 0;

private: // ������ü���� �����ϴ� �����̳�.
	map<const _tchar*, CGameObject*>*			m_pmapPrototype = nullptr;
	typedef map<const _tchar*, CGameObject*>	MAPPROTOTYPE;

private: // �� ������ �� ����� ��ü���� �����ϴ� �����̳�.	
	map<const _tchar*, CLayer*>*		m_pmapObject = nullptr;
	typedef map<const _tchar*, CLayer*>	MAPOBJECT;
private:
	CLayer*			Find_Layer(const _uint& iSceneIdx, const _tchar* pLayerTag);
public:
	virtual void	Free();
};

_END