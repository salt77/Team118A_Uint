#include "Hod_Cannon.h"
#include "Hod.h"
#include "UI_Manager.h"
#include "UI_Damage.h"
#include "Boss_HpBar.h"
#include "CollisionManager.h"
#include "GameManager.h"

CHod_Cannon::CHod_Cannon(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CMonster(pGraphicDev, eObjTag, L"Hod_Cannon")
{
}

CHod_Cannon::CHod_Cannon(const CHod_Cannon & rhs)
	: CMonster(rhs)
{
}

CHod_Cannon::~CHod_Cannon()
{
}

CHod_Cannon * CHod_Cannon::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CHod_Cannon* pInstance = new CHod_Cannon(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHod_Cannon::Free()
{
	CMonster::Free();
}

HRESULT CHod_Cannon::Ready_Object()
{
	CMonster::Ready_Object();

	m_tUnitInfo.iAttack = 500;
	m_tUnitInfo.iCritical;
	m_tUnitInfo.iCriticalDamage;
	m_tUnitInfo.iCrowdReinforce;
	m_tUnitInfo.iCrowdResist;
	m_tUnitInfo.iDeffence;
	m_tUnitInfo.iEvasion;
	m_tUnitInfo.iHeal;
	m_tUnitInfo.iHitRatio;
	m_tUnitInfo.iMaxHp = 999999;
	m_tUnitInfo.iNowHp = m_tUnitInfo.iMaxHp;
	m_tUnitInfo.iRange = 0;
	m_tUnitInfo.iStability;

	m_szBodyBoneName = "body_Rot_01_Tan";
	m_szWeaponBoneName = "Point008";
	Create_Collider(&m_pPoint008, L"Col_Point008", 0.25f);

	m_tGlowAnim_FullBody.eGlowAnimMode = E_GlowAnimMode::Loop;
	m_tGlowAnim_FullBody.eEase = E_EASE::OutSine;
	m_tGlowAnim_FullBody.vStart_Min_UV = _vec2(0.f, 0.f);
	m_tGlowAnim_FullBody.vStart_Max_UV = _vec2(1.f, 1.f);
	m_tGlowAnim_FullBody.vEnd_Min_UV = _vec2(0.f, 0.f);
	m_tGlowAnim_FullBody.vEnd_Max_UV = _vec2(1.f, 1.f);
	m_tGlowAnim_FullBody.vBeginEnd_Power = _vec2(0.f, 1.f);
	m_tGlowAnim_FullBody.fGlowBlue = 1.f;
	m_tGlowAnim_FullBody.fGlowBright = 0.4f;

	m_tGlowAnim_ToNormal.eGlowAnimMode = E_GlowAnimMode::One;
	m_tGlowAnim_ToNormal.eEase = E_EASE::None;
	m_tGlowAnim_ToNormal.vStart_Min_UV = _vec2(1.f, 1.f);
	m_tGlowAnim_ToNormal.vStart_Max_UV = _vec2(1.f, 1.f);
	m_tGlowAnim_ToNormal.vEnd_Min_UV = _vec2(1.f, 1.f);
	m_tGlowAnim_ToNormal.vEnd_Max_UV = _vec2(1.f, 1.f);
	m_tGlowAnim_ToNormal.vBeginEnd_Power = _vec2(1.f, 1.f);
	m_tGlowAnim_ToNormal.fGlowBlue = 1.f;
	m_tGlowAnim_ToNormal.fGlowBright = 0.3f;

	return S_OK;
}

_int CHod_Cannon::Start_Object(const _float & fTimeDelta)
{
	CMonster::Start_Object(fTimeDelta);

	return RETURN_OK;
}

_int CHod_Cannon::Update_Object(const _float & fTimeDelta)
{
	CMonster::Update_Object(fTimeDelta);

	m_fStartDamage += fTimeDelta;

	if (m_fStartDamage >= 12.f)
	{
		m_bStartAttack = true;
	}

	CHod* pHod = static_cast<CHod*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Monster));
	if (pHod->Get_Dead())
	{
		m_tUnitInfo.iMaxHp = 0;
		m_tUnitInfo.iNowHp = m_tUnitInfo.iMaxHp;
		Set_Dead(true);

		return S_OK;
	}

	//Not Show HP
	m_pHealthBar->Set_Active(false);
	m_pCombatObject->Set_Active(false);

	Update_Attack(fTimeDelta);

	return RETURN_OK;
}

