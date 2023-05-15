#include <stdio.h>
#include <wchar.h>
#include <Windows.h>

#include<regex>
#include <stack>
#include <list>
#include <string>
#include <iostream>

using namespace::std;

//console definitions
#define ESC "\x1b"
#define CSI "\x1b["
//gameplay definitions
#define MAX_POINTS 121
//handSize, drawSize
//pacing definitions
#define STD_DELAY 1000
//rendering/graphics definitions

//helpful documentation on Windows console:
//  https://learn.microsoft.com/en-us/windows/console

//hides user input from the user, pretending it didn't happen
bool DisableInput() {
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    if (hIn == INVALID_HANDLE_VALUE) {
        return false;
    }

    DWORD dwMode = 0;
    if (!GetConsoleMode(hIn, &dwMode)) {
        return false;
    }

    if (dwMode & ENABLE_ECHO_INPUT) {
        dwMode ^= ENABLE_ECHO_INPUT;
        if (!SetConsoleMode(hIn, dwMode)) {
            return false;
        }
    }
    return true;
}

//enables user input, deletes any past unread input
bool EnableInput() {
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    if (hIn == INVALID_HANDLE_VALUE) {
        return false;
    }

    DWORD dwMode = 0;
    if (!GetConsoleMode(hIn, &dwMode)) {
        return false;
    }

    if (!(dwMode & ENABLE_ECHO_INPUT)) {
        dwMode |= ENABLE_ECHO_INPUT;
        if (!SetConsoleMode(hIn, dwMode)) {
            return false;
        }
    }
    cin.clear();
    return true;
}

//sets the buffer to fill the window
//returns the size of the window
COORD ResizeConsole() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    // retrieve screen buffer info
    CONSOLE_SCREEN_BUFFER_INFO scrBufferInfo;
    GetConsoleScreenBufferInfo(hOut, &scrBufferInfo);

    // current window size
    short winWidth = scrBufferInfo.srWindow.Right - scrBufferInfo.srWindow.Left + 1;
    short winHeight = scrBufferInfo.srWindow.Bottom - scrBufferInfo.srWindow.Top + 1;

    // current screen buffer size
    short scrBufferWidth = scrBufferInfo.dwSize.X;
    short scrBufferHeight = scrBufferInfo.dwSize.Y;

    // to remove the scrollbar, make sure the window height matches the screen buffer height
    COORD newSize;
    newSize.X = scrBufferWidth;
    newSize.Y = winHeight;

    // set the new screen buffer dimensions and return the size of the window (regardless of success)
    if (SetConsoleScreenBufferSize(hOut, newSize)) {
        newSize.X = winWidth;
        newSize.Y = winHeight;
    }
    return newSize;
}

