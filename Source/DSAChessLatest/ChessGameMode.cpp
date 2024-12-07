#include "ChessGameMode.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"

// Constructor
AChessGameMode::AChessGameMode()
{
    // Initialize the array to hold 64 tiles
    Tiles.SetNum(64);
}

// BeginPlay
void AChessGameMode::BeginPlay()
{
    Super::BeginPlay();
    PopulateTileArray();
    UE_LOG(LogTemp, Warning, TEXT("BeginPlay in ChessGameMode is running!"));
    LoadPieceClasses();
    PlaceInitialPieces();
}

void AChessGameMode::PopulateTileArray()
{
    if (!GetWorld()) return;

    TArray<AActor*> BlackTileActors;
    TArray<AActor*> WhiteTileActors;

    // Fetch all actors in the world
    TArray<AActor*> AllActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllActors);

    // Load the Blueprint-generated classes dynamically
    UClass* BlackTileClass = LoadObject<UClass>(nullptr, TEXT("/Game/Chess/Blueprints/Board/BlackTile.BlackTile_C"));
    UClass* WhiteTileClass = LoadObject<UClass>(nullptr, TEXT("/Game/Chess/Blueprints/Board/WhiteTile.WhiteTile_C"));

    if (!BlackTileClass || !WhiteTileClass)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load BlackTile or WhiteTile classes. Check the paths!"));
        return;
    }

    // Filter the actors by class
    for (AActor* Actor : AllActors)
    {
        if (Actor->IsA(BlackTileClass))
        {
            BlackTileActors.Add(Actor);
        }
        else if (Actor->IsA(WhiteTileClass))
        {
            WhiteTileActors.Add(Actor);
        }
    }

    // Sort tiles by their names for consistent ordering
    SortActorsByName(BlackTileActors);
    SortActorsByName(WhiteTileActors);

    // Combine tiles into a single array
    TArray<AActor*> AllTiles = BlackTileActors;
    AllTiles.Append(WhiteTileActors);

    // Sort tiles by their names (convert names to integers for proper numeric sorting)
    AllTiles.Sort([](const AActor& A, const AActor& B) {
        int32 NameA = FCString::Atoi(*A.GetName());
        int32 NameB = FCString::Atoi(*B.GetName());
        return NameA < NameB;
        });

    // Resize the Tiles array and assign
    Tiles.SetNum(AllTiles.Num());
    for (int32 i = 0; i < AllTiles.Num(); ++i)
    {
        Tiles[i] = AllTiles[i];
    }

    // Log the sorted tiles for debugging
    for (int32 i = 0; i < Tiles.Num(); ++i)
    {
        if (Tiles[i])
        {
            UE_LOG(LogTemp, Warning, TEXT("Tile %d: %s"), i + 1, *Tiles[i]->GetName());
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Tile %d: nullptr"), i + 1);
        }
    }
}




// SortActorsByName
void AChessGameMode::SortActorsByName(TArray<AActor*>& Actors)
{
    Actors.Sort([](const AActor& A, const AActor& B) {
        return FCString::Atoi(*A.GetName()) < FCString::Atoi(*B.GetName());
        });
}

// GetTileAt
AActor* AChessGameMode::GetTileAt(int32 Row, int32 Column) const
{
    int32 Index = Row * 8 + Column;
    if (Index >= 0 && Index < Tiles.Num())
    {
        return Tiles[Index];
    }
    return nullptr;
}

