#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int prompt_for_mcAdvice();
char* print_card(Card*);
int print_player_full(Player*, Option* option);
char* convert_advice(char);
Option* get_user_choice(Player*, Option*, Option*);
int get_user_input();
int print_player_full2(Player*, Option*, Option*, int);
int print_pot(Table*);
int print_final_hand(Player*);
int comp_players(const void*, const void*);


Option* player_prompt(Table* table, Option* mcAdvice, Option* userChoice){
    print_player_full(table->players[0], mcAdvice);
    if(prompt_for_mcAdvice() == 1)
        return mcAdvice;
    /*else prompt_for_cards();*/
    else return get_user_choice(table->players[0], mcAdvice, userChoice);
    return 0;
}

Option* create_user_option(){
    /*create one option, for user input*/
    Option* userChoice = (Option*) malloc(sizeof(Option));
    userChoice->data = (char*) malloc(sizeof(char)*5);
    return userChoice;
}

Option* get_user_choice(Player* player, Option* mcAdvice, Option* userChoice){
    /*this is what is used if the user chooses to discard/hold
     his own cards*/
    int i;

    printf("%s\n", "CHOOSE WHICH CARD TO HOLD/DISCARD");
    for(i = 0; i < 5; i++){
        printf("%s " "%d" "%s" "%s" "%s\n", "Would You Like to Hold Card", i+1, ": "
               , print_card(player->hand[i]), "?");
        if(get_user_input() == 1){
            printf("%s" "%s\n", "You Chose to Hold the", print_card(player->hand[i]));
            userChoice->data[i] = '0';
        }
        else {
            printf("%s " "%s\n", "You Chose to Discard the", print_card(player->hand[i]));
            userChoice->data[i] = '1';
        }
        print_player_full2(player, mcAdvice, userChoice, i+1);
        
    }

    printf("%s\n", "Are You Happy With Your Choice? (Y)es or (N)o");
    if(get_user_input() == 0){
         printf("%s\n", "Let's Try Again");
        return get_user_choice(player, mcAdvice, userChoice);
    }
         printf("%s\n", "Sending In Your Changes");
        return userChoice;
}

int prompt_for_mcAdvice(){
    /*prompts user to determine if they would like to take MC advice*/
    printf("%s\n", "Would You Like To Use The Monte Carlo Advisor's Advice? (Y)es or (N)o");
    return get_user_input();
}

int prompt_for_continue(int roundNum){
    /*after round, waits for prompt to start new round*/
    printf("%s\n"
           , "---------------------------------------------------------------------");
    printf("%s %d %s\n", "Round", roundNum, "Over.  Would You Like To Start Next Round? (Y)es OR (Q)uit");
    printf("%s\n"
           , "---------------------------------------------------------------------");
    return get_user_input();
}

int get_user_input(){
    /*sanitize input*/
    char test[100];
    scanf("%s", test);
    if(test[0] == 'Y' || test[0] == 'y'){
        printf("%s\n", "[You Said Yes (Y)]");
        return 1;
    }
    else if(test[0] == 'N' || test[0] == 'n'){
        printf("%s\n", "[You Said No (N)]");
        return 0;
    }
    else if (test[0] == 'Q' || test[0] == 'q' || test[0] == EOF){
        printf("%s\n", "GoodBye!");
        exit(1);
    }
    else {
        printf("%s\n", "(Y or N)");
        return get_user_input();
    }
}

int get_round_winner(Table* table){
    /*goes through players and shows scores, then returns the winner*/
    int i;
    int bestPlayer = 0;
    int bestScore = get_hand_score(table->players[0]->hand);
    printf("%s\n"
           , "---------------------------------------------------------------------");
    

    for(i = 0; i < 4; i++){
        /*check if player is still in game*/
        if(table->players[i]->inGame == 1){
            /*determine if player has highest score*/
            if(get_hand_score(table->players[i]->hand) > bestScore){
                bestScore = get_hand_score(table->players[i]->hand);
                bestPlayer = i;
            }
            
            /*print score to screen*/
            printf("%s " "%s " "%d " "%s "
                   , table->players[i]->name, "scores"
                   , get_hand_score(table->players[i]->hand)
                   ,"with a");
            describe_hand(table->players[i]);
        }
    }
    printf("%s\n"
           , "------------------------------------------------------");
    
    /*print information about winner*/
    printf("%s ", table->players[bestPlayer]->name);
    if(table->players[bestPlayer]->pnum == 0)
        printf("%s\n", "Win!!!");
    else printf("%s\n", "Wins!");

    return bestPlayer;
}

