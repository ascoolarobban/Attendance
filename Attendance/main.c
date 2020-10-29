#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "safeinput.h"
#include <stdbool.h>
#include <time.h>

// Innehåller f.open!
//  main.c
//  Attendance
//
//  Created by Robin Ellingsen on 2020-10-21.
//  Copyright © 2020 Robin Ellingsen. All rights reserved.
//



void sleeper(int number_of_seconds)
{
    time_t start_time = time(NULL);
    while (time(NULL) < start_time + number_of_seconds);
}
void loading()
{
    printf("Loading");
    for(int i=0; i<10; i++)
    {
        printf(".");
        sleeper(1);
    }
}

void clearConsole()
{
    for(int i=0; i<80; i++)
    {
        printf("\n");
    }
}

typedef struct
{
    int number;
    bool access;
    time_t time;
}CARD;

typedef struct
{
    CARD* allCards;
    int numberOfCards;
    
}CARDLIST;

char *timestamp(time_t tiktok)
{
    char *timeString = (char *)malloc(sizeof(char) * 30);
    struct tm *tm;
    tm=localtime(&tiktok);
    strftime(timeString,30,"%c", tm);
    return timeString;
}


void fakeOpenDoor()
{
    printf("*bzzzt*GREEN LIGHT*DOOR OPEN*bzzzt*");
    sleeper(3);
}

void listOfAllCards(CARDLIST* state)
{
    
    for (int i = 0; i<state->numberOfCards; i++)
    {
        CARD c = state ->allCards[i];
        char* access = c.access == 1?"Has Access.":"Does not have access.";
        printf("%d %s %s \n", c.number, access, timestamp(c.time));
    }
    loading();
}
    
void NewCard(CARDLIST *state)
{
    printf("ADD NEW CARD\n");

    int indexForTheNewOne;
    if (state->numberOfCards == 0)
    {
        state->allCards = malloc(sizeof(CARD));
        state->numberOfCards = 1;
        indexForTheNewOne = 0;
        
    }
    else
    {
        state->allCards = realloc(state->allCards,
            sizeof(CARD) * (state->numberOfCards + 1));
        indexForTheNewOne = state->numberOfCards;
        state->numberOfCards++;
    }
    int cardNumber;
    char gotAccess;
    GetInputInt("Enter Card Number:-> ", &cardNumber);
    printf("Grant access? [y/n]: \n");
    scanf("%c",&gotAccess);
    bool access = false;
    if(gotAccess == 'y')
    {
        access = true;
    }
    else if (gotAccess == 'n')
    {
        access = false;
    }
    CARD cardHolder = {cardNumber, access, time(0)};
    state->allCards[indexForTheNewOne].number=cardHolder.number;
    state->allCards[indexForTheNewOne].access=cardHolder.access;
    state->allCards[indexForTheNewOne].time=cardHolder.time;
//    FILE *f = fopen("rfidcards.txt", "a+");
//    fprintf(f,"%d %d %s",cardHolder.number, cardHolder.access, cardHolder.time);
//    fclose(f);
    printf("Card %d successfully added: %s\n", cardNumber, timestamp(time(0)));
    for(int i=0; i<6; i++)
    {
        printf(".");
        sleeper(1);
    }

}
void ChangeAccess(CARDLIST* state,int lookFor)
{
    char choice;
    for(int i=0; i < state->numberOfCards; i++)
    {
        if (state ->allCards[i].number == lookFor)
        {
            printf("Change access? [y/n]: \n");
            scanf("%c",&choice);
            if(choice == 'y')
            {
                state ->allCards[i].access = true?false:true;
                CARD c = state ->allCards[i];
                char* access = c.access == 1?"Access.":"No Access.";
                printf("Access changed to: \"%s\".\n", access);
                loading();
            }
            else{
                break;
            }
            
        }
        else
        {
            printf("This card does not exist.");
            NewCard(state);
        }
    }
    
}

void addRemoveMenu(CARDLIST* state)
{
    while (true)
    {
        printf("===ADD CARD ===\n");
        printf("1. Add Card.\n2.Exit.\n");
        int selection;
        if (GetInputInt("Choice: >", &selection)== false)
            continue;
        switch (selection)
        {
            case 1:
                NewCard(state);
                break;
            case 2:
                break;
        }
        break;
    }
}

void remoteAccess(CARDLIST* state, int lookfor)
{
    CARD card;
    for (int i =0; i< state ->numberOfCards; i++)
    {
        if(state->allCards[i].number == lookfor)
        {
            card = state->allCards[i];
        }
    }
    
    const char* out = card.access == true?"ACCESS GRANTED *GREEN LIGHT*\n":"ACCESS DENIED *RED LIGHT*\n";
    printf("%s", out);
    loading();
    
}


void Huvudmeny(CARDLIST *state)
{
    while (true)
    {
        clearConsole();
        printf("***WELCOME***\n");
        printf("1. List all cards.\n2. Add Card.\n3. Remote Access.\n4. Open door.\n5. Change Access\n");
        int choice;
        int selection;
        int lookFor;
        if (GetInputInt("Choice:->", &selection) == false)
            continue;
        switch (selection)
        {
        case 1:
                clearConsole();
                listOfAllCards(state);
                break;
                
        case 2:
                clearConsole();
                addRemoveMenu(state);
                break;
        
        case 3:
                
                GetInputInt("Enter Card Number: ->\n", &lookFor);
                remoteAccess(state,lookFor);
                break;
                
        case 4:
                clearConsole();
                fakeOpenDoor();
                break;
        case 5:
                GetInputInt("Enter card to change access: ->\n", &choice);
                ChangeAccess(state, choice);
                printf("Acces has been changed for #%d %s", choice, timestamp(time(0)));
                

        }
    }
}

int main()
{
    CARDLIST state;
    state.allCards = NULL;
    state.numberOfCards = 0;
    Huvudmeny(&state);
    return 0;
}


