// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Person.h"
#include "Teacher.generated.h"

/** 선생의 정보를 갖고 있는 클래스입니다. */
UCLASS()
class OBJECTREFLECTION_API UTeacher : public UPerson
{
	GENERATED_BODY()
	
public:
	UTeacher();

	/** 선생의 수업 참여 상태를 나타냅니다. */
	virtual void DoLesson() override;

private:

	UPROPERTY()
	int32 Id;
};