int print_players(Table* table){
    /*prints the player number and hand for all four players*/
    int i;
    for(i = 0; i < NUMBEROFPLAYERS; i++){
        printf("%s", "-------");
        printf("%s " "%d", "Player", print_player(table->players[i]));
        printf("%s\n", "-------");
        print_hand(table->players[i]->hand);
        putchar('\n');
        
    }
    return 0;
}

Player** copy_players(Table* table){
    Player** temp = (Player**) malloc(sizeof(Player*)*4);
    int i;
    for(i = 0; i < 4; i++){
        temp[i] = (Player*) malloc(sizeof(Player));
        temp[i] = table->players[i];
    }
    return temp;
}

Player** sort_players(Player** playerList){
    qsort(playerList, 4, sizeof(Player*), comp_players);
    return playerList;
}

int comp_players(const void* playera, const void* playerb){
    /*used in qsort when sort players for display*/
    Player* player1 = *(Player**) playera;
    Player* player2 = *(Player**) playerb;
    if(player1->chips > player2->chips)
        return -1;
    else if (player1->chips < player2->chips)
        return 1;
    else return 0;
}


char* convert_advice(char x){
    /*converts the option advice provided by MC advisor to 
     "hold" or "discard" statements*/
    char* toReturn = (char*) malloc(sizeof(char)*7);
    if((x-'0') == 1)
        toReturn = "Discard";
    else toReturn = "Hold";
    return toReturn;
}

char* print_card(Card* card){
    /*prints an individual card's rank and suit*/
    char* rank;
    char* suit;
    char* cardStats = (char*) malloc(sizeof(char)*20);
    rank = print_rank(card);
    suit = print_suit(card);
    strcat(cardStats, rank);
    strcat(cardStats, " of ");
    strcat(cardStats, suit);

    return cardStats;
}

char* print_rank(Card* card){
    /*method by which rank is sent to print statement*/
    char* rank = (char*) malloc(sizeof(char)*6);
    switch(card->rank){
        case 2: rank = "Two"; break;
        case 3: rank = "Three"; break;
        case 4: rank = "Four"; break;
        case 5: rank = "Five"; break;
        case 6: rank = "Six"; break;
        case 7: rank = "Seven"; break;
        case 8: rank = "Eight"; break;
        case 9: rank = "Nine"; break;
        case 10: rank = "Ten"; break;
        case 11: rank = "Jack"; break;
        case 12: rank = "Queen"; break;
        case 13: rank = "King"; break;
        case 14: rank = "Ace"; break;
    }
    return rank;
}

char* print_suit(Card* card){
    /*method by which the suit of a card is sent to print statements*/
    char* suit = (char*) malloc(sizeof(char)*8);
    switch(card->suit){
        case 0: suit = "Hearts"; break;
        case 1: suit = "Diamonds"; break;
        case 2: suit = "Spades"; break;
        case 3: suit = "Clubs"; break;
    }
    return suit;
}

