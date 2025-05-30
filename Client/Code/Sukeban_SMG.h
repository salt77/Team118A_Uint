#pragma once

#ifndef __SUKEBAN_SMG_H__
#define __SUKEBAN_SMG_H__

#include "Monster.h"

class CSukeban_SMG : public CMonster
{
protected:
	explicit CSukeban_SMG(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CSukeban_SMG(const CSukeban_SMG& rhs);
	virtual ~CSukeban_SMG();
public:
	static CSukeban_SMG*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	virtual void			Free() override;

public:
	virtual	HRESULT	Ready_Object() override;
	virtual _int	Start_Object(const _float& fTimeDelta) override;
	virtual	_int	Update_Object(const _float& fTimeDelta) override;
	virtual	_int	LateUpdate_Object(const _float& fTimeDelta) override;

	virtual void	Set_Constantable() override;
public:
	virtual void	OnEnable() override;
	virtual void	OnDisable() override;
	virtual void	OnDestroy() override;

};

#endif