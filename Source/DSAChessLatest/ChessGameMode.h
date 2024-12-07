#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ChessGameMode.generated.h"

class AActor;

UCLASS()
class DSACHESSLATEST_API AChessGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    // Constructor
    AChessGameMode();

    // Function to place pieces on the board
    UFUNCTION(BlueprintCallable, Category = "Chess")
        void PlaceInitialPieces();

    // Array to store the 64 tiles
    UPROPERTY(EditAnywhere)
        TArray<AActor*> Tiles;

    // Function to populate the TileArray
    UFUNCTION(BlueprintCallable, Category = "Chess")
        void PopulateTileArray();

    // Function to access a tile like a 2D array
    UFUNCTION(BlueprintCallable, Category = "Chess")
        AActor* GetTileAt(int32 Row, int32 Column) const;

protected:
    virtual void BeginPlay() override;

private:
    // Helper function to sort actors by name
    void SortActorsByName(TArray<AActor*>& Actors);

    // Function to load the chess piece classes
    void LoadPieceClasses();

    // Function to spawn a piece at a given location
    void SpawnPiece(TSubclassOf<AActor> PieceClass, FVector Location, FRotator Rotation);

    // Piece class references
    TSubclassOf<AActor> W_Pawn, W_Rook, W_Knight, W_Bishop, W_Queen, W_King;
    TSubclassOf<AActor> B_Pawn, B_Rook, B_Knight, B_Bishop, B_Queen, B_King;
};
