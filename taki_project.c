/*
* Michael Shuster
* 209516210
* The program executes the "TAKI" game
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h> //in order to use the "malloc" function
#include <string.h> 
#include <stdlib.h> // in order to use the "rand" and "srand" functions
#include <time.h>	// in order to use "time" function

//defines of sizes 
#define MAX_NAME_SIZE 20
#define AMOUNT_START_CARDS 4
#define MAX_TYPE_CARD_SIZE 6
#define AMOUNT_COLORS_CARDS 4
#define AMOUNT_TYPES_CARD 14
#define AMOUNT_NUMBERS_CARDS 9
#define WIDTH_LINE1 7
#define WIDTH_LINE2 4

//defines of directions
#define RIGHT_DIRECTION 1
#define LEFT_DIRECTION 0

//defines of colors id
#define YELLOW_CARD_ID 1
#define RED_CARD_ID 2
#define BLUE_CARD_ID 3
#define GREEN_CARD_ID 4

//defines of special cards id
#define TAKI_CARD_ID 10
#define COLOR_CARD_ID 11
#define CHANGE_DIRACTION_CARD_ID 12
#define STOP_CARD_ID 13
#define PLUS_CARD_ID 14

//defines of colors
#define YELLOW 'Y'
#define RED 'R'
#define BLUE 'B'
#define GREEN 'G'
#define NO_COLOR ' '

//defines of type cards names
#define TAKI_STRING "TAKI"
#define COLOR_STRING "COLOR"
#define STOP_STRING "STOP"
#define CHANGE_DIRACTION_STRING "<->"
#define PLUS_STRING "+"

//defines of  no specfic category
#define FIRST_LETTER 0
#define SECOND_LETTER 1
#define INITIAL_FREQUENCY 0
#define ZERO_CHAR '0'
#define VERTICAL_lINE '|'
#define END_OF_STRING '\0'



struct card
{
	char type_card[MAX_TYPE_CARD_SIZE];
	char color_card;
};
typedef struct card Card;

struct player
{
	char name[MAX_NAME_SIZE];
	Card* cards;
	int amountCards;
	int physicalSize_cards;
};
typedef struct player Player;

struct card_statistics
{
	char type_card[MAX_TYPE_CARD_SIZE]; 
	int frequency;
}; 
typedef struct card_statistics card_Statistics;

struct counter
{
	int amount;
	char type_cards[AMOUNT_TYPES_CARD][MAX_TYPE_CARD_SIZE]; //array of type cards names
};
typedef struct counter Counter;

void scanNames(Player players[], int number_of_players);
void divideCards(Player players[], int number_of_players, card_Statistics* statistics);
char processNumberToColorCard(int number);
void processNumberToTypeCard(int number,char type_card[], card_Statistics* statistics);
void PrintCardsOfPlayer(Player player,Card upperCard);
void printCard(char type_card[], char color_card);
Card createUpperCard(card_Statistics* statistics);
Card createCard(card_Statistics* statistics);
void turnsManagment(Player players[], int num_of_players,Card upperCard, card_Statistics* statistics);
int  processPlayerChoice(Player* player, Card* upperCard,bool isTaki,card_Statistics* statistics);
void processColorChoice(Card* upperCard);
void cardCpy(Card* dest, Card src);
void deleteCard(Player* player, int index);
void addCard(Player* player, card_Statistics* statistics);
void stopCard(int move_direction, int* current_player, int num_of_players, Player* player, card_Statistics* statistics);
void changeDirectionCard(int* move_direction, int* current_player);
void turnNextPlayer(int move_direction, int* current_player);
void printOptionsPlayer(int amount_cards,bool isTaki);
void freeMemory(Player* players, int num_of_players);
Card* adjustSizeCards(Card* cards, int size, int newSize);
void initialize_Statistics(card_Statistics* statistics);
void printStatistics(card_Statistics* statistics);
int processTakiMode(Player* player, Card* upperCard, card_Statistics* statistics);
int processNotTakiMode(Player* player, Card* upperCard, card_Statistics* statistics);
bool sameColorCard(Player* player, Card* upperCard, int player_choice);
bool sameNumberCard(Player* player, Card* upperCard, int player_choice);
bool colorCard(Player* player, Card* upperCard, int player_choice);
int invalidCard(int amountCards, bool taki_Mode);
bool specialCardsCheck(Card upperCard, int* move_direction, int* current_player, int num_of_players, Player* player, card_Statistics* statistics);
void printStartGame();
Player* createPlayersArray(int* number_of_players);
void processEnd_Players(int* current_player, int move_direction, int num_of_players);
bool checkTwoCategories(Player* player, Card* upperCard, int player_choice);
void countingSort(card_Statistics* statistics, int size);
int findMaxFrequency(card_Statistics* statistics, int size);
void fill_Counters(Counter* counters, int size, card_Statistics* statistics);
void initialize_Counters(Counter* counters, int size);
void initialize_SpecialCardsStatistics(card_Statistics* statistics);
void scanNumberPlayers(int* number_of_players);


void main()
{
	//the start of the program
	Card upperCard;
	card_Statistics statistics[AMOUNT_TYPES_CARD];
	int number_of_players;
	Player* players;
	printStartGame();
	players = createPlayersArray(&number_of_players);
	srand(time(NULL));
	initialize_Statistics(statistics);
	scanNames(players, number_of_players);
	divideCards(players, number_of_players,statistics);
	upperCard = createUpperCard(statistics);
	turnsManagment(players, number_of_players, upperCard,statistics);
	freeMemory(players, number_of_players);
}

void printStartGame()
{
	//the function prints the start of the game.
	printf("************  Welcome to TAKI game !!! ***********\n");
	printf("Please enter the number of players: ");
}


Player* createPlayersArray(int* number_of_players)
{
	/*
	* the function gets as a parmater integer by refernce
	* the function gets as input from the user the number of players and stores it
	  in the integer variable.
	* the function creates a players array with dynamic allocation
	  and then returns the players array as a return value.
	*/
	Player* players;
	scanNumberPlayers(number_of_players);
	players = (Player*)malloc( (*number_of_players) * sizeof(Player));
	if (players == NULL)
	{
		printf("memory error!\n");
		exit(1);
	}
}


