//
// -------------------------------------------------------------------------
//  File name:   SteamOnlineLobbySystemComponent.cpp
//  Created:     23/09/2019 by Von Bismarck, Jonathan Green
//  Description: This Class file is for calling Steam API for our lobby system
// -------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"

#include "SteamOnlineSystemComponent.h"

#include <CrySchematyc/Reflection/TypeDesc.h>
#include <CrySchematyc/Utils/EnumFlags.h>
#include <CrySchematyc/Env/IEnvRegistry.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include <CrySchematyc/Env/Elements/EnvSignal.h>
#include <CrySchematyc/ResourceTypes.h>
#include <CrySchematyc/MathTypes.h>
#include <CrySchematyc/Utils/SharedString.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterSteamLobbySystemComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CSteamLobbySystemComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterSteamLobbySystemComponent);
}

void CSteamLobbySystemComponent::Initialize()
{
}

Cry::Entity::EventFlags CSteamLobbySystemComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::BecomeLocalPlayer |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}