_int CHod_Cannon::LateUpdate_Object(const _float & fTimeDelta)
{
	CMonster::LateUpdate_Object(fTimeDelta);

	if (m_pDMeshCom && m_pGlowAnim)
	{
		m_pGlowAnim->fLeftTime += fTimeDelta;
		_float fPower = m_pGlowAnim->fGet_Power();
		_float fBright = m_pGlowAnim->fGlowBright;
		_float fBlue = m_pGlowAnim->fGlowBlue;
		_vec2 vMin = m_pGlowAnim->vGet_Min();
		_vec2 vMax = m_pGlowAnim->vGet_Max();

		m_pDMeshCom->Set_Glow_Option(0, fBright, _vec3(0.f, 0.f, fBlue), fPower, vMin.x, vMax.x, vMin.y, vMax.y, _vec3(1.f, 1.f, 1.f));

		if (m_pGlowAnim->fTotalTime < m_pGlowAnim->fLeftTime)
		{
			m_pGlowAnim->fLeftTime = 0.f;
			m_tGlowAnim_ToNormal.vStart_Min_UV = vMin;
			m_tGlowAnim_ToNormal.vStart_Max_UV = vMax;
			m_tGlowAnim_ToNormal.vEnd_Min_UV = vMin;
			m_tGlowAnim_ToNormal.vEnd_Max_UV = vMax;

			m_pGlowAnim = nullptr;
		}
	}

	return RETURN_OK;
}

void CHod_Cannon::Render_Object(_int iPass)
{
	CMonster::Render_Object(iPass);
}

void CHod_Cannon::Set_Constantable()
{
	CMonster::Set_Constantable();
}

void CHod_Cannon::Idle(_float fDeltaTime)
{
	m_pGlowAnim = &m_tGlowAnim_ToNormal;

	m_eMainAction = E_ACTION::Normal_Shoot;

	if (m_pTarget != nullptr)
	{
		if (m_pTarget->Get_UnitDead())
		{
			FSM_FunctionRelease();
			return;
		}
	}

	m_eCurrentAction = m_eMainAction;
}

void CHod_Cannon::OnEnable()
{
	CMonster::OnEnable();
}

void CHod_Cannon::OnDisable()
{
	CMonster::OnDisable();
}

void CHod_Cannon::OnDestroy()
{
	CMonster::OnDestroy();
}

void CHod_Cannon::Trigger_Attack_1()
{
	CHod* pHod = static_cast<CHod*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Monster));

	if (m_bStartAttack)
	{
		m_bAttack = true;
	}

	m_pBullet = CSkill_Manager::GetInstance()->SetSkill(L"Hod_Cannon.skill", E_LAYERTAG::Effect, m_pPoint008->GetPos(), Get_Angle(), VEC_ONE * 2.f);
}

void CHod_Cannon::Trigger_1()
{
	CHod* pHod = static_cast<CHod*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Monster));

	if (m_bStartAttack)
	{
		CSkill_Manager::GetInstance()->SetSkill(L"Binah_Shield_3.skill", E_LAYERTAG::Effect, pHod->Get_Col_Head()->GetPos() + _vec3(-2.f, 0.f, 2.f), Get_Angle(), VEC_ONE * 3.f);
		static_cast<CUnit*>(pHod)->Set_Damage(Get_UnitInfo(), Get_MonsterInfo().eAtkType);
		m_bAttack = false;
	}
}

void CHod_Cannon::Update_Collider(const _float & fTimeDelta)
{
	Collider_SetMatrix(m_pSphereCollider, m_szBodyBoneName);
	Collider_SetMatrix(m_pWeaponCollider, m_szWeaponBoneName);
	Collider_SetMatrix(m_pPoint008, m_szPoint008);
}

void CHod_Cannon::Update_Attack(const _float & fTimeDelta)
{
	CHod* pHod = static_cast<CHod*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Monster));

	if (m_bAttack)
	{
		_vec3 vSkillPos = m_pBullet->Get_Position();
		_vec3 vHodPosDest = pHod->Get_Position();
		_vec3 vecSkillDir = vHodPosDest - vSkillPos;
		D3DXVec3Normalize(&vecSkillDir, &vecSkillDir);

		_vec3 vMoveAmount = vecSkillDir * fTimeDelta * 3.f;
		m_pBullet->Add_Pos(vMoveAmount);
	}
}
