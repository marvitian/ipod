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
//#define osObjectsPublic                     // define objects in main module
//#include "osObjects.h"                      // RTOS object definitions
#ifdef led
#include "LED.h"
#endif
#include "LPC17xx.h"
#include "KBD.h"

#define __use_LCD 1
#define __FI 1

// define joystick input up=0008 down=0020 left=0040 right=0010 select=0001
#define UP 0x08
#define DOWN 0x20
#define LEFT 0x40
#define RIGHT 0x10
#define SELECT 0x01
#define REST 0x00

    //define main menu options
    typedef enum{
        OPTION_1,
        OPTION_2,
        OPTION_3,
        OPTION_4,
        MENU_SIZE
    } MENU_Item;

		MENU_Item current_item = OPTION_1;
// prototype functions
void inputController(void const *argument);
void menuController();
#define MAIN_MENU(OPTION) do { \
    GLCD_SetBackColor(White); \
    GLCD_SetTextColor(Black); \
    GLCD_DisplayString(0, 0, __FI, "  media centre "); \
    GLCD_DisplayString(1, 0, __FI, "1. OPTION 1"); \
    GLCD_DisplayString(2, 0, __FI, "2. OPTION 2"); \
    GLCD_DisplayString(3, 0, __FI, "3. OPTION 3"); \
    GLCD_DisplayString(4, 0, __FI, "4. OPTION 4"); \
    switch(OPTION){ \
        case 1: \
            GLCD_SetBackColor(Blue); \
            GLCD_SetTextColor(White); \
            GLCD_DisplayString(1, 0, __FI, "1. OPTION 1"); \
            GLCD_SetBackColor(White); \
            GLCD_SetTextColor(Black); \
            break; \
        case 2: \
            GLCD_SetBackColor(Blue); \
            GLCD_SetTextColor(White); \
            GLCD_DisplayString(2, 0, __FI, "2. OPTION 2"); \
            GLCD_SetBackColor(White); \
            GLCD_SetTextColor(Black); \
            break; \
        case 3: \
            GLCD_SetBackColor(Blue); \
            GLCD_SetTextColor(White); \
            GLCD_DisplayString(3, 0, __FI, "3. OPTION 3"); \
            GLCD_SetBackColor(White); \
            GLCD_SetTextColor(Black); \
            break; \
        case 4: \
            GLCD_SetBackColor(Blue); \
            GLCD_SetTextColor(White); \
            GLCD_DisplayString(4, 0, __FI, "4. OPTION 4"); \
            GLCD_SetBackColor(White); \
            GLCD_SetTextColor(Black); \
            break; \
        default: \
            break; \
    } \
} while (0)


osMutexDef(lcd_mutex);
osMutexId(lcd_mutexID);
/*
        #ifdef __use_LCD
						#ifdef led
            LED_Out(8);
						#endif
            sprintf(text, "task d status: %f", result);
            osMutexWait(lcd_mutexID, osWaitForever);
            GLCD_SetTextColor(Black);    
            GLCD_DisplayString(3, 0, __FI, "");
            GLCD_DisplayString(8, 0, __FI, (unsigned char *) text);
            osMutexRelease(lcd_mutexID);
        #endif

*/

osThreadId inputControllerID;
osThreadDef(inputController,osPriorityNormal, 1, 0);
/*
 * main: initialize and start the system
 */
int main (void) {
	
	int in;
	  KBD_Init();			/* KBD initialization */
	// prepare lcd mutex
	
	    lcd_mutexID = osMutexCreate(osMutex(lcd_mutex));
	
	    // Prepare LCD
    #ifdef __use_LCD
        GLCD_Init();                               /* Initialize graphical LCD (if enabled */
        #ifdef led
				LED_Init();
				#endif
        GLCD_Clear(White);                         /* Clear graphical LCD display   */
        GLCD_SetBackColor(Blue);
        GLCD_SetTextColor(Yellow);
        GLCD_DisplayString(0, 0, __FI, "  media centre ");
        //	GLCD_SetTextColor(White);
        //	GLCD_DisplayString(1, 0, __FI, "Lab Demo #4");
        GLCD_SetTextColor(White);
        MAIN_MENU(1); //instantiate main menu
    
    #endif
		
		
		inputControllerID = osThreadCreate(osThread(inputController),NULL);
		osKernelInitialize();
		osKernelStart();
	while(1){
      
    }
		
}



    // ##################
    // testing region 
    // ##################
    int InputBuffer[100];
    int InputBufferIndex = 0;


//threads


// input thread
    void inputController(void const *argument){
			while(1){
        int input = get_button();
				char text[10];
				sprintf(text,"%x", input);
				//printf("%x, %d", input, input);
				GLCD_DisplayString(8, 0, __FI, (unsigned char *) text);

				if(input != REST){
             // display menu
            // check input
            // if input is select, then run the current menu item
            // if input is up, then move up the menu
            // if input is down, then move down the menu
            // if input is left, then move left the menu
            // if input is right, then move right the menu
            
            switch(input){
                case UP:
                    current_item = (current_item - 1) % MENU_SIZE;
                    break;
                case DOWN:
                    current_item = (current_item + 1) % MENU_SIZE;
                    break;
                case SELECT:
                    switch(current_item){
                        case OPTION_1:
                            // run option 1
                            MAIN_MENU(1);
														osDelay(10);
                            break;
                        case OPTION_2:
                            // run option 2
                            MAIN_MENU(2);
														osDelay(10);
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
                    break;
                default:
                    // do nothing
                    break;
            }
						switch(current_item){
								case OPTION_1:
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
        InputBuffer[InputBufferIndex] = input;
        InputBufferIndex = (InputBufferIndex + 1) % 100;
			}
    }

    // control state thread
    void controlStateController(){
        // read input buffer
        // update menu state    
        
    }





//functions 

    /*

    // might have to change the type of input
    int menuController(int input){
        MENU_Item current_item = OPTION_1;
        
            // display menu
            // check input
            // if input is select, then run the current menu item
            // if input is up, then move up the menu
            // if input is down, then move down the menu
            // if input is left, then move left the menu
            // if input is right, then move right the menu
           // int input = input;
            switch(input){
                case UP:
                    current_item = (current_item + 1) % MENU_SIZE;
                    break;
                case DOWN:
                    current_item = (current_item - 1) % MENU_SIZE;
                    break;
                case SELECT:
                    switch(current_item){
                        case OPTION_1:
                            // run option 1
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
        
    }
    
		*/

	// testing region
    /*
		checkInput(){
            // figure out handling joystick input
			char text[10];
			int dir = KBD_get();
			int dir2 = get_button();
			sprintf(text,"0x%04X", dir2);
            #ifdef __use_LCD
                osMutexWait(lcd_mutexID, osWaitForever);
			    GLCD_DisplayString(4, 0, __FI, (unsigned char *) text);
                osMutexRelease(lcd_mutexID);
            #endif

		}
    */
