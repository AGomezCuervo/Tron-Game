#include <raylib.h>
#include <string.h>
#define MAX_TAIL_LENGTH 2560
#define SQUARE_SIZE 5

const int WIDTH = 600;
const int HEIGHT = 400;

typedef struct Tail {
  Vector2 position;
  Vector2 size;
  Vector2 speed;
  Color color;
} Tail;

typedef struct Tron {
  Tail tail[MAX_TAIL_LENGTH];
  int tail_counter;
} Tron;


char *winner_player_1 = "Player 1 wins";
char *winner_player_2 = "Player 2 wins";
char winner[13];
bool allow_move = false;
bool allow_move2 = false;
bool game_over = false;
Vector2 window = {0};
Tron player_1;
Tron player_2;

void initGame(void);         // Initialize game
void updateGame(void);       // Update game (one frame)
void drawGame(void);         // Draw game (one frame)
void updateDrawFrame(void);  // Update and Draw (one frame)

// Movement and Collaiders
void playersCollider(Tron *player_1,Tron *player_2);
void handlePlayersInput(Tron *player_1, Tron *player_2, bool *allowMove1, bool *allowMove2);
void playerMovement(Tron *player, bool *allowMove);
void wallCollaider(Tron *player_1, Tron *player_2);

int main()
{
  InitWindow(WIDTH, HEIGHT, "Tron");
  initGame();
  SetTargetFPS(40);

  while (!WindowShouldClose()) {
    updateDrawFrame();
  }
  CloseWindow();
  return 0;
}

void initGame()
{
  window.x = WIDTH;
  window.y = HEIGHT;

  player_1.tail_counter = 1;
  player_2.tail_counter = 1;

  for (int i = 0; i < MAX_TAIL_LENGTH; i++)
  {
    player_1.tail[i].position = (Vector2){WIDTH/2, HEIGHT/2};  
    player_1.tail[i].size = (Vector2){SQUARE_SIZE, SQUARE_SIZE};  
    player_1.tail[i].speed = (Vector2){SQUARE_SIZE, 0};  
    player_1.tail[i].color = GOLD;

    player_2.tail[i].position = (Vector2){WIDTH/2, HEIGHT/2};  
    player_2.tail[i].size = (Vector2){SQUARE_SIZE, SQUARE_SIZE};  
    player_2.tail[i].speed = (Vector2){-SQUARE_SIZE, 0};  
    player_2.tail[i].color = PURPLE;
  }
}

void updateDrawFrame()
{
  updateGame();
  drawGame();
}

void updateGame()
{
  if (!game_over)
  {
    handlePlayersInput(&player_1, &player_2 , &allow_move, &allow_move2);

    playerMovement(&player_1, &allow_move);
    playerMovement(&player_2, &allow_move2);

    playersCollider(&player_1, &player_2);

    wallCollaider(&player_1, &player_2);
  }
  else
  {
    if(IsKeyPressed(KEY_ENTER))
    {
      ClearBackground(BLACK);
      initGame();
      game_over = false;
    }
  }
}

void drawGame() 
{ 
  BeginDrawing();
  if(!game_over)
  {
    for (int i = 0; i < player_1.tail_counter ; i++)
      DrawRectangleV(player_1.tail[i].position, player_1.tail[i].size, player_1.tail[i].color);

    for (int i = 0; i < player_2.tail_counter ; i++)
      DrawRectangleV(player_2.tail[i].position, player_2.tail[i].size, player_2.tail[i].color);
  }
  else
  {
    DrawText(winner, GetScreenWidth()/2 - MeasureText(winner, 20)/2, GetScreenHeight()/2 - 70, 20, GRAY);
    DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth()/2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20)/2, GetScreenHeight()/2 - 50, 20, GRAY);
  }
  EndDrawing();
}