// LoadPieceClasses
void AChessGameMode::LoadPieceClasses()
{
    // Load White Pieces
    W_Pawn = LoadObject<UClass>(nullptr, TEXT("/Game/Chess/Blueprints/ChessPiece/White/W_Pawn.W_Pawn_C"));
    W_Rook = LoadObject<UClass>(nullptr, TEXT("/Game/Chess/Blueprints/ChessPiece/White/W_Rook.W_Rook_C"));
    W_Knight = LoadObject<UClass>(nullptr, TEXT("/Game/Chess/Blueprints/ChessPiece/White/W_Knight.W_Knight_C"));
    W_Bishop = LoadObject<UClass>(nullptr, TEXT("/Game/Chess/Blueprints/ChessPiece/White/W_Bishop.W_Bishop_C"));
    W_Queen = LoadObject<UClass>(nullptr, TEXT("/Game/Chess/Blueprints/ChessPiece/White/W_Queen.W_Queen_C"));
    W_King = LoadObject<UClass>(nullptr, TEXT("/Game/Chess/Blueprints/ChessPiece/White/W_King.W_King_C"));

    // Load Black Pieces
    B_Pawn = LoadObject<UClass>(nullptr, TEXT("/Game/Chess/Blueprints/ChessPiece/Black/B_Pawn.B_Pawn_C"));
    B_Rook = LoadObject<UClass>(nullptr, TEXT("/Game/Chess/Blueprints/ChessPiece/Black/B_Rook.B_Rook_C"));
    B_Knight = LoadObject<UClass>(nullptr, TEXT("/Game/Chess/Blueprints/ChessPiece/Black/B_Knight.B_Knight_C"));
    B_Bishop = LoadObject<UClass>(nullptr, TEXT("/Game/Chess/Blueprints/ChessPiece/Black/B_Bishop.B_Bishop_C"));
    B_Queen = LoadObject<UClass>(nullptr, TEXT("/Game/Chess/Blueprints/ChessPiece/Black/B_Queen.B_Queen_C"));
    B_King = LoadObject<UClass>(nullptr, TEXT("/Game/Chess/Blueprints/ChessPiece/Black/B_King.B_King_C"));

    // Check if all pieces were loaded successfully
    if (!W_Pawn || !W_Rook || !W_Knight || !W_Bishop || !W_Queen || !W_King ||
        !B_Pawn || !B_Rook || !B_Knight || !B_Bishop || !B_Queen || !B_King)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load chess piece Blueprints! Check the paths!"));
    }
}

// PlaceInitialPieces
void AChessGameMode::PlaceInitialPieces()
{
    // White pieces positions
    SpawnPiece(W_Rook, Tiles[0]->GetActorLocation() + FVector(0, -0.5, 5), FRotator::ZeroRotator);
    SpawnPiece(W_Knight, Tiles[1]->GetActorLocation() + FVector(0, -0.5, 5), FRotator::ZeroRotator);
    SpawnPiece(W_Bishop, Tiles[2]->GetActorLocation() + FVector(0, -0.5, 5), FRotator::ZeroRotator);
    SpawnPiece(W_Queen, Tiles[3]->GetActorLocation() + FVector(0, -0.5, 5), FRotator::ZeroRotator);
    SpawnPiece(W_King, Tiles[4]->GetActorLocation() + FVector(0, -0.5, 5), FRotator::ZeroRotator);
    SpawnPiece(W_Bishop, Tiles[5]->GetActorLocation() + FVector(0, -0.5, 5), FRotator::ZeroRotator);
    SpawnPiece(W_Knight, Tiles[6]->GetActorLocation() + FVector(0, -0.5, 5), FRotator::ZeroRotator);
    SpawnPiece(W_Rook, Tiles[7]->GetActorLocation() + FVector(0, -0.5, 5), FRotator::ZeroRotator);

    for (int32 i = 0; i < 8; ++i)
    {
        SpawnPiece(W_Pawn, Tiles[8 + i]->GetActorLocation() + FVector(0, -0.5, 5), FRotator::ZeroRotator);
    }

    // Black pieces positions
    SpawnPiece(B_Rook, Tiles[56]->GetActorLocation() + FVector(0, -0.5, 5), FRotator::ZeroRotator);
    SpawnPiece(B_Knight, Tiles[57]->GetActorLocation() + FVector(0, -0.5, 5), FRotator::ZeroRotator);
    SpawnPiece(B_Bishop, Tiles[58]->GetActorLocation() + FVector(0, -0.5, 5), FRotator::ZeroRotator);
    SpawnPiece(B_Queen, Tiles[59]->GetActorLocation() + FVector(0, -0.5, 5), FRotator::ZeroRotator);
    SpawnPiece(B_King, Tiles[60]->GetActorLocation() + FVector(0, -0.5, 5), FRotator::ZeroRotator);
    SpawnPiece(B_Bishop, Tiles[61]->GetActorLocation() + FVector(0, -0.5, 5), FRotator::ZeroRotator);
    SpawnPiece(B_Knight, Tiles[62]->GetActorLocation() + FVector(0, -0.5, 5), FRotator::ZeroRotator);
    SpawnPiece(B_Rook, Tiles[63]->GetActorLocation() + FVector(0, -0.5, 5), FRotator::ZeroRotator);

    for (int32 i = 0; i < 8; ++i)
    {
        SpawnPiece(B_Pawn, Tiles[48 + i]->GetActorLocation() + FVector(0, -0.5, 5), FRotator::ZeroRotator);
    }
}

// SpawnPiece
void AChessGameMode::SpawnPiece(TSubclassOf<AActor> PieceClass, FVector Location, FRotator Rotation)
{
    if (PieceClass && GetWorld())
    {
        GetWorld()->SpawnActor<AActor>(PieceClass, Location, Rotation);
    }
}
