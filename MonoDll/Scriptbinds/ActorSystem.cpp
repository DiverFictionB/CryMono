#include "StdAfx.h"
#include "ActorSystem.h"

#include "Actor.h"

#include <IGameFramework.h>

CActorSystem::TActorClasses CActorSystem::m_monoActorClasses = TActorClasses();

CActorSystem::CActorSystem()
{
	REGISTER_METHOD(GetPlayerHealth);
	REGISTER_METHOD(SetPlayerHealth);
	REGISTER_METHOD(GetPlayerMaxHealth);
	REGISTER_METHOD(SetPlayerMaxHealth);

	REGISTER_METHOD(GetActorInfoByChannelId);
	REGISTER_METHOD(GetActorInfoById);

	REGISTER_METHOD(RegisterActorClass);
	REGISTER_METHOD(CreateActor);
	REGISTER_METHOD(RemoveActor);

	REGISTER_METHOD(GetClientActorId);

	gEnv->pEntitySystem->AddSink(this, IEntitySystem::OnSpawn, 0);
}

EMonoActorType CActorSystem::GetMonoActorType(const char *actorClassName)
{
	for each(auto classPair in m_monoActorClasses)
	{
		if(!strcmp(classPair.first, actorClassName))
			return classPair.second;
	}

	return EMonoActorType_None;
}

void CActorSystem::OnSpawn(IEntity *pEntity,SEntitySpawnParams &params)
{
	EMonoActorType actorType = GetMonoActorType(pEntity->GetClass()->GetName());

	if(actorType != EMonoActorType_None)
	{
		if(IActor *pActor = gEnv->pGameFramework->GetIActorSystem()->GetActor(pEntity->GetId()))
		{
			IMonoObject *pScript  = gEnv->pMonoScriptSystem->InstantiateScript(pEntity->GetClass()->GetName(), eScriptFlag_Actor);

			IMonoClass *pActorInfoClass = gEnv->pMonoScriptSystem->GetCryBraryAssembly()->GetClass("ActorInfo");

			SMonoActorInfo actorInfo(pActor);

			pScript->CallMethod("InternalSpawn", pActorInfoClass->BoxObject(&actorInfo));

			if(actorType == EMonoActorType_Managed)
				static_cast<CActor *>(pActor)->SetScript(pScript);
		}
	}
}

SMonoActorInfo CActorSystem::GetActorInfoByChannelId(uint16 channelId)
{
	if(IActor *pActor = gEnv->pGameFramework->GetIActorSystem()->GetActorByChannelId(channelId))
		return SMonoActorInfo(pActor);

	return SMonoActorInfo();
}

SMonoActorInfo CActorSystem::GetActorInfoById(EntityId id)
{
	if(IActor *pActor = gEnv->pGameFramework->GetIActorSystem()->GetActor(id))
		return SMonoActorInfo(pActor);

	return SMonoActorInfo();
}

void CActorSystem::RegisterActorClass(mono::string name, bool isNative)
{
	const char *className = ToCryString(name);

	if(!isNative)
		gEnv->pGameFramework->RegisterFactory(className, (CActor *)0, false, (CActor *)0);

	m_monoActorClasses.insert(TActorClasses::value_type(className, isNative ? EMonoActorType_Native : EMonoActorType_Managed));
}

SMonoActorInfo CActorSystem::CreateActor(int channelId, mono::string name, mono::string className, Vec3 pos, Quat rot, Vec3 scale)
{
	const char *sClassName = ToCryString(className);

	if(IActor *pActor = gEnv->pGameFramework->GetIActorSystem()->CreateActor(channelId, ToCryString(name), sClassName, pos, rot, scale))
		return SMonoActorInfo(pActor);

	return SMonoActorInfo();
}

void CActorSystem::RemoveActor(EntityId id)
{
	gEnv->pGameFramework->GetIActorSystem()->RemoveActor(id);
}

EntityId CActorSystem::GetClientActorId()
{
	return gEnv->pGameFramework->GetClientActorId();
}

float CActorSystem::GetPlayerHealth(IActor *pActor)
{
	return pActor->GetHealth();
}

void CActorSystem::SetPlayerHealth(IActor *pActor, float newHealth)
{
	pActor->SetHealth(newHealth);
}

float CActorSystem::GetPlayerMaxHealth(IActor *pActor)
{
	return pActor->GetMaxHealth();
}

void CActorSystem::SetPlayerMaxHealth(IActor *pActor, float newMaxHealth)
{
	pActor->SetMaxHealth(newMaxHealth);
}