#pragma once
#include "Defines.h"
#include "Base.h"
#include "Graphic_Device.h"

_BEGIN(Engine)
class CManagement;
_END

_BEGIN(Client)

class CMainApp : public CBase
{
private:
	explicit CMainApp();
	virtual ~CMainApp() = default;
public:
	HRESULT Ready_MainApp(); // 내 게임의 초기화 과정을 수행한다.
	_int	Update_MainApp(const _float& fTimeDelta);
	HRESULT Render_MainApp();
private:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;
private:
	CManagement*			m_pManagement = nullptr;
private: // For. FPS
	_uint	m_iRenderCnt = 0;
	_float	m_fTimeAcc = 0.f;
	_tchar	m_szFPS[128] = L"";
private:
	HRESULT Ready_Default_Setting(CGraphic_Device::WINMODE eType, const _uint& iWinCX, const _uint& iWinCY);
	HRESULT Ready_Scene(SCENEID eID);
public:
	static CMainApp* Create();	// 생성 관련된 함수
	virtual void Free();		// 소멸 관련된 함수.
};

_END