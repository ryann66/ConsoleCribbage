#include <iostream>
#include <regex>
#include <string>
#include <stack>
#include <list>
using namespace::std;

//class representation of the cribbage board
//keeps track of points and winner
class Board {
public:
    int playerPoints, computerPoints;
    bool playerDeal;
    
    //constructor, default values of no points, randomly selects the first player
    Board() {
        playerPoints = 0;
        computerPoints = 0;
        playerDeal = (bool)(rand() % 2);
    }

    //checks if the game is over (one player has at least 121 points)
    //returns true if the game is over
    bool gameOver() {
        if (playerPoints >= 121) {
            playerPoints = 121;
            return true;
        }
        if (computerPoints >= 121) {
            computerPoints = 121;
            return true;
        }
        return false;
    }

    //returns a string representing if the computer or player has won, or if game is ongoing
    string winner() {
        if (gameOver()) {
            if (playerPoints > computerPoints) return "Congrats! You win!";
            return "Sorry, you lose.";
        }
        return "The game isn't over yet!";
    }

    //returns a string representation of the number of points that each player has
    string toString() {
        string s = "Player: ";
        s += to_string(playerPoints);
        s += "\nComputer: ";
        s += to_string(computerPoints);
        s += "\n\n";
        return s;
    }

    //advances which player's turn it is
    void advanceTurn() {
        playerDeal = !playerDeal;
    }
};

//enumberation for the suit of a card
//includes 0, representing no suit present
enum suit {
    NOSUIT = 0,
    HEARTS,
    CLUBS,
    SPADES,
    DIAMONDS
};

//enumeration for the number/rank of a card
//set in order to card number from 1-10
//ace is low, jack = 11, queen = 12, king = 13
//includes 0, representing no number present
enum number {
    NONUMBER = 0,
    ONE = 1,
    TWO = 2,
    THREE = 3,
    FOUR = 4,
    FIVE = 5,
    SIX = 6,
    SEVEN = 7,
    EIGHT = 8,
    NINE = 9,
    TEN = 10,
    JACK = 11,
    QUEEN = 12,
    KING = 13
};

//struct representation of a card, defined by a suit and number
struct card {
    suit s;
    number n;

    //less than operator, only compares number
    bool operator<(card rhs) {
        return (*this).n < rhs.n;
    }

    //greater than operator, only compares number
    bool operator>(card rhs) {
        return rhs < (*this);
    }

    //equals operator, must be exact same card to match
    bool operator==(const card rhs) const{
        if ((*this).s != rhs.s) return false;
        if ((*this).n != rhs.n) return false;
        return true;
    }
};