void scanNumberPlayers(int* number_of_players)
{
	/*
	* the function gets as a parmater integer by refernce
	* the function gets as input from the user the number of players and stores it
	  in the integer variable.
	*/
	bool invailid_input = true;
	while (invailid_input)
	{
		scanf("%d", number_of_players);
		if ((*number_of_players) <= 0)
		{
			printf("Invalid number of players! Try again.\n");
			printf("Please enter the number of players: ");
		}
		else
			invailid_input = false;
	}
}


void initialize_Statistics(card_Statistics* statistics)
{
	/*
	* the paramters of the function:
	  1)array of cards statistics

	* the function  initializing the array of the cards statistics with 0
	  in the frequency and with the respective type card string by the index number.
	*/
	int i;
	for (i = 0; i < AMOUNT_TYPES_CARD; i++)
	{
		if (i < AMOUNT_NUMBERS_CARDS)
		{
			statistics[i].type_card[FIRST_LETTER] = i + 1 + ZERO_CHAR; // transletes the index to char.
			statistics[i].type_card[SECOND_LETTER] = END_OF_STRING;
		}
		statistics[i].frequency = INITIAL_FREQUENCY;
	}
	initialize_SpecialCardsStatistics(statistics);
}


void initialize_SpecialCardsStatistics(card_Statistics* statistics)
{
	/*
	* the paramters of the function:
	  1)array of cards statistics

	* the function  initializing the special cards statistics with
	  the respective type card string by the index number.
	*/
	strcpy(statistics[TAKI_CARD_ID - 1].type_card, TAKI_STRING);
	strcpy(statistics[COLOR_CARD_ID - 1].type_card, COLOR_STRING);
	strcpy(statistics[CHANGE_DIRACTION_CARD_ID - 1].type_card, CHANGE_DIRACTION_STRING);
	strcpy(statistics[STOP_CARD_ID - 1].type_card, STOP_STRING);
	strcpy(statistics[PLUS_CARD_ID - 1].type_card, PLUS_STRING);
}


