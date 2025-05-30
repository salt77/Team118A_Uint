#pragma once
#ifndef __Lobby_H__
#define __Lobby_H__

#include "Scene.h"
class CClientScene_Lobby final : public CScene
{
private:
	CClientScene_Lobby(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CClientScene_Lobby();

public:
	static CClientScene_Lobby*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free() override;

public:
	virtual HRESULT	Ready_Scene() override;
	virtual _int	Start_Scene(const _float& fTimeDelta) override;
	virtual _int	Update_Scene(const _float& fTimeDelta);
	virtual _int	LateUpdate_Scene(const _float& fTimeDelta);
	virtual void	Render_Scene() override;

protected:
	virtual HRESULT	Ready_SceneObject() override; 

private:
	HRESULT			Ready_Prototype();
	HRESULT			Ready_LightInfo();

};

#endif // !__Lobby_H__