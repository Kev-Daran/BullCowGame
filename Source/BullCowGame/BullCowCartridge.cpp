// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    ValidWords = GetValidWords(HiddenWordsList);
    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if(bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else
    {
        ProcessGuess(Input);
    }
}



void UBullCowCartridge::SetupGame() //Setting up the game
{
    HiddenWord = ValidWords[FMath::RandRange(0,ValidWords.Num() - 1)];
    Lives = HiddenWord.Len() * 2;

    //PrintLine(FString::Printf(TEXT("The hidden word is %s."), *HiddenWord));   // Debug Line
    PrintLine(TEXT("Welcome to the Bull Cow game!"));
    PrintLine(FString::Printf(TEXT("+Guess the %i letter word"), HiddenWord.Len()));   
    PrintLine(TEXT("+Press enter to continue..."));

    bGameOver = false;
}

void UBullCowCartridge::EndGame()
{
    PrintLine(TEXT("\nPress Enter to play again..."));

    bGameOver = true;
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (HiddenWord == Guess) //Correct guess
    {
        PrintLine(TEXT("You win (;"));
        bGameOver = true;
        EndGame();
        return;
    }

    if (HiddenWord.Len() != Guess.Len()) //Wrong length of guess
    {
        PrintLine(FString::Printf(TEXT("The word contains %i letters!"), HiddenWord.Len()));  
        return;
    }

    if (!IsIsogram(Guess))         
    {
        PrintLine(TEXT("Repeating letters not allowed!\nTry again..."));
        return;
    }

    PrintLine(TEXT("You guessed incorrectly!\nYou lost a life..."));

    FBullCowCount Counts = GetBullCow(Guess);

    PrintLine(TEXT("You got %i Bulls and %i Cows."), Counts.Bulls, Counts.Cows);

    if (--Lives > 0)
    {
        PrintLine(TEXT("You currently have %i lives"),Lives);
    }

    else
    {
        PrintLine(TEXT("You ran out of lives...\nThe correct word was %s"), *HiddenWord);
    }

    if (Lives == 0 || bGameOver)        //Check if lives is greater than zero
    {
        EndGame();
    }
}


bool UBullCowCartridge::IsIsogram(const FString& UserInput) const      //Check for isogram
{
    for (int32 Index = 0; Index < (UserInput.Len()-1); Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < UserInput.Len(); Comparison++)
        {
            if (UserInput[Index] == UserInput[Comparison])
            {
                return false;
            }
        }
    }

    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const //Valid words are between the size 4 and 8 and are isograms
{
    TArray<FString> ValidWords;

    for (FString Word : WordList)
    {
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }

    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCow(const FString& PlayerGuess) const   // if the letter is in  the same index as the hiddenword, its a bull. If the letter exists in the HiddenWord but in different index, it's a cow
{
    FBullCowCount Counts;

    for (int32 GuessIndex = 0; GuessIndex < PlayerGuess.Len(); GuessIndex ++)
    {
        if (PlayerGuess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Counts.Bulls++;
            continue;
        }

        for (int32 WordIndex = 0; WordIndex < PlayerGuess.Len(); WordIndex ++)
        {
            if (PlayerGuess[GuessIndex] == HiddenWord[WordIndex])
            {
                Counts.Cows++;
                break;
            }
        }
    }

    return Counts;
}