void scanNames(Player players[], int number_of_players)
{
	/*
	* the paramters of the function:
	  1)array of players
	  2)number of players

	* the function  gets as input from the players their first names
	  and stores the in the players array.
	*/
	int i;
	for (i = 0; i < number_of_players; i++)
	{
		printf("Please enter the first name of player #%d:\n", i+1);
		scanf("%s", players[i].name);
	}
	printf("\n");
}


void freeMemory(Player* players, int num_of_players)
{
	/*
	* the paramters of the function:
	   1)array of players
	  2)number of players

	* the function  frees the memory of each player's cards array and then
	  then function frees the memory of the players array.
	*/
	int i;
	for (i = 0; i < num_of_players; i++)
	{
		free(players[i].cards);
	}
	free(players);
}


void divideCards(Player players[], int number_of_players,card_Statistics* statistics)
{
	/*
	* the paramters of the function:
	  1)array of players
	  2)number of players
	  3)array of cards statistics

	* the function creates for each player 4 cards 
	  with random type card and color card.
	*/
	int i,j;
	for (i = 0; i < number_of_players; i++)
	{
		players[i].cards = (Card*)malloc(AMOUNT_START_CARDS * sizeof(Card));
	
		if (players[i].cards == NULL)
		{
			printf("memory error!\n");
			exit(1);
		}
		for (j = 0; j < AMOUNT_COLORS_CARDS; j++) // generting 4 random type cards for the  first 4 cards
		{
			players[i].cards[j] = createCard(statistics);
		}
		players[i].amountCards = AMOUNT_START_CARDS;
		players[i].physicalSize_cards = AMOUNT_START_CARDS;	
	}
}


void processNumberToTypeCard(int number, char type_card[], card_Statistics* statistics)
{
	/*
	* the paramters of the function:
	  1)integer number in range of [1,15)
	  2)string that represents the type card
	  3)array of cards statistics

	* the function transletes the number to its respective string
	  and then stores the string the type card string.
	  in addition the function  increases the frequency of the respective type card by one 
	  in the respective index of the  statistics array
	*/
	if (number < AMOUNT_NUMBERS_CARDS+1)
	{
		type_card[0] = number + ZERO_CHAR;
		type_card[1] = END_OF_STRING;
		statistics[number - 1].frequency++;
	}
	else
	{
		switch (number)
		{
		case TAKI_CARD_ID:
			strcpy(type_card,TAKI_STRING);
			statistics[TAKI_CARD_ID-1].frequency++;
			break;
		case COLOR_CARD_ID:
			strcpy(type_card,COLOR_STRING);
			statistics[COLOR_CARD_ID-1].frequency++;
			break;
		case CHANGE_DIRACTION_CARD_ID:
			strcpy(type_card,CHANGE_DIRACTION_STRING);
			statistics[CHANGE_DIRACTION_CARD_ID-1].frequency++;
			break;
		case STOP_CARD_ID:
			strcpy(type_card ,STOP_STRING);
			statistics[STOP_CARD_ID-1].frequency++;
			break;
		case PLUS_CARD_ID:
			strcpy(type_card ,PLUS_STRING);
			statistics[PLUS_CARD_ID-1].frequency++;
			break;

		default:
			break;
		}
	}
}


char processNumberToColorCard(int number)
{
	/*
	* the paramters of the function:
	  1)integer number in range of [1,4]


	* the function transletes the number to its respective color
	  and then the function returns the color.
	*/
	char color;
	switch (number)
	{
	case YELLOW_CARD_ID:
		color = YELLOW;
		break;
	case RED_CARD_ID:
		color = RED;
		break;
	case BLUE_CARD_ID:
		color = BLUE;
		break;
	case GREEN_CARD_ID:
		color = GREEN;
		break;
	default:
		break;
	}
	return color;
}


void PrintCardsOfPlayer(Player player, Card upperCard)
{
	/*
	* the paramters of the function:
	  1)struct of player
	  2)struct of the upper card

	* the function prints the upper card and the cards of the given player.
	*/
	int i;
	printf("Upper card:\n");
	printCard(upperCard.type_card, upperCard.color_card);
	printf("%s's turn:\n\n", player.name);
	for (i = 0; i < player.amountCards; i++)
	{
		printf("Card #%d:\n", i+1);
		printCard(player.cards[i].type_card, player.cards[i].color_card);
	}
}


