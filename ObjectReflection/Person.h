// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Person.generated.h"

/** 사람에 대한 기본 클래스입니다. */
UCLASS()
class OBJECTREFLECTION_API UPerson : public UObject
{
	GENERATED_BODY()
	
public:
	UPerson();

	/** 학생이나 선생이 레슨에 참여했는지 확인합니다. */
	UFUNCTION()
	virtual void DoLesson();

	const FString& GetName() const;
	void SetName(const FString& InName);

protected:
	/** 학생이나 선생의 이름입니다. */
	UPROPERTY()
	FString Name;

	/** 학생은 학년, 선생은 근속연수를 나타냅니다. */
	int32 Year;


private:
};
