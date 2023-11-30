


void gameLibraryController(void const *argument){
    while(1){
        osSignalWait(0x01, osWaitForever);
        osSemaphoreWait(running_semaphoreID, osWaitForever);
        int input;
        int laserx[10];
        int lasery[10] = {-1};
        int laserCount = 0;
        int playerX, playerY;
        int exp_plX, exp_plY;
        int enemyX[10], enemyY[10], enemyTimer;
        int exp_enX[10], exp_enY[10];
        int score;
        int gameOver;
        int laserTimer;
        int movingRight = 1;
        playerX = SCREEN_WIDTH / 2;
        playerY = SCREEN_HEIGHT - 10;
        for(int i = 0; i < 10; i++){
        enemyX[i] = (i * 20) + 20;
        enemyY[i] =  20 ; 


        //experimenting with framerate
        
        
        }
        score = 0;
        gameOver = 0;
				GLCD_Clear(Black);
 

        while(input = get_button() != DOWN){

            //exp


				switch(get_button()) {
					case LEFT:
							if(playerX > 0){exp_plX =playerX ;playerX = playerX -15;}
							break;
					case RIGHT:
							if(playerX < SCREEN_WIDTH - 1){exp_plY ;playerX = playerX +15;}
							break;
					case UP:
							//shoot
                            laserCount++;
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


            //     //GLCD_ClearLn(0,1);
			// 	// SMARTCLEAN(enemyy[1]);
            //     GLCD_SetTextColor(Black);
            // for(int i=0;i<10;i++){
            //     if(enemyY[i] != -1){
                
            //         for(int x = -3; x <= 6; x++){
            //             for(int y = -3; y<= 6; y++ ){
            //                 GLCD_PutPixel(enemyX[i] + x,enemyY[i] + y);
            //             }
            //         }
                
            //     }
            // }
        
            if(movingRight == 1){

                for(int i = 0; i < 10; i++)
                {   
                    exp_enX[i] = enemyX[i];
                    exp_enY[i] = enemyY[i];
                    if(enemyY[i] > SCREEN_HEIGHT){
                        gameOver = 1;
                    }
                    if(enemyY[i] != -1){
                        enemyX[i] = enemyX[i] + 5;
                    }
                    if(enemyX[9] > SCREEN_WIDTH - 20){
                        enemyY[i] = enemyY[i] + 5;
                        movingRight = 0;
                         
                    }
                }
            }else{
                for(int i = 0; i < 10; i++)
                {   
                    exp_enX[i] = enemyX[i];
                    exp_enY[i] = enemyY[i];
                    if(enemyY[i] > SCREEN_HEIGHT){
                        gameOver = 1;
                    }
                    if(enemyY[i] != -1){
                        enemyX[i] = enemyX[i] - 5;
                    }
                    if(enemyX[0] < 25){
                        enemyY[i] = enemyY[i] + 5; 
                        movingRight = 1;
                    }
                }
            }
        }
        // Check for collisions
        for(int i = 0; i<10 ; i++){
            for(int j = 0; j<10; j++){
                // if(laserx[i] == enemyX[j] && lasery[i] == enemyY[j]){
                if((laserx[i] - enemyX[j])*(laserx[i] - enemyX[j]) < 25 && lasery[i] == enemyY[j] ){     //square difference so its positive
                    score++;
                    enemyY[j] = -1;
                    lasery[i] = -1;
                }
            }
        }

        //update display
        //GLCD_Clear(Black);
        //GLCD_SetTextColour(Green);

                //GLCD_ClearLn(0,1);
				// SMARTCLEAN(enemyy[1]);
                
        if(enemyTimer == 0){
            for(int i=0;i<10;i++){
                if(enemyY[i] != -1){
                
                    for(int x = -3; x <= 6; x++){
                        for(int y = -3; y<= 6; y++ ){

                            GLCD_SetTextColor(Black);
                            GLCD_PutPixel(exp_enX[i] + x,exp_enY[i] + y);
                            GLCD_SetTextColor(Green);
                            GLCD_PutPixel(enemyX[i] + x,enemyY[i] + y);
                        }
                    }
                    

                }
            }

        }
        //SMARTCLEAN(playerY);
        for(int x = -10; x<= 10; x++){
            for(int y = 0; y <= 10 ; y++){
                GLCD_SetTextColor(Black);
                GLCD_PutPixel(exp_plX + x, exp_plY + y);
                GLCD_SetTextColor(White);
                GLCD_PutPixel(playerX + x, playerY + y);
            }
        }

        // render bullets
        for(int numb = 0; numb < 10; numb++){
            if(lasery[numb] != -1){
                for(int x = -2; x <= 2; x++){
                    for(int y = -2; y<=0; y++){
                        GLCD_PutPixel(laserx[numb] + x, lasery[numb] + y);
                    }
                        GLCD_PutPixel(laserx[numb] + x, lasery[numb] +2);
                }
            }
        }


            // clear old 


        }

        GLCD_Clear(White);
        osSignalSet(inputControllerID, 0x01);
        osSemaphoreRelease(running_semaphoreID);
    }
}

unsigned char spaceInvaderBitmap[10 * 10 * 2] = {
    // Row 1
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    // Row 2
    0x00, 0x00, 0x00, 0x00, 0x07, 0xE0, 0x07, 0xE0, 0x00, 0x00,
    0x00, 0x00, 0x07, 0xE0, 0x07, 0xE0, 0x00, 0x00, 0x00, 0x00,
    // Row 3
    0x00, 0x00, 0x07, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x07, 0xE0,
    0x07, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x07, 0xE0, 0x00, 0x00,
    // Row 4
    0x00, 0x00, 0x07, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xE0, 0x00, 0x00,
    // Row 5
    0x00, 0x00, 0x07, 0xE0, 0x07, 0xE0, 0x07, 0xE0, 0x07, 0xE0,
    0x07, 0xE0, 0x07, 0xE0, 0x07, 0xE0, 0x07, 0xE0, 0x00, 0x00,
    // Row 6
    0x00, 0x00, 0x07, 0xE0, 0x00, 0x00, 0x07, 0xE0, 0x07, 0xE0,
    0x07, 0xE0, 0x00, 0x00, 0x07, 0xE0, 0x00, 0x00, 0x00, 0x00,
    // Row 7
    0x00, 0x00, 0x00, 0x00, 0x07, 0xE0, 0x00, 0x00, 0x00, 0x00,
    0x07, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    // Row 8
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    // Row 9
    0x00, 0x00, 0x00, 0x00, 0x07, 0xE0, 0x00, 0x00, 0x00, 0x00,
    0x07, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    // Row 10
    0x00, 0x00, 0x00, 0x00, 0x07, 0xE0, 0x00, 0x00, 0x00, 0x00,
    0x07, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// all green 10 x 10 rgb565 bitmap