void printCard(char type_card[], char color_card)
{
	/*
	* the paramters of the function:
	  1)string of the type card
	  2)char of the color card

	* the function prints the card's type 
	  and the card's color with a frame of '*'.
	*/
	int length = strlen(type_card);
	printf("*********\n");
	printf("*\t*\n");
	printf("*%*s\t*\n",WIDTH_LINE1-((WIDTH_LINE1-length)/2),type_card);
	printf("*%*c\t*\n",WIDTH_LINE2, color_card);
	printf("*\t*\n");
	printf("*********\n\n");

}


Card createUpperCard(card_Statistics* statistics)
{
	/*
	* the paramters of the function:
	  1) array of card statistics

	* the function creates the upper card with a random type card and color card
	  and then the function returns the card as a return value.
	*/
	int random_number;
	Card card;
	random_number = rand() % AMOUNT_NUMBERS_CARDS +1;
	processNumberToTypeCard(random_number, card.type_card, statistics);
	random_number =  rand() % AMOUNT_COLORS_CARDS +1;
	card.color_card = processNumberToColorCard(random_number);
	return card;
}


Card createCard(card_Statistics* statistics)
{
	/*
	* the paramters of the function:
	  1) array of card statistics

	* the function creates a card with a random type card and color card
	  and then the function returns the card as a return value.
	*/
	int random_number;
	Card card;
	random_number = 1 + rand() % (AMOUNT_TYPES_CARD);
	processNumberToTypeCard(random_number, card.type_card, statistics);
	if (random_number != COLOR_CARD_ID)
	{
		random_number = 1 + rand() % AMOUNT_COLORS_CARDS;
		card.color_card = processNumberToColorCard(random_number);
	}
	else
		card.color_card = NO_COLOR;

	return card;
}


void stopCard(int move_direction, int* current_player,int num_of_players,Player* player, card_Statistics* statistics)
{
	/*
	* the paramters of the function:
	  1) integer that represents the direction turns of the game 
	  2) pointer to the current turn of the players
	  3) integer that represents the number of players in the game
	  4) pointer to struct player of the current player
	  5) array of cards statistics

	* the function implements the stop card  in the game
	  by the rules of the Taki game.
	*/
	if (num_of_players == 2 && (*player).amountCards == 0)
		addCard(player, statistics);
	switch (move_direction)
	{
	case RIGHT_DIRECTION:
		if ((*current_player) == (num_of_players - 1))
			(*current_player) = 1;
		else
			(*current_player)=+2;

		break;
	case LEFT_DIRECTION:
		if ((*current_player) == 0)
			(*current_player) = num_of_players - 2;
		else
			(*current_player)=-2;

		break;
	default:
		break;
	}
}


void changeDirectionCard(int* move_direction, int* current_player)
{
	/*
	* the paramters of the function:
	  1) pointer to the integer that represents the direction turns of the game
	  2) pointer to the current turn of the players

	* the function implements the change direction card  in the game
	  by the rules of the Taki game.
	*/
	switch ( (*move_direction))
	{
	case RIGHT_DIRECTION:
		(*move_direction) = LEFT_DIRECTION;
		break;
	case LEFT_DIRECTION:
		(*move_direction) = RIGHT_DIRECTION;
		break;
	default:
		break;
	}
}


void turnNextPlayer(int move_direction, int* current_player)
{
	/*
	* the paramters of the function:
	  1) integer that represents the direction turns of the game
	  2) pointer to the current turn of the players

	* the function moves the turn of the current player to the next player
      by the last updated direction turns of the game
	*/
	switch (move_direction)
	{
	case RIGHT_DIRECTION:
		(*current_player)++;
		break;
	case LEFT_DIRECTION:
		(*current_player)--;
		break;
	default:
		break;
	}
}


