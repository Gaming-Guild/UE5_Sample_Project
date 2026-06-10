#include "Misc/AutomationTest.h"
#include "Engine/DataTable.h"
#include "AssetRegistry/AssetRegistryModule.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPopulateCardsDataTable, "Gaming.PopulateCardsDataTable", EAutomationTestFlags::EditorContext)

bool FPopulateCardsDataTable::RunTest(const FString& Parameters)
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	// Cargar el Data Table
	UDataTable* DataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, TEXT("/Game/DT_Cards")));

	if (!DataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("No se pudo cargar DT_Cards"));
		return false;
	}

	// Limpiar filas existentes (opcional)
	// DataTable->RowMap.Empty();

	// Mapeo de palos a enum
	const FString SuitNames[] = { TEXT("club"), TEXT("diamond"), TEXT("heart"), TEXT("spade") };

	int32 RowsAdded = 0;

	for (int32 SuitIndex = 0; SuitIndex < 4; ++SuitIndex)
	{
		for (int32 Rank = 1; Rank <= 13; ++Rank)
		{
			FString RowName = FString::Printf(TEXT("%d_%s"), Rank, *SuitNames[SuitIndex]);
			FString TexturePath = FString::Printf(
				TEXT("/Game/Assets/Standard 52 Cards/solitaire/individuals/%s/%d_%s.%d_%s"),
				*SuitNames[SuitIndex], Rank, *SuitNames[SuitIndex], Rank, *SuitNames[SuitIndex]
			);

			// Cargar la textura
			UTexture2D* CardTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *TexturePath));

			if (!CardTexture)
			{
				UE_LOG(LogTemp, Warning, TEXT("No se encontró textura: %s"), *TexturePath);
			}

			// Crear estructura S_CardVisual
			// Note: Esto es pseudocódigo, ya que necesitarías acceso a la estructura real
			// FS_CardVisual CardData;
			// CardData.Suit = (E_CardSuit)SuitIndex;
			// CardData.Rank = Rank;
			// CardData.CardTexture = CardTexture;
			// DataTable->AddRow(FName(*RowName), CardData);

			RowsAdded++;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Intento de agregar %d filas al Data Table"), RowsAdded);

	return true;
}
