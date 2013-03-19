
/*prototypes*/
int single_cards(HandData*, Card**);
int at_least_a_pair(HandData*);
int at_least_a_three_of_a_kind(HandData*);
int four_of_a_kind(HandData*);
int print_rem(Card*);
int single_cards_describe(HandData*, Card**);
int at_least_a_pair_describe(HandData*);
int at_least_a_three_of_a_kind_describe(HandData*);
int four_of_a_kind_describe(HandData*);
int describe_hand(Player*);
int compar2(const void*, const void*);


/*NOTE:  This file deals with ranking a player's hand by analyzing
 the suit, rank, and count of each card within a given hand.*/


Helper* hand_helper_init(int count, Card* card){
    /*creates a hand helper, which is used to help find
     the value of the hand by analyzing rank and count of 
     that rank. e.g. it will find if there are
     three sevens, or two queens.*/
    Helper* helper = (Helper*) malloc(sizeof(Helper));
    helper->count = count;
    helper->card = card;
    return helper;
}

HandData* hand_data_init(){
    /*handData is a group of hand helpers that help determine 
     rank and count for the hand as a whole.  e.g. if there 
     are three sevens and two queens in the hand, this handData
     will hold all of that information while the helpers only hold
     the information of an individual rank*/
    HandData* newHandData = (HandData*) malloc(sizeof(HandData));
    newHandData->data = (Helper**) malloc(sizeof(Helper*)*5);
    return newHandData;
}

int get_count(Card** hand, int cardNum){
    /*find the count for a known rank in a hand e.g. if 
     there is clearly a seven in the hand, it will look for
     other sevens in the same hand*/
    int i, count = 1;
    for(i = 0; i < 5; i++)
        if(i != cardNum)
            if(hand[i]->rank == hand[cardNum]->rank)
                count++;
    return count;
}

int sort_handData(Helper** data, int numElements){
    /*sorts the hand helper data so that it is easier to 
     determine the value of the hand*/
    qsort(data, numElements, sizeof(Helper*), compar2);
    return 0;
}

int compar2(const void* helpera, const void* helperb){
    /*compar2 compares the count in the hand helper option, putting
     the most counts first.  Thereby sorting the helper hand data.
     The sorted helper data is used to get the value of the hand.
     by sorting the helper it is easier to find if there are
     4, 3, or 2 of a kinds*/
    Helper* helper1 = *(Helper**) helpera;
    Helper* helper2 = *(Helper**) helperb;
    
    if(helper1->count > helper2->count)
        return -1;
    else if (helper1->count < helper2->count)
        return 1;
    else return 0;
}

HandData* analyze_hand(Card** hand){
    /*this function does all the work envision within the hand helpers
     and hand data.  It get the hand helpers, finds the count for each
     rank within the hand, then sorts that rank so that the "determining"
     card and count are first.  e.g. if there are three kings in a hand, the 
     handdata should have a cound of 3, connected to a rank of king as its
     first data element, since this will likely determine what the value 
     of the hand is*/
    
    /*create handData*/
    HandData* newHandData = hand_data_init();
    int i, j = 1;
    /*create first hand helper for at least the first card*/
    newHandData->data[0] = hand_helper_init(get_count(hand, 0), hand[0]);

    /*find the count for each unique rank within the hand*/
    for(i = 1; i < 5; i++){
        if(hand[i]->rank != hand[i-1]->rank){
            newHandData->data[j] = hand_helper_init(get_count(hand, i), hand[i]);
            j++;
        }
    }
    
    /*sorts the hand helper data (count and rank) by count so that the determining
     rank is first in the data element*/
    sort_handData(newHandData->data, j);
    
    return newHandData;
}

int check_for_straight(HandData* handData){
    /*checks to see if the hand contains a straight*/
    int i;
    /*check for lowest straight i.e. A,2,3,4,5*/
    if(handData->data[0]->card->rank == 14
       && handData->data[1]->card->rank == 5
       && handData->data[2]->card->rank == 4
       && handData->data[3]->card->rank == 3
       && handData->data[4]->card->rank == 2)
        return 2;

    /*check for all other straights*/
    for(i = 0; i < 4; i++){
        if(handData->data[i]->card->rank == (handData->data[i+1]->card->rank) + 1){
        }
        else return 0;
    }
    return 1;
}

int check_for_flush(Card** hand){
    /*checks to see if the hand contains a flush*/
    int i;
    for(i = 0; i < 4; i++){
        if(hand[i]->suit == hand[i+1]->suit){}
        else return 0;
    }
    return 1;
}

