/*this file creates the structures used in the 
 poker program*/

typedef enum suit{HEARTS, DIAMONDS, SPADES, CLUBS} Suit;

/*card structure
 creates a card*/
typedef struct card_ {
    Suit suit;
    int rank;
    int num;
} Card;

/*deck structure*/
typedef struct deck_ {
    Card** cards;
    int topDeck;
} Deck;

/*player structure*/
typedef struct player_ {
    Card** hand;
    int pnum;
    int inGame;
    int chips;
    char* name;
    int handRank;
    int bet;
    
} Player;

/*(poker) table structure*/
typedef struct table_ {
    Player** players;
    Deck* deck;
    Deck* testDeck;
    int pot;
} Table;

/*this struct is used to determine
 if a hand has more than one card 
 of the of a single rank i.e. two
 twos, four kings, etc.  It is mainly
 used by the ranker*/
typedef struct hand_helper_ {
    int count;
    Card* card;
} Helper;

/*this struct keeps track of the
 hand_helper data needed for the ranker.
 while the helper keeps track of the 
 count of each rank in the hand, 
 this struct keeps track of the whole hand.*/
typedef struct hand_data_ {
    Helper** data;
} HandData;

/*option struct holds the options a player
 has when considering discarding cards in
 his hand.  There are 32 separate options
 and more info can be found in the mcadvisor.c
 file*/
typedef struct option_ {
    char* data;
    int sum;
    int count;
} Option;

/*option list is just a list of options*/
/*I made it its own structure for clarity*/
typedef struct option_list_ {
    Option** list;
} Option_List;