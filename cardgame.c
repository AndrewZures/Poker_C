#include<stdio.h>
#include<stdlib.h>
#include"cardgame.h"
#include<time.h>

#define NUMBEROFPLAYERS 4

/*prototypes*/
int sort_hand(Card**);
char* print_card(Card*);
int print_hand(Card**);
int print_deck(Deck*);
char* print_suit(Card*);
char* print_rank(Card*);
int swap(Card*, Card*);
int print_player(Player*);
int get_rand();
int shuffle(Deck*);
int describe_hand(Player*);
int get_hand_score(Card** hand);
int compar(const void*, const void*);

/*NOTE:  This file deals mostly with the create of the basic 
 elements of the card game, including the cards themselves,
 the deck, players, the table, and the mechanics of getting
 the game ready*/

Card* card_init(int suit, int rank, int num){
    /*initiates a new card*/
    Card* crd = (Card*) malloc(sizeof(Card));
    crd->suit = suit;
    crd->rank = rank;
    crd->num = num;
    return crd;
}

Deck* deck_init(){
    /*initiate 52 cards with four suits*/
    int i, j, k = 0;
    Deck* deck = (Deck*) malloc(sizeof(Deck));
    deck->cards = (Card**) malloc(sizeof(Card*)*52);
    deck->topDeck = 0;

    
    for(i = 0; i < 4; i++)
        for(j = 2; j < 15; j++){
                deck->cards[k] = card_init(i,j, k);
                k++;
        }
    /*returns unshuffled deck of card*/
    return deck;
}

Player* player_init(int num){
    /*creates players*/
    Player* player = (Player*) malloc(sizeof(Player));
    player->pnum = num;  /*keeps track of player number*/
    player->inGame = 1; /*if 0, player no longer in game*/
    player->chips = 300; /*number of chips for each player*/
    player->bet = 0; /*keeps track of the player's current bet*/
    switch(num){
        case 0: player->name = "Player 1 (You)"; break;
        case 1: player->name = "Player 2"; break;
        case 2: player->name = "Player 3"; break;
        case 3: player->name = "Player 4"; break;
    }
    return player;
}

Table* table_init(){
    /*creates table*/
    int i;
    Table* table = (Table*) malloc(sizeof(Table));
    table->deck = deck_init();  /*creates a deck of card for the table*/
    table->testDeck = deck_init(); /*create test deck*/
    table->pot = 0;
    srand(time(NULL)); /*seed the random number generator*/
    shuffle(table->deck);  /*shuffle deck*/
    
    /*create players*/
    table->players = (Player**) malloc(sizeof(Player*)*4);
    for(i = 0; i < NUMBEROFPLAYERS; i++){
        table->players[i] = player_init(i);
    }
    return table;
}

int shuffle(Deck* deck){
    /*shuffle the deck by swapping the places*/
    /*of two cards a certain amount of times*/
    int i;
    for(i = 0; i < 200; i++)
        swap(deck->cards[get_rand()], deck->cards[get_rand()]);
    return 0;
}

Card* deal_card(Deck* deck){
    /*deals the initial cards*/
    Card* temp;
    temp = deck->cards[deck->topDeck];
     if(deck->topDeck == 51)
         deck->topDeck = 0;
     else deck->topDeck++;
     return temp;
}


int swap_card(Card** hand, int cardNum, Deck* deck){
    /*Here is where you see my queue implementation.  I have a
     52 element array of Card*.  I implement the Queue by dealing cards
     from the front of the queue to initially create the player's hands.  This
     number can change based on how many players are playing.  The queue 
     effectively starts at the topDeck, and is therefore only half of the array,
     the other half being the player's hands themselves, which I keep in the
     same array.  The queue have of the array works by having the card at the 
     topDeck index (20 for a four person game) move to the index position of the
     card to be discarded, which must be less than the index of topDeck, since the
     hand are at the front of the array (0-19 for four man game). cards with 
     index greater than topCard are shifted down and the discarded card is placed
     at the 51st position in the array, thereby creating a queue. system*/
    Card* temp;
    int j;
    int i = 0;
    /*find the index of the card to be swapped*/
    while(hand[cardNum]->num != deck->cards[i]->num){
        i++;
    }
    /*temp gets the card to be discarded
     to save its information*/
    temp = deck->cards[i];
    
    /*hand gets the new card*/
    hand[cardNum] = deck->cards[deck->topDeck];
    
    /*discarded card position is replaced by new card*/
    deck->cards[i] = deck->cards[deck->topDeck];
    
    /*cards are shifted down*/
    for(j = deck->topDeck; j < 51; j++)
        deck->cards[j] = deck->cards[j+1];
    
    /*discarded card gets new place at end of queue*/
    deck->cards[51] = temp;

    /*print_card(hand[cardNum]);*/
    return 0;
}