int get_hand_score(Card** hand){
    /*this function get the handData and 
     determines the value of the hand*/
    
    
    HandData* handData = analyze_hand(hand);
    
    /*finds the score for the hand provided*/
    if(handData->data[0]->count == 1)
        /*check hand for straight*/
        return single_cards(handData, hand);
            
    else if(handData->data[0]->count == 2)
        return at_least_a_pair(handData);
        
    else if(handData->data[0]->count == 3)
        /*hand has at least three pair*/
        return at_least_a_three_of_a_kind(handData);

    else if(handData->data[0]->count == 4)
        return four_of_a_kind(handData);
    else return 0;
}

int describe_hand(Player* player){
    /*used to describe the hand to the user*/
    
    HandData* handData = analyze_hand(player->hand);
    /*finds the score for the hand provided*/
    if(handData->data[0]->count == 1)
    /*check hand for straight*/
        return single_cards_describe(handData, player->hand);
    
    else if(handData->data[0]->count == 2)
        return at_least_a_pair_describe(handData);
    
    else if(handData->data[0]->count == 3)
    /*hand has at least three pair*/
        return at_least_a_three_of_a_kind_describe(handData);
    
    else if(handData->data[0]->count == 4)
        return four_of_a_kind_describe(handData);
    else return 0;
}




/*-------------Ranking Helper Functions Below--------------*/


/*---scenario 1: All Single Cards-------------------*/
int single_cards(HandData* handData, Card** hand){
        int straightCheck;
        int flushCheck = check_for_flush(hand);
    
        /*check hand for straight*/
        if((straightCheck = check_for_straight(handData)) == 1 || straightCheck == 2){
            if(flushCheck == 1){  /*check hand for flush*/
                /*check for Royal Flush*/
                if(handData->data[0]->card->rank == 14){
                    return 97;
                }
                /*hand is some type of Straigh Flush*/

                /*check for A,2,3,4,5 flush straight (unique case)*/
                if(straightCheck == 2)
                    return 88;
                
                /*check for all other flush straights*/
                else{
                    switch(handData->data[0]->card->rank){
                        case 6: return 140;
                        case 7: return 141;
                        case 8: return 142;
                        case 9: return 143;
                        case 10: return 144;
                        case 11: return 145;
                        case 12: return 146;
                        case 13: return 147;
                        /*case 6: return 89;
                        case 7: return 90;
                        case 8: return 91;
                        case 9: return 92;
                        case 10: return 93;
                        case 11: return 94;
                        case 12: return 95;
                        case 13: return 96*/
                    }
                }
                
            }
            else{
                if(straightCheck == 2)
                    return 52;
                /*hand is straight but not flush*/
                
                /*score straight*/
                switch(handData->data[0]->card->rank){
                    case 6: return 100;
                    case 7: return 101;
                    case 8: return 102;
                    case 9: return 103;
                    case 10: return 104;
                    case 11: return 105;
                    case 12: return 106;
                    case 13: return 107;
                    case 14: return 108;
                    /*case 6: return 52;
                    case 7: return 53;
                    case 8: return 54;
                    case 9: return 55;
                    case 10: return 56;
                    case 11: return 57;
                    case 12: return 58;
                    case 13: return 59;
                    case 14: return 60;*/
                }
            }
        }
        
        /*hand is not straight, check for flush*/
        else if(flushCheck == 1){
            /*score flush*/
            return 62;
        }
        
        else{
        /*hand is not a straight or a flush*/
        /*therefore hand is "Nothing"*/
            
            /*score Nothing hand*/
            switch(handData->data[0]->card->rank){
                case 7: return 6;
                case 8: return 7;
                case 9: return 8;
                case 10: return 9;
                case 11: return 10;
                case 12: return 11;
                case 13: return 12;
                case 14: return 13;
            }
        }
    return 0;
}


/*----scenario 2: at least one pair in hand------*/

int at_least_a_pair(HandData* handData){
    if(handData->data[1]->count == 2){
        /*hand is two pair*/

        /*score two pair*/
        switch(handData->data[0]->card->rank){
            case 3: return 27;
            case 4: return 28;
            case 5: return 29;
            case 6: return 30;
            case 7: return 31;
            case 8: return 32;
            case 9: return 33;
            case 10: return 34;
            case 11: return 35;
            case 12: return 36;
            case 13: return 37;
            case 14: return 38;
        }
    }
    else {
        /*hand has only one pair*/
        
        /*score single pair*/
        switch(handData->data[0]->card->rank){
            case 2: return 14;
            case 3: return 15;
            case 4: return 16;
            case 5: return 17;
            case 6: return 18;
            case 7: return 19;
            case 8: return 20;
            case 9: return 21;
            case 10: return 22;
            case 11: return 23;
            case 12: return 24;
            case 13: return 25;
            case 14: return 26;
        }
    }
    return 0;
}

