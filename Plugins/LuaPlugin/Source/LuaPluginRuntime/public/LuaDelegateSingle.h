// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ScriptDelegates.h"
#include "LuaDelegate.h"
#include "LuaDelegateSingle.generated.h"

UCLASS()
class LUAPLUGINRUNTIME_API ULuaDelegateSingle : public UObject
{
	GENERATED_BODY()

	FScriptDelegate* TheDelegatePtr;
	FScriptDelegate* TheDelegatePtrInHeap;
	UFunction* FunSig;
	ULuaDelegateSingle():LuaCallBack(-1), FunSig(nullptr), TheDelegatePtr(nullptr), TheDelegatePtrInHeap(nullptr) {}
public:
	int LuaCallBack;

	void Init(FScriptDelegate* InTheDelegatePtr, UFunction* InFunSig);

	UFUNCTION()
		void NoUseAtAll();

	virtual void ProcessEvent(UFunction* Function, void* Parms) override;

	UFUNCTION()
		static ULuaDelegateSingle* Create(Flua_State inL, Flua_Index StackIndex);

	static ULuaDelegateSingle* CreateInCppCopy(FScriptDelegate* InTheDelegatePtr, UFunction* InFuncSig);
	static ULuaDelegateSingle* CreateInCppRef(FScriptDelegate* InTheDelegatePtr, UFunction* InFuncSig);

	FScriptDelegate* GetCppDelegate();

	void SetFuncSig(UFunction* InFunSig);

	UFUNCTION()
		int Bind(Flua_State inL, Flua_Index StackIndex);

	UFUNCTION()
		void Clear();

	UFUNCTION()
		void BindUFunction(UObject* InObject, const FName& InFunctionName);

	UFUNCTION()
		bool IsBound();

	UFUNCTION()
		void Destroy();

	UFUNCTION()
		void Fire(Flua_State inL);

};