int processPlayerChoice(Player* player,Card* upperCard,bool taki_Mode, card_Statistics* statistics)
{
	/*
	* the paramters of the function:
	  1) pointer to the current player
	  2) pointer to the the upper card
	  3) boolean taki mode
	  4) array of cards  statistics

	* the function navigates the prccess of the player choice to the
	* right process function by the state of the taki mode in the game.
	 
	* the return value of the function: the player choice
	*/
	int player_choice;

	if (taki_Mode)
		player_choice=processTakiMode(player, upperCard, statistics);

	else
		player_choice = processNotTakiMode(player, upperCard, statistics);

	return player_choice;
}


int processTakiMode(Player* player, Card* upperCard, card_Statistics* statistics)
{
	/*
	* the paramters of the function:
	  1) pointer to the current player
	  2) pointer to the the upper card
	  3) array of cards  statistics

	* the function process the choice from the current player 
	  by the rules of the game when the taki mode is enabled in the game.

	 * the return value of the function: the player choice
	*/
	int player_choice;
	bool input_invaild = true;
	scanf("%d", &player_choice);
	while (input_invaild)
	{
		if (player_choice > 0 && player_choice <= (*player).amountCards)
			input_invaild=checkTwoCategories(player, upperCard, player_choice);
		else if (player_choice == 0)
			input_invaild = false;
		else
			player_choice = invalidCard((*player).amountCards, true);
	}
	return player_choice;

}


bool checkTwoCategories(Player* player, Card* upperCard, int player_choice)
{
	/*
	* the parameters of the function:
	  1)pointer to the current player
	  2)pointer to the upper card
	  3)integer that represents the player choice of card

	* the function checks the chosen card of the player with two categories:
	  1)same color card as the upper card
	  2)the chosen card is the color card
	
	the return value of the function: the status of the input
	*/
	bool input_invaild = true;
	input_invaild = sameColorCard(player, upperCard, player_choice);
	if (input_invaild)
		input_invaild = colorCard(player, upperCard, player_choice);
	return input_invaild;
}


int processNotTakiMode(Player* player, Card* upperCard, card_Statistics* statistics)
{
	/*
	* the paramters of the function:
	  1) pointer to the current player
	  2) pointer to the the upper card
	  3) array of cards  statistics

	* the function process the choice from the current player
	  by the rules of the game when the taki mode is turned off in the game.

	* the return value of the function: the player choice
	*/
	int player_choice;
	bool input_invaild = true;
	scanf("%d", &player_choice);
	while (input_invaild)
	{
		if (player_choice > 0 && player_choice <= (*player).amountCards)
		{
			input_invaild = checkTwoCategories(player, upperCard, player_choice);
			if (input_invaild)
				input_invaild = sameNumberCard(player, upperCard, player_choice);

			if(input_invaild)
				player_choice= invalidCard((*player).amountCards, false);
		}
		else if (player_choice == 0)
		{
			input_invaild = false;
			addCard(player, statistics);
		}
		else
			player_choice=invalidCard((*player).amountCards, false);
	}
	return player_choice;
}


bool sameColorCard(Player* player, Card* upperCard,int player_choice)
{
	/*
	* the paramters of the function:
	  1) pointer to the current player
	  2) pointer to the the upper card
	  3) integer that represents the choice of the player

	* the function checks if the  color of the  chosen card of the player is
	  the same as the color of the upper card,
	  and if the color is the same the function changes the upper card to the chosen card
	  and deletes the card from the array of cards of the current player.

	* the return value of the function: the status of the input
	*/
	bool  input_invaild = true;
	if ((*player).cards[player_choice - 1].color_card == (*upperCard).color_card)
	{
		input_invaild = false;
		cardCpy(upperCard, (*player).cards[player_choice - 1]);
		deleteCard(player, player_choice - 1);
	}
	return input_invaild;
}


bool colorCard(Player* player, Card* upperCard, int player_choice)
{
	/*
	* the paramters of the function:
	  1) pointer to the current player
	  2) pointer to the the upper card
	  3) integer that represents the choice of the player

	* the function checks if the  chosen card of the player is
	  the COLOR card and then if its true the function implements 
	  the COLOR card by the rules of the TAKI game.

	* the return value of the function: the status of the input
	*/
	bool input_invalid = true;
	if (strcmp((*player).cards[player_choice - 1].type_card, COLOR_STRING) == 0)
	{
		input_invalid = false;
		cardCpy(upperCard, (*player).cards[player_choice - 1]);
		processColorChoice(upperCard);
		deleteCard(player, player_choice - 1);
	}
	return input_invalid;
}