//creates a card with the given suit and number
//params:
//  s the suit the card should be
//  n the number the card should be
//returns a card with the given suit and number
card newCard(suit s, number n) {
    card c;
    c.s = s;
    c.n = n;
    return c;
}
/*
//returns the best guess for what (enum) suit the card is, if failed, it will return NONUMBER
//params:
//  s the string to guess suit from
suit getSuit(string s) {
    if (regex_search(s, regex("heart"))) return HEARTS;
    if (regex_search(s, regex("club"))) return CLUBS;//buggy
    if (regex_search(s, regex("diamond"))) return DIAMONDS;
    if (regex_search(s, regex("spade"))) return SPADES;
    return NOSUIT;
}

//returns the best guess for what (enum) number the card is, if failed, it will return NONUMBER
//params:
//  s the string to guess number from
number getNumber(string s) {
    if (regex_search(s, regex("10"))) return (number)10;
    if (regex_search(s, regex("11"))) return (number)11;
    if (regex_search(s, regex("12"))) return (number)12;
    if (regex_search(s, regex("13")))return (number)13;
    if (regex_search(s, regex("1"))) return (number)1;
    if (regex_search(s, regex("2"))) return (number)2;
    if (regex_search(s, regex("3"))) return (number)3;
    if (regex_search(s, regex("4"))) return (number)4;
    if (regex_search(s, regex("5"))) return (number)5;
    if (regex_search(s, regex("6"))) return (number)6;
    if (regex_search(s, regex("7"))) return (number)7;
    if (regex_search(s, regex("8"))) return (number)8;
    if (regex_search(s, regex("9"))) return (number)9;
    if (regex_search(s, regex("ace"))) return (number)1;
    if (regex_search(s, regex("ten"))) return (number)10;
    if (regex_search(s, regex("jack"))) return (number)11;
    if (regex_search(s, regex("queen"))) return (number)12;
    if (regex_search(s, regex("king"))) return (number)13;
    if (regex_search(s, regex("one"))) return (number) 1;
    if (regex_search(s, regex("two"))) return (number)2;
    if (regex_search(s, regex("three"))) return (number)3;
    if (regex_search(s, regex("four"))) return (number)4;
    if (regex_search(s, regex("five"))) return (number)5;
    if (regex_search(s, regex("six"))) return (number)6;
    if (regex_search(s, regex("seven"))) return (number)7;
    if (regex_search(s, regex("eight"))) return (number)8;
    if (regex_search(s, regex("nine"))) return (number)9;
    if (regex_search(s, regex("j"))) return (number)11;
    if (regex_search(s, regex("q"))) return (number)12;
    if (regex_search(s, regex("k"))) return (number)13;
    if (regex_search(s, regex("a"))) return (number)1;
    return NONUMBER;
}

//returns a lowercase version of the provided string
//params:
//  s string to go to lowercase
string toLowerString(string s) {
    int n = s.length();
    for (int i = 0; i < n; i++)
        s[i] = tolower(s[i]);
    return s;
}

//prompts the user to enter a string that is used to guess which card
//the user is referencing, guarentees that the card returned will exist
card getCard() {
    card c;
    c.n = NONUMBER;
    c.s = NOSUIT;
    string s;
    do {
        getline(cin, s);
        s = toLowerString(s);
        c.n = getNumber(s);
        c.s = getSuit(s);
        cout << "\n";
    } while (c.n == NONUMBER || c.s == NOSUIT);
    return c;
}
*/
//creates an array from the stack
//params:
//  s the stack of cards to be added to the array
//returns an array of cards containing all the cards in the stack s
//returned array will be of length s.size()
card* stackToArr(stack<card> s) {
    int j = s.size();
    card* ret = new card[j];
    for (int i = 0; i < j; i++) {
        ret[i] = s.top();
        s.pop();
    }
    return ret;
}

//only use with arrays of length four
list<card> arr4ToList(card* arr) {
    list<card> ret;
    for (int i = 0; i < 4; i++)
        ret.push_back(arr[i]);
    return ret;
}

//class to represent the deck of cards, can draw cards and return all cards to the deck
//This implementation works best when you do not need to draw all or most of the cards in the deck
//Does not contain either joker
class Deck {
private: 
    list<card> drawnCards;

    //checks if the given card has been drawn
    //params:
    //  card the card to check if has been drawn
    //returns true if the card has been drawn
    bool isDrawn(card card) {
        auto iter = drawnCards.begin();
        while (iter != drawnCards.end()) {
            if ((*iter).n == card.n && (*iter).s == card.s) return true;
            iter++;
        }
        return false;
    }
       
public: 
    //draws a card from the deck
    //Warning: if 52 cards have been drawn, it will result in an infinite loop
    //returns a random card from the deck
    card drawCard() {
        card c;
        do {
            c.n = (number)((rand() % 13) + 1);
            c.s = (suit)((rand() % 4) + 1);
        } while (isDrawn(c));
        drawnCards.push_back(c);
        return c;
    }
      
    //draws n cards from the deck
    //Warning: if there are fewer than n cards in the deck, it will result in an infinite loop
    card* drawCards(int n) {
        card* cards = new card[n];
        for (int i = 0; i < n; i++)
            cards[i] = drawCard();
        return cards;
    }

    //returns the number of cards that have been drawn out of the deck
    //There are 52 cards in the deck.
    int numDrawn() {
        return drawnCards.size();
    }

    //resets the deck, adding all drawn cards back to the deck
    void reset() {
        drawnCards.clear();
    }
};

