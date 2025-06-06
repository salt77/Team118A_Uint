#include "KumabotPapa_Slumpia_SG.h"
#include "UI_Damage.h"

//생성할때 자신의 태그를 입력해서 자신의 메쉬를 불러오도록 함
CKumabotPapa_Slumpia_SG::CKumabotPapa_Slumpia_SG(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CMonster(pGraphicDev, eObjTag, L"KumabotPapa_Slumpia_SG")
{
}

CKumabotPapa_Slumpia_SG::CKumabotPapa_Slumpia_SG(const CKumabotPapa_Slumpia_SG & rhs)
	:CMonster(rhs)
{
}

CKumabotPapa_Slumpia_SG::~CKumabotPapa_Slumpia_SG()
{
}

CKumabotPapa_Slumpia_SG * CKumabotPapa_Slumpia_SG::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CKumabotPapa_Slumpia_SG* pInstance = new CKumabotPapa_Slumpia_SG(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKumabotPapa_Slumpia_SG::Free()
{
	CMonster::Free();
}

HRESULT CKumabotPapa_Slumpia_SG::Ready_Object()
{
	CMonster::Ready_Object();

	m_szBodyBoneName = "Bip001 Pelvis";
	m_szWeaponBoneName = "bone_Rgun1";

	Set_AmmoMax(30);

	m_tUnitInfo.iAttack = 300;
	m_tUnitInfo.iCritical;
	m_tUnitInfo.iCriticalDamage;
	m_tUnitInfo.iCrowdReinforce;
	m_tUnitInfo.iCrowdResist;
	m_tUnitInfo.iDeffence;
	m_tUnitInfo.iEvasion;
	m_tUnitInfo.iHeal;
	m_tUnitInfo.iHitRatio;
	m_tUnitInfo.iMaxHp = 5000;
	m_tUnitInfo.iNowHp = 5000;
	m_tUnitInfo.iRange = 300;
	m_tUnitInfo.iStability;

	return S_OK;
}

_int CKumabotPapa_Slumpia_SG::Start_Object(const _float & fTimeDelta)
{
	CMonster::Start_Object(fTimeDelta);

	return RETURN_OK;
}

_int CKumabotPapa_Slumpia_SG::Update_Object(const _float & fTimeDelta)
{
	CMonster::Update_Object(fTimeDelta);

	return RETURN_OK;
}

_int CKumabotPapa_Slumpia_SG::LateUpdate_Object(const _float & fTimeDelta)
{
	CMonster::LateUpdate_Object(fTimeDelta);

	return RETURN_OK;
}

void CKumabotPapa_Slumpia_SG::Render_Object(_int iPass)
{
	CMonster::Render_Object(iPass);
}

void CKumabotPapa_Slumpia_SG::Set_Constantable()
{
	CMonster::Set_Constantable();
}

void CKumabotPapa_Slumpia_SG::OnEnable()
{
	CMonster::OnEnable();
}

void CKumabotPapa_Slumpia_SG::OnDisable()
{
	CMonster::OnDisable();
}

void CKumabotPapa_Slumpia_SG::OnDestroy()
{
	CMonster::OnDestroy();
}

void CKumabotPapa_Slumpia_SG::Trigger_Attack_1()
{
	Hit_Target();

	_vec3 vDirLook		= *D3DXVec3Normalize(&vDirLook,		&Get_Info(E_INFO::INFO_LOOK));
	_vec3 vDirRight		= *D3DXVec3Normalize(&vDirRight,	&Get_Info(E_INFO::INFO_RIGHT));
	_vec3 vDirUp		= *D3DXVec3Normalize(&vDirUp,		&Get_Info(E_INFO::INFO_UP));
	_float fLookRatio	= 0.4f;
	_float fRightRatio	= 0.0f;
	_float fUpRatio		= 0.05f;

	_vec3 vWeaponPos = m_pWeaponCollider->GetPos();
	vWeaponPos += vDirLook	* fLookRatio;
	vWeaponPos += vDirRight	* fRightRatio;
	vWeaponPos += vDirUp	* fUpRatio;

	CSkill_Manager::GetInstance()->SetSkill(L"Test_Minhyun_1231_Fire_00.skill", E_LAYERTAG::Effect, vWeaponPos, Get_Angle());
}