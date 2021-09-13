#pragma once

#include<string>
#include<iostream>

using namespace std;

class Card {
private:
	string name = "What are you doing here?";
	int value = -1;
public:
	int getValue() {
		return value;
	}
	string getName() {
		return name;
	}

	//this function sets the value of a card, so that the value of the card is set only once, and the
	//values of ALL the cards in the players hands are not changed when the player will bust if he receives
	//an Ace
	//
	//notes : make SURE the card's value is set BEFORE it is put into the player's hand, do not try to 
	//return a card object
	void setValue(int handValue) {
		bool willBust = false;    //willBust measures whether an 11 will make the player bust
		if (handValue + 11 > 21)
			willBust = true;

		if (name[1] == '0') //if second character in name is 0, it must be 10.
			this->value = 10;
		if (name[0] == 'J')
			this->value = 10;
		else {
			if (name[0] == 'Q')
				this->value = 10;
			else {
				if (name[0] == 'K')
					this->value = 10;
				else {
					if (name[0] == 'A' && !willBust)
						this->value = 11;
					else {
						if(name[0] == 'A' && willBust)
						this->value = 1;
					}
				}
			}
		}
		for (int i = 2; i < 10; i++) {
			if ((int)name[0] == (48 + i))
				this->value = i;
		}
	}
	void setName(string name) {
		this->name = name;
	}

	void printCard() {
		cout << name;
	}
};