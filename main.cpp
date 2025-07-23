#include <iostream>
#include <raylib.h>
#include <cstdlib>
#include <ctime>

using namespace std;


Color Blue = Color{160, 205, 255, 255};
Color Grey = Color{178, 178, 200, 255};
Color DarkBlue = Color{135, 165, 255, 255};
Color Beige = Color{237, 232, 208, 255};



int playerScore = 0;
int highScore = 0;
bool gameActive = false;


class player {
    public:

    int x = 100;
    int y = 0; 
    int ySpeed = 0;
    int gravity = 1;
    Sound jump;
    
    void update(int height) {
     
        // Jump if on ground
        y += ySpeed;
        if (IsKeyPressed(KEY_SPACE) && y >= 460 - height) {
            ySpeed = -20; // Jump impulse
            PlaySound(jump);
        }
        if (y < 460 - height) {
            ySpeed += gravity;
            
        }
        
        if (IsKeyDown(KEY_A) && x > 20) {
            x -= 5;
        }
        if (IsKeyDown(KEY_D) && x < 175) {
            x += 5;
        }

        // Ground collision 
        if (y > 460 - height) {
            
            ySpeed = 0;
            y = 460 - height;
        }
       

    }


};


class obstacles {
public:
    int x = 800;
    int y = 200;
    int xSpeed = 5;
    int picked = 0; // store current sprite index
    int scale = 0;
    Texture2D choices[3];
    Texture2D choicesFly[3];

    void update(int height) {
        
        x -= xSpeed + scale;
        y = 460 - height;
        

        // reset if off screen
        if (x < -50) {
            x = rand() % (1600 - 950 + 1) + 950;
            picked = rand() % 3; // pick a new sprite here
            playerScore += 100;

            //speed up every 250 pts
            if (playerScore > 250) {
                scale = playerScore/250;
              
            }
        }
    }

    void draw() {
        DrawTextureEx(choices[picked], (Vector2){(float)x, (float)y}, 0.0f, 0.6f, WHITE);
    }
};

class flying: public obstacles {
public:

int xSpeed = 2;
int scale = 0;
void update(int height) {
        x -= xSpeed + scale;
        y = 300 - height;

        // reset if off screen
        if (x < -50) {
            x = rand() % (1600 - 950 + 1) + 950;
            picked = rand() % 2; // pick a new sprite here
            playerScore += 50;

            //speed up every 250 pts 
            if (playerScore > 350) {
                scale = playerScore/350;
                
            }
        }
    }

    void draw() {
        DrawTextureEx(choicesFly[picked], (Vector2){(float)x, (float)y}, 355.0f, 0.3f, WHITE);
    }


};

class berry {
public:

int x = 900;
int y = 430;
int xSpeed = rand() % (15 - 2 + 1) + 2;
Texture2D leppa;

void update() {
    x -= xSpeed;
    if (x < -50) {
            x = rand() % (10000 - 6000 + 1) + 6000;
}
}

void draw() {
        DrawTextureEx(leppa, (Vector2){(float)x, (float)y}, 355.0f, 0.1f, WHITE);
    }

};

class projectiles {
    public:

    int x;
    int y;
    int xSpeed = 5;
    bool canShoot = false;
    int shots = 0;

    Texture2D move;

    void update() {
        x += xSpeed;
    }
    void draw() {
        DrawTextureEx(move, (Vector2){(float)x, (float)y}, 0.0f, 1.0f, WHITE);
    }
};




#include <vector>

std::vector<projectiles> bullets;
int shotsRemaining = 0;

