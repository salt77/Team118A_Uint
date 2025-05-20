#pragma once
#ifndef __HOD_Tower_H__
#define __HOD_Tower_H__

#include "BossMonster.h"

typedef struct tagGlowAnimationForTower
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

}T_GlowAnim_Tower;

class ENGINE_DLL CHod_Tower final : public CMonster
{
private:
	enum class E_Random_Guide
{
	Random_Guide_Cross, Random_Guide_Thron, Random_Guide_Elect,
	Random_Guide_END
};

protected:
	explicit			CHod_Tower(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit			CHod_Tower(const CHod_Tower& rhs);
	virtual				~CHod_Tower();

public:
	static CHod_Tower*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	virtual void		Free() override;

public:
	virtual	HRESULT		Ready_Object() override;
	virtual _int		Start_Object(const _float& fTimeDelta) override;
	virtual	_int		Update_Object(const _float& fTimeDelta) override;
	virtual	_int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual	void		Render_Object(_int iPass) override;

	virtual void		Set_Constantable() override;
	virtual void		Begin_Battle(_float fDeltaTime) override;
	virtual void		Dead(_float fDeltaTime);
	virtual void		Idle(_float fDeltaTime);
	void				Boss_Attack_1(_float fDeltaTime);
	void				Boss_Attack_2(_float fDeltaTime);

public:
	virtual void		OnEnable() override;
	virtual void		OnDisable() override;
	virtual void		OnDestroy() override;

protected:
	virtual void		Trigger_1() override;
	virtual void		Trigger_2() override;

public:
	void				Random_Pattern(_bool bRand);

private:
	_float				m_fGage = 0.f;
	E_ACTION			m_eMainAction = E_ACTION::ACTION_END;

private:
	E_Random_Guide		m_eRandomGuide = E_Random_Guide::Random_Guide_END;
	_int				m_iPatternRand = 0;

private:
	_bool				m_bGlowAnim = false;
	T_GlowAnim_Tower*	m_pGlowAnim = nullptr;
	T_GlowAnim_Tower	m_tGlowAnim_Arm;
	T_GlowAnim_Tower	m_tGlowAnim_FullBody;
	T_GlowAnim_Tower	m_tGlowAnim_ToNormal;
	
private:
	_bool				m_bSound = true;
};

#endif //__HOD_Tower_H__