//returns the value of the card c (face cards count as 10)
//params:
//  c the card
//returns the value of the card, less than or equal to 10
int valueOf(card c) {
    return min((int)c.n, 10);
}

//returns a string representation of the given card
//params:
//  c the card to be represented as a string
string cardToString(card c) {
    string ret = "";
    if (c.n == 1) ret += "Ace";
    else if (c.n == 2) ret += "Two";
    else if (c.n == 3) ret += "Three";
    else if (c.n == 4) ret += "Four";
    else if (c.n == 5) ret += "Five";
    else if (c.n == 6) ret += "Six";
    else if (c.n == 7) ret += "Seven";
    else if (c.n == 8) ret += "Eight";
    else if (c.n == 9) ret += "Nine";
    else if (c.n == 10) ret += "Ten";
    else if (c.n == 11) ret += "Jack";
    else if (c.n == 12) ret += "Queen";
    else if (c.n == 13) ret += "King";
    else ret += "No number";
    ret += " of ";
    if (c.s == SPADES) ret += "Spades";
    else if (c.s == CLUBS) ret += "Clubs";
    else if (c.s == HEARTS) ret += "Hearts";
    else if (c.s == DIAMONDS) ret += "Diamonds";
    else ret += "No suit";
    return ret;
}

//recursive helper method for calculating points from fifteen
//params:
//  cards stack of cards in the hand, can be in any order
//  value the value (face cards count as 10) of cards already added to the total
//        set as 0, must be less than 15
//returns the number of points from fifteen that can be made from cards and the given total
int recursivePointsFromFifteenSum(stack<card> cards, int val) {
    if (val == 15) return 2;
    if (val > 15) return 0;
    if (cards.empty()) return 0;
    int i = valueOf(cards.top());
    cards.pop();
    return recursivePointsFromFifteenSum(cards, val) + recursivePointsFromFifteenSum(cards, val + i);
}

//returns the number of points from fifteen that the hand is worth
//params:
//  cards card array of length 5 representing the cards in the hand
//returns the number of points from runs the hand is worth, by cribbage rules
int pointsFromFifteenSum(card* cards) {
    stack<card> cardStack;
    for (int i = 0; i < 5; i++)
        cardStack.push(cards[i]);
    return recursivePointsFromFifteenSum(cardStack, 0);
}

//returns the number of points from pairs that the hand is worth
//params:
//  cards card array of length 5 representing the cards in the hand
//returns the number of points from runs the hand is worth, by cribbage rules
int pointsFromPairs(card* cards) {
    int points = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = i + 1; j < 5; j++)
            if (cards[i].n == cards[j].n) points += 2;
    }
    return points;
}

//returns the number of points from nobs that the hand is worth
//params:
//  cards card array of length 5 representing the cards in the hand
//returns the number of points from runs the hand is worth, by cribbage rules (0 or 1)
int pointsFromNobs(card* cards) {
    for (int i = 1; i < 5; i++) {
        if (cards[i].n == JACK && cards[i].s == cards[0].s) return 1;
    }
    return 0;
}

//returns the number of points from flush that the hand is worth
//params:
//  cards card array of length 5 representing the cards in the hand
//  isCrib bool representing if the set of cards is the crib
int pointsFromFlush(card* cards, bool isCrib) {
    for (int i = 2; i < 5; i++) {
        if (cards[i].s != cards[1].s) return 0;
    }
    if (cards[0].s == cards[1].s) return 5;
    if(!isCrib) return 4;
    return 0;
}

//returns the number of points from runs that the hand is worth
//params:
//  cards card array of length 5 representing the cards in the hand
//returns the number of points from runs the hand is worth, by cribbage rules
int pointsFromRun(card* cards) {
    int* quantity = new int[14];
    for (int i = 0; i < 14; i++) quantity[i] = 0;
    for (int i = 0; i < 5; i++)
        quantity[(int)cards[i].n]++;
    int row = 0;
    int mult = 1;
    for (int i = 1; i < 14; i++) {
        if (quantity[i] == 0) {
            if (row >= 3) return mult * row;
            row = 0;
            mult = 1;
        }
        else {//quantity > 0
            row++;
            mult *= quantity[i];
        }
    }
    return 0;
}