//formats the console correctly
//enables escape sequences
//removes the scroll bar
//returns false if setup failure
bool InitialConsoleSetup()
{
    // Set output mode to handle virtual terminal sequences
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode))
    {
        return false;
    }

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode))
    {
        return false;
    }

    ResizeConsole();

    DisableInput();

    return true;
}

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

    card() : s(NOSUIT), n(NONUMBER) {}

    card(suit s, number n) {
        this->s = s;
        this->n = n;
    }

    //less than operator, only compares number
    bool operator<(card rhs) {
        return (*this).n < rhs.n;
    }

    //greater than operator, only compares number
    bool operator>(card rhs) {
        return rhs < (*this);
    }

    //equals operator, must be exact same card to match
    bool operator==(const card rhs) const {
        if ((*this).s != rhs.s) return false;
        if ((*this).n != rhs.n) return false;
        return true;
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

//creates an array from the stack
//params:
//  s the stack of cards to be added to the array
//returns an array of cards containing all the cards in the stack s
//returned array will be of length s.size()
card* stackToArr(stack<card> s) {
    int j = (int)s.size();
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

card getIndex(int i, list<card> l) {
    auto iter = l.begin();
    for (int j = 0; j < i; j++) {
        iter++;
    }
    return *(iter);
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
        return (int)drawnCards.size();
    }

    //resets the deck, adding all drawn cards back to the deck
    void reset() {
        drawnCards.clear();
    }
};

//class representation of the cribbage board
//keeps track of points and winner
class Board {
private:
    int playerPoints, computerPoints;
    bool isPlayerDeal;

public:
    //constructor, default values of no points, randomly selects the first player
    Board() {
        playerPoints = 0;
        computerPoints = 0;
        isPlayerDeal = (bool)(rand() % 2);
    }

    bool playerDeal() {
        return isPlayerDeal;
    }

    int getPlayerPoints() {
        return playerPoints;
    }

    int getComputerPoints() {
        return computerPoints;
    }

    //adds the points to the player, returns true if game over
    bool givePlayerPoints(int points) {
        playerPoints += points;
        if (playerPoints >= MAX_POINTS) {
            playerPoints = MAX_POINTS;
            return true;
        }
        return false;
    }

    bool giveComputerPoints(int points) {
        computerPoints += points;
        if (computerPoints >= MAX_POINTS) {
            computerPoints = MAX_POINTS;
            return true;
        }
        return false;
    }

    //checks if the game is over (one player has at least MAX_POINTS points)
    //returns true if the game is over
    bool gameOver() {
        return playerPoints >= MAX_POINTS || computerPoints >= MAX_POINTS;
    }

    //returns true if the player has won
    //assumes the player surrendered if the game is not over
    //logic: computer never gives up. EVER. haha
    bool playerWin() {
        if (gameOver() && playerPoints > computerPoints) return true;
        return false;
    }

    //advances which player's turn it is
    void advanceTurn() {
        isPlayerDeal = !isPlayerDeal;
    }
};

//renders the endgame scrren, showing who won
void renderEndgame(Board board) {
    //TODO
}

//renders just the board/score
void renderBoard(Board board) {
    //TODO
}

//renders a series of cards to the screen, if computerCards == NULL then will show card backs (unexposed cards)
//always shows numbers
//cards will be numbered in their iteration order in the list (begin->end)
void render(Board board, int nPlayerCards, card* playerCards, int nComputerCards, card* computerCards, card cut) {
    //TODO
}

//renders the current state of the running
//always shows numbers
//cards will be numbered in their iteration order in the list (begin->end)
void renderRunning(Board board, list<card> playerCards, list<card> computerCards, stack<card> history, int total) {
    //TODO
}

//shows a message/prompt to the user
void renderMessage(string message) {
    //TODO
}

//clears the message shown to the user
void clearMessage() {
    //TODO
}

//Shows the user the given prompt then gets the user's response
//clears the message when done
string getInputString(string prompt) {
    //TODO
    return NULL;
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
    if (!isCrib) return 4;
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
//Does not print anything to the output!  Does not affect score/points!
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
    int points = 0;
    points += pointsFromFifteenSum(cards);
    points += pointsFromPairs(cards);
    points += pointsFromRun(cards);
    points += pointsFromFlush(cards, isCrib);
    points += pointsFromNobs(cards);
    return points;
}

//verbose version of getPoints()
//Auto-adds points gained to board!
//used in counting points
//returns the number of points the hand is worth
int countPoints(Board board, card* hand, bool isCrib, bool isPlayerHand) {
    string s;
    int tempInt, points = 0;
    tempInt = pointsFromFifteenSum(hand);
    if (tempInt) {
        if (isPlayerHand) {
            board.givePlayerPoints(tempInt);
            s = "You get";
        }
        else {
            board.giveComputerPoints(tempInt);
            s = "Computer gets ";
        }
        s += tempInt;
        s += " points from fifteens";
        renderMessage(s);
        renderBoard(board);
        Sleep(STD_DELAY);
        clearMessage();
        points += tempInt;
    }
    tempInt = pointsFromRun(hand);
    if (tempInt) {
        if (isPlayerHand) {
            board.givePlayerPoints(tempInt);
            s = "You get";
        }
        else {
            board.giveComputerPoints(tempInt);
            s = "Computer gets ";
        }
        s += tempInt;
        s += " points from runs";
        renderMessage(s);
        renderBoard(board);
        Sleep(STD_DELAY);
        clearMessage();
        points += tempInt;
    }
    tempInt = pointsFromFlush(hand, isCrib);
    if (tempInt) {
        if (isPlayerHand) {
            board.givePlayerPoints(tempInt);
            s = "You get";
        }
        else {
            board.giveComputerPoints(tempInt);
            s = "Computer gets ";
        }
        s += tempInt;
        s += " points from flush";
        renderMessage(s);
        renderBoard(board);
        Sleep(STD_DELAY);
        clearMessage();
        points += tempInt;
    }
    tempInt = pointsFromNobs(hand);
    if (tempInt) {
        if (isPlayerHand) {
            board.givePlayerPoints(tempInt);
            s = "You get";
        }
        else {
            board.giveComputerPoints(tempInt);
            s = "Computer gets ";
        }
        s += tempInt;
        s += " points from nobs";
        renderMessage(s);
        renderBoard(board);
        Sleep(STD_DELAY);
        clearMessage();
        points += tempInt;
    }
    return points;
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
    return getPoints(hand, card(), false) + discardValue(discard, ownCrib);
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
//Does not render cards!
bool* playerDiscard(card* hand) {
    string s = "Choose two cards to discard (two numbers in one line)";
    bool* ret = new bool[6];
read: for (int i = 0; i < 6; i++) ret[i] = false;
    s = getInputString(s);
    for (unsigned int i = 0; i < s.length(); i++) {
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
        s = "Invalid response. Try entering two numbers like this (1,2)";
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
    cout << "\nComputer cards selected\n\n";
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
    for (int i = 0; i < 14; i++)
        presentSet[i] = false;
    stack<card> pushStack;
    while (!history.empty()) {
        if (presentSet[history.top().n]) break;//already found card, look no further
        presentSet[history.top().n] = true;
        pushStack.push(history.top());
        history.pop();
    }
    int depth = (int)pushStack.size();//num cards back to search (1 <= n <= history.size())
    while (!pushStack.empty()) {
        //push back onto history
        history.push(pushStack.top());
        pushStack.pop();
    }

    //initial set of valid cards present in valid history;
    //while set is noncontinuous, trim first noncontinuous card off and recompute
    int cont;
    while (true) {
        cont = indexNoncontinuous(presentSet, rootNum);
        if (cont == -1) break;//if continuous
        for (int i = 0; i < 14; i++)//reset presentSet
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
//hand must be <=4, larger numbers cannot be selected!
//Does not render cards!
card playerRunningCardSelector(list<card> hand, int total) {
    //jump to readNew to append the options to the prompt s and get new input from user into s
    //out is number entered; (out > 4) indicates multiple numbers; (out = 0) indicates no numbers
    int out;
    //indicates if the player is expected to say go
    bool go = !canPlayCard(hand, total);
    //indicates that a g has been found, trigger for assuming player said go
    bool goFlag = false;
    //dual purpose: gets user input and sends message to user
    string s = "Select a card to play ";
readNew: if (go) s += "(go)";
    else if (hand.size() == 1) s += "(1)";
    else {
        s += "(1-" + hand.size();
        s += ")";
    }
    s = getInputString(s);
    out = 0;
    for (unsigned int j = 0; j < s.length(); j++) {
        if (s[j] == '1') out = (out << 2) + 1;
        if (s[j] == '2') out = (out << 2) + 2;
        if (s[j] == '3') out = (out << 2) + 3;
        if (s[j] == '4') out = (out << 2) + 4;
        if (s[j] == 'g' || s[j] == 'G') goFlag = true;
        if ((s[j] == 'o' || s[j] == 'O') && goFlag && (s[j-1] == 'g' || s[j-1] == 'G')) goto go;//explicit go found
    }
    if (goFlag && !out) {//assume player go
    go:
        if (go) return card();
        s = "Go invalid. Select a card ";
        goto readNew;
    }
    int i = 1;
    for (card c : hand) {//get selected card and test is valid
        if (out == i) {
            if (valueOf(c) + total > 31) {
                s = "Card too large! Try a smaller card ";
                goto readNew;
            }
            return c;
        }
        i++;
    }
    s = "Invalid response. Try entering a number  ";//failed to find card
    goto readNew;
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
    card bestCard = card();
    int bestValue = INT16_MIN;

    for (card c : hand) {
        if (total + valueOf(c) == 31) return c;
        else if (total + valueOf(c) > 31) continue;
        int value = 0;
        if (total + valueOf(c) == 15) value += 2;
        history.push(c);
        value += runningPointsFromPairs(history);
        value += runningPointsFromRun(history);
        history.pop();
        if (value > bestValue) {
            bestCard = c;
            bestValue = value;
        }
    }
    if (bestValue == -1) return *(hand.begin());
    return bestCard;
}

//plays a game of cribbage against the computer on the console
//returns if the player won the game
bool playGame() {
    Board board;
    Deck deck;
    do {//turn loop
        //draw cards
        card* playerCards = new card[6];
        card* computerCards = new card[6];
        card cut;
        card* temp = deck.drawCards(13);
        for (int i = 0; i < 6; i++) {
            playerCards[i] = temp[2 * i + (int)board.playerDeal()];
            computerCards[i] = temp[2 * i + (int)(!(board.playerDeal()))];
        }
        cut = temp[12];
        delete[] temp;

        //render starting cards
        render(board, 6, playerCards, 6, NULL, card());

        //discard cards (playerCards and computerCards length trimmed to 4)
        int count = 0;
        int cribCount = 0;
        bool* discardCards = playerDiscard(playerCards);
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
        delete[] playerCards;
        playerCards = temp;
        delete(discardCards);
        count = 0;
        discardCards = aiDiscard(computerCards, board.playerDeal());
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
        delete[] computerCards;
        computerCards = temp;
        delete(discardCards);

        //nibs/cut (also show discard happened)
        render(board, 4, playerCards, 4, NULL, cut);
        if (cut.s == JACK) {
            if (board.playerDeal()) {
                renderMessage("You got 2 points from nibs!");
                if (board.givePlayerPoints(2)) goto endgame;
            }
            else {
                renderMessage("The computer got 2 points from nibs!");
                if (board.giveComputerPoints(2)) goto endgame;
            }
            renderBoard(board);
        }
        Sleep(STD_DELAY);
        clearMessage();

        //running
        stack<card> history;
        list<card> playerCardsList = arr4ToList(playerCards);
        list<card> computerCardsList = arr4ToList(computerCards);
        int total = 0;
        int tempInt;
        bool playerTurn = !board.playerDeal();
        bool playerLastCard = true;
        goto start;
    newTotal: total = 0;
        playerTurn = !playerLastCard;
        while (!history.empty()) history.pop();//clear history
    start:
        renderRunning(board, playerCardsList, computerCardsList, history, total);
        if (!(playerCardsList.empty() && computerCardsList.empty())) {//if neither party could play because no cards, end running
            while (true) {
                switch (playerTurn) {
                case true://player turn
                    if (!canPlayCard(playerCardsList, total)) {//cannot play card
                        if (playerLastCard) {//opponent cannot play a card either, take a point for last card
                            board.givePlayerPoints(1);
                            renderBoard(board);
                            renderMessage("You got 1 point for last card");
                            Sleep(STD_DELAY);
                            clearMessage();
                            if (board.gameOver()) goto endgame;
                            goto newTotal;
                        }
                        //playerExpected to say go
                        playerRunningCardSelector(playerCardsList, total);
                        goto skip;
                    }
                    //play card
                    history.push(playerRunningCardSelector(playerCardsList, total));
                    playerCardsList.remove(history.top());
                    playerLastCard = true;
                    break;
                case false://computer turn
                    if (!canPlayCard(computerCardsList, total)) {//cannot play card
                        if (playerLastCard) {//opponent cannot play a card either, take a point for last card
                            board.giveComputerPoints(1);
                            renderBoard(board);
                            renderMessage("Computer gets 1 point for last card");
                            Sleep(STD_DELAY);
                            clearMessage();
                            if (board.gameOver()) goto endgame;
                            goto newTotal;
                        }
                        //computer says go (no need for function call)
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
                renderRunning(board, playerCardsList, computerCardsList, history, total);
                //check for points
                if (total == 15) {//15 checking has independent checking because it can overlap with the other point giving combos
                    if (playerLastCard) {
                        board.givePlayerPoints(2);
                        renderMessage("15 for 2 points");
                    }
                    else {
                        board.giveComputerPoints(2);
                        renderMessage("Computer gets 2 points for 15");
                    }
                    renderBoard(board);
                    Sleep(STD_DELAY);
                    clearMessage();
                    if (board.gameOver()) goto endgame;
                }
                tempInt = runningPointsFromPairs(history);
                if (tempInt) {
                    string s;
                    s += tempInt;
                    if (playerLastCard) {
                        board.givePlayerPoints(tempInt);
                        renderMessage(s + " points from pairs");
                    }
                    else {
                        board.giveComputerPoints(tempInt);
                        renderMessage("Computer gets " + s + " points from pairs");
                    }
                    renderBoard(board);
                    Sleep(STD_DELAY);
                    clearMessage();
                    if (board.gameOver()) goto endgame;
                }
                else {//if points scored from pairs, no points for run possible
                    tempInt = runningPointsFromRun(history);
                    if (tempInt) {
                        string s;
                        s += tempInt;
                        if (playerLastCard) {
                            board.givePlayerPoints(tempInt);
                            renderMessage(s + " points from a run");
                        }
                        else {
                            board.giveComputerPoints(tempInt);
                            renderMessage("Computer gets a run for " + s + " points");
                        }
                        renderBoard(board);
                        Sleep(STD_DELAY);
                        clearMessage();
                        if (board.gameOver()) goto endgame;
                    }
                }
                if (total == 31) {
                    if (playerLastCard) {
                        board.givePlayerPoints(2);
                        renderMessage("31 for 2 points");
                    }
                    else {
                        board.giveComputerPoints(2);
                        renderMessage("Computer got 31 for 2 points");
                    }
                    renderBoard(board);
                    Sleep(STD_DELAY);
                    clearMessage();
                    if (board.gameOver()) goto endgame;
                }
            skip: playerTurn = !playerTurn;
            }
        }

        //counting
        render(board, 4, playerCards, 4, computerCards, cut);
        if (board.playerDeal()) {
            countPoints(board, computerCards, false, false);
            if (board.gameOver()) goto endgame;
            countPoints(board, playerCards, false, true);
            if (board.gameOver()) goto endgame;
            countPoints(board, crib, true, true);
        }
        else {
            countPoints(board, playerCards, false, true);
            if (board.gameOver()) goto endgame;
            countPoints(board, computerCards, false, false);
            if (board.gameOver()) goto endgame;
            countPoints(board, crib, true, false);
        }
        board.advanceTurn();
    }while (!board.gameOver());
        endgame: 
    renderEndgame(board);
    Sleep(2 * STD_DELAY);//change to continue on keypress?
    return board.playerWin();
}

//Play cribbage on the console against the computer
int main()
{
    if (!InitialConsoleSetup()) {
        printf("Console setup failure");
        return 1;
    }
    
    srand((unsigned int)time(NULL));

    //intended later implementation of a menu
    playGame();

    return 0;
}