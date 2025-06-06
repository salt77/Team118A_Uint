#pragma once
#ifndef __Sukeban_MG_H__
#define __Sukeban_MG_H__

#include "Monster.h"

class ENGINE_DLL CSukeban_MG final : public CMonster
{
protected:
	explicit CSukeban_MG(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CSukeban_MG(const CSukeban_MG& rhs);
	virtual ~CSukeban_MG();
public:
	static CSukeban_MG*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	virtual void			Free() override;

public:
	virtual	HRESULT	Ready_Object() override;
	virtual _int	Start_Object(const _float& fTimeDelta) override;
	virtual	_int	Update_Object(const _float& fTimeDelta) override;
	virtual	_int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual	void	Render_Object(_int iPass) override;

	virtual void	Set_Constantable() override;

public:
	virtual void	OnEnable() override;
	virtual void	OnDisable() override;
	virtual void	OnDestroy() override;

protected:
	void			Trigger_Attack_1() override;

private:
};

#endif