//scores the number of points in the given hand
//params:
//  hand array of length five that represents the hand
//       hand[0] is the cut card
//  isCrib bool for if the current hand is the crib, only matters for flushes
//returns the the number of points the hand is worth, by cribbage rules
int getPoints(card* hand, bool isCrib) {//hand[0] is the cut card
    int points = 0;
    points += pointsFromFifteenSum(hand);
    //cout << points << "\n";
    points += pointsFromPairs(hand);
    //cout << points << "\n";
    points += pointsFromRun(hand);
    //cout << points << "\n";
    points += pointsFromFlush(hand, isCrib);
    //cout << points << "\n";
    points += pointsFromNobs(hand);
    //cout << points << "\n";
    return points;
}

//scores the number of points in the given hand
//params:
//  hand array of length four that represents the hand
//  cut the cut card
//  isCrib bool for if the current hand is the crib, only matters for flushes
//returns the the number of points the hand is worth, by cribbage rules
int getPoints(card* hand, card cut, bool isCrib) {
    card* cards = new card[5];
    cards[0] = cut;
    for (int i = 0; i < 4; i++)
        cards[i + 1] = hand[i];
    return getPoints(cards, isCrib);
}

//helper method for scoreChoice() that gets the algorithm score for a set of cards to be discarded
//params:
//  discard card array of length two that represents the two cards discarded
//  ownCrib bool representing if the crib points should be counted favorably
//returns the int value of the algorithm score for the discarded cards, can be negative
int discardValue(card* discard, bool ownCrib) {
    int score = 0;

    if (valueOf(discard[0]) + valueOf(discard[1]) == 15 || discard[0].n == discard[1].n) score = 2;
    if (abs(discard[0].n - discard[1].n) == 1) score += 1;
    if (discard[0].n == FIVE || discard[1].n == FIVE) score += 1;

    if (ownCrib) return score;
    return -1 * score;
}

//scores the given hand and discard combination
//used by aiDiscard() algorithm to determine value of choices
//params:
//  hand card array of length 4 representing cards retained in the hand
//  discard card array of length 2 representing the two cards discarded
//  ownCrib bool representing if the crib points should be counted favorably
//return int value representing the algorithm score of the given choice of cards
//can be negative
int scoreChoice(card* hand, card* discard, bool ownCrib) {
    return getPoints(hand, newCard(NOSUIT, NONUMBER), false) + discardValue(discard, ownCrib);
}

//returns the average value of cards in the given hand, face cards count as 10
//params:
//  hand card array of length 4
double avgValue(card* hand) {
    double sum = 0;
    for (int i = 0; i < 4; i++)
        sum += valueOf(hand[i]);
    return (sum / 4);
}

//prompts the player to select two cards to be discarded in the counting
//params:
//  cards array of length 6 of cards that are in the players hand
//  playerDeal true if it is currently the player's deal (crib is hostile)
//return bool array of length 6; will be all false except two true values,
//the true values represent the two cards to be discarded into the crib
bool* playerDiscard(card* hand, bool playerDeal) {
    cout << "Choose two cards to discard (two numbers in one line). ";
    if (playerDeal) cout << "You are the dealer";
    else cout << "You are not the dealer";
    cout << "\n";
    for (int i = 0; i < 6; i++) {
        cout << (i + 1);
        cout << ". " << cardToString(hand[i]) << "\n";
    }
    //read two numbers between 1-6, inclusive
    string s;
    bool* ret = new bool[6];
read: for (int i = 0; i < 6; i++) ret[i] = false;
    getline(cin, s);
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == '1') ret[0] = true;
        else if (s[i] == '2') ret[1] = true;
        else if (s[i] == '3') ret[2] = true;
        else if (s[i] == '4') ret[3] = true;
        else if (s[i] == '5') ret[4] = true;
        else if (s[i] == '6') ret[5] = true;
    }
    int i = 0;
    for (int j = 0; j < 6; j++) i += (int)ret[j];
    if (i != 2) {
        cout << "Invalid response. Try entering two numbers like this (1,2)\n";
        goto read;
    }
    return ret;
}