// Detects de collision of the players
void playersCollider(Tron *player_1, Tron *player_2)
{
  for (int i = 1; i < player_1->tail_counter; i++)
  {
    if((player_1->tail[0].position.x == player_1->tail[i].position.x) && 
        (player_1->tail[0].position.y == player_1->tail[i].position.y))
    {
      strcpy(winner, winner_player_2);
      game_over = true;
    }

    if((player_2->tail[0].position.x == player_2->tail[i].position.x) && 
        (player_2->tail[0].position.y == player_2->tail[i].position.y))
    {
      strcpy(winner, winner_player_1);
      game_over = true;
    }

    if((player_1->tail[0].position.x == player_2->tail[i].position.x) && 
        (player_1->tail[0].position.y == player_2->tail[i].position.y))
    {
      strcpy(winner, winner_player_2);
      game_over = true;
    }

    if((player_2->tail[0].position.x == player_1->tail[i].position.x) && 
        (player_2->tail[0].position.y == player_1->tail[i].position.y))
    {
      strcpy(winner, winner_player_1);
      game_over = true;
    }

    if((player_2->tail[0].position.x == player_1->tail[0].position.x) && 
        (player_2->tail[0].position.y == player_1->tail[0].position.y))
    {
      strcpy(winner, "empates");
      game_over = true;
    }
  }
}
void handlePlayersInput(Tron *player_1, Tron *player_2, bool *allowMove1, bool *allowMove2) 
{
  // Controlls Player_1
  if (IsKeyPressed(KEY_RIGHT) && *allowMove1 && player_1->tail[0].speed.x == 0) {
    player_1->tail[0].speed = (Vector2){SQUARE_SIZE, 0};
    *allowMove1 = false;
  }
  if (IsKeyPressed(KEY_LEFT) && *allowMove1 && player_1->tail[0].speed.x == 0) {
    player_1->tail[0].speed = (Vector2){-SQUARE_SIZE, 0};
    *allowMove1 = false;
  }
  if(IsKeyPressed(KEY_UP) && *allowMove1 && player_1->tail[0].speed.y == 0)
  {
    player_1->tail[0].speed = (Vector2){0, -SQUARE_SIZE};
    *allowMove1 = false;
  }
  if(IsKeyPressed(KEY_DOWN) && *allowMove1 && player_1->tail[0].speed.y == 0)
  {
    player_1->tail[0].speed = (Vector2){0, SQUARE_SIZE};
    *allowMove1 = false;
  }

  // Controlls Player_2
  if (IsKeyPressed(KEY_D) && *allowMove2 && player_2->tail[0].speed.x == 0) {
    player_2->tail[0].speed = (Vector2){SQUARE_SIZE, 0};
    *allowMove2 = false;
  }
  if (IsKeyPressed(KEY_A) && *allowMove2 && player_2->tail[0].speed.x == 0) {
    player_2->tail[0].speed = (Vector2){-SQUARE_SIZE, 0};
    *allowMove2 = false;
  }
  if(IsKeyPressed(KEY_W) && *allowMove2 && player_2->tail[0].speed.y == 0)
  {
    player_2->tail[0].speed = (Vector2){0, -SQUARE_SIZE};
    *allowMove2 = false;
  }
  if(IsKeyPressed(KEY_S) && *allowMove2 && player_2->tail[0].speed.y == 0)
  {
    player_2->tail[0].speed = (Vector2){0, SQUARE_SIZE};
    *allowMove2 = false;
  }
}

void playerMovement(Tron *player, bool *allowMove)
{
  Vector2 aux = player->tail[0].position;
  Vector2 t = aux;
  for (int i = 0; i < player->tail_counter; i++)
  {
    if (i == 0)
    {
      player->tail[0].position.x += player->tail[0].speed.x;
      player->tail[0].position.y += player->tail[0].speed.y;
      *allowMove = true;
    }
    else
    {
      t = player->tail[i].position;
      player->tail[i].position = aux;
      aux = t;
    }
  }
  player->tail_counter++;
}
void wallCollaider(Tron *player_1, Tron *player_2)
{
  if(player_1->tail[0].position.x >= WIDTH || player_1->tail[0].position.x < 0)
    {
      strcpy(winner, winner_player_2);
      game_over = true;
    }

  if(player_1->tail[0].position.y >= HEIGHT || player_1->tail[0].position.y < 0)
    {
      strcpy(winner, winner_player_2);
      game_over = true;
    }

  if(player_2->tail[0].position.x >= WIDTH || player_2->tail[0].position.x < 0)
    {
      strcpy(winner, winner_player_1);
      game_over = true;
    }

  if(player_2->tail[0].position.y >= HEIGHT || player_2->tail[0].position.y < 0)
    {
      strcpy(winner, winner_player_1);
      game_over = true;
    }
}
