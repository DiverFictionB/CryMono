#include "StdAfx.h"
#include "MonoConverter.h"

#include "MonoCVars.h"

#include "MonoArray.h"
#include "MonoObject.h"

#include <IMonoAssembly.h>

IMonoArray *CConverter::CreateArray(int numArgs, IMonoClass *pElementClass)
{
	return new CScriptArray(numArgs, pElementClass); 
}

IMonoArray *CConverter::ToArray(mono::object arr)
{
	CRY_ASSERT(arr);

	return new CScriptArray(arr);
}

IMonoObject *CConverter::ToObject(mono::object obj, bool allowGC)
{
	CRY_ASSERT(obj);

	return new CScriptObject((MonoObject *)obj, allowGC);
}

mono::object CConverter::BoxAnyValue(MonoAnyValue &any)
{
	switch(any.type)
	{
	case eMonoAnyType_Boolean:
		return (mono::object)mono_value_box(mono_domain_get(), mono_get_boolean_class(), &any.b);
	case eMonoAnyType_Integer:
		return (mono::object)mono_value_box(mono_domain_get(), mono_get_int32_class(), &any.i);
	case eMonoAnyType_UnsignedInteger:
		{
			if(g_pMonoCVars->mono_boxUnsignedIntegersAsEntityIds)
			{
				IMonoClass *pEntityIdClass = gEnv->pMonoScriptSystem->GetCryBraryAssembly()->GetClass("EntityId");
				return (mono::object)mono_value_box(mono_domain_get(), (MonoClass *) pEntityIdClass->GetManagedObject(), &mono::entityId(any.u));
			}
			else
				return (mono::object)mono_value_box(mono_domain_get(), mono_get_uint32_class(), &any.u);
		}
		break;
	case eMonoAnyType_Short:
		return (mono::object)mono_value_box(mono_domain_get(), mono_get_int16_class(), &any.i);
	case eMonoAnyType_UnsignedShort:
		return (mono::object)mono_value_box(mono_domain_get(), mono_get_uint16_class(), &any.u);
	case eMonoAnyType_Float:
		return (mono::object)mono_value_box(mono_domain_get(), mono_get_single_class(), &any.f);
	case eMonoAnyType_String:
		MonoWarning("IMonoConverter::BoxAnyValue does not support strings, utilize ToMonoString instead");
	case eMonoAnyType_Vec3:
		{
			IMonoClass *pVec3Class = gEnv->pMonoScriptSystem->GetCryBraryAssembly()->GetClass("Vec3");
			return (mono::object)mono_value_box(mono_domain_get(), (MonoClass *)pVec3Class->GetManagedObject(), &any.vec3);
		}
		break;
	}

	return nullptr;
}