int main() {
    
    srand(time(NULL));

    player playerPokemon;
    obstacles rocket;
    flying flyers; 
    berry leppaBerry;
    projectiles bullet;

    
 
    InitWindow(800, 600, "Rocket Run");
    SetTargetFPS(60);

    InitAudioDevice();
    

    //Resource loading

    Font titleFont = LoadFont("resources/TitleFont.TTF");
    Texture2D starters[3];
    Texture2D types[3];

    starters[0] = LoadTexture("resources/Bulbasaur.png");
    starters[1] = LoadTexture("resources/Squirtle.png");
    starters[2] = LoadTexture("resources/Charmander.png");

    Texture2D chosen = starters[0];
    int height = (int)(chosen.height*0.08);
    types[0] = LoadTexture("resources/grass.png");
    types[1] = LoadTexture("resources/water.png");
    types[2] = LoadTexture("resources/fire.png");

    Texture2D bulbasaur = LoadTexture("resources/Bulbasaur.png");
    Texture2D squirtle = LoadTexture("resources/Squirtle.png");
    Texture2D charmander = LoadTexture("resources/Charmander.png");

    Texture2D leppa = LoadTexture("resources/Berry.png");
    
    leppaBerry.leppa = leppa;

    Texture2D enemies[3];
    Texture2D enemiesFly[2];
    

    enemies[0] = LoadTexture("resources/Jesse.png");
    enemies[1] = LoadTexture("resources/James.png");
    enemies[2] = LoadTexture("resources/Meowth.png");

    enemiesFly[0] = LoadTexture("resources/Koffing.png");
    enemiesFly[1] = LoadTexture("resources/Inkay.png");

    Music menu[2];
    menu[0] = LoadMusicStream("resources/music/menu1.mp3"); 
    menu[1] = LoadMusicStream("resources/music/menu2.mp3"); 
    
    Music battle[5];
    battle[0] = LoadMusicStream("resources/music/run1.mp3");
    battle[1] = LoadMusicStream("resources/music/run2.mp3");
    battle[2] = LoadMusicStream("resources/music/run3.mp3");
    battle[3] = LoadMusicStream("resources/music/run4.mp3");
    battle[4] = LoadMusicStream("resources/music/run5.mp3");
    
    SetMusicVolume(menu[0], 0.1);  
    SetMusicVolume(menu[1], 0.1);  

    SetMusicVolume(battle[0], 0.1);
    SetMusicVolume(battle[1], 0.1);
    SetMusicVolume(battle[2], 0.1);
    SetMusicVolume(battle[3], 0.1);
    SetMusicVolume(battle[4], 0.1);

    Sound jumpSound = LoadSound("resources/sfx/jump.mp3");
    Sound hitSound = LoadSound("resources/sfx/hit.mp3");
    Sound deathSound = LoadSound("resources/sfx/death.mp3");
    Sound grabSound = LoadSound("resources/sfx/berryGrab.mp3");

    SetSoundVolume(jumpSound, 0.2);
    SetSoundVolume(grabSound, 0.9);
    SetSoundVolume(deathSound, 0.2);
    SetSoundVolume(hitSound, 0.1);

    playerPokemon.jump = jumpSound;
    //send to obstacle class
    rocket.choices[0] = enemies[0];
    rocket.choices[1] = enemies[1];
    rocket.choices[2] = enemies[2];

    flyers.choicesFly[0] = enemiesFly[0];
    flyers.choicesFly[1] = enemiesFly[1];

    

    
   
    int rHeight[3];
    int fHeight[2];
    rHeight[0] = (int)(enemies[0].height*0.6);
    rHeight[1] = (int)(enemies[1].height*0.6);
    rHeight[2] = (int)(enemies[2].height*0.6);

    fHeight[0] = (int)(enemiesFly[0].height*0.6);
    fHeight[1] = (int)(enemiesFly[1].height*0.6);

    int menuPick = rand() % 2;
    int runPick = rand() % 5;
   

    while (!WindowShouldClose()) {
        BeginDrawing();

        if (gameActive) {
            //main game loop
            StopMusicStream(menu[menuPick]);
             
            
            if (!IsMusicStreamPlaying(battle[runPick]))
            {
                PlayMusicStream(battle[runPick]);
            }
            UpdateMusicStream(battle[runPick]);

            ClearBackground(Blue);
            DrawRectangle(0, 450, 800, 150, Beige);
            DrawTextureEx(chosen, (Vector2){(float)playerPokemon.x, (float)playerPokemon.y}, 0.0f, 0.08f, WHITE);
            playerPokemon.update(height);
            rocket.update(rHeight[rocket.picked]);
            flyers.update(fHeight[flyers.picked]);
            leppaBerry.update();
            rocket.draw();
            flyers.draw();
            leppaBerry.draw();

            for (size_t i = 0; i < bullets.size(); i++) {
            bullets[i].update();
            bullets[i].draw();
                }
            DrawTextEx(titleFont, TextFormat("%i", playerScore), (Vector2){GetScreenWidth()/2 - MeasureTextEx(titleFont, TextFormat("%i", playerScore), 30, 1).x/2, 15}, 30, 1, DarkBlue);
            std::string shotsText = "Shots: " + std::to_string(shotsRemaining);
            DrawTextEx(titleFont, shotsText.c_str(), 
                (Vector2){650, 20}, 
                20, 1, DarkBlue);
            DrawTextEx(titleFont, "A+D to Move", 
                (Vector2){10, 10}, 
                15, 1, DarkBlue);
            DrawTextEx(titleFont, "Space to Jump", 
                (Vector2){10, 25}, 
                15, 1, DarkBlue);
            DrawTextEx(titleFont, "F to Use a Move", 
                (Vector2){10, 40}, 
                15, 1, DarkBlue);

            //firing

            if (IsKeyPressed(KEY_F) && shotsRemaining > 0) {  // KEY_F is shoot key
            projectiles newBullet;
            newBullet.x = playerPokemon.x + chosen.width * 0.08f; // start at player's front
            newBullet.y = playerPokemon.y + chosen.height * 0.08f / 2 - 25;
            newBullet.move = bullet.move; // assign texture
            bullets.push_back(newBullet);
            shotsRemaining--;
            }

            

            //collision rectangles

           Rectangle playerRect = {
            (float)playerPokemon.x,
            (float)playerPokemon.y + 5,
            (chosen.width * 0.08f) - 10,
            (chosen.height * 0.08f) - 10
                };

            Rectangle obstacleRect = {
            (float)rocket.x,
            (float)rocket.y + 5,
            (enemies[rocket.picked].width * 0.6f) - 15,
            (enemies[rocket.picked].height * 0.6f) - 5
                };

            Rectangle flyingRect = {
            (float)flyers.x,
            (float)flyers.y - 30,
            (enemiesFly[flyers.picked].width * 0.6f) - 40,
            (enemiesFly[flyers.picked].height * 0.6f) - 50
                };

            Rectangle berryRect = {
            (float)leppaBerry.x,
            (float)leppaBerry.y - 10,
            leppa.width * 0.1f - 10,
            leppa.height * 0.1f - 10
                };
        
        if (CheckCollisionRecs(playerRect, obstacleRect)) {
            PlaySound(deathSound);
            gameActive = false;
            menuPick = rand() % 2;
            runPick = rand() % 5;
            
            
        }
        if (CheckCollisionRecs(playerRect, flyingRect)) {
            PlaySound(deathSound);
            gameActive = false;
            menuPick = rand() % 2;
            runPick = rand() % 5;
            
          
        }

        if (CheckCollisionRecs(playerRect, berryRect)) {
            leppaBerry.x = rand() % (5000 - 4000 + 1) + 4000;
            leppaBerry.xSpeed = rand() % (15 - 2 + 1) + 2;
            shotsRemaining += 1;
            PlaySound(grabSound);
         }

         for (size_t i = 0; i < bullets.size(); ) {
    Rectangle bulletRect = {
        (float)bullets[i].x,
        (float)bullets[i].y,
        bullet.move.width * 0.5f,  // adjust scale if needed
        bullet.move.height * 0.5f
    };

    bool hit = false;

    // Check collision with ground enemy (rocket)
    if (CheckCollisionRecs(bulletRect, obstacleRect)) {
        rocket.x = rand() % (1600 - 950 + 1) + 950;
        rocket.picked = rand() % 3;
        hit = true;
        PlaySound(hitSound);
        
    }

    // Check collision with flying enemy
    else if (CheckCollisionRecs(bulletRect, flyingRect)) {
        flyers.x = rand() % (1600 - 950 + 1) + 950;
        flyers.picked = rand() % 2;
        hit = true;
        PlaySound(hitSound);
        }

        // Remove bullet if it hit something or off-screen
        if (hit || bullets[i].x > GetScreenWidth()) {
            bullets.erase(bullets.begin() + i);
        } else {
            i++;
        }
}
        }
    

        //bullet collision logic

        
        else {
            StopMusicStream(battle[runPick]);
            if (IsSoundPlaying(deathSound)) {
                continue;
            }
            

            else if (!IsSoundPlaying(deathSound)){
                

            if (!IsMusicStreamPlaying(menu[menuPick]))
            {
                PlayMusicStream(menu[menuPick]);
            }
            
            UpdateMusicStream(menu[menuPick]);
            ClearBackground(Blue);
            std::string highScoreText = "High Score: " + std::to_string(highScore);
            if (playerScore > highScore) {
                highScore = playerScore;
            }
            DrawTextEx(titleFont, "ROCKET RUN!", (Vector2){GetScreenWidth()/2 - MeasureTextEx(titleFont, "ROCKET RUN!", 50, 1).x/2, 150}, 50, 1, DarkBlue);
            DrawTextEx(titleFont, "Click screen for audio", (Vector2){GetScreenWidth()/2 - MeasureTextEx(titleFont, "Click screeen for audio", 15, 1).x/2, 10}, 15, 1, DarkBlue);
            DrawTextEx(titleFont, "Pick a starter to begin the game", (Vector2){GetScreenWidth()/2 - MeasureTextEx(titleFont, "Pick a starter to begin the game", 27, 1).x/2, 250}, 27, 1, DarkBlue);
            
            DrawTextEx(titleFont, highScoreText.c_str(), 
                (Vector2){GetScreenWidth()/2 - MeasureTextEx(titleFont, highScoreText.c_str(), 30, 1).x/2, 300}, 
                30, 1, DarkBlue);
           
            DrawTextureEx(bulbasaur, (Vector2){150, 422}, 0.0f, 0.15f, WHITE);
            DrawTextEx(titleFont, TextFormat("%i", 1), (Vector2){180, 515}, 30, 1, DarkBlue);
           
            DrawTextureEx(squirtle, (Vector2){(float)GetScreenWidth()/2.0f - squirtle.width*0.08f, 415}, 0.0f, 0.15f, WHITE);
            DrawTextEx(titleFont, TextFormat("%i", 2), (Vector2){390, 515}, 30, 1, DarkBlue);
            
            DrawTextureEx(charmander, (Vector2){565, 412}, 0.0f, 0.15f, WHITE);
            DrawTextEx(titleFont, TextFormat("%i", 3), (Vector2){605, 515}, 30, 1, DarkBlue);
            
            shotsRemaining = 0;

            if (IsKeyPressed(KEY_ONE)) {
                chosen = starters[0];
                bullet.move = types[0];
                height = (int)(chosen.height*0.08);
               
                playerScore = 0; // Reset score when starting the game

                 // Reset player position
                playerPokemon.x = 100;
                playerPokemon.y = 0;
                playerPokemon.ySpeed = 0;

                // Reset rocket
                rocket.x = 800;
                rocket.picked = rand() % 3;
                rocket.xSpeed = 5;
                rocket.scale = 0;

                // Reset flying obstacle
                flyers.x = 1000;  // or another starting x
                flyers.picked = rand() % 2;
                flyers.xSpeed = 2;
                flyers.scale = 0;
                gameActive = true;

            }
            if (IsKeyPressed(KEY_TWO)) {
                chosen = starters[1];
                bullet.move = types[1];
                height = (int)(chosen.height*0.08);
                
                playerScore = 0; // Reset score when starting the game

                 // Reset player position
                playerPokemon.x = 100;
                playerPokemon.y = 0;
                playerPokemon.ySpeed = 0;

                // Reset rocket
                rocket.x = 800;
                rocket.picked = rand() % 3;
                rocket.xSpeed = 5;
                rocket.scale = 0;

                // Reset flying obstacle
                flyers.x = 1000;  // or another starting x
                flyers.picked = rand() % 2;
                flyers.xSpeed = 2;
                flyers.scale = 0;
                gameActive = true;
            }
            if (IsKeyPressed(KEY_THREE)) {
                chosen = starters[2];
                bullet.move = types[2];
                height = (int)(chosen.height*0.08);
                
                playerScore = 0; // Reset score when starting the game

                 // Reset player position
                playerPokemon.x = 100;
                playerPokemon.y = 0;
                playerPokemon.ySpeed = 0;

                // Reset rocket
                rocket.x = 800;
                rocket.picked = rand() % 3;
                rocket.xSpeed = 5;
                rocket.scale = 0;

                // Reset flying obstacle
                flyers.x = 1000;  // or another starting x
                flyers.picked = rand() % 2;
                flyers.xSpeed = 2;
                flyers.scale = 0;
                gameActive = true;
            }
            }
        }
        

        EndDrawing();
    }
    
    //resource Unloading
    UnloadFont(titleFont);
    UnloadTexture(chosen);
    UnloadTexture(enemies[0]);
    UnloadTexture(enemies[1]);
    UnloadTexture(enemies[2]);
    UnloadTexture(starters[0]);
    UnloadTexture(starters[1]);
    UnloadTexture(starters[2]);
    UnloadTexture(types[0]);
    UnloadTexture(types[1]);
    UnloadTexture(types[2]);
    UnloadTexture(enemiesFly[0]);
    UnloadTexture(enemiesFly[1]);

    UnloadMusicStream(menu[0]);
    UnloadMusicStream(menu[1]);

    UnloadMusicStream(battle[0]);
    UnloadMusicStream(battle[1]);
    UnloadMusicStream(battle[2]);
    UnloadMusicStream(battle[3]);
    UnloadMusicStream(battle[4]);

    UnloadSound(jumpSound);
    UnloadSound(hitSound);
    UnloadSound(deathSound);
 
    UnloadSound(grabSound);
    CloseWindow();
    CloseAudioDevice();
    return 0;
}