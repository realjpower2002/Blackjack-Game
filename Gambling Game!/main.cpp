#include<iostream>
#include<cstdlib>
#include<string>
#include"Card.h"
#include"Hand.h"

//these includes are just for the sleep command to work!
#include<stdlib.h>
#include <stdio.h>
#include <time.h>
#include <dos.h>
#include <windows.h>

using namespace std;

//Populates a deck with a set of 52 unique cards. That is, 4 suits of 9 numbered
//cards and 4 face cards
void populateDeck(Card deck[]);

//the hand value is so that the setValue can make aces become ones
//if the hand's value + 11 > 21
// 
//It is vitally important that the hand is passed as reference, so
//that this function can directly alter the hand itself instead of
//just a copy of its data!
void dealCard(Card deck[], Hand &hand, int handValue);

//Shuffles the deck using a simple algorithm in which the last item in an array is 
//swapped with a random card (from 0 to the Last Item), and then the item
//before the Last Item (Last Item minus 1) is swapped with a random card (from
//0 to the Last Item minus One) and so on
void shuffleDeck(Card deck[], int cardsRemaining);

//Returns one of 11 different game states based on the current game data
int getGameState(Hand playerHand, Hand dealerHand, int turns, bool playerStands);

//Determines whether the dealer should keep drawing cards
bool shouldDealerKeepDrawing(Hand dealerHand);

//Prints the ending conditions of the game
void printEndingConditions(int gameState, Hand playerHand, Hand dealerHand);

//takes an integer [0,3], 0 = Hearts, 1 = Diamonds, 2 = Spades, 3 = Clubs
string intToSuit(int suitIntegerEquivalent);

int main() {
	srand(time(NULL));

	Card deck[52];

	int turns; //records the number of turns in the game

	bool playerStands; //records whether player stands

	string playerData; //takes in keyboard inputs of the player

	Hand playerHand; //holds the card objects held by the player and the dealer
	Hand dealerHand;

	int gameState; //holds one of 10 game states that the game logic uses to decide when to terminate the game

	bool playAgain = true; //Holds the players decision of whether they want to play again

	while (playAgain) {

		cout << "__________________________________________________________________________" << endl << endl;

		populateDeck(deck);       //resets all of the game data for each loop

		cout << "Generating new deck of cards..." << endl;
		Sleep(500);

		shuffleDeck(deck, 51);

		cout << "Shuffling deck..." << endl;
		Sleep(500);

		playerHand.clearHand();
		dealerHand.clearHand();

		cout << "Clearing hands..." << endl;
		Sleep(500);

		gameState = 0;
		turns = 0;
		playerStands = false;


		cout << "Resetting other data..." << endl << endl;
		Sleep(500);

		cout << "Rules : Normal casino-style blackjack, but the dealer reveals every other card." << endl << endl;

		cout << "__________________________________________________________________________" << endl << endl;

		while (gameState == 0) {
			//On the first turn, two cards are dealt to both players
			if (turns == 0) {
				dealCard(deck, playerHand, playerHand.getValue());
				dealCard(deck, playerHand, playerHand.getValue());

				dealCard(deck, dealerHand, dealerHand.getValue());
				dealCard(deck, dealerHand, dealerHand.getValue());
			}

			//A card is dealt to the player each time they do not ask to stand (they do
			//not want to stand by default) and a card is dealt to the dealer if their
			//hand is worth less than 18
			else {
				dealCard(deck, playerHand, playerHand.getValue());
				if (shouldDealerKeepDrawing(dealerHand))
					dealCard(deck, dealerHand, dealerHand.getValue());
			}

			//game state covers whether the player stands and detects an automatic loss, so it
			//must be called both at the end of a program (after the player says stand or hit)
			//and just after dealing (to detect automatic losses)
			//
			//In this case, "gameState" is detecting an automatic win or loss (for example, 
			//if either player goes bust or reaches 21
			gameState = getGameState(playerHand, dealerHand, turns, playerStands);

			// 0 is the game state in which the game has not reached the end, this checks to make sure
			//the game state is still 0 after the cards have been dealt
			if (gameState == 0) {
				
				//The hands of the dealer and the player are displayed
				cout << "Your hand : ";
				playerHand.printHand();
				cout << "\n";

				cout << "Dealer's hand : ";
				//dealerHand.printHand();
				dealerHand.printDealerHand();
				cout << "\n";

				if (turns > 5)
					cout << "This dealer is Bussin!" << endl;

				//The program asks whether the player wants to stand or hit
				//and takes in the answer
				cout << "Will you stand or hit? (enter \"stand\" or \"hit\") : ";
				cin >> playerData;

				//Detects if there are any errors in the input of the player
				while (playerData != "stand" && playerData != "hit") {
					cout << endl << "Invalid option." << endl;
					cout << "Will you stand or hit? (enter \"stand\" or \"hit\") : ";
					cin >> playerData;
				}

				cout << endl;

				//Updates whether the player chooses to stand
				if (playerData == "stand")
					playerStands = true;

				//Updates the game state
				gameState = getGameState(playerHand, dealerHand, turns, playerStands);
			}

			//Updates the turn counter
			turns++;
		}

		//Prints ending conditions
		printEndingConditions(gameState, playerHand, dealerHand);

		//Asks the player if they want to play again, and takes in the
		//player input
		cout << endl << "Play again? (Enter \"yes\" or \"no\") : ";
		cin >> playerData;

		while (playerData != "yes" && playerData != "no") {
			cout << "Invalid option. " << endl;
			cout << "Play again? (Enter \"yes\" or \"no\") : ";
			cin >> playerData;
		}

		if (playerData == "n")
			playAgain = false;


	}
	

	return 0;
}

