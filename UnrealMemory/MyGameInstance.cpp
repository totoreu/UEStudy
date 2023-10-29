// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Student.h"
#include "StudentManager.h"

void CheckUObjectIsValid(const UObject* InObject, const FString& InTag)
{
    if (InObject->IsValidLowLevel())
    {
        UE_LOG(LogTemp, Log, TEXT("[%s] 유효한 언리얼 오브젝트"), *InTag);
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("[%s] 유효하지 않은 언리얼 오브젝트"), *InTag);
    }
}

void CheckUObjectIsNull(const UObject* InObject, const FString& InTag)
{
    if (InObject == nullptr)
    {
        UE_LOG(LogTemp, Log, TEXT("[%s] 널 포인터인 언리얼 오브젝트"), *InTag);
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("[%s] 널 포인터가 아닌 언리얼 오브젝트"), *InTag);
    }
}

void UMyGameInstance::Init()
{
    Super::Init();

    NonPropStudent = NewObject<UStudent>();
    PropStudent = NewObject<UStudent>();

    NonPropStudents.Emplace(NewObject<UStudent>());
    PropStudents.Emplace(NewObject<UStudent>());
    
    StudentManager = new FStudentManager(NewObject<UStudent>());

}

void UMyGameInstance::Shutdown()
{
    Super::Shutdown();

    const UStudent* StudenetInManager = StudentManager->GetStudent();

    delete StudentManager;
    StudentManager = nullptr;

    CheckUObjectIsNull(StudenetInManager, TEXT("StudenetInManager"));
    CheckUObjectIsValid(StudenetInManager, TEXT("StudenetInManager"));


    // 결과 null 포인터가 아니면서 유효하지 않은 오브젝트
    CheckUObjectIsNull(NonPropStudent, TEXT("NonPropStudent"));
    CheckUObjectIsValid(NonPropStudent, TEXT("NonPropStudent"));

    // 결과 null 포인터가 아니면서 유효한 오브젝트
    CheckUObjectIsNull(PropStudent, TEXT("PropStudent"));
    CheckUObjectIsValid(PropStudent, TEXT("PropStudent"));

    // 결과 null 포인터가 아니면서 유효하지 않은 오브젝트
    CheckUObjectIsNull(NonPropStudents[0], TEXT("NonPropStudents"));
    CheckUObjectIsValid(NonPropStudents[0], TEXT("NonPropStudents"));

    // 결과 null 포인터가 아니면서 유효한 오브젝트
    CheckUObjectIsNull(PropStudents[0], TEXT("PropStudents"));
    CheckUObjectIsValid(PropStudents[0], TEXT("PropStudents"));
}
