//POPA STEFANIA, 315CD
#include "utils.h"

void* (* Abilities[4])(void* x) = {RotateMatrix, DecodeString, InvertArray, KillPlayer};

char *fromEnumtoString(PlayerRoles playerRole)
{
	char *str = calloc(MAX_LEN_STR_ATR, sizeof(char));
	switch(playerRole)
	{
		case Rotator:
			str = strcpy(str, "Rotator");
			break;
		case Decoder:
			str = strcpy(str, "Decoder");
			break;
		case Invertor:
			str = strcpy(str, "Invertor");
			break;
		case Impostor:
			str = strcpy(str, "Impostor");
			break;
	}
	return str;
}

int nr_digits(int x)
{
	int nr_dig;
	nr_dig = 0;

	while( x != 0){
		x/= 10;
		nr_dig++;
	}
	
	return nr_dig;
}

int ten_power(int x)
{
	int ten_pow = 1;
	
	while(x > 0){
		ten_pow*= 10;
		x--;
	}

	return ten_pow;
}

// Task 1
void *RotateMatrix(void *input)
{
	int n, i, j, nr_dig, ten_pow;
	int **matrix;

	n = *(int *)input;
	matrix = (int**)calloc( n, sizeof(int *) );
	if(matrix == NULL){
		exit(1);
	}

	for( i = 0; i < n; i++ ){
		matrix[i] = calloc( n, sizeof(int) );
		if(matrix[i] == NULL){
			exit(1);
		}
	}

	char *output_string = calloc(MAX_LEN_STR_OUT, sizeof(char));
	if(output_string == NULL){
		exit(1);
	}

	for( i = 0; i < n; i++ ){
		nr_dig = nr_digits(i + 1);
		ten_pow = ten_power(nr_dig);

		char *buffer = calloc(100, sizeof(char));
		if(buffer == NULL){
			exit(1);
		}

		for( j = 0; j < n; j++ ){
			matrix[i][j] = (n - j) * ten_pow + (i + 1);
			int nr_chrs = sprintf(buffer, "%d ", matrix[i][j]);
			if(nr_chrs > 0){
				strcat(output_string, buffer);
			}
		}
		free(buffer);
		if(i < (n - 1)){
			strcat(output_string, "\n");
		}
	}
	for(i = 0; i < n; i++){
		free(matrix[i]);
	}
	free(matrix);
	
	return (void *)output_string;
}

// Task 2
void *DecodeString(void *input)
{
	char *input_string;
	input_string = (char *)input;

	char *ptr;
	int a_i;
	int sum = 0;

	ptr = strtok(input_string,"_");

	while(ptr != NULL){
		a_i = atoi(ptr);
		sum+= a_i;
		ptr = strtok(NULL, "_\n");
	}

	char *output_sum = calloc(nr_digits(sum) + 2, sizeof(char));
	if(output_sum == NULL){
		exit(1);
	}
	sprintf(output_sum, "%d", sum);
	
	return (void *)output_sum;
}

//Task 3
void *InvertArray(void *input)
{
	int *numbers = (int *)input;
	int i, aux, nr = numbers[0];

	if(nr % 2 == 0){
		for(i = 1; i < nr; i+=2){
			aux = numbers[i];
			numbers[i] = numbers[i + 1];
			numbers[i + 1] = aux;
		}
	}
	else
	{
		for(i = 1; i <= nr/2; i++){
			aux = numbers[i];
			numbers[i] = numbers[nr + 1 - i];
			numbers[nr + 1 - i] = aux;
		}
	}

	char *buffer = calloc(10,sizeof(char));
	if(buffer == NULL){
		exit(1);
	}
	char *output_sum = calloc(MAX_LEN_STR_OUT,sizeof(char));
	if(output_sum == NULL){
		exit(1);
	}
	for(i = 1; i <= nr; i++){
		sprintf(buffer, "%d ", numbers[i]);
		strcat(output_sum, buffer);
	}
	free(buffer);
	
	return (void *)output_sum;
}