/*-----Scenario 3: At least a three of a kind----------*/

int at_least_a_three_of_a_kind(HandData* handData){
    if(handData->data[1]->count == 2){
        
        /*score full house*/
        switch(handData->data[0]->card->rank){
            case 2: return 109;
            case 3: return 110;
            case 4: return 111;
            case 5: return 121;
            case 6: return 122;
            case 7: return 123;
            case 8: return 124;
            case 9: return 125;
            case 10: return 126;
            case 11: return 127;
            case 12: return 128;
            case 13: return 129;
            case 14: return 130;
            /*case 3: return 63;
            case 4: return 64;
            case 5: return 65;
            case 6: return 66;
            case 7: return 67;
            case 8: return 68;
            case 9: return 69;
            case 10: return 70;
            case 11: return 71;
            case 12: return 72;
            case 13: return 73;
            case 14: return 74;*/
        }
        
    }
    else {
        /*hand not full house but three of a kind*/
        /*score three of a kind*/
        switch(handData->data[0]->card->rank){
            case 2: return 39;
            case 3: return 40;
            case 4: return 41;
            case 5: return 42;
            case 6: return 43;
            case 7: return 44;
            case 8: return 45;
            case 9: return 46;
            case 10: return 47;
            case 11: return 48;
            case 12: return 49;
            case 13: return 50;
            case 14: return 51;
        }
    }
    return 0;
}

/*----Scenario 4:  Four Of A Kind------------*/

int four_of_a_kind(HandData* handData){
    
    /*score four of a kind*/
    switch(handData->data[0]->card->rank){
        case 2: return 75;
        case 3: return 76;
        case 4: return 77;
        case 5: return 78;
        case 6: return 79;
        case 7: return 80;
        case 8: return 81;
        case 9: return 82;
        case 10: return 83;
        case 11: return 84;
        case 12: return 85;
        case 13: return 86;
        case 14: return 87;
    }
    return 0;
}

int print_rem(Card* card){
    printf("%s ", ",");
    printf("%s", print_rank(card));
    printf("%s\n", " High");
    return 0;
}



/*-----------Describer Helper Functions Below---------------------*/



int single_cards_describe(HandData* handData, Card** hand){
    int straightCheck;
    int flushCheck = check_for_flush(hand);
    /*printf("%s-->" "%d\n", "Flush Flag", flushCheck);*/
    /*check hand for straight*/
    if((straightCheck = check_for_straight(handData)) == 1 || straightCheck == 2){
        if(flushCheck == 1){  /*check hand for flush*/
            /*check for Royal Flush*/
            if(handData->data[0]->card->rank == 14){
                printf("%s\n", "ROYAL FLUSH!!!");
                return 0;
            }
            else{
                /*hand is some type of Straigh Flush*/
                printf("%s", "Straight Flush");
                print_rem(handData->data[0]->card);
                return 0;
            }
        }
        else{
            printf("%s", "Straight");
            print_rem(handData->data[0]->card);
            return 0;
        }
    }
    else if(flushCheck == 1){
        printf("%s", "Flush");
        print_rem(handData->data[0]->card);
        return 0;
    }
    else{
        /*hand is not a straight or a flush*/
        /*therefore hand is "Nothing"*/
        printf("%s", "Nothing");
        print_rem(handData->data[0]->card);
        return 0;
    }
    return 0;
}


/*----scenario 2: at least one pair in hand------*/

int at_least_a_pair_describe(HandData* handData){
    if(handData->data[1]->count == 2){
        /*hand is two pair*/
        printf("%s", "Two Pair");
        print_rem(handData->data[0]->card);
        return 0;
    }
    else {
        /*hand has only one pair*/
        printf("%s", "Single Pair");
        print_rem(handData->data[0]->card);
        return 0;
    }
    return 0;
}

/*-----Scenario 3: At least a three of a kind----------*/

int at_least_a_three_of_a_kind_describe(HandData* handData){
    if(handData->data[1]->count == 2){
        printf("%s", "Full House");
        print_rem(handData->data[0]->card);
    }
    else {
        /*hand not full house but three of a kind*/
        printf("%s", "Three Of A Kind");
        print_rem(handData->data[0]->card);
        return 0;
    }
    return 0;
}

/*----Scenario 4:  Four Of A Kind------------*/

int four_of_a_kind_describe(HandData* handData){
    printf("%s", "Four Of A Kind");
    print_rem(handData->data[0]->card);
    return 0;
}
