#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"

void potion(struct character *player){
    printf("\ndid this run\n");
    player->health = player->health + 5;
}

int selectItem(struct character *player, char *item){
    for(int i = 0; player->items[i].name != NULL; i++){
        if(strcmp(item ,player->items[i].name) == 0){
            if(strcmp(item, "potion") == 0){
                potion(player);
                return 1;
            }
        }
    }
    return -1;
}

void listitems(struct item *items){
    for(int i = 0; items[i].name != NULL; i++){
        printf("\nYou have %s\n", items[i].name);
    }
}


void combat(struct character *player1, struct character *player2){
    int playerTurn = 0;
    char *playerChoice;
    size_t len = 0;
    while(player1->health > 0 && player2->health > 0){
        printf("player1 health = %d\nplayer2 health = %d\n", player1->health,player2->health);
        if(playerTurn == 0){
            printf("\nwhat would you like to do?\n 1:Select item \n 2:attack\n");
            getline(&playerChoice, &len, stdin);
            playerChoice[strcspn(playerChoice, "\r\n")] = 0;
            if(strcmp(playerChoice,"attack") == 0){
                player2->health = player2->health - player1->characterEquipment.weapon.damage;
            }
            if(strcmp(playerChoice,"item") == 0){
                listitems(player1->items);
                getline(&playerChoice, &len, stdin);
                playerChoice[strcspn(playerChoice, "\r\n")] = 0;
                printf("\n%d\n", selectItem(player1, playerChoice));
            }
        }

        else{
            printf("what would you like to do\n 1:Select item \n 2:attack\n");
            getline(&playerChoice, &len, stdin);
            playerChoice[strcspn(playerChoice, "\r\n")] = 0;
            if(strcmp(playerChoice,"attack") == 0){
                player1->health = player1->health - player2->characterEquipment.weapon.damage;
            }
        }
        playerTurn = playerTurn ? 0 : 1;
    }
}

int main(){
    struct character player1 = { 0 };
    struct character player2 = { 0 };
    player1.health = 10;
    player2.health = 11;
    player1.characterEquipment.weapon.damage = 1;
    player2.characterEquipment.weapon.damage = 2;
    printf("\ndid we make it here?\n");
    fflush(stdout);
    player1.items = malloc(sizeof(struct item) * 2);
    player1.items[0].name = malloc(sizeof(char) * strlen("potion") + 1);
    strcpy(player1.items[0].name, "potion");
    player1.items[1].name = NULL;
    combat((struct character *) &player1, (struct character *) &player2);
}