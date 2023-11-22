#include <stdio.h>
#include <KBD.h>

// Constants

#define UP 0x08
#define DOWN 0x20
#define LEFT 0x40
#define RIGHT 0x10
#define SELECT 0x01
#define REST 0x00

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define ENEMY_TIMER 1000
#define LASER_TIMER 500

// Game state
int laserx[10];
int lasery[10] = {-1};
int laserCount = 0;
int playerX, playerY;
int enemyX[10], enemyY[10], enemyTimer;
int score;
int gameOver;
int laserTimer;
int movingRight = 1;
// Initialize game
void initGame() {
    playerX = SCREEN_WIDTH / 2;
    playerY = SCREEN_HEIGHT - 10;
    for(int i = 0; i < 10; i++){
        enemyX[i] = (i * 20) + 20;
        enemyY[i] =  20 ; 
    }
    score = 0;
    gameOver = 0;
}

// Update game state
void updateGame() {
    // Update player position based on user input
    switch(get_button()) {
        case LEFT:
            if(playerX > 0){playerX--;}
            break;
        case RIGHT:
            if(playerX < SCREEN_WIDTH - 1){playerX++;}
            break;
        case UP:
            //shoot
            laserx[laserCount] = playerX;
            lasery[laserCount] = 1;
            break;
        
    }
    // Update laser position and movement
    laserTimer= (laserTimer + 1) % LASER_TIMER;
    if(laserTimer == 0)
    {
        for(int i = 0; i < laserCount; i++)
        {
            if(lasery[i] != -1)
            {
                lasery[i]++;
                if(lasery[i] > SCREEN_HEIGHT){
                    lasery[i] = -1;
                }
            }
        }
    }


    // Update enemy position and movement
    enemyTimer = (enemyTimer + 1) % ENEMY_TIMER;
    if(enemyTimer == 0)
    {
        if(movingRight == 1){

            for(int i = 0; i < 10; i++)
            {   
                
                if(enemyY[i] > SCREEN_HEIGHT){
                    gameOver = 1;
                }
                if(enemyY[i] != -1){
                    enemyX[i] = (i * 20) + 20;
                    enemyY[i]++; 
                }
                if(enemyX[9] > SCREEN_WIDTH - 20){
                    movingRight = 0;
                }
            }
        }else{
            for(int i = 0; i < 10; i++)
            {   
                
                if(enemyY[i] > SCREEN_HEIGHT){
                    gameOver = 1;
                }
                if(enemyY[i] != -1){
                    enemyX[i] = (i * 20) + 20;
                    enemyY[i]--; 
                }
                if(enemyX[0] < 20){
                    movingRight = 1;
                }
            }
        }
    }
    // Check for collisions
    for(int i = 0; i<10 ; i++){
        for(int j = 0; j<10; j++){
            if(laserx[i] == enemyX[j] && lasery[i] == enemyY[j]){
                score++;
                enemyY[j] = -1;
                lasery[i] = -1;
            }
        }
    }
    // Update score and game over state
}

// Render game state
void renderGame() {
    // Clear screen
    
    // Render player
    
    // Render enemy invaders
    
    // Render score
    
    // Render game over message
}

// Main game loop
void gameLoop() {
    while (!gameOver) {
        // Update game state
        
        // Render game state
        
        // Wait for user input
    }
}

int main() {
    initGame();
    gameLoop();
    return 0;
}
