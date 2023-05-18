#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include "hangman.h"

char secretWord[WORDLENGTH];
char totalTries[26];
int userTries = 0;
int userFails = 0;

void gameTitle() {
    printf("**********************************\n");
    printf("*  WELCOME TO THE HANGMAN GAME!  *\n");
    printf("**********************************\n\n");
}

int letterExists(char letter) {
    for (int j = 0; j < strlen(secretWord); j++) {
        if (letter == secretWord[j]) {
            return 1;
        }
    }
    return 0;
}

void letterTry() {
    char letterTry;
    do {
        printf("Choose a letter: ");
        scanf(" %c", &letterTry);
        printf("\n");
        letterTry = toupper(letterTry);

    } while (!isalpha(letterTry));


    if (letterExists(letterTry)) {
        printf("You got it right! %c exists in this word!!\n\n", letterTry);
    } else {
        printf("\nYou got it wrong! %c does not exist in this word!\n\n", letterTry);
        userFails++;
        printf("You have %d error(s) already! If you get to 5 the game will end!\n\n", userFails);
    }


    totalTries[userTries] = letterTry;
    userTries++;
}

int alreadyTried(char letter) {
    int found = 0;
    for (int j = 0; j < userTries; j++) {
        if (totalTries[j] == letter) {
            found = 1;
            break;
        }
    }
    return found;
}

void hangmanDrawing() {
    for (int i = 0; i < strlen(secretWord); i++) {
        if (alreadyTried(secretWord[i])) {
            printf("%c ", secretWord[i]);
        } else {
            printf("_ ");
        }
    }
    printf("\n");
}

void wordChoice() {
    FILE *f;
    f = fopen("hangman.txt", "r");
    if (f == NULL) {
        printf("This file cannot be opened!\n\n");
        exit(1);
    }
    int wordsNum;
    fscanf(f, "%d", &wordsNum);

    srand(time(0));
    int random = rand() % wordsNum;

    for (int i = 0; i <= random; i++) {
        fscanf(f, "%s", secretWord);
    }

    fclose(f);
}

int playerWasHung() {
    int letterErrors = 0;

    for (int i = 0; i < userTries; i++) {
        if (!alreadyTried(totalTries[i])) {
            letterErrors++;
        }
    }

    return letterErrors >= 5;
}

int playerWon() {
    for (int i = 0; i < strlen(secretWord); i++) {
        if (!alreadyTried(secretWord[i])) {
            return 0;
        }
    }
    return 1;
}

void addNewWord() {
    char wantToAdd;
    printf("Do you want to add a new word? (Y/N): ");
    scanf(" %c", &wantToAdd);
    wantToAdd = toupper(wantToAdd);

    if (wantToAdd == 'Y') {
        char newWord[WORDLENGTH];

        printf("New word: ");
        scanf("%s", newWord);

        for (int i = 0; i < strlen(newWord); i++) {
            newWord[i] = toupper(newWord[i]);
        }

        FILE *f;
        f = fopen("hangman.txt", "a+");
        if (f == NULL) {
            printf("This file cannot be opened!\n\n");
            exit(1);
        }

        int quantity;
        fscanf(f, "%d", &quantity);
        quantity++;

        fseek(f, 0, SEEK_SET);

        fseek(f, 0, SEEK_END);
        fprintf(f, "\n%s", newWord);


        fclose(f);
    }
}

int main() {
    wordChoice();
    gameTitle();

    do {
        hangmanDrawing();
        letterTry();
    } while (!playerWon() && !playerWasHung() && userFails < 5);

    if (playerWon()) {
        printf("Congratulations! You have won the game! :)\n");
    } else {
        printf("You have lost the game! :(\n");
        printf("The secret word was %s\n\n", secretWord);
    }

    addNewWord();

    return 0;
}