//automatically selects two cards to be discarded in the counting
//params:
//  cards array of length 6 of cards that are in the players hand
//  playerDeal true if it is currently the player's deal (crib is hostile)
//return bool array of length 6; will be all false except two true values,
//the true values represent the two cards to be discarded into the crib
bool* aiDiscard(card* cards, bool playerDeal) {
    bool debug = true;//debug options
    int bestI = 0, bestJ = 1, bestScore = INT16_MIN;
    double bestAvgValue = 14;
    int score;
    card* hand = new card[4];
    card* discard = new card[2];
    bool ownCrib = !playerDeal;
    for (int i = 0; i < 6; i++) {
        discard[0] = cards[i];
        for (int j = i + 1; j < 6; j++) {
            discard[1] = cards[j];
            int f = 0;
            for (int k = 0; k < 6; k++) {
                if (k == i || k == j) continue;
                hand[f] = cards[k];
                f++;
            }
            score = scoreChoice(hand, discard, ownCrib);
            if (debug) {
                cout << "Scored " << (i + 1);
                cout << ", " << (j + 1);
                cout << " at " << score;
                cout << " points\n";
            }
            if (score >= bestScore) {
                double newAvgValue = avgValue(hand);
                if (score > bestScore || newAvgValue < bestAvgValue) {//if new set is better or has lower number cards
                    bestI = i;
                    bestJ = j;
                    bestScore = score;
                    bestAvgValue = newAvgValue;
                }
            }
        }
    }
    bool* ret = new bool[6];
    for (int i = 0; i < 6; i++) ret[i] = false;
    ret[bestI] = true;
    ret[bestJ] = true;

    if (debug) {
        cout << "Cards:\n";
        for (int i = 0; i < 6; i++) {
            cout << (i + 1);
            cout << ". " << cardToString(cards[i]) << "\n";
        }
        cout << "\n";
        cout << "Selected cards: " << (bestI + 1);
        cout << ", " << (bestJ + 1);
        cout << "\n";
        cout << "Algorithm score " << bestScore;
        cout << "\n";
    }

    cout << "Computer cards selected\n\n";
    return ret;
}

//checks if any of the cards in the hand can be played in the running
//params:
//  hand list of cards in the player's hand
//  total the current total of points in the running (0 - 30 inclusive)
//returns true if a card can be played
bool canPlayCard(list<card> hand, int total) {
    for (card c : hand) {
        if (valueOf(c) + total <= 31) return true;
    }
    return false;
}

//returns a string representation of a stack of cards from first played (bottom)
//to last played (top)
//empty string if no cards
//Cards printed on one line, comma delimited
string printStack(stack<card> cards) {
    if (cards.empty()) return "";
    stack<card> pushStack;
    while (!cards.empty()) {
        pushStack.push(cards.top());
        cards.pop();
    }
    string s = "";
    while (true) {
        s += cardToString(pushStack.top());
        cards.push(pushStack.top());
        pushStack.pop();
        if (!pushStack.empty()) s += ", ";
        else break;
    }
    return s;
}

//tallys the points earned from pairs by the last player to play a card in the running
//params:
//  history the stack of cards that has been played by all players
//          with the most recent at the top of the stack
//returns the number of points earned, will be positive or 0
int runningPointsFromPairs(stack<card> history) {
    if (history.empty()) return 0;
    stack<card> pushStack;
    card top = history.top();
    history.pop();
    int row = 1;
    while (!history.empty() && history.top().n == top.n) {
        row++;
        pushStack.push(history.top());
        history.pop();
    }
    while (!pushStack.empty()) {
        history.push(pushStack.top());
        pushStack.pop();
    }
    history.push(top);
    //calc points from row (row of 1 = 0)
    int points = 0;
    for (int i = 0; i < row; i++) {
        for (int j = i + 1; j < row; j++) {
            points += 2;
        }
    }
    return points;
}

//returns the index of the first true
//arr must be an array of length 14
//index 0 is ignored
//returns -1 if no true values
int firstTrueIndex(bool* arr) {
    for (int i = 0; i < 14; i++)
        if (arr[i]) return i;
    return -1;
}

