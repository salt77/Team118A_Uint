#pragma once
#ifndef __HOD_CANNON_H__
#define __HOD_CANNON_H__

#include "BossMonster.h"

typedef struct tagGlowAnimationForCannon
{
	_float Get_Ratio()
	{
		_float fRatio = fLeftTime / fTotalTime;

		if (eEase != E_EASE::None)
		{
			fRatio = CUtility::LerpEase_float(eEase, fRatio);
		}
		return fRatio;
	}

	_vec2 vGet_Min()
	{
		return _vec2(CUtility::Lerp_float(vStart_Min_UV.x, vEnd_Min_UV.x, Get_Ratio()), CUtility::Lerp_float(vStart_Min_UV.y, vEnd_Min_UV.y, Get_Ratio()));
	}

	_vec2 vGet_Max()
	{
		return _vec2(CUtility::Lerp_float(vStart_Max_UV.x, vEnd_Max_UV.x, Get_Ratio()), CUtility::Lerp_float(vStart_Max_UV.y, vEnd_Max_UV.y, Get_Ratio()));
	}

	_float fGet_Power()
	{
		return CUtility::Lerp_float(vBeginEnd_Power.x, vBeginEnd_Power.y, Get_Ratio());
	}

	E_GlowAnimMode eGlowAnimMode = E_GlowAnimMode::One;

	E_EASE eEase = E_EASE::None;
	_int iLoopTime = 10;
	_float fTotalTime = 3.f;
	_float fLeftTime = 0.f;
	_float fGlowBright = 0.f;
	_float fGlowBlue = 1.f;

	_vec2 vBeginEnd_Power = _vec2(0.f, 1.f);
	_vec2 vStart_Min_UV = _vec2(0.f, 0.f);
	_vec2 vStart_Max_UV = _vec2(1.f, 1.f);
	_vec2 vEnd_Min_UV = _vec2(0.f, 0.f);
	_vec2 vEnd_Max_UV = _vec2(1.f, 1.f);

}T_GlowAnim_Cannon;

class ENGINE_DLL CHod_Cannon final : public CMonster
{
protected:
	explicit			CHod_Cannon(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit			CHod_Cannon(const CHod_Cannon& rhs);
	virtual				~CHod_Cannon();

public:
	static CHod_Cannon*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	virtual void		Free() override;

public:
	virtual	HRESULT		Ready_Object() override;
	virtual _int		Start_Object(const _float& fTimeDelta) override;
	virtual	_int		Update_Object(const _float& fTimeDelta) override;
	virtual	_int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual	void		Render_Object(_int iPass) override;

	virtual void		Set_Constantable() override;
	virtual void		Idle(_float fDeltaTime);

public:
	virtual void		OnEnable() override;
	virtual void		OnDisable() override;
	virtual void		OnDestroy() override;

protected:
	virtual void		Trigger_Attack_1() override;
	virtual void		Trigger_1() override;

protected:
	virtual void		Update_Collider(const _float& fTimeDelta);

public:
	void				Update_Attack(const _float& fTimeDelta);

private:
	_float				m_fGage = 0.f;
	E_ACTION			m_eMainAction = E_ACTION::ACTION_END;

private:
	_bool				m_bGlowAnim = false;
	T_GlowAnim_Cannon*	m_pGlowAnim = nullptr;
	T_GlowAnim_Cannon	m_tGlowAnim_Arm;
	T_GlowAnim_Cannon	m_tGlowAnim_FullBody;
	T_GlowAnim_Cannon	m_tGlowAnim_ToNormal;

private:
	CSphereCollider*	m_pPoint008 = nullptr;
	const char*			m_szPoint008 = "Point008";

private:
	CSkillObject*		m_pBullet = nullptr;
	_bool				m_bAttack = false;

private:
	_float				m_fStartDamage = 0.f;
	_bool				m_bStartAttack = false;
};

#endif //__HOD_CANNON_H__