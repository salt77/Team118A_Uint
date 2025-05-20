#include "Hod_Tower.h"
#include "Hod.h"
#include "UI_Manager.h"
#include "UI_Damage.h"
#include "Boss_HpBar.h"
#include "CollisionManager.h"
#include "GameManager.h"
#include "Warning_Mgr.h"

CHod_Tower::CHod_Tower(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CMonster(pGraphicDev, eObjTag, L"Hod_Tower")
{
}

CHod_Tower::CHod_Tower(const CHod_Tower & rhs)
	: CMonster(rhs)
{
}

CHod_Tower::~CHod_Tower()
{
}

CHod_Tower * CHod_Tower::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CHod_Tower* pInstance = new CHod_Tower(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHod_Tower::Free()
{
	CMonster::Free();
}

HRESULT CHod_Tower::Ready_Object()
{
	CMonster::Ready_Object();

	m_szBodyBoneName = "bone_TemporaryTower_Body";
	m_szWeaponBoneName = "bone_TemporaryTower_Body";

	m_tUnitInfo.iAttack = 30;
	m_tUnitInfo.iCritical;
	m_tUnitInfo.iCriticalDamage;
	m_tUnitInfo.iCrowdReinforce;
	m_tUnitInfo.iCrowdResist;
	m_tUnitInfo.iDeffence;
	m_tUnitInfo.iEvasion;
	m_tUnitInfo.iHeal;
	m_tUnitInfo.iHitRatio;
	m_tUnitInfo.iMaxHp = 30000;
	m_tUnitInfo.iNowHp = m_tUnitInfo.iMaxHp;
	m_tUnitInfo.iRange = 3000;
	m_tUnitInfo.iStability;

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
	m_tGlowAnim_ToNormal.fGlowBlue = 0.f;
	m_tGlowAnim_ToNormal.fGlowBright = 0.05f;

	return RETURN_OK;
}

_int CHod_Tower::Start_Object(const _float & fTimeDelta)
{
	CMonster::Start_Object(fTimeDelta);

	return RETURN_OK;
}

_int CHod_Tower::Update_Object(const _float & fTimeDelta)
{
	CMonster::Update_Object(fTimeDelta);

	if (m_tUnitInfo.iNowHp <= 0)
	{
		m_fDeadShaderFilter += fTimeDelta;

		if (m_fDeadShaderFilter > 1.f)
		{
			Set_Active(false);
		}
	}

	return RETURN_OK;
}

_int CHod_Tower::LateUpdate_Object(const _float & fTimeDelta)
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

		m_pDMeshCom->Set_Glow_Option(0, fBright, _vec3(1.f, 1.f, fBlue), fPower, vMin.x, vMax.x, vMin.y, vMax.y, _vec3(1.f, 1.f, 1.f));

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

void CHod_Tower::Render_Object(_int iPass)
{
	CMonster::Render_Object(iPass);
}

void CHod_Tower::Set_Constantable()
{
	CMonster::Set_Constantable();
}

void CHod_Tower::Begin_Battle(_float fDeltaTime)
{
	if (m_bSound)
	{
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_HOD_EX3_Create_Tower.wav", 2.f);
		m_bSound = false;
	}

	m_eMainAction = E_ACTION::Appear;
	if (Is_AnimFinished() && m_eMainAction == m_eCurrentAction)
	{
		FSM_FunctionSetting(&CUnit::Idle, nullptr);
	}
	else
	{
		m_eCurrentAction = m_eMainAction;
	}
}

void CHod_Tower::Dead(_float fDeltaTime)
{
	m_pGlowAnim = &m_tGlowAnim_ToNormal;
	m_eMainAction = E_ACTION::Vital_Death;
	m_pTarget = nullptr;
}

void CHod_Tower::Idle(_float fDeltaTime)
{
	_int iRand = CWall512::GetInstance()->GetValue(0, 100);

	if (iRand < 20)
	{
		FSM_FunctionSetting(&CUnit::Boss_Attack_1, nullptr);
	}
	else if (iRand < 40)
	{
		FSM_FunctionSetting(&CUnit::Boss_Attack_2, nullptr);
	}

	m_eMainAction = E_ACTION::Normal_Idle;

	if (m_pTarget != nullptr)
	{
		if (m_pTarget->Get_UnitDead())
		{
			m_pGlowAnim = &m_tGlowAnim_ToNormal;

			FSM_FunctionRelease();
			return;
		}
	}

	m_eCurrentAction = m_eMainAction;
}

void CHod_Tower::Boss_Attack_1(_float fDeltaTime)
{
	m_eMainAction = E_ACTION::Normal_Shoot;
	m_pGlowAnim = &m_tGlowAnim_FullBody;

	if (Is_AnimFinished() && m_eMainAction == m_eCurrentAction)
	{
		FSM_FunctionSetting(&CUnit::Idle, nullptr);
	}
	else
	{
		m_eCurrentAction = m_eMainAction;
	}
}

void CHod_Tower::Boss_Attack_2(_float fDeltaTime)
{
	m_eMainAction = E_ACTION::ExSkill;
	m_pGlowAnim = &m_tGlowAnim_FullBody;

	if (Is_AnimFinished() && m_eMainAction == m_eCurrentAction)
	{
		FSM_FunctionSetting(&CUnit::Idle, nullptr);
	}
	else
	{
		m_eCurrentAction = m_eMainAction;
	}
}

void CHod_Tower::OnEnable()
{
	FSM_FunctionSetting(&CUnit::Begin_Battle, &CUnit::Idle);
	m_eCurrentAction = E_ACTION::Appear;
	Animation_Change(0.f);
	m_pTarget = nullptr;
	m_fDeadShaderFilter = 0.f;
	m_bDead = false;
	Clear_Astar();
}

void CHod_Tower::OnDisable()
{
	CMonster::OnDisable();
}

void CHod_Tower::OnDestroy()
{
	CMonster::OnDestroy();
}

void CHod_Tower::Trigger_1()
{
	Random_Pattern(true);
}

void CHod_Tower::Trigger_2()
{
	Random_Pattern(true);
}

void CHod_Tower::Random_Pattern(_bool bRand)
{
	Find_Target_InRange(m_eTargetLayerTag, m_eTargetObjTag);

	if (nullptr == m_pTarget)
	{
		return;
	}

	if (bRand)
	{
		_int iRand = CWall512::GetInstance()->GetValue(0, (_int)E_Random_Guide::Random_Guide_END);
		m_eRandomGuide = E_Random_Guide(iRand);
	}

	_int iWarnTypeRand = CWall512::GetInstance()->GetValue(0, (_int)E_WarnType::WarnEnd);
	CWarning* pWarning = nullptr;
	_vec3 vCenter = (m_pTarget->Get_Position() + Get_Position()) * 0.5f;
	_float fAddY = 0.001f;
	vCenter.y = 0.1f;

	switch (m_eRandomGuide)
	{
	case E_Random_Guide::Random_Guide_Cross:
	{
		for (_int i = 0; i < 12; ++i)
		{
			fAddY += 0.001f;
			pWarning = CWarning::Create(m_pGraphicDev);
			_vec3 vAddPos = _vec3(CWall512::GetInstance()->GetFloatValue(-10.f, 10.f), 0.f, CWall512::GetInstance()->GetFloatValue(-10.f, 10.f));
			vAddPos.y += fAddY;
			pWarning->Set_Warning(E_WarnType::Circle_Fill, E_WarnType::Circle_InsideFill, vCenter + vAddPos, 2.f, CWall512::GetInstance()->GetFloatValue(0.f, 1.5f), 3.f, D3DXCOLOR(C_Blue));
			pWarning->Set_Damage(100);
			pWarning->Set_AfterSkillName(L"Hod_Cross.skill", VEC_ZERO, VEC_ONE * 2.f);
			pWarning->Set_SoundName(L"SFX_HOD_Cross.wav", 2.f);
			CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pWarning);
		}
	}
	break;
	case E_Random_Guide::Random_Guide_Thron:
	{
		for (_int i = 0; i < 1; ++i)
		{
			pWarning = CWarning::Create(m_pGraphicDev);
			fAddY += 0.001f;
			vCenter.y = 0.1f;
			pWarning->Set_Warning(E_WarnType::Circle_Fill, E_WarnType::Circle_InsideFill, vCenter, 2.f, 0.f, 3.5f);
			pWarning->Set_AfterSkillName(L"Hod_Thron.skill", VEC_ZERO, VEC_ONE * 2.f);
			pWarning->Set_SoundName(L"SFX_HOD_Thron.wav", 2.f);
			pWarning->Set_Damage(100);
			CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pWarning);

			CWarning_Mgr::GetInstance()->Create_Circle(pWarning, 0.f, 0.f, 6, i * 3.0f, 0.1f, vCenter + _vec3(0.f, fAddY, i * 2.f), 0.3f, 5.f, C_Blue, C_White);
			CWarning_Mgr::GetInstance()->Create_Circle(pWarning, 60.f, 30.f, 6, i * 3.0f, 0.1f, vCenter + _vec3(0.f, fAddY, i * 2.f), 0.3f, 5.f, C_Blue, C_White);
			CWarning_Mgr::GetInstance()->Create_Circle(pWarning, 120.f, 60.f, 6, i * 3.0f, 0.1f, vCenter + _vec3(0.f, fAddY, i * 2.f), 0.3f, 5.f, C_Blue, C_White);
			CWarning_Mgr::GetInstance()->Create_Circle(pWarning, 180.f, 90.f, 6, i * 3.0f, 0.1f, vCenter + _vec3(0.f, fAddY, i * 2.f), 0.3f, 5.f, C_Blue, C_White);
			CWarning_Mgr::GetInstance()->Create_Circle(pWarning, 240.f, 120.f, 6, i * 3.0f, 0.1f, vCenter + _vec3(0.f, fAddY, i * 2.f), 0.3f, 5.f, C_Blue, C_White);
			CWarning_Mgr::GetInstance()->Create_Circle(pWarning, 300.f, 150.f, 6, i * 3.0f, 0.1f, vCenter + _vec3(0.f, fAddY, i * 2.f), 0.3f, 5.f, C_Blue, C_White);
			CWarning_Mgr::GetInstance()->Create_Circle(pWarning, 360.f, 180.f, 6, i * 3.0f, 0.1f, vCenter + _vec3(0.f, fAddY, i * 2.f), 0.3f, 5.f, C_Blue, C_White);
		}
	}
	break;
	case E_Random_Guide::Random_Guide_Elect:
	{
		for (_int i = 0; i < 12; ++i)
		{
			fAddY += 0.001f;
			pWarning = CWarning::Create(m_pGraphicDev);
			_vec3 vAddPos = _vec3(CWall512::GetInstance()->GetFloatValue(-10.f, 10.f), 0.f, CWall512::GetInstance()->GetFloatValue(-10.f, 10.f));
			vAddPos.y += fAddY;
			pWarning->Set_Warning(E_WarnType::Circle_Fill, E_WarnType::Circle_InsideFill, vCenter + vAddPos, 2.f, CWall512::GetInstance()->GetFloatValue(0.f, 1.5f), 3.f, D3DXCOLOR(C_Blue));
			pWarning->Set_Damage(100);
			pWarning->Set_AfterSkillName(L"Hod_EXS4_Hit.skill", VEC_ZERO, VEC_ONE * 2.f);
			pWarning->Set_SoundName(L"SFX_HOD_EX4_Quake.wav", 2.f);
			CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pWarning);
		}
	}
	break;
	default:
		break;
	}

	/*Safe_Release(pWarning);*/
}