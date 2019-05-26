#pragma once
#include "afxcmn.h"
#include "Tool_Defines.h"
#include "afxwin.h"
#include "Scene.h"

// CMeshTab 대화 상자입니다.
class CMeshTab : public CDialogEx
{
	DECLARE_DYNAMIC(CMeshTab)

public:
	CMeshTab(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMeshTab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MESHTAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CScene*	m_pScene = nullptr;

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

	CButton Mesh_RenderST_Solild;
	CButton Mesh_MouseSelType_Object;
	CButton Mesh_ObjectType_Static;
	CButton Mesh_NaviVtxMove_Together;

	CTreeCtrl Tree_Mesh_Object;
	CTreeCtrl Tree_Mesh_StaticObj;

	CEdit Obj_X_Scaling;
	CEdit Obj_Y_Scaling;
	CEdit Obj_Z_Scaling;
	CEdit Obj_X_Rotation;
	CEdit Obj_Y_Rotation;
	CEdit Obj_Z_Rotation;
	CEdit Obj_X_Position;
	CEdit Obj_Y_Position;
	CEdit Obj_Z_Position;

	CSpinButtonCtrl Obj_X_Scaling_Btn;
	CSpinButtonCtrl Obj_Y_Scaling_Btn;
	CSpinButtonCtrl Obj_Z_Scaling_Btn;
	CSpinButtonCtrl Obj_X_Rotation_Btn;
	CSpinButtonCtrl Obj_Y_Rotation_Btn;
	CSpinButtonCtrl Obj_Z_Rotation_Btn;
	CSpinButtonCtrl Obj_X_Position_Btn;
	CSpinButtonCtrl Obj_Y_Position_Btn;
	CSpinButtonCtrl Obj_Z_Position_Btn;

public:
	afx_msg void OnBnClickedSolid();
	afx_msg void OnBnClickedWireFrame();
	afx_msg void OnBnClickedObject();
	afx_msg void OnBnClickedNavi_Mesh();
	afx_msg void OnBnClickedStatic();
	afx_msg void OnBnClickedDynamic();
	afx_msg void OnBnClickedTogether();
	afx_msg void OnBnClickedSelected();

	afx_msg void OnTree_Mesh_Object(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTree_Mesh_StaticObj(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnNMClickTreeMeshObject(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickTreeMeshObject(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickTreeStaticObj(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnSpin_Scaling_X(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSpin_Scaling_Y(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSpin_Scaling_Z(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSpin_Rotate_X(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSpin_Rotate_Y(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSpin_Rotate_Z(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSpin_Position_X(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSpin_Position_Y(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSpin_Position_Z(NMHDR *pNMHDR, LRESULT *pResult);

public:
	// Mesh_Transform_Variable
	float m_fScalingX;
	float m_fScalingY;
	float m_fScalingZ;
	float m_fRotX;
	float m_fRotY;
	float m_fRotZ;
	float m_fPosX;
	float m_fPosY;
	float m_fPosZ;

	CString	strLayerTag;

public:
	HRESULT		Add_StaticObject();
	HRESULT		MakeItemForTree();

private:
	HTREEITEM		Root = nullptr;
	HTREEITEM		SelectedObjectItem = nullptr;
	HTREEITEM		SelectedStaticObject = nullptr;
	TCHAR			szFullPath[MAX_PATH] = L"";
	CString			strComponentPrototypeTag;
	CString			strObjectName;
	CString			strXfileName;
	TCHAR			szFullPathForAdd[MAX_PATH] = L"";
	CGameObject*	pSelectedObj = nullptr;
	_bool			bDblClkTreeStaticObj = false;
	map<CString, HTREEITEM>	mapTreeItem;
	map<CString, CString> mapStaticObj_Com_Prototype;

private:
	void		InitTreeCtrl_Object();
	HRESULT		MakeArgVariableForStaticObj();

};