int swap_test_card(Card** hand, int cardNum, Deck* deck){
    /*swapping a test card doesn't require the queue functionality
     since it's just a random card to swap and order, deck integrity
     don't really matter*/
    int i; int flag = 0;
    Card* temp;
    
    while(flag == 0){
        /*get new card*/
        temp = deck->cards[get_rand()];
        flag = 1;
        /*check to see if exact card is already in hand*/
        for(i = 0; i < 5; i++){
            if(temp->suit == hand[i]->suit && temp->rank == hand[i]->rank)
                flag = 0;
        }
        if(flag==1)
            hand[cardNum] = temp;
        /*if card is in hand, do the whole thing again until
         you get a card that isn't in the hand*/
    }
    
    /*print_card(hand[cardNum]);*/
    return 0;
}

Card** create_hand(Deck* deck){
    /*creatse a hand of five cards*/
    int i;
    Card** hand = (Card**) malloc(sizeof(Card*)*5);
    for(i = 0; i < 5; i++){
        hand[i] = deal_card(deck);
    }
    /*sorts the hand based on rank*/
    sort_hand(hand);
    return hand;
}

int sort_hand(Card** hand){
    /*qsort for sorting of hand*/
    qsort(hand, 5, sizeof(Card*), compar);
    return 0;
}

int compar(const void* carda, const void* cardb){
    /*compares the rank of two cards for th card sort function*/
    Card* card1 = *(Card**) carda;
    Card* card2 = *(Card**) cardb;
    /*printf("%s " "%d " "%d\n", "Comparing", card1->rank, card2->rank);*/
    if(card1->rank > card2->rank)
        return -1;
    else if (card1->rank < card2->rank)
        return 1;
    else return 0;
}

int deal(Table* table){
    /*deals the initial hands to the players at the table*/
    int i;
    for(i = 0; i < 4; i++)
        if(table->players[i]->inGame == 1){
            table->players[i]->hand = create_hand(table->deck);
            table->players[i]->handRank = get_hand_score(table->players[i]->hand);
        }
    return 0;
}

int swap(Card* card1, Card* card2){
    /*swap the position of two cards within the deck*/
    Card* temp = (Card*) malloc(sizeof(Card));
    *temp = *card2;
    *card2 = *card1;
    *card1 = *temp;
    free(temp);
    return 0;
}


/*----------------------------------------------*/
/*simple helper functions*/

int get_rand(){
    /*returns a random number between 0 and 51*/
    return rand() % 52;
}

int print_player(Player* player){
    /*prints the player's number*/
    return player->pnum;
}

int print_deck(Deck* deck){
    /*prints all the cards in the deck*/
    int i;
    for(i = 0; i < 52; i++){
        printf("%d %s %d\n", i, print_card(deck->cards[i]), deck->cards[i]->num);
    }
    return 0;
}

int print_hand(Card** hand){
    /*prints all the cards in the deck*/
    int i;
    for(i = 0; i < 5; i++){
        printf("%d ", i);
        print_card(hand[i]);
    }
    return 0;
}

int destroy_game(Table* table, Option* userChoice){
    /*frees the malloc information to
     stop memory leaks*/
    int i;

    free(userChoice);
    /*free cards*/
    for(i = 0; i < 52; i++){
        free(table->deck->cards[i]);
        free(table->testDeck->cards[i]);
    }
    
    /*free player hands (cards themselves 
     already freed) and the players themselves*/
    for(i = 0; i < NUMBEROFPLAYERS; i++){
        free(table->players[i]->hand);
        free(table->players[i]);
    }
    /*free deck, player list, and table*/
    free(table->deck);
    free(table->testDeck);
    free(table->players);
    free(table);
    return 0;
}

