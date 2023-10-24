// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Person.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCOMPOSITION_API UPerson : public UObject
{
	GENERATED_BODY()
	
public:
	UPerson();

	// 이런 간단한 함수들은 인라인을 시도하는게 좋다.
	// 100% 인라인으로 동작한다는 보장은 없지만 그래도 시도하는게 좋다.
	FORCEINLINE const FString& GetName() const { return Name; }
	FORCEINLINE void SetName(const FString& InName) { Name = InName; }

	FORCEINLINE class UCard* GetCard() const { return Card; }
	FORCEINLINE void SetCard(class UCard* InCard) { Card = InCard; }

protected:
	UPROPERTY()
	FString Name;

	//컴포지션 관계에선 전방선언 통해 include 하지 않고, 포인터로 관리하며, 의존성을 최대한 없앨수 있음
	UPROPERTY()
	TObjectPtr<class UCard> Card;	// (UE5 기준)

	//class UCard* Card; (UE4 기준)


private:
	
};