//searches set, starting at index root, to find if there is a true value that is not continuous
//with root.
//example: root is 3
// 1T, 2F, 3T, 4T, 5T, 6F, 7F, (etc. to 13)
//Index 1 is noncontinuous because 2 is false and 2 is between 1 and 3 (root)
//params:
//  set an array of bools of size 14
//      s`et[0] will be ignored, set[root] is assumed to be true
//  root the index to search from, between 1 and 13 inclusive
//returns the index of a true value that is not continuous with the root (1, 13 inclusive), OR 
//        -1 if all trues are continuous
int indexNoncontinuous(bool* set, int root) {
    //serach forwards
    int last = 14;
    for (int i = root + 1; i < 14; i++) {//find first false
        if (!set[i]) {
            last = i;
            break;
        }
    }
    for (int i = last + 1; i < 14; i++) {//return index if search finds a true
        if (set[i]) return i;
    }
    //search backwards
    last = 0;
    for (int i = root - 1; i > 0; i--) {
        if (!set[i]) {
            last = i;//find first false
            break;
        }
    }
    for (int i = last - 1; i > 0; i--) {//return index if search finds a true
        if (set[i]) return i;
    }
    return -1;//continuous
}

//tallys the points earned for a run by the last player to play a card in the running
//params:
//  history the stack of cards that has been played by all players
//          with the most recent at the top of the stack
//returns the number of points earned, will be positive or 0
int runningPointsFromRun(stack<card> history) {
    int rootNum = history.top().n;
    bool* presentSet = new bool[14];
    for (int i = 0; i < 14;i++)
        presentSet[i] = false;
    stack<card> pushStack;
    while (!history.empty()) {
        if (presentSet[history.top().n]) break;//already found card, look no further
        presentSet[history.top().n] = true;
        pushStack.push(history.top());
        history.pop();
    }
    int depth = pushStack.size();//num cards back to search (1 <= n <= history.size())
    while (!pushStack.empty()) {
        //push back onto history
        history.push(pushStack.top());
        pushStack.pop();
    }
    
    //initial set of valid cards present in valid history;
    //while set is noncontinuous, trim first noncontinuous card off and recompute
    int cont;
    while(true) {
        cont = indexNoncontinuous(presentSet, rootNum);
        if (cont == -1) break;//if continuous
        for (int i = 0; i < 14;i++)//reset presentSet
            presentSet[i] = false;
        while (!history.empty()) {
            if (history.top().n == cont) break;//stopper hit
            presentSet[history.top().n] = true;//add card to presentSet
            pushStack.push(history.top());
            history.pop();
        }
        while (!pushStack.empty()) {
            //push back onto history
            history.push(pushStack.top());
            pushStack.pop();
        }
    }

    //get length of continuous run in presentSet
    int count = 0;
    for (int i = firstTrueIndex(presentSet); i < 14; i++) {
        if (!presentSet[i]) break;
        count++;
    }
    if (count < 3) return 0;
    return count;
}

//prompts the player to select a card
//Warning: one of the cards in the hand must be playable! Failure will result infinite loop
//params:
//  hand the list of cards that the player has in hand
//  history the stack of cards that has been played by all players 
//          with the most recent at the top of the stack
//  total the count that the running has reached, 0-31 inclusive
//returns the card to be played in the running
card playerRunningCardSelector(list<card> hand, stack<card> history, int total) {
    if (!history.empty()) 
        cout << "\nHistory:\n" << printStack(history) << "\n\n";
    cout << "Choose one card to play:\n";
    int i = 1;
    for (card c : hand) {
        cout << i;
        cout << ". " << cardToString(c) << "\n";
        i++;
    }
read: int out = 0;
    string s;
    getline(cin, s);
    for (int j = 0; j < s.length(); j++) {
        if (s[j] == '1') out = out * 10 + 1;
        if (s[j] == '2') out = out * 10 + 2;
        if (s[j] == '3') out = out * 10 + 3;
        if (s[j] == '4') out = out * 10 + 4;
    }
    i = 1;
    for (card c : hand) {//get selected card and test is valid
        if (out == i) {
            if (valueOf(c) + total > 31) {
                cout << "Card too large, try a different card!\n";
                goto read;
            }
            return c;
        }
        i++;
    }
    cout << "Invalid response. Try entering a number between 1 and " << hand.size();//failed to find card
    cout << "\n";
    goto read;
}

