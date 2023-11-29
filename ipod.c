/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module

#include "cube.h"
#include "menu.c"
#include <stdlib.h>
#include "cmsis_os.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "GLCD.h"
#include "LPC17xx.h"
#include "KBD.h"

#define __use_LCD 1
#define __FI 1

#define UP 0x08
#define DOWN 0x20
#define LEFT 0x40
#define RIGHT 0x10
#define SELECT 0x01
#define REST 0x00

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define ENEMY_TIMER 1
#define LASER_TIMER 1
#define RGB565(r, g, b) (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3))

// uint16_t color = RGB565(255, 0, 0); // Red color



#define SMARTCLEAN(y) do {\
    for(int line_to_clean = -10; line_to_clean <= 20 ; line_to_clean++){\
        for(int x = 0; x< SCREEN_WIDTH; x++){\
            GLCD_SetTextColor(Black);\
            GLCD_PutPixel(x, line_to_clean);\
        }\
    }\
} while(0)




//define main menu options
typedef enum{
        PHOTO_GALLERY,
        OPTION_2,
        GAMES,
        OPTION_4,
        MENU_SIZE
} MENU_Item;
MENU_Item current_item = PHOTO_GALLERY;
// #########################################################
// ####       PROGRAM START                             ####     
// #########################################################




// prototype functions
void inputController(void const *argument);
void photoGalleryController(void const *argument);
void gameLibraryController(void const *argument);
void menuController();

// mutex
	// #########################################################
	// #                  semaphore creation                   #
	// #########################################################
	osSemaphoreDef(running_semaphore);
	osSemaphoreId(running_semaphoreID);
	



// #########################################################
// #                    thread handlers                    #
// #########################################################
osThreadId gameLibraryControllerID;
osThreadId inputControllerID;
osThreadId photoGalleryControllerID;
osThreadDef(inputController,osPriorityNormal, 1, 0);
osThreadDef(photoGalleryController,osPriorityNormal, 1, 0);
osThreadDef(gameLibraryController,osPriorityNormal, 1, 0);






// #########################################################
// #########################################################
// ####               main function                      ###
// #########################################################
// #########################################################

int main (void) {


    // #########################################################
    // #                    mutex creation                     #
    // #########################################################
	osMutexDef(lcd_mutex);
    osMutexId(lcd_mutexID);
    lcd_mutexID = osMutexCreate(osMutex(lcd_mutex));
    
	osMutexDef(running_mutex);
    osMutexId(running_mutexID);
    running_mutexID = osMutexCreate(osMutex(running_mutex));
    
	
	running_semaphoreID = osSemaphoreCreate(osSemaphore(running_semaphore), 1);
    


    // #########################################################
    // #                    initialization                     #
    // #########################################################
    #ifdef __use_LCD
        
        KBD_Init();			/* KBD initialization */
        GLCD_Init();        /* Initialize graphical LCD (if enabled */
				GLCD_Clear(White);
        MAIN_MENU(1);       //instantiate main menu
    #endif
    
    // #########################################################
    // #                    thread creation                    #
    // #########################################################
    inputControllerID = osThreadCreate(osThread(inputController),NULL);
    photoGalleryControllerID = osThreadCreate(osThread(photoGalleryController),NULL);
    gameLibraryControllerID = osThreadCreate(osThread(gameLibraryController),NULL);
    osKernelInitialize();
    osKernelStart();
	
    
	
}



    // ##################
    // testing region 
    // ##################

//not currently used: 
int InputBuffer[100];
int InputBufferIndex = 0;




