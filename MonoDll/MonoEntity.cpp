#include "StdAfx.h"
#include "MonoEntity.h"
#include "MonoEntityPropertyHandler.h"

#include "Scriptbinds\Entity.h"

#include <IEntityClass.h>

#include <IMonoScriptSystem.h>
#include <IMonoAssembly.h>
#include <IMonoClass.h>
#include <IMonoConverter.h>

#include <MonoCommon.h>

CEntity::CEntity()
	: m_pScript(nullptr)
	, m_bInitialized(false)
	, m_pAnimatedCharacter(nullptr)
{
}

CEntity::~CEntity()
{
	SAFE_RELEASE(m_pScript);

	if (m_pAnimatedCharacter)
	{
		IGameObject *pGameObject = GetGameObject();
		pGameObject->ReleaseExtension("AnimatedCharacter");
	}
}

bool CEntity::Init(IGameObject *pGameObject)
{
	SetGameObject(pGameObject);

	pGameObject->EnablePrePhysicsUpdate( ePPU_Always );
	pGameObject->EnablePhysicsEvent( true, eEPE_OnPostStepImmediate );

	if (!GetGameObject()->BindToNetwork())
		return false;

	IEntity *pEntity = GetEntity();
	IEntityClass *pEntityClass = pEntity->GetClass();

	m_pScript = gEnv->pMonoScriptSystem->InstantiateScript(pEntityClass->GetName(), eScriptFlag_Entity);

	IMonoClass *pEntityInfoClass = gEnv->pMonoScriptSystem->GetCryBraryAssembly()->GetClass("EntityInfo");

	SMonoEntityInfo entityInfo(pEntity);

	m_pScript->CallMethod("InternalSpawn", pEntityInfoClass->BoxObject(&entityInfo));

	int numProperties;
	auto pProperties = static_cast<CEntityPropertyHandler *>(pEntityClass->GetPropertyHandler())->GetQueuedProperties(pEntity->GetId(), numProperties);

	if(pProperties)
	{
		for(int i = 0; i < numProperties; i++)
		{
			auto queuedProperty = pProperties[i];

			SetPropertyValue(queuedProperty.propertyInfo, queuedProperty.value.c_str());
		}
	}

	m_bInitialized = true;

	return true;
}

void CEntity::PostInit(IGameObject *pGameObject)
{
	Reset(false);
}

void CEntity::Reset(bool enteringGamemode)
{
	if(m_pAnimatedCharacter)
		m_pAnimatedCharacter->ResetState();
	else if(m_pAnimatedCharacter = static_cast<IAnimatedCharacter *>(GetGameObject()->QueryExtension("AnimatedCharacter")))
		m_pAnimatedCharacter->ResetState();
}

void CEntity::ProcessEvent(SEntityEvent &event)
{
	switch(event.event)
	{
	case ENTITY_EVENT_LEVEL_LOADED:
		m_pScript->CallMethod("OnInit");
		break;
	case ENTITY_EVENT_RESET:
		{
			bool enterGamemode = event.nParam[0]==1;

			if(!enterGamemode && GetEntity()->GetFlags() & ENTITY_FLAG_NO_SAVE)
			{
				gEnv->pEntitySystem->RemoveEntity(GetEntityId());
				return;
			}

			m_pScript->CallMethod("OnEditorReset", enterGamemode);

			Reset(enterGamemode);
		}
		break;
	case ENTITY_EVENT_COLLISION:
		{
			EventPhysCollision *pCollision = (EventPhysCollision *)event.nParam[0];

			EntityId targetId = 0;

			IEntity *pTarget = pCollision->iForeignData[1]==PHYS_FOREIGN_ID_ENTITY ? (IEntity*)pCollision->pForeignData[1]:0;
			if(pTarget)
				targetId = pTarget->GetId();

			m_pScript->CallMethod("OnCollision", targetId, pCollision->pt, pCollision->vloc[0].GetNormalizedSafe(), pCollision->idmat[0], pCollision->n);
		}
		break;
	case ENTITY_EVENT_START_GAME:
		m_pScript->CallMethod("OnStartGame");
		break;
	case ENTITY_EVENT_START_LEVEL:
		m_pScript->CallMethod("OnStartLevel");
		break;
	case ENTITY_EVENT_ENTERAREA:
		m_pScript->CallMethod("OnEnterArea", (EntityId)event.nParam[0], (int)event.nParam[1], event.fParam[0]);
		break;
	case ENTITY_EVENT_MOVEINSIDEAREA:
		m_pScript->CallMethod("OnMoveInsideArea", (EntityId)event.nParam[0], (int)event.nParam[1], event.fParam[0]);
		break;
	case ENTITY_EVENT_LEAVEAREA:
		m_pScript->CallMethod("OnLeaveArea", (EntityId)event.nParam[0], (int)event.nParam[1], event.fParam[0]);
		break;
	case ENTITY_EVENT_ENTERNEARAREA:
		m_pScript->CallMethod("OnEnterNearArea", (EntityId)event.nParam[0], (int)event.nParam[1], event.fParam[0]);
		break;
	case ENTITY_EVENT_MOVENEARAREA:
		m_pScript->CallMethod("OnMoveNearArea", (EntityId)event.nParam[0], (int)event.nParam[1], event.fParam[0]);
		break;
	case ENTITY_EVENT_LEAVENEARAREA:
		m_pScript->CallMethod("OnLeaveNearArea", (EntityId)event.nParam[0], (int)event.nParam[1], event.fParam[0]);
		break;
	case ENTITY_EVENT_XFORM:
		m_pScript->CallMethod("OnMove");
		break;
	case ENTITY_EVENT_ATTACH:
		m_pScript->CallMethod("OnAttach", (EntityId)event.nParam[0]);
		break;
	case ENTITY_EVENT_DETACH:
		m_pScript->CallMethod("OnDetach", (EntityId)event.nParam[0]);
		break;
	case ENTITY_EVENT_DETACH_THIS:
		m_pScript->CallMethod("OnDetachThis", (EntityId)event.nParam[0]);
		break;
	case ENTITY_EVENT_PREPHYSICSUPDATE:
		m_pScript->CallMethod("OnPrePhysicsUpdate");
		break;
	}
}