//automatically selects a card to be played in the running (DEBUG: currently just routes to player card selector) TODO!
//Warning: one of the cards in the hand must be playable! Failure will result infinite loop
//params:
//  hand the list of cards that the player has in hand
//  history the stack of cards that has been played by all players 
//          with the most recent at the top of the stack
//  total the count that the running has reached, 0-31 inclusive
//returns the card to be played in the running
card aiRunningCardSelector(list<card> hand, stack<card> history, int total) {
    cout << "***AI_RUNNING_CARD_SELECTOR***\n";
    card ret = playerRunningCardSelector(hand, history, total);//temp solution for debug purposes
    cout << "***END_AI_RUNNING_CARD_SELECTOR***\n";
    return ret;
}

//plays a game of cribbage against the computer on the console
void playGame() {
    Board board;
    Deck deck;
    while (true) {//turn loop
        //draw cards
        card* playerCards = new card[6];
        card* computerCards = new card[6];
        card cut;
        card* temp = deck.drawCards(13);
        for (int i = 0; i < 6; i++) {
            playerCards[i] = temp[2 * i + (int)board.playerDeal];
            computerCards[i] = temp[2 * i + (int)(!(board.playerDeal))];
        }
        cut = temp[12];
        delete(temp);

        //discard cards
        int count = 0;
        int cribCount = 0;
        bool* discardCards = playerDiscard(playerCards, board.playerDeal);
        temp = new card[4];
        card* crib = new card[4];
        for (int i = 0; i < 6; i++) {
            if (discardCards[i]) {
                crib[cribCount] = playerCards[i];
                cribCount++;
            }
            else {
                temp[count] = playerCards[i];
                count++;
            }
        }
        delete(playerCards);
        playerCards = temp;
        delete(discardCards);
        count = 0;
        discardCards = aiDiscard(computerCards, board.playerDeal);
        temp = new card[4];
        for (int i = 0; i < 6; i++) {
            if (discardCards[i]) {
                crib[cribCount] = computerCards[i];
                cribCount++;
            }
            else {
                temp[count] = computerCards[i];
                count++;
            }
        }
        delete(computerCards);
        computerCards = temp;
        delete(discardCards);

        //nibs/cut
        cout << "The cut is a: " << cardToString(cut) << "\n";
        if (cut.s == JACK) {
            if (board.playerDeal) {
                cout << "You get 2 points from nibs!\n";
                board.playerPoints += 2;
                if (board.gameOver()) goto endgame;
            }
            else {
                cout << "The computer gets 2 points from nibs!\n";
                board.computerPoints += 2;
                if (board.gameOver()) goto endgame;
            }
        }
        cout << "\n";

        //running
        stack<card> history;
        list<card> playerCardsList = arr4ToList(playerCards);
        list<card> computerCardsList = arr4ToList(computerCards);
        int total = 0;
        int tempInt;
        bool playerTurn = !board.playerDeal;
        bool playerLastCard = true;
        goto start;

    newTotal: total = 0;
        playerTurn = !playerLastCard;
        while (!history.empty()) history.pop();//clear history
    start:
        if (!(playerCardsList.empty() && computerCardsList.empty())) {//if neither party could play because no cards, end running
            while (true) {
                switch (playerTurn) {
                case true://player turn
                    if (!canPlayCard(playerCardsList, total)) {//cannot play card
                        if (playerLastCard) {//opponent cannot play a card either, take a point for last card
                            cout << "1 point for last card\n";
                            board.playerPoints++;
                            if (board.gameOver()) goto endgame;
                            goto newTotal;
                        }
                        else cout << "Player: go\n";
                        goto skip;
                    }
                    //play card
                    history.push(playerRunningCardSelector(playerCardsList, history, total));
                    playerCardsList.remove(history.top());
                    playerLastCard = true;
                    break;
                case false://computer turn
                    if (!canPlayCard(computerCardsList, total)) {//cannot play card
                        if (!playerLastCard) {//opponent cannot play a card either, take a point for last card
                            cout << "1 point for last card\n";
                            board.computerPoints++;
                            if (board.gameOver()) goto endgame;
                            goto newTotal;
                        }
                        else cout << "Computer: go\n";
                        goto skip;
                    }
                    //play card
                    history.push(aiRunningCardSelector(computerCardsList, history, total));
                    computerCardsList.remove(history.top());
                    playerLastCard = false;
                    break;
                }
                //add to total
                total += valueOf(history.top());
                //check for points
                if (total == 15) {
                    if (playerLastCard) {
                        cout << "Player: ";
                        board.playerPoints += 2;
                    }
                    else {
                        cout << "Computer: ";
                        board.computerPoints += 2;
                    }
                    cout << "15 for 2 points\n";
                    if (board.gameOver()) goto endgame;
                }
                tempInt = runningPointsFromPairs(history);
                if (tempInt != 0) {
                    if (playerLastCard) {
                        cout << "Player: ";
                        board.playerPoints += tempInt;
                    }
                    else {
                        cout << "Computer: ";
                        board.computerPoints += tempInt;
                    }
                    cout << "pairs for " << tempInt;
                    cout << " points\n";
                    if (board.gameOver()) goto endgame;
                }
                else {//if points scored from pairs, no points for run possible
                    tempInt = runningPointsFromRun(history);
                    if (tempInt != 0) {
                        if (playerLastCard) {
                            cout << "Player: ";
                            board.playerPoints += tempInt;
                        }
                        else {
                            cout << "Computer: ";
                            board.computerPoints += tempInt;
                        }
                        cout << "run for " << tempInt;
                        cout << " points\n";
                        if (board.gameOver()) goto endgame;
                    }
                }
                cout << "Total: " << total;
                cout << "\n";
                if (total == 31) {
                    if (playerLastCard) {
                        cout << "Player: ";
                        board.playerPoints += 2;
                    }
                    else {
                        cout << "Computer: ";
                        board.computerPoints += 2;
                    }
                    cout << "31 for 2 points\n";
                    if (board.gameOver()) goto endgame;
                    goto newTotal;
                }
            skip: playerTurn = !playerTurn;
            }
        }

        //counting
        cout << "\nCounting:\n";
        int holder;
        if (board.playerDeal) {
            holder = getPoints(computerCards, cut, false);
            cout << "Computer cards worth: " << holder;
            cout << " points\n";
            board.computerPoints += holder;
            if (board.gameOver()) goto endgame;
            holder = getPoints(playerCards, cut, false);
            cout << "Player cards worth: " << holder;
            cout << " points\n";
            board.playerPoints += holder;
            if (board.gameOver()) goto endgame;
            holder = getPoints(crib, cut, true);
            cout << "Player's crib worth: " << holder;
            cout << " points\n";
            board.playerPoints += holder;
        }
        else {
            holder = getPoints(playerCards, cut, false);
            cout << "Player cards worth: " << holder;
            cout << " points\n";
            board.playerPoints += holder;
            if (board.gameOver()) goto endgame;
            holder = getPoints(computerCards, cut, false);
            cout << "Computer cards worth: " << holder;
            cout << " points\n";
            board.computerPoints += holder;
            if (board.gameOver()) goto endgame;
            holder = getPoints(crib, cut, true);
            cout << "Computer's crib worth: " << holder;
            cout << " points\n";
            board.computerPoints += holder;
        }
        if (board.gameOver()) goto endgame;
        cout << "\n\nEnd of turn\n" << board.toString() << "\n";
        board.advanceTurn();
    }
endgame: cout << "\n";
    cout << board.toString();
    cout << board.winner();
    cout << "\n\n\n";
}

//Play cribbage on the console against the computer
int main()
{
    srand(time(NULL));//necessary

    
    playGame();
}