bool sameNumberCard(Player* player, Card* upperCard, int player_choice)
{
	/*
	* the paramters of the function:
	  1) pointer to the current player
	  2) pointer to the the upper card
	  3) integer that represents the choice of the player

	* the function checks if the chosen card of the player is a number card,
	  and then the function checks if the number of the chosen card of the player is
	  the same as the number of the upper card,
	  and if the number  is the same the function changes the upper card to the chosen card
	  and deletes the card from the array of cards of the current player.

	* the return value of the function: the status of the input
	*/
	bool input_invaild = true;
	if ((*player).cards[player_choice - 1].type_card[0] - ZERO_CHAR <= AMOUNT_NUMBERS_CARDS)
	{
		if (strcmp((*player).cards[player_choice - 1].type_card, (*upperCard).type_card) == 0)
		{
			input_invaild = false;
			cardCpy(upperCard, (*player).cards[player_choice - 1]);
			deleteCard(player, player_choice - 1);
		}
	}
	return input_invaild;

}


int invalidCard(int amountCards,bool taki_Mode)
{
	/*
	* the paramters of the function:
	  1) amount of cards
	  2) the status of the taki mode in the game
	  
	* the function prints the invalid card massage to the player
	  with the options that the player have to choose from.
	  and then the function get as input from the player the new choice.

	 * the return value of the function: the player choice.
	*/
	int player_choice;
	printf("Invalid card! Try again.\n");
	printOptionsPlayer(amountCards, taki_Mode);
	scanf("%d", &player_choice);
	return player_choice;
}


void printOptionsPlayer(int amount_cards, bool taki_Mode)
{
	/*
	* the paramters of the function:
	  1) amount of cards
	  2) the status of the taki mode in the game

	* the function prints the the options to the player
	  that the he has to choose from.
	*/
	if(taki_Mode)
		printf("Please enter 0 if you want to finish your turn\n");
	else
		printf("Please enter 0 if you want to take a card from the deck\n");
	printf("or 1 - %d if you want to put one of your cards in the middle :\n", amount_cards);
}


void processColorChoice(Card* upperCard)
{
	/*
	* the paramters of the function:
	  1) pointer to the upper card

	* the function prints the options of colors to choose from
	  then the function gets the color choice from the player 
	  and changes the color of the upper card to the chosen color.
	*/
	int player_choice;
	bool invalid_input = true;
	while (invalid_input)
	{
		printf("Please enter your color choice:\n");
		printf("1 - Yellow\n");
		printf("2 - Red\n");
		printf("3 - Blue\n");
		printf("4 - Green\n");
		scanf("%d", &player_choice);
		if (player_choice > 0 && player_choice <= AMOUNT_COLORS_CARDS)
		{
			(*upperCard).color_card = processNumberToColorCard(player_choice);
			invalid_input = false;
		}
		else
			printf("Invalid color! Try again.\n");
	}
}


void cardCpy(Card* dest, Card src)
{
	/*
	* the paramters of the function:
	  1) pointer to a destination card
	  2) a source card

	* the function copys the type and color of the source card to the destination card.
	*/
	(*dest).color_card = src.color_card;
	strcpy((*dest).type_card, src.type_card);
}


void deleteCard(Player* player, int index)
{
	/*
	* the paramters of the function:
	  1) pointer the the current player
	  2) index the represents the card that need to be deleted from the player's card

	* the function deletes from the player's cards ,the card that is located 
	  in the index of the cards array of the player.
	*/
	Card temp= (*player).cards[index];
	(*player).cards[index] = (*player).cards[(*player).amountCards - 1];
	(*player).cards[(*player).amountCards - 1] = temp;
	(*player).amountCards--;
}


