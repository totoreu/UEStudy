// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameInstance.h"
#include "Student.h"
#include "Teacher.h"
#include "Staff.h"
#include "Card.h"

UMyGameInstance::UMyGameInstance()
{
    // CDO의 기본값을 설정하는 코드
    SchoolName = TEXT("기본학교");
}

void UMyGameInstance::Init()
{
    Super::Init();

    UE_LOG(LogTemp, Log, TEXT("==========================="));
    TArray<UPerson*> Persons = {NewObject<UStudent>(), NewObject<UTeacher>(), NewObject<UStaff>()};
    for (const auto Person : Persons)
    {
        const UCard* OwnCard = Person->GetCard();
        check(OwnCard);
        //if (OwnCard) { } 어차피 카드가 다있는데, 이런 경우엔 if문 대신 체크를 해주자

        ECardType CardType = OwnCard->GetCardType();
        UE_LOG(LogTemp, Log, TEXT("%s님이 소유한 카드는 %d입니다."), *Person->GetName(), CardType);

        const UEnum* CardEnumType = FindObject<UEnum>(nullptr, TEXT("/Script/UnrealComposition.ECardType"));
        if (CardEnumType)
        {
            FString CardMetaData = CardEnumType->GetDisplayNameTextByValue((int64)CardType).ToString();
            UE_LOG(LogTemp, Log, TEXT("%s님이 소유한 카드명은 %s입니다."), *Person->GetName(), *CardMetaData);
        }
    }

    UE_LOG(LogTemp, Log, TEXT("==========================="));
}