void CEntity::FullSerialize(TSerialize ser)
{
	IEntity *pEntity = GetEntity();

	ser.BeginGroup("Properties");
	auto pPropertyHandler = static_cast<CEntityPropertyHandler *>(pEntity->GetClass()->GetPropertyHandler());
	for(int i = 0; i < pPropertyHandler->GetPropertyCount(); i++)
	{
		if(ser.IsWriting())
		{
			IEntityPropertyHandler::SPropertyInfo propertyInfo;
			pPropertyHandler->GetPropertyInfo(i, propertyInfo);

			ser.Value(propertyInfo.name, pPropertyHandler->GetProperty(pEntity, i));
		}
		else
		{
			IEntityPropertyHandler::SPropertyInfo propertyInfo;
			pPropertyHandler->GetPropertyInfo(i, propertyInfo);

			char *propertyValue = nullptr;
			ser.ValueChar(propertyInfo.name, propertyValue, 0);

			pPropertyHandler->SetProperty(pEntity, i, propertyValue);
		}
	}
	ser.EndGroup();
}

void CEntity::SetPropertyValue(IEntityPropertyHandler::SPropertyInfo propertyInfo, const char *value)
{
	m_pScript->CallMethod("SetPropertyValue", propertyInfo.name, propertyInfo.type, value);
}

///////////////////////////////////////////////////
// Entity RMI's
///////////////////////////////////////////////////
CEntity::RMIParams::RMIParams(IMonoArray *pArray, const char *funcName, int targetScript)
	: methodName(funcName)
	, scriptId(targetScript)
{
	length = pArray->GetSize();

	if(length > 0)
	{
		anyValues = new MonoAnyValue[length];

		for(int i = 0; i < length; i++)
			anyValues[i] = pArray->GetItem(i)->GetAnyValue();
	}
}

void CEntity::RMIParams::SerializeWith(TSerialize ser)
{
	ser.Value("length", length);
	ser.Value("methodName", methodName);
	ser.Value("scriptId", scriptId);

	if(length > 0)
	{
		if(!anyValues)
			anyValues = new MonoAnyValue[length];

		for(int i = 0; i < length; i++)
			anyValues[i].SerializeWith(ser);
	}
}

IMPLEMENT_RMI(CEntity, SvScriptRMI)
{
	IMonoArray *pArgs = NULL;
	if(params.length > 0)
	{
		pArgs = CreateMonoArray(params.length);

		for(int i = 0; i < params.length; i++)
			pArgs->Insert(params.anyValues[i]);
	}

	IMonoObject *pScriptInstance = gEnv->pMonoScriptSystem->GetScriptManager()->CallMethod("GetScriptInstanceById", params.scriptId, eScriptFlag_Any);

	pScriptInstance->CallMethod(params.methodName.c_str(), pArgs);

	return true;
}

IMPLEMENT_RMI(CEntity, ClScriptRMI)
{
	IMonoArray *pArgs = NULL;
	if(params.length > 0)
	{
		pArgs = CreateMonoArray(params.length);

		for(int i = 0; i < params.length; i++)
			pArgs->Insert(params.anyValues[i]);
	}

	IMonoObject *pScriptInstance = gEnv->pMonoScriptSystem->GetScriptManager()->CallMethod("GetScriptInstanceById", params.scriptId, eScriptFlag_Any);

	pScriptInstance->CallMethod(params.methodName.c_str(), pArgs);

	return true;
}