void addCard(Player* player, card_Statistics* statistics)
{
	/*
	* the paramters of the function:
	  1) pointer the the current player
	  2) array of cards statistics

	* the function adds one new card to the  current player's cards.
	*/
	if ((*player).physicalSize_cards == (*player).amountCards)
	{
		(*player).cards = adjustSizeCards((*player).cards, (*player).amountCards, (*player).amountCards * 2);
		(*player).physicalSize_cards = (*player).amountCards * 2;
	}
	(*player).cards[(*player).amountCards] = createCard(statistics);
	(*player).amountCards++;
}


Card* adjustSizeCards(Card* cards,int size,int newSize)
{
	/*
	* the paramters of the function:
	  1) array of cards 
	  2)the current size of cards
	  3)the new size of cards

	* the function creates  an  array of cards by newSize spac with a dynamic assignment,
	  then the functions copys the cards from the array cards to the new cards array.

	*the return value of the function : the new cards array.
	*/
	int i;
	Card* newCards;
	newCards = (Card*)malloc(newSize * sizeof(Card));
	if (newCards == NULL)
	{
		printf("memory error\n");
		exit(1);
	}
	if (cards != NULL)
	{
		for (i = 0; i < size; i++)
		{
			newCards[i] = cards[i];
		}
	}
	free(cards);
	return newCards;
}


void printStatistics(card_Statistics* statistics)
{
	/*
	* the paramters of the function:
	  1)array of statistics of the cards

	* the function prints the statistics of the cards with their type card name
	  and their respective frequency 
	  (the amount of times each of the diffrent type cards had been taken from the deck).
	*/
	int i, length;
	printf("************ Game Statistics ************\n");
	printf("Card # | Frequency\n");
	printf("__________________\n");
	countingSort(statistics, AMOUNT_TYPES_CARD);
	for (i = 0; i < AMOUNT_TYPES_CARD; i++)
	{
		length = strlen(statistics[i].type_card);
		printf("%*s%*c",WIDTH_LINE1 - ((WIDTH_LINE1- length) / 2), statistics[i].type_card,(WIDTH_LINE2 - length/2),VERTICAL_lINE);
		printf("%5d\n",statistics[i].frequency);
	}
	printf("\n");
}

 
void countingSort(card_Statistics* statistics, int size)
{
	/*
	* the paramters of the function:
	  1)array of statistics of the cards
	  2)the size of the array

	* the function sorts the array of statistics 
	  with the "counting sort" method by ascending order.
	*/
	int i,j,writeInd,max_frequency = findMaxFrequency(statistics,size);
	Counter* counters = (Counter*)malloc(sizeof(Counter) * (max_frequency + 1));
	
	if (counters == NULL)
	{
		printf("memory error!\n");
		exit(1);
	}

	initialize_Counters(counters, max_frequency + 1);
	fill_Counters(counters, size, statistics);

	for (i = 0, writeInd = size-1; i < max_frequency + 1; i++)
	{
		for (j = 1; j <= counters[i].amount; j++, writeInd--)
		{
			statistics[writeInd].frequency = i;
			strcpy(statistics[writeInd].type_card, counters[i].type_cards[j-1]);
		}

	}
	free(counters);
}


void fill_Counters(Counter* counters,int size, card_Statistics* statistics)
{
	/*
	* the paramters of the function:
	  1)array of counters
	  2)the size of the statistics array
	  3)array of statistics of the cards
	  

	* the function fiils the the respective indexs in the counters array
	  by the frequency of the cards.
	*/
	int i, writeInd;
	for (i = 0; i < size; i++)
	{
		counters[statistics[i].frequency].amount++;
		writeInd = counters[statistics[i].frequency].amount - 1;
		//fiiling the (statistics[i].frequency) index in the counters array with the respective type cards strings.
		strcpy(counters[statistics[i].frequency].type_cards[writeInd], statistics[i].type_card);
	}
}


void initialize_Counters(Counter* counters, int size)
{
    /*
	* the paramters of the function:
	  1)array of counters
	  2)the size of the counters array
	  
	* the function initializes the the amount 
	  of each counter in the counters array to zero.
	*/
	int i;
	for (i = 0; i < size; i++)
		counters[i].amount = INITIAL_FREQUENCY;
}


