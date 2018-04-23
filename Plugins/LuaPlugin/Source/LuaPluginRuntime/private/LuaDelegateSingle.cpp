// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#include "LuaDelegateSingle.h"
#include "tableutil.h"
#include "BPAndLuaBridge.h"

void ULuaDelegateSingle::NoUseAtAll()
{

}

void ULuaDelegateSingle::ProcessEvent(UFunction* Function, void* Parms)
{
	check(TheDelegatePtr && FunSig);
	if(LuaCallBack > 0 && FunSig && TheDelegatePtr)
		UTableUtil::call(UTableUtil::GetRunningState(), LuaCallBack, FunSig, Parms);
}

ULuaDelegateSingle* ULuaDelegateSingle::Create(Flua_State inL, Flua_Index StackIndex)
{
	ULuaDelegateSingle* NewOne = NewObject<ULuaDelegateSingle>();
	NewOne->TheDelegatePtrInHeap = new FScriptDelegate;
	NewOne->TheDelegatePtr = NewOne->TheDelegatePtrInHeap;
	const FName FuncName("NoUseAtAll");
	NewOne->TheDelegatePtr->BindUFunction(NewOne, FuncName);
	NewOne->Bind(inL, StackIndex);
	return NewOne;
}

ULuaDelegateSingle* ULuaDelegateSingle::CreateInCppCopy(FScriptDelegate* InTheDelegatePtr, UFunction* InFuncSig)
{
	ULuaDelegateSingle* NewOne = NewObject<ULuaDelegateSingle>();
	NewOne->SetFuncSig(InFuncSig);
	NewOne->TheDelegatePtrInHeap = new FScriptDelegate;
	NewOne->TheDelegatePtr = NewOne->TheDelegatePtrInHeap;
	*(NewOne->TheDelegatePtr) = *InTheDelegatePtr;
	return NewOne;
}

ULuaDelegateSingle* ULuaDelegateSingle::CreateInCppRef(FScriptDelegate* InTheDelegatePtr, UFunction* InFuncSig)
{
	ULuaDelegateSingle* NewOne = NewObject<ULuaDelegateSingle>();
	NewOne->SetFuncSig(InFuncSig);
	NewOne->TheDelegatePtr = InTheDelegatePtr;
	return NewOne;
}

FScriptDelegate* ULuaDelegateSingle::GetCppDelegate()
{
	return TheDelegatePtr;
}

void ULuaDelegateSingle::SetFuncSig(UFunction* InFunSig)
{
	FunSig = InFunSig;
}

int ULuaDelegateSingle::Bind(Flua_State inL, Flua_Index StackIndex)
{
	const FName FuncName("NoUseAtAll");
	TheDelegatePtr->BindUFunction(this, FuncName);
	if(LuaCallBack > 0)
		UTableUtil::unref(UTableUtil::GetTheOnlyLuaState(), LuaCallBack);
	LuaCallBack = UTableUtil::ref_luavalue(inL.TheState, StackIndex);
	return LuaCallBack;
}

void ULuaDelegateSingle::Clear()
{
	if (TheDelegatePtr)
		TheDelegatePtr->Clear();
	if (LuaCallBack > 0)
	{
		LuaCallBack = -1;
		UTableUtil::unref(UTableUtil::GetTheOnlyLuaState(), LuaCallBack);
	}

}

void ULuaDelegateSingle::BindUFunction(UObject* InObject, const FName& InFunctionName)
{
	if (TheDelegatePtr)
		TheDelegatePtr->BindUFunction(InObject, InFunctionName);
}

bool ULuaDelegateSingle::IsBound()
{
	if (TheDelegatePtr)
		return TheDelegatePtr->IsBound();
	else
		return false;
}	

void ULuaDelegateSingle::Destroy()
{
	if (LuaCallBack > 0)
	{
		UTableUtil::unref(UTableUtil::GetTheOnlyLuaState(), LuaCallBack);
		LuaCallBack = -1;
	}
	if (TheDelegatePtrInHeap)
		delete TheDelegatePtrInHeap;
}

void ULuaDelegateSingle::Fire(Flua_State inL)
{
	check(TheDelegatePtr && FunSig);
	if (TheDelegatePtr->IsBound())
	{
		UFunction* Function = FunSig;
		uint8* Buffer = (uint8*)FMemory_Alloca(Function->ParmsSize);
		FScopedArguments scoped_arguments(Function, Buffer);

		int ArgIndex = 3;
		int ArgCount = lua_gettop(inL);

		// Iterate over input parameters
		for (TFieldIterator<UProperty> It(Function); It && (It->GetPropertyFlags() & (CPF_Parm)); ++It)
		{
			auto Prop = *It;
			if (ArgIndex <= ArgCount)
			{
				UTableUtil::popproperty(inL, ArgIndex, Prop, Buffer);
				++ArgIndex;
			}
		}
		TheDelegatePtr->ProcessDelegate<UObject>(Buffer);
	}
}