void populateDeck(Card deck[]) {
	string suitName;
	int startingPoint; //this is the starting point for each run of the loop
	for (int i = 0; i < 4; i++) {
		suitName = intToSuit(i);
		startingPoint = i * 13;
		for (int j = 1; j < 10; j++)
			deck[startingPoint + j].setName(to_string(j+1) + " of " + suitName);
		deck[startingPoint + 10].setName("Jack of " + suitName);
		deck[startingPoint + 11].setName("Queen of " + suitName);
		deck[startingPoint + 12].setName("King of " + suitName);
		deck[startingPoint].setName("Ace of " + suitName);
	}
}

//the "hand" object may be different from the "playerhand" object
void dealCard(Card deck[], Hand &hand, int handValue) {
	deck[51].setValue(handValue);
	hand.addCard(deck[51]);

	for (int i = 50; i > -1; i--) {
		deck[i + 1].setName(deck[i].getName());
	}
}

void shuffleDeck(Card deck[], int cardsRemaining) {
	string temp;
	int randomIndex;
	for (int i = 0; i < cardsRemaining; i++) {
		randomIndex = 51 - i - (rand() % (cardsRemaining - i));
		temp = deck[51 - i].getName();
		deck[51-i].setName(deck[randomIndex].getName());
		deck[randomIndex].setName(temp);
	}
}

int getGameState(Hand playerHand, Hand dealerHand, int turns, bool playerStands) {
	int playerHandValue = playerHand.getValue();
	int dealerHandValue = dealerHand.getValue();

	if (turns == 52) {
		return 10; //condition 10 - deck is empty
	}
	if (playerStands) {
		if (playerHandValue == 21 && dealerHandValue == 21) 
			return 4; //condition 4 - player and dealer tie at 21 
		else {
			if (playerHandValue == 21)
				return 5;  //condition 5 - player automatically wins
			else {
				if (playerHandValue == dealerHandValue)
					return 7;  //condition 7 - push
				else {
					if (playerHandValue > dealerHandValue)
						return 8;  //condition 8 - player wins
					else {
						if (playerHandValue < dealerHandValue)
							return 9;   //condition 9 - dealer wins
					}
				}
			}
		}
	}
	else {
		if (dealerHand.getValue() == 21)
			return 6;  //condition 6 - dealer automatically wins
		else {
			if (playerHandValue > 21 && dealerHandValue > 21)
				return 1;   //condition 1 - both dealer and player go bust
			else {
				if (playerHandValue > 21)
					return 2;   //condition 2 - player goes bust
				else {
					if (dealerHandValue > 21)
						return 3; //condition 3 - dealer goes bust
					else {
						return 0; //condition 0 - game is ongoing
					}
				}
			}
		}
	}
}

void printEndingConditions(int gameState, Hand playerHand, Hand dealerHand) {
	if (gameState == 1)
		cout << "Dealer and Player went bust!" << endl;
	if (gameState == 2)
		cout << "Player went bust!" << endl;
	if (gameState == 3)
		cout << "Dealer went bust!" << endl;
	if (gameState == 4)
		cout << "Player and Dealer both have 21!" << endl;
	if (gameState == 5)
		cout << "Player has 21 and automatically wins!" << endl;
	if (gameState == 6)
		cout << "Dealer has 21 and automatically wins!" << endl;
	if (gameState == 7)
		cout << "Dealer and Player draw!" << endl;
	if (gameState == 8)
		cout << "Player wins!" << endl;
	if (gameState == 9)
		cout << "Dealer wins!" << endl;
	if (gameState == 10)
		cout << "Deck is emptied...." << endl << "How\'d that happen?..." << endl;
	cout << "Stats : Player ended with " << playerHand.getValue() << " points." << endl;
	cout << "        Dealer ended with " << dealerHand.getValue() << " points." << endl;
	cout << "        Player's Final Hand : ";
	playerHand.printHand();
	cout << "\n        Dealer's Final Hand : ";
	dealerHand.printHand();
	cout << "\n";
}

bool shouldDealerKeepDrawing(Hand dealerHand) {
	if (dealerHand.getValue() < 18)
		return true;
	return false;
}

string intToSuit(int suitIntegerEquivalent) {
	if (suitIntegerEquivalent == 0)
		return "Hearts";
	else {
		if (suitIntegerEquivalent == 1) {
			return "Diamonds";
		}
		else {
			if (suitIntegerEquivalent == 2) {
				return "Spades";
			}
			else
				return "Clubs";
		}
	}
}
