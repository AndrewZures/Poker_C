#include"cardgame.c"
#include"rank.c"
#include"mcadvisor.c"
#include"interface.c"
#include"betting.c"

/*NOTE: this file  is the driver and deals with the high-level
 operation of the game*/

int main(){
    
    int winner;
    int gameCheck = 1;
    int i = 0; /*keeps track of rounds*/
    Option* userChoice = create_user_option();
    Option_List* list1 = get_option_list(); /*get option list*/
    Table* table = table_init();  /*create table, players, deck*/
    
    while(gameCheck == 1){
        i++;
        /*shuffle and deal*/
        shuffle(table->deck);
        deal(table);
        
        /*start round*/
        printf("%s " "%d " "%s\n", "-----------------ROUND", i, "----------------");
        print_scores(table);  /*prints scores before round*/
        table->pot = ante_up(table, i);  /*ante up bets*/
        
        /*get the user's discard decision*/
        userChoice = player_prompt(table, ask_MC_Advisor(table->players[0]->hand, list1, table->testDeck), userChoice);
        
        /*round of betting*/
        round_of_betting(table);
        
        /*execute first round of discards*/
        execute_round(table, userChoice, list1);
        
        /*get the user's second discard decision*/
        userChoice = player_prompt(table, ask_MC_Advisor(table->players[0]->hand, list1, table->testDeck), userChoice);
        
        /*execute second round of discards*/
        execute_round(table, userChoice, list1);
        
        print_final_hand(table->players[0]);
        /*determine winner of round and give him the pot*/
        winner = get_round_winner(table);
        divvy_pot(table, winner);
        
        print_scores(table);
        /*check to see if a player has won*/
        gameCheck = check_for_game_winner(table);
        
        /*prompt user to continue after round ends*/
        if(gameCheck == 1)
            prompt_for_continue(i);
        
    }
    
    /*free malloc if still around*/
    destroy_game(table, userChoice);
    return 0;
}