//Task 4
Player *allocPlayer()
{
	Player *new_player = (Player *)malloc(sizeof(Player));
	if(new_player == NULL){
		exit(1);
	}
	new_player->name = malloc(MAX_LEN_STR_ATR * sizeof(char));
	if(new_player->name == NULL){
		exit(1);
	}
	new_player->color = malloc(MAX_LEN_STR_ATR * sizeof(char));
	if(new_player->color == NULL){
		exit(1);
	}
	new_player->hat = malloc(MAX_LEN_STR_ATR * sizeof(char));
	if(new_player->hat == NULL){
		exit(1);
	}
	new_player->alive = 1;
	return new_player;
}

//Task 4
Game *allocGame()
{
	Game *new_game = (Game *)malloc(sizeof(Game));
	if(new_game== NULL){
		exit(1);
	}
	new_game->name = malloc(MAX_LEN_STR_ATR * sizeof(char));
	if(new_game->name == NULL){
		exit(1);
	}
	return new_game;
}

//Task 5
Player *ReadPlayer(FILE *inputFile)
{
	int i;
	Player *new_player = allocPlayer();

	fscanf(inputFile, "%s", new_player->name);
	fscanf(inputFile, "%s", new_player->color);
	fscanf(inputFile, "%s", new_player->hat);
	fscanf(inputFile, "%d\n", &(new_player->numberOfLocations));

	new_player->locations = (Location *)malloc((new_player->numberOfLocations) * sizeof(Location));
	if(new_player->locations == NULL){
		exit(1);
	}

	for( i = 0; i < (new_player->numberOfLocations); i++){
		fscanf(inputFile, "(%d,%d) ", &(new_player->locations[i].x), &(new_player->locations[i].y));
	}

	char *pl_role =  malloc(10 * sizeof(char));
	if(pl_role == NULL){
		exit(1);
	}
	fscanf(inputFile, "%s", pl_role);

	if(strcmp(pl_role, "Rotator") == 0)
	{
		new_player->playerRole = Rotator;
	}
	else if(strcmp(pl_role, "Decoder") == 0){
		new_player->playerRole = Decoder;
	}
	else if(strcmp(pl_role, "Invertor") == 0){
		new_player->playerRole = Invertor;
	}
	else
	{
		new_player->playerRole = Impostor;
	}
	free(pl_role);

	new_player->indexOfLocation = 0;
	new_player->ability = Abilities[new_player->playerRole];
	
	return new_player;
}

// Task 5
Game *ReadGame(FILE *inputFile)
{
	int i;
	Game *new_game = allocGame();
	
	fscanf(inputFile, "%s", new_game->name);
	fscanf(inputFile, "%d\n", &(new_game->killRange));
	fscanf(inputFile, "%d\n", &(new_game->numberOfCrewmates));

	new_game->crewmates = (Player **)malloc((new_game->numberOfCrewmates) * sizeof(Player * ));
	if(new_game->crewmates == NULL){
		exit(1);
	}

	for(i = 0; i < (new_game->numberOfCrewmates); i++){
		new_game->crewmates[i] = ReadPlayer(inputFile);
	}
	new_game->impostor = ReadPlayer(inputFile);
	
	return new_game;
}

// Task 6
void WritePlayer(Player *player, FILE *outputFile)
{
	int i; 

	char *pl_role = fromEnumtoString(player->playerRole);

	fprintf(outputFile, "Player %s with color %s, hat %s and role %s has entered the game.\n", player->name, player->color, player->hat, pl_role);

	free(pl_role);

	fprintf(outputFile, "Player's locations: ");
	for(i = 0; i < (player ->numberOfLocations); i++){
		fprintf(outputFile,"(%d,%d) ", player->locations[i].x, player->locations[i].y);

	}
	fprintf(outputFile, "\n");

	return;
}

