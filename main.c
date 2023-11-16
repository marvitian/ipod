/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module

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
void inputController(void const *argument);
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
osThreadId inputControllerID;
osThreadId photoGalleryControllerID;
osThreadDef(inputController,osPriorityNormal, 1, 0);
osThreadDef(photoGalleryController,osPriorityNormal, 1, 0);







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
    
	
	running_semaphoreID = osSemaphoreCreate(osSemaphore(running_semaphore), 0);
    


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
    inputControllerID = osThreadCreate(osThread(inputController),NULL);
    photoGalleryControllerID = osThreadCreate(osThread(photoGalleryController),NULL);
    osKernelInitialize();
    osKernelStart();
	
    
    while(1){
    // do nothing  
    }
		
}



    // ##################
    // testing region 
    // ##################

//not currently used: 
int InputBuffer[100];
int InputBufferIndex = 0;




// input thread
void inputController(void const *argument){
    while(1)
    {
        //osMutexWait(running_mutexID, osWaitForever);
        osSemaphoreWait(running_semaphoreID, osWaitForever);
        #ifdef __use_LCD
            osSignalWait(0x02, osWaitForever);
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
                        current_item = (current_item - 1) % MENU_SIZE;
                        break;
                    case DOWN:
                        current_item = (current_item + 1) % MENU_SIZE;
                        break;
                    case SELECT:
                        switch(current_item)
                        {
                            case PHOTO_GALLERY:
                                // run option 1
                                //osMutexRelease(running_mutexID);
                                osSemaphoreRelease(running_semaphoreID);
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
        #ifndef __use_LCD
        //osMutexRelease(running_mutexID);
        #endif
    }
}

// photo gallery controller
void photoGalleryController(void const *argument){
    while(1)
    {
        osSemaphoreWait(running_semaphoreID, osWaitForever);
        #ifdef __use_LCD
            
            // GLCD_Bitmap arguments description:
            // x: x coordinate of top left corner of image
            // y: y coordinate of top left corner of image
            // w: width of image
            // h: height of image
            // bitmap: pointer to image data
            GLCD_Bitmap(0, 0, 320, 240, (unsigned char *) photoGallery);
            while(get_button() != SELECT)
            {
                // wait for select button to be pressed to return to main menu
            }
            osSemaphoreRelease(running_semaphoreID);
            
        #endif
    }
}