// input thread
void inputController(void const *argument){
	while(0){
		GLCD_Bitmap(0, 0, 240, 240, GIMP_IMAGE_PIXEL_DATA);
	}
	
		while(1)
    {
        //osMutexWait(running_mutexID, osWaitForever);
        osSemaphoreWait(running_semaphoreID, osWaitForever);
        #ifdef __use_LCD
            
            int input = get_button();
            char text[10];
            sprintf(text,"%x", input);
            //printf("%x, %d", input, input);
            GLCD_DisplayString(8, 0, __FI, (unsigned char *) text);

            if(input != REST)
            {
                switch(input)
                {
                    case UP:
												if(current_item == 0){
													current_item = 4;
													DELAY;
												}else{
                        current_item = (current_item - 1) % MENU_SIZE;
													DELAY;
												}
                        break;
                    case DOWN:
                        current_item = (current_item + 1) % MENU_SIZE;
												DELAY;
                        break;
                    case SELECT:
                        switch(current_item)
                        {
                            case PHOTO_GALLERY:
                                // run option 1
                                //osMutexRelease(running_mutexID);
                                osSemaphoreRelease(running_semaphoreID);
                                osSignalSet(photoGalleryControllerID,0x01);
                                osSignalWait(0x01, osWaitForever);
                                break;
                            case OPTION_2:
                                // run option 2
                                
                                break;
                            case GAMES:
                                // run option 3
                                osSemaphoreRelease(running_semaphoreID);
                                osSignalSet(gameLibraryControllerID,0x01);
                                osSignalWait(0x01, osWaitForever);
                                break;
                            case OPTION_4:
                                // run option 4
                                
                                break;
                            default:
                                // do nothing
                                break;
                        }
                        break;
                    default:
                    // do nothing
                        break;
                }   
                switch(current_item)
                {
                    case PHOTO_GALLERY:
                            // run option 1
                            MAIN_MENU(1);
                            
                            break;
                    case OPTION_2:
                            // run option 2
                            MAIN_MENU(2);
                            
                            break;
                    case GAMES:
                            // run option 3
                            MAIN_MENU(3);
                            
                            break;
                    case OPTION_4:
                            // run option 4
                            MAIN_MENU(4);
                            
                            break;
                    default:
                            // do nothing
                            break;
                }

            }
            //not currently used:
            InputBuffer[InputBufferIndex] = input;
            InputBufferIndex = (InputBufferIndex + 1) % 100;
        #endif
        #ifndef __use_LCD
        //osMutexRelease(running_mutexID);
        #endif
        osSemaphoreRelease(running_semaphoreID);
    }
}

// photo gallery controller
void photoGalleryController(void const *argument){
    while(1)
    {
				osSignalWait(0x01, osWaitForever);
				
			osSemaphoreWait(running_semaphoreID, osWaitForever);
			
        #ifdef __use_LCD
            
            // GLCD_Bitmap arguments description:
            // x: x coordinate of top left corner of image
            // y: y coordinate of top left corner of image
            // w: width of image
            // h: height of image
            // bitmap: pointer to image data
            GLCD_Bitmap(0, 0, 240, 240, GIMP_IMAGE_PIXEL_DATA);
            while(get_button() != LEFT)
            {
                // wait for select button to be pressed to return to main menu
            }
            GLCD_Clear(White);
            osSignalSet(inputControllerID, 0x01);
            osSemaphoreRelease(running_semaphoreID);
            
        #endif
    }
}

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

        // unsigned char space_bitmap[320][240];
        // #define WIDTH 128  // Example width of GLCD
        // #define HEIGHT 64  // Example height of GLCD

        uint16_t bitmap[SCREEN_HEIGHT][SCREEN_WIDTH]; // Each byte represents 8 pixels


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
                    if(enemyX[9] > SCREEN_WIDTH - 30){
                        enemyY[i] = enemyY[i] + 5;
                        movingRight = 0;
                         
                    }
                    bitmap[enemyY[i]][enemyX[i]] = RGB565(19,157,8); // Green
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
                    if(enemyX[0] < 40){
                        enemyY[i] = enemyY[i] + 5; 
                        movingRight = 1;
                    }
                    bitmap[enemyY[i]][enemyX[i]] = RGB565(19,157,8); // Green
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
        
        // create bitmap where player is white, enemy is green, laser is red
        
        // RGB(19,157,8) green enemy
        
        GLCD_Bitmap(0, 0, 320, 240, bitmap);

        }

        GLCD_Clear(White);
        osSignalSet(inputControllerID, 0x01);
        osSemaphoreRelease(running_semaphoreID);
    }
}

