
#define NUMTESTHANDS 15000

/*prototypes*/
Option_List* get_option_list();
int find_option_score(Deck*, Option*, Card**, int);
Option* find_best_option(Option_List*);
int print_option_results(Option_List*);
int make_decision(Option*, Player*, Deck*, Option_List*);
Option* ask_MC_Advisor(Card**, Option_List*, Deck*);
char* get_perm(int);
int* get_perm2(int);
Card** copy_hand(Card**);
int clear_option_list(Option_List*);
int destroy_deck(Table*);


/*NOTE:  This file deals with the MC Advisor, its comparing of discard
 options and the eventual advice to the player.*/

int execute_round(Table* table, Option* userChoice, Option_List* list1){
    /*overarching function that executes mechanics of a round*/
    int i;
    /*get the decision from User*/
    printf("%s\n"
           , "------------------------------------------------------");
    make_decision(userChoice, table->players[0], table->deck, list1);

    /*decision from computer players*/
    for(i = 1; i < 4; i++)
        if(table->players[i]->inGame == 1){
            make_decision(ask_MC_Advisor(table->players[i]->hand, list1, table->testDeck)
                      , table->players[i], table->deck, list1);
        
        }
    printf("%s\n"
           , "------------------------------------------------------");
        return 0;
}

int make_decision(Option* option, Player* player, Deck* deck, Option_List* list1){
    /*this function goes about switching out the cards after a player has made his
     final decision and requests new cards for his hand*/
    int i;
    int k = 0;
    
    /*switching out the cards*/
    for(i = 0; i < 5; i++){
        if((option->data[i]-'0') == 1){
            swap_card(player->hand, i, deck);
            k++;
        }
    }
    /*sorting the new hand*/
    sort_hand(player->hand);
    
    /*get the score for the newly created hand*/
    player->handRank = get_hand_score(player->hand);
    
    /*output to console how many cards the player took*/
    printf("%s " "%s " "%d " "%s\n", player->name, "Takes", k, "cards.");
    return 0;
}

Option* ask_MC_Advisor(Card** hand, Option_List* list1, Deck* testDeck){
    /*this function does manages the main part of the advisor, the 
     tallying of potential hands*/
    int i;
    Option* temp;
    
    /*create test deck that will be used to switch out cards when
     making hypothetical hands*/

    shuffle(testDeck);
    
    /*for each of the 32 options go about finding the option score*/
    /*the last argument is the number of test hands to check*/
    for(i = 0; i < 32; i++)
        find_option_score(testDeck, list1->list[i], hand, NUMTESTHANDS);
    
    /*find the best option*/
    temp = find_best_option(list1);
    
    /*clears the option list*/
    clear_option_list(list1);
    
    /*returns the best option*/
    return temp;

}

int clear_option_list(Option_List* list1){
    /*clears the option list so that the next testhand 
     has proper initial values*/
    int i;
    for(i = 0; i < 32; i++){
        list1->list[i]->sum = 0;
        list1->list[i]->count = 0;
    }
    return 0;
}

Option* find_best_option(Option_List* list1){
    /*after all the option sums are tallied, this 
     functions finds the max sum and its position*/
    int i;
    /*setting initial values to first option*/
    int bestPosition = 0; int best;
    best = list1->list[0]->sum;
    
    /*loop to find max*/
    for(i = 1; i < 32; i++)
        if(list1->list[i]->sum > best){
            best = list1->list[i]->sum;
            bestPosition = i;
        }
    
    /*return best option*/
    return list1->list[bestPosition];
}

int test_hand(Card** hand, Deck* deck, Option* option){
    /*creates a copy of the user's hand and swaps out card 
     based on the option provided, returning the value of 
     this test_hand, which is used to determine which option
     is best*/
    int i; int value;
    Card** temp;
    
    /*create copy hand*/
    temp = copy_hand(hand);

    /*swap out cards of copied hand based on option*/
    for(i = 0; i < 5; i++){
        if((option->data[i]-'0') == 1){
            swap_test_card(temp, i, deck);
        }
    }
    /*value gets the the value(rank) of hypo swapped hand*/
    value = get_hand_score(temp);
    
    /*copied hand is freed*/
    free(temp);

    return value;
}

int find_option_score(Deck* deck, Option* option, Card** hand, int num){
    int i;
    /*this is the loop that does most of the work for the MC Advisor*/
    /*for num times, the value of the test hand will be added to the 
     option "sum" and the count for that option will also increment*/
    for(i = 0; i < num; i++){
        option->sum += test_hand(hand, deck, option);
        option->count += 1;
    }
    return 0;
}

Card** copy_hand(Card** hand){
    /*copies the hand so that it can be manipulated without 
     changing the data of the original hand*/
    int i;
    Card** temp = (Card**) malloc(sizeof(Card*)*5);
    for(i = 0; i < 5; i++)
        temp[i] = hand[i];
    return temp;
}


Option_List* get_option_list(){
    /*creates a list of "options" that are compared to find the best
     option which is eventually provided to the user as the MC advisor's
     advice*/
    int i;
    Option_List* Olist = (Option_List*) malloc (sizeof(Option_List));
    Olist->list = (Option**) malloc(sizeof(Option*)*32);
    for(i = 0; i < 32; i++){
        Olist->list[i] = (Option*) malloc(sizeof(Option));
        Olist->list[i]->data = get_perm(i);
        Olist->list[i]->sum = 0;
        Olist->list[i]->count = 0;
    }
    return Olist;
}

char* get_perm(int permNum){
    /*gets all the potential discard options for any hand*/
    /*if there were more than 32 options one might consider writing
     a permutation function but with just 32 that won't change I hardcoded
     them in*/
    char* perm = (char*) malloc(sizeof(char)*5);
    
    switch(permNum){
        case 0: perm = "00000"; break;
            
        case 1: perm = "10000"; break;
        case 2: perm = "01000"; break;
        case 3: perm = "00100"; break;
        case 4: perm = "00010"; break;
        case 5: perm = "00001"; break;
            
        case 6: perm = "11000"; break;
        case 7: perm = "10100"; break;
        case 8: perm = "10010"; break;
        case 9: perm = "10001"; break;
        case 10: perm = "01100"; break;
        case 11: perm = "00110"; break;
        case 12: perm = "00011"; break;
        case 13: perm = "01010"; break;
        case 14: perm = "01001"; break;
        case 15: perm = "00101"; break;
            
        case 16: perm = "11100"; break;
        case 17: perm = "11010"; break;
        case 18: perm = "11001"; break;
        case 19: perm = "10110"; break;
        case 20: perm = "10011"; break;
        case 21: perm = "10101"; break;
        case 22: perm = "00111"; break;
        case 23: perm = "01011"; break;
        case 24: perm = "01101"; break;
        case 25: perm = "01110"; break;
            
        case 26: perm = "01111"; break;
        case 27: perm = "10111"; break;
        case 28: perm = "11011"; break;
        case 29: perm = "11101"; break;
        case 30: perm = "11110"; break;
            
        case 31: perm = "11111"; break;
            
    }
    return perm;
}

int print_option_results(Option_List* list1){
    /*this is a test function for checking the advisor data*/
    int i;
    for(i = 0; i < 32; i++)
        printf("%d " "%s " "%d " "%d\n"
               ,i
               ,list1->list[i]->data
               ,list1->list[i]->sum
               ,list1->list[i]->count
               );
    return 0;
}


