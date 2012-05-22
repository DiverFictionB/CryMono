///////////////////////////////////////////////////////////////////////////*
//Ink Studios Source File.
//Copyright (C), Ink Studios, 2011.
//////////////////////////////////////////////////////////////////////////
// CryENGINE UI system scriptbind
//////////////////////////////////////////////////////////////////////////
// 22/03/2012 : Created by Filip 'i59' Lundgren
////////////////////////////////////////////////////////////////////////*/
#ifndef __UI_H__
#define __UI_H__

#include <MonoCommon.h>
#include <IMonoScriptBind.h>

#include <IFlashUI.h>

struct SEventSystemHandler;

class CUI 
	: public IMonoScriptBind
{
public:
	CUI();
	~CUI() {}

	static CUI *GetInstance() { return m_pUI; }

protected:
	// IMonoScriptBind
	virtual const char *GetClassName() override { return "UI"; }
	// ~IMonoScriptBind

	static IUIEventSystem *CreateEventSystem(mono::string name, IUIEventSystem::EEventSystemType eventType);

	static unsigned int RegisterFunction(IUIEventSystem *pEventSystem, mono::string name, mono::string desc, mono::array inputs);
	static unsigned int RegisterEvent(IUIEventSystem *pEventSystem, mono::string name, mono::string desc, mono::array outputs);

	static void SendEvent(IUIEventSystem *pEventSystem, unsigned int eventId, mono::array args);

private:
	static CUI *m_pUI;
};

struct SEventSystemHandler
	: public IUIEventListener
{
	SEventSystemHandler(const char *name, IUIEventSystem::EEventSystemType eventType)
	{
		m_pEventSystem = gEnv->pFlashUI->CreateEventSystem(name, eventType);

		if(eventType == IUIEventSystem::eEST_UI_TO_SYSTEM)
			m_pEventSystem->RegisterListener(this, "SEventSystemHandler");
	}

	// IUIEventListener
	virtual void OnEvent(const SUIEvent& event);
	// ~IUIEventListener

	~SEventSystemHandler() {}

	IUIEventSystem *GetEventSystem() { return m_pEventSystem; }

private:

	IUIEventSystem  *m_pEventSystem;
};

#endif //__UI_H__