int print_player_full(Player* player, Option* option){
    /*prints out the Users hand, and MC advisor data, also used later if the user
     would like to choose his/her own cards to discard*/
    int i;
    char* card1;
    putchar('\n');
    printf("%s\n"
           , "---------------------------------------------------------------------");
    
    printf("%1s%-25s%1s%-20s%1s%-20s%1s\n"
           , "|", "          YOUR HAND", "|", "    YOUR CHOICE", "|" ,"      MC ADVISOR", "|");
    printf("%1s%-25s%1s%-20s%1s%-20s%1s\n"
           , "|", "-------------------------", "|", "--------------------", "|" ,"--------------------", "|");
    
    
    
    for(i = 0; i < 5; i++){
        card1 = print_card(player->hand[i]);
        printf("%1s%-24s%1s%-19s%1s%-19s%1s\n"
               , "| ", card1, "| ", "------------------ ", "| " ,convert_advice(option->data[i]), "|");
    }
    /*free(card);*/ /*free card that was malloced in print_card*/
    printf("%s\n"
           , "---------------------------------------------------------------------");
    
    
    printf("%1s %5s ", "|", "SUMMARY OF YOUR HAND:");
    describe_hand(player);
    
    printf("%s\n"
           , "---------------------------------------------------------------------");
    
    return 0;
}

int print_player_full2(Player* player, Option* option, Option* userChoice, int num){
    /*second print function for hand, used when player opts to choose his/her own
     cards to discard/hold.  The for loops is the only difference.  only the cards
     chosen will be displayed*/
    int i;
    putchar('\n');
    printf("%s\n"
           , "---------------------------------------------------------------------");
    
    printf("%1s%-25s%1s%-20s%1s%-20s%1s\n"
           , "|", "          YOUR HAND", "|", "    YOUR CHOICE", "|" ,"      MC ADVISOR", "|");
    printf("%1s%-25s%1s%-20s%1s%-20s%1s\n"
           , "|", "-------------------------", "|", "--------------------", "|" ,"--------------------", "|");
    
    
    
    for(i = 0; i < num; i++){
        printf("%1s%-24s%1s%-19s%1s%-19s%1s\n"
               , "| ", print_card(player->hand[i]), "| ", convert_advice(userChoice->data[i]), "| " ,convert_advice(option->data[i]), "|");
    }
    printf("%s\n"
           , "---------------------------------------------------------------------");
    
    return 0;
}


int print_final_hand(Player* player){
    int i;
    putchar('\n');
    printf("%s\n"
           , "---------------------------------------------------------------------");
    
    printf("%1s%-25s%1s%-20s%1s%-20s%1s\n"
           , "|", "          YOUR HAND", "|", "    YOUR CHOICE", "|" ,"      MC ADVISOR", "|");
    printf("%1s%-25s%1s%-20s%1s%-20s%1s\n"
           , "|", "-------------------------", "|", "--------------------", "|" ,"--------------------", "|");
    
    
    
    for(i = 0; i < 5; i++){
        printf("%1s%-24s%1s%-19s%1s%-19s%1s\n"
               , "| ", print_card(player->hand[i]), "| ", "-------------------", "| " ,"-", "|");
    }
    printf("%s\n"
           , "---------------------------------------------------------------------");
    
    
    
    printf("%1s %5s ", "|", "FINAL HAND:");
    describe_hand(player);
    
    printf("%s\n"
           , "---------------------------------------------------------------------");
    
    return 0;
}





int print_scores(Table* table){
    int i;
    Player** temp = copy_players(table);
     sort_players(temp);
    
    
    
    
    printf("%s\n"
           , "+----------------------------------------------+");
    
    printf("%1s%-25s%1s%-20s%1s\n"
           , "|", "          PLAYER", "|", "    SCORE", "|");
    printf("%1s%-25s%1s%-20s%1s\n"
           , "|", "-------------------------", "|", "--------------------", "|");
    
    
    
    for(i = 0; i < 4; i++){
        printf("%1s%-24s%1s%-19d%1s\n"
               , "| ", temp[i]->name , "| ", temp[i]->chips , "| ");
    }
    printf("%s\n"
           , "+----------------------------------------------+");
    

    return 0;
}

int print_pot(Table* table){
        printf("%s " "%d\n", "CURRENT POT:", table->pot);
    return 0;
}

int print_pointers(Table* table){
    printf("%s %d\n", "POINTERS:", table->deck->topDeck);
    return 0;
}


int set_default_pnames(Table* table){
    table->players[0]->name = "Player 1 (You)";
    table->players[1]->name = "Player 2";
    table->players[2]->name = "Player 3";
    table->players[3]->name = "Player 4";
    
    return 0;
}