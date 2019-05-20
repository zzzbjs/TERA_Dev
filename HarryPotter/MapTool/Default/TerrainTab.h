#pragma once
#include "afxcmn.h"
#include "Tool_Defines.h"
#include "afxwin.h"
#include "EventManagerTool.h"

// CTerrainTab 대화 상자입니다.
class CSceneTerrain;
class CTerrainTab : public CDialogEx
{
	DECLARE_DYNAMIC(CTerrainTab)

public:
	CTerrainTab(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTerrainTab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TERRAINTAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CSceneTerrain*	m_pScene = nullptr;

	virtual void OnOK();
	virtual BOOL OnInitDialog();

	CButton Terrain_RenderST_Solild;
	CButton Terrain_RenderCood_Axis;

	afx_msg void OnBnClickedWireFrame();
	afx_msg void OnBnClickedSolid();

	CEdit Vertex_X_Count;
	CEdit Vertex_Z_Count;
	CEdit Vertex_Interval;
	CEdit Vertex_Detail;
	CEdit PositionX;
	CEdit PositionY;
	CEdit PositionZ;
	CEdit RotationX;
	CEdit RotationY;
	CEdit RotationZ;

	CSpinButtonCtrl Vertex_X_Count_Btn;
	CSpinButtonCtrl Vertex_Z_Count_Btn;
	CSpinButtonCtrl Vertex_Interval_Btn;
	CSpinButtonCtrl Vertex_Detail_Btn;
	CSpinButtonCtrl PositionX_Btn;
	CSpinButtonCtrl PositionY_Btn;
	CSpinButtonCtrl PositionZ_Btn;
	CSpinButtonCtrl RotaionX_Btn;
	CSpinButtonCtrl RotaionY_Btn;
	CSpinButtonCtrl RotaionZ_Btn;

public:
	afx_msg void OnBnClickedTerrain_Apply();
	afx_msg void OnSpin_Trans_PosX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSpin_Trans_PosY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSpin_Trans_PosZ(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSpin_Trans_RotX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSpin_Trans_RotY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSpin_Trans_RotZ(NMHDR *pNMHDR, LRESULT *pResult);

public:
	void InitTreeCtrl();

	afx_msg void OnTree_Terrain_Texture(NMHDR *pNMHDR, LRESULT *pResult);

	CStatic m_Terrain_Texture;
	CTreeCtrl m_Tree_Terrain_Texture;

	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	map<CString, CImage*>		m_mapPngImage;
	CString						strImagName;

private:
	// Terrain_Vertex_Variable
	_int	m_iNumVtxX = 0;
	_int	m_iNumVtxZ = 0;
	_float	m_fInterval = 0;
	_float	m_fDetail = 0;

	// Terrain_Transform_Variable
	_int	m_iPosX = 0;
	_int	m_iPosY = 0;
	_int	m_iPosZ = 0;
	_float	m_fRotX = 0.f;
	_float	m_fRotY = 0.f;
	_float	m_fRotZ = 0.f;

private:
	void Rotation_Axis(const _vec3 & vAxis, const _float & fRadianPerSec);
	void ImageProcess(HTREEITEM	_hSelected);
	void ImageSizing(CImage* pImage);

};