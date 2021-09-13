#pragma once

#include<iostream>
#include"Card.h"

using namespace std;

class Hand {
private:
	Card hand[20];
	int howManyCards = 0;
public:
	void addCard(Card card) {
		hand[howManyCards] = card;
		howManyCards++;
	}
	void clearHand() {
		howManyCards = 0;
	}

	Card getCard(int index) {
		return this->hand[index];
	}
	int getValue() {
		int value = 0;
		for (int i = 0; i < howManyCards; i++) {
			value += hand[i].getValue();
		}
		return value;
	}

	void printHand() {
		if (howManyCards == 1)
			hand[0].printCard();
		else {
			for (int i = 0; i < howManyCards-1; i++) {
				hand[i].printCard();
				cout << ", ";
			}
			hand[howManyCards-1].printCard();
			cout << ". ";
		}
	}
	void printDealerHand() {
		if (howManyCards < 3) {
			hand[0].printCard();
			cout << ", ??? ";
		}
		else {
			for (int i = 0; i < howManyCards-2; i += 2) {
				hand[i].printCard();
				cout << ", ???, ";
			}
			if (howManyCards % 2 == 1) {
				hand[howManyCards-1].printCard();
				cout << ". ";
			}
		}
	}
};