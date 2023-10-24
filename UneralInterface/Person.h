// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Person.generated.h"

/**
 * 
 */
UCLASS()
class UNREALINTERFACE_API UPerson : public UObject
{
	GENERATED_BODY()
	
public:
	UPerson();

	// 이런 간단한 함수들은 인라인을 시도하는게 좋다.
	// 100% 인라인으로 동작한다는 보장은 없지만 그래도 시도하는게 좋다.
	FORCEINLINE FString& GetName() { return Name; }
	FORCEINLINE void SetName(const FString& InName) { Name = InName; }

protected:
	UPROPERTY()
	FString Name;

private:
	
};