// Task 6
void WriteGame(Game *game, FILE *outputFile)
{
	int i;

	fprintf(outputFile, "Game %s has just started!\n", game->name);
	fprintf(outputFile, "\tGame options:\n");
	fprintf(outputFile, "Kill Range: %d\n", game->killRange);
	fprintf(outputFile, "Number of crewmates: %d\n\n", game ->numberOfCrewmates);
	fprintf(outputFile, "\tCrewmates:\n");

	for(i = 0; i < (game->numberOfCrewmates); i++){
		WritePlayer(game->crewmates[i], outputFile);
	}

	fprintf(outputFile, "\n\tImpostor:\n");
	WritePlayer(game->impostor, outputFile);

	return;
}

//Task 7
void *KillPlayer(void *input)
{
	Game *game = (Game *)input;
	int i;
	int distkp = game->killRange; 
	int index = -1; 
	int dist; 
	int indOfLoc;

	for(i = 0; i < (game->numberOfCrewmates); i++)
	{
		if(game->crewmates[i]->alive){
			indOfLoc = game->crewmates[i]->indexOfLocation;
			dist = abs((game->crewmates[i]->locations[indOfLoc].x) - (game->impostor->locations[game->impostor->indexOfLocation].x)) + abs((game->crewmates[i]->locations[indOfLoc].y) - (game->impostor->locations[game->impostor->indexOfLocation].y));
			if(dist <= distkp){
				index = i;
				distkp = dist;
			}
		}
	}

	char *output = calloc(MAX_LEN_STR_OUT, sizeof(char));
	if(output == NULL){
		exit(1);
	}

	if(index >= 0){
		sprintf(output, "Impostor %s has just killed crewmate %s from distance %d.", game->impostor->name, game->crewmates[index]->name, distkp);
		game->crewmates[index]->alive = 0;
	}
	else
	{
		sprintf(output, "Impostor %s couldn't kill anybody.", game->impostor->name);
	}

	return (void *)output;
}

// Task 8
void CalcuateNextCycleOfGame(Game *game, FILE *outputFile, void **inputMatrix)
{
	int i, indOfLoc;

	for(i = 0; i < (game->numberOfCrewmates); i++){
		if((game->crewmates[i]->alive) == 0){
			fprintf(outputFile, "Crewmate %s is dead.\n", game->crewmates[i]->name);
		}
		else
		{
			if(((game->crewmates[i]->indexOfLocation) + 1) < (game->crewmates[i]->numberOfLocations)){
				(game->crewmates[i]->indexOfLocation)++;
			}
			else
			{
				game->crewmates[i]->indexOfLocation = 0;
			}
			indOfLoc = game->crewmates[i]->indexOfLocation;
			fprintf(outputFile, "Crewmate %s went to location (%d,%d).\n", game->crewmates[i]->name, game->crewmates[i]->locations[indOfLoc].x, game->crewmates[i]->locations[indOfLoc].y);
			fprintf(outputFile, "Crewmate %s's output:\n", game->crewmates[i]->name);
			char *abil = (char *)game->crewmates[i]->ability(inputMatrix[i]);
			fprintf(outputFile, "%s\n", abil);
			free(abil);
		}
	}
	if(((game->impostor->indexOfLocation) + 1) < (game->impostor->numberOfLocations)){
		(game->impostor->indexOfLocation)++;
	}
	else
	{
		game->impostor->indexOfLocation = 0;
	}
	indOfLoc = game->impostor->indexOfLocation;
	fprintf(outputFile, "Impostor %s went to location (%d,%d).\n", game->impostor->name, game->impostor->locations[indOfLoc].x, game->impostor->locations[indOfLoc].y);
	fprintf(outputFile, "Impostor %s's output:\n", game->impostor->name);
	char *abil = (char *)KillPlayer((void *)game);
	fprintf(outputFile, "%s\n", abil);
	free(abil);

	return;
}

// Task 9
void FreePlayer(Player *player)
{
	free(player->name);
	free(player->color);
	free(player->hat);
	free(player->locations);
	free(player);

	return;
}

// Task 9
void FreeGame(Game *game)
{
	int i;

	free(game->name);

	for(i = 0; i < (game->numberOfCrewmates); i++){
		FreePlayer(game->crewmates[i]);
	}

	free(game->crewmates);
	FreePlayer(game->impostor);
	free(game);

	return;
}