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

	// �̷� ������ �Լ����� �ζ����� �õ��ϴ°� ����.
	// 100% �ζ������� �����Ѵٴ� ������ ������ �׷��� �õ��ϴ°� ����.
	FORCEINLINE const FString& GetName() const { return Name; }
	FORCEINLINE void SetName(const FString& InName) { Name = InName; }

	FORCEINLINE class UCard* GetCard() const { return Card; }
	FORCEINLINE void SetCard(class UCard* InCard) { Card = InCard; }

protected:
	UPROPERTY()
	FString Name;

	//�������� ���迡�� ���漱�� ���� include ���� �ʰ�, �����ͷ� �����ϸ�, �������� �ִ��� ���ټ� ����
	UPROPERTY()
	TObjectPtr<class UCard> Card;	// (UE5 ����)

	//class UCard* Card; (UE4 ����)


private:
	
};