int findMaxFrequency(card_Statistics* statistics, int size)
{
	/*
	* the paramters of the function:
	  1)array of statistics of the cards
	  2)the size of the array

	* the function finds the maximum frequency from the array of statistics of the cards
	  and then the function returns the value of the maximum frequency.
	*/
	int i, max = 0;
	for (i = 0; i < size; i++)
	{
		if (max < statistics[i].frequency)
			max = statistics[i].frequency;
	}
	return max;
}


void turnsManagment(Player players[], int num_of_players, Card upperCard, card_Statistics* statistics)
{
	/*
	* the paramters of the function:
	  1)array of players 
	  2)integer that represents the number of players.
	  3)the upper card
	  4)array of statistics of the cards  

	* the function manages the turns of the players that are in the game
	  by the rules of the "TAKI" game.
	  and in the end of the game announces the winner of the game.
	*/
	int move_direction = RIGHT_DIRECTION,current_player = 0, player_choice, previous_player;
	bool game_over = false, taki_Mode = false, move_next_player = true;
	while (!game_over)
	{
		PrintCardsOfPlayer(players[current_player], upperCard);
		printOptionsPlayer(players[current_player].amountCards, taki_Mode);
		player_choice = processPlayerChoice(&players[current_player], &upperCard, taki_Mode, statistics);
		previous_player = current_player;
		if (strcmp(upperCard.type_card, TAKI_STRING) == 0)
			taki_Mode = true;
		if (taki_Mode)
		{
			move_next_player = false;
			if (player_choice == 0 || strcmp(upperCard.type_card, COLOR_STRING) == 0 || players[current_player].amountCards == 0)
			{ // if the taki mode needs to be turned off
				taki_Mode = false;
				move_next_player=specialCardsCheck(upperCard, &move_direction, &current_player,num_of_players, &players[current_player], statistics);
			}
				
		}
		else if(player_choice!=0) // if the player is not in taki mode and pressed not zero
			move_next_player=specialCardsCheck(upperCard, &move_direction, &current_player, num_of_players, &players[current_player], statistics);
	
		if(move_next_player) 
			turnNextPlayer(move_direction, &current_player);

		processEnd_Players(&current_player, move_direction, num_of_players);
		game_over = (players[previous_player].amountCards == 0);
	}
	printf("\nThe winner is... %s! Congratulations!\n\n", players[previous_player].name);
	printStatistics(statistics);

}


void processEnd_Players(int* current_player, int move_direction, int num_of_players)
{
	/*
	* the parameters of the function:
	  1)pointer to the integer that represents the current turn in the game
	  2)integer that represents the current direction turns of the game
	  3)integer that represents the number of players in the game

	* the function checks if the current turn is in the last player by 
	  the direction turns of the game and then resets the turns of the player if needed.
	*/
	if (move_direction == RIGHT_DIRECTION && (*current_player) > (num_of_players - 1))
		(*current_player) = 0;
	else if (move_direction == LEFT_DIRECTION && (*current_player) < 0)
		(*current_player) = num_of_players - 1;
}


bool specialCardsCheck(Card upperCard,int* move_direction,int* current_player,int num_of_players,Player* player,card_Statistics* statistics)
{
	/*
	* the paramters of the function:
	  1)the upper card
	  2)pointer to the integer that represents the current direction turns of the game.
	  3)pointer to the integer that represents the current turn in the game
	  4)integer that represents the number of players in the game
	  5)pointer to the current player 
	  6)array of statistics of the cards

	* the function checks if the current upper card is one of the special cards of the TAKI game
	  and if its true then the function performs what is required by the rules of the TAKI game.

	* the return value of the function: the status of the turns.
	*/
	bool move_next_player = true;
	if (strcmp(upperCard.type_card, STOP_STRING) == 0)
	{
		stopCard(*move_direction, current_player, num_of_players, player, statistics);
		move_next_player = false;
	}

	else if (strcmp(upperCard.type_card, CHANGE_DIRACTION_STRING) == 0)
		changeDirectionCard(move_direction, current_player);

	else if (strcmp(upperCard.type_card, PLUS_STRING) == 0)
	{
		if ((*player).amountCards == 0)
			addCard(player, statistics);
		else
			move_next_player = false;
	}
	return move_next_player;

	
}