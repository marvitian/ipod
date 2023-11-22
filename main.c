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





//define main menu options
typedef enum{
        PHOTO_GALLERY,
        OPTION_2,
        OPTION_3,
        OPTION_4,
        MENU_SIZE
} MENU_Item;
MENU_Item current_item = PHOTO_GALLERY;
// #########################################################
// ####       PROGRAM START                             ####     
// #########################################################




// prototype functions
void mainMenuController(void const *argument);
void photoGalleryController(void const *argument);
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
osThreadId mainMenuControllerID;
osThreadId photoGalleryControllerID;
osThreadDef(mainMenuController,osPriorityNormal, 1, 0);
osThreadDef(photoGalleryController,osPriorityNormal, 1, 0);







// #########################################################
// #########################################################
// ####                                                  ###
// ####               main function                      ###
// ####                                                  ###
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
        MAIN_MENU(1);       //instantiate main menu
    #endif
    
    // #########################################################
    // #                    thread creation                    #
    // #########################################################
    mainMenuControllerID = osThreadCreate(osThread(mainMenuController),NULL);
    photoGalleryControllerID = osThreadCreate(osThread(photoGalleryController),NULL);
    osKernelInitialize();
    osKernelStart();
	
}
    
	




// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
// #                                                       &
// #                    Threads                            &
// #                                                       &
// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

//not currently used: 
int InputBuffer[100];
int InputBufferIndex = 0;

// input thread
void mainMenuController(void const *argument){
    while(1)
    {
        //osMutexWait(running_mutexID, osWaitForever);
        osSemaphoreWait(running_semaphoreID, osWaitForever);
        #ifdef __use_LCD
            int input = get_button();
            
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

                                // release running semaphore -> signal to option -> wait for return to main menu,
                                osSemaphoreRelease(running_semaphoreID);
                                osSignalSet(photoGalleryControllerID,0x01);
                                osSignalWait(0x01, osWaitForever);

                                break;
                            case OPTION_2:
                                // run option 2
                                
                                break;
                            case OPTION_3:
                                // run option 3
                                
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


                // display handler for main menu
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
                    case OPTION_3:
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
			
            // input handler for photo gallery
            int input = get_button();
            if(input != REST)
            {
                switch(input)
                {
                    case UP:
                        // do nothing
                        break;
                    case DOWN:
                        // do nothing
                        break;
                    case SELECT:
                        // do nothing
                        break;
                    default:
                    // do nothing
                        break;
                }   
            }

            
            // GLCD_Bitmap arguments description:
            // x: x coordinate of top left corner of image
            // y: y coordinate of top left corner of image
            // w: width of image
            // h: height of image
            // bitmap: pointer to image data
            GLCD_Bitmap(0, 0, 240, 240, GIMP_IMAGE_PIXEL_DATA);
            while(get_button() != SELECT)
            {
                // wait for select button to be pressed to return to main menu
                // hopefully it maintains it state ? 
            
            }
            osSignalSet(mainMenuControllerID, 0x01);
            osSemaphoreRelease(running_semaphoreID);
            
        #endif
    }
}
