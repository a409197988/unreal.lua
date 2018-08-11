// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#include "TableUtil.h"
#include "CoreMinimal.h"
#include "ILuaPluginRuntime.h"
#include "Animation/AnimationAsset.h"
const TArray<FName> FMarkerTickContext::DefaultMarkerNames;

class FLuaPluginRuntime : public ILuaPluginRuntime
{

};

IMPLEMENT_MODULE(FLuaPluginRuntime, LuaPluginRuntime)
