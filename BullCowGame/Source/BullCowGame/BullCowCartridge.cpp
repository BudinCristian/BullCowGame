// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"


void UBullCowCartridge::BeginPlay() // When the game starts
{
	Super::BeginPlay();

	Isograms = GetValidWords(Words);

	SetUpGame();
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
	if (bGameOver) 
	{
		ClearScreen();
		SetUpGame();
	}
	else //Checking PlayerGuess
	{
		ProcessGuess(PlayerInput);
	}
}

void UBullCowCartridge::SetUpGame()
{
	//Welcoming The Player
	PrintLine(TEXT("Welcome to Bull Cows!"));

	HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num()-1)];
	Lives = HiddenWord.Len();
	bGameOver = false;

	PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len());
	//PrintLine(TEXT("The HiddenWord is: %s."), *HiddenWord);//Debug Line
	PrintLine(TEXT("You have %i lives."), Lives);
	PrintLine(TEXT("Type in your guess. \nPress enter to continue...")); // Prompt Player For Guess
}

void UBullCowCartridge::EndGame()
{
	bGameOver = true;
	PrintLine(TEXT("\nPress enter to play again."));
}

void UBullCowCartridge::ProcessGuess(const FString&  Guess) 
{
	if (Guess == HiddenWord)
	{
		PrintLine(TEXT("You have Won!"));
		EndGame();
		return;
	}

	if (Guess.Len() != HiddenWord.Len())
	{
		PrintLine(TEXT("The hidden word is %i letters long"), HiddenWord.Len());
		PrintLine(TEXT("Sorry, try guessing again! \nYou have %i lives left."), Lives);
		return;
	}

	//Check If Isogram
	if (!IsIsogram(Guess)) {
		/** code */
		PrintLine(TEXT("No repeating letters, guess again"));
		return;
	}
	
	//Remove Life
	PrintLine(TEXT("Lost a life!"));
	--Lives;

	//Check If Lives >0
	if (Lives <= 0)
	{
		ClearScreen();
		PrintLine(TEXT("You have no lives left!"));
		PrintLine(TEXT("The hidden word was: %s"), *HiddenWord);
		EndGame();
		return;
	}

	//Show the player Bulls and Cows
	FBullCowCount Score = GetBullCows(Guess);

	PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);

	PrintLine(TEXT("Guess again, you have %i lives left"), Lives);
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
	for (int32 IndexWord = 0; IndexWord < Word.Len(); IndexWord++)
	{
		for (int32 Comparison = IndexWord + 1; Comparison < Word.Len(); Comparison++)
		{
			if (Word[IndexWord] == Word[Comparison])
			{
				return false;
			}
		}
	}
	return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
	TArray<FString> ValidWords;

	for (FString Word : WordList)
	{
		if (Word.Len() >= 4 && Word.Len() <= 8) {
			if(IsIsogram(Word))
			{
				ValidWords.Emplace(Word);
			}
		}
	}
	return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
	FBullCowCount Count;

	for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
	{
		if (Guess[GuessIndex] == HiddenWord[GuessIndex]) 
		{
			Count.Bulls++;
			continue;
		}

		for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
		{
			if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
			{
				Count.Cows++;
				break;
			}
		}
	}
	return Count;
}


