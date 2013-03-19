int get_ante(Player*, int);
int get_bet(Player*);
int divvy_pot(Table*, int);
int get_rand_bet();
int check_scores(Table*);

/*This file focuses on the betting mechanics behind the game.  Currently, the betting
 mechanism only allows for a simple AI betting scheme and no "calling" or user input*/

int round_of_betting(Table* table){
    /*initiates a round of betting*/
    int i, bet;
    printf("%s\n"
           , "------------------------------------------------------");
    
    /*get bets for each player still active in game*/
    for(i = 0; i < 4; i++){
        if(table->players[i]->inGame == 1){
            /*inGame is used as flag to determine which players
             are still in the game*/
            bet = get_bet(table->players[i]);
            printf("%s %s %d %s\n", table->players[i]->name, "bets", bet, "chips.");
            table->pot += bet;
        }
    }
    
    /*prints pot total*/
    printf("%s %d %s\n", "Pot Is Now:", table->pot, "chips.");
    return 0;
}


int get_bet(Player* player){
    /*very basic AI that will find how much a player is willing to bet*/
    int bet;
    
    /*get random number to create random chance element to AI*/
    int perc = get_rand_bet();
    
    /*analysis of the rank of the players hand with a simple chance
     component to provide variability.*/
    if(player->handRank < 20)
        if(perc > 10) bet = player->bet * .6;
        else bet = 0;
    else if(player->handRank < 30)
        if (perc < 20) bet = player->bet;
        else if (perc < 60) bet = player->bet * 1.8;
        else if (perc > 90) bet = player->chips;
        else bet = player->bet * 2.0;
    else if(player->handRank >= 36)
        if (perc < 5) bet = player->bet;
        else if (perc < 60) bet = player->bet * 1.8;
        else if (perc > 75) bet = player->chips;
        else bet = player->bet * 3.0;
        else printf("%s\n", "problem");
    
    /*check to make sure bet isn't greater than # of chips
     available to that player*/
    if(bet > player->chips)
        bet = player->chips;
    
    player->chips -= bet;
    return bet;
}

int ante_up(Table* table, int roundNum){
    /*gets antes from still active players*/
    int i, ante;
    int bet = 0;
    int pot = 0;
    
    /*simple ante increase over rounds*/
    if(roundNum < 5)
        ante = 40;
    else if (roundNum < 9)
        ante = 80;
    else if (roundNum < 13)
        ante = 120;
    else if (roundNum < 17)
        ante = 200;
    else ante = 500;
    
    /*print current ante*/
    printf("%s %d\n", "Current Ante Is:", ante);
    
    /*get antes from active players*/
    for(i = 0; i < 4; i++)
        if(table->players[i]->inGame == 1){
            bet = get_ante(table->players[i], ante);
            
            /*print ante status for players*/
            if(bet == ante)
                printf("%s " "%s " "%d " "%s\n", table->players[i]->name, "antes", bet, "chips.");
            else printf("%s " "%s " "%d " "%s\n", table->players[i]->name, "antes", bet, "chips (all chips left).");
            
            pot += bet;
        }
    
    return pot;
}

int get_ante(Player* player, int ante){
    /*mechanics behind finding a player's ante*/
    
    /*biggest concern is ante is greater than available
     chips to that player*/
    
    /*if ante is greater than chips available, all chips are 
     put into the the ante and player is allowed to stay in
     current round*/
    if(player->chips > ante){
        /*normal option*/
        player->chips -= ante;
    }
    
    else{
        /*option when chips are less than ante*/
        ante = player->chips;
        player->chips = 0;
    }
    
    player->bet += ante;
    return ante;
}

int divvy_pot(Table* table, int winner){
    /*divvys pot at end of round*/
    int i;

    table->players[winner]->chips += table->pot;
    for(i = 0; i < 4; i++)
        if(table->players[i]->chips == 0)
            table->players[i]->inGame = 0;
    
    /*print pot winner and pot amount*/
    printf("%s %s %d %s\n"
           , table->players[winner]->name, "has won", table->pot, "chips.");
    
    printf("%s\n"
           , "---------------------------------------------------------------------");
    /*reset pot*/
    table->pot = 0;
    return 0;
}

int check_for_game_winner(Table* table){
    /*check if there is a winner to the entire game.
     this is either Player 1 (user) losing all chips
     or other three players have zero chips at end of round*/
    
    /*check_scores does most of the work behind determining 
     the winner.*/
    int check = check_scores(table);
    
    /*if check == 1, game continues*/
    if(check == 1)
        return 1;
    
    /*option if user has won*/
    else if (check == 2){
        printf("%s\n", "YOU HAVE WON THE GAME!!!");
        return 0;
    }
    /*option if user has lost*/
    else if (check == 0){
        printf("%s\n", "YOU HAVE LOST!!!");
         printf("%s\n", "THANKS FOR PLAYING");
        return 0;
    }
    /*option if something went wrong*/
    else printf("%s\n", "PROBLEM");
    
    return 0;
    
}

int check_scores(Table* table){
    /*check to determine if there is a game winner
     by comparing scores*/
    if(table->players[0]->chips <= 0)
        return 0;
    if(table->players[1]->chips <= 0 && table->players[2]->chips <= 0
        && table->players[3]->chips <= 0)
        return 2;
    return 1;
        
}

int get_rand_bet(){
    /*returns a random number between 0 and 99*/
    return rand() % 100;
}
