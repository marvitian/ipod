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





// #########################################################
// ####       PROGRAM START                             ####     
// #########################################################

//define main menu options
typedef enum{
        OPTION_1,
        OPTION_2,
        OPTION_3,
        OPTION_4,
        MENU_SIZE
} MENU_Item;

MENU_Item current_item = OPTION_1;
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


// prototype functions
void inputController(void const *argument);
void menuController();

// mutex
osMutexDef(lcd_mutex);
osMutexId(lcd_mutexID);

osThreadId inputControllerID;
osThreadDef(inputController,osPriorityNormal, 1, 0);

int main (void) {
    // lcd mutex not in use*
	lcd_mutexID = osMutexCreate(osMutex(lcd_mutex));
	
	
	    
    #ifdef __use_LCD
        KBD_Init();			/* KBD initialization */
        GLCD_Init();                               /* Initialize graphical LCD (if enabled */
        GLCD_Clear(White);                         /* Clear graphical LCD display   */
        GLCD_SetBackColor(Blue);
        GLCD_SetTextColor(Yellow);
        GLCD_DisplayString(0, 0, __FI, "  media centre ");
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

//not currently used: 
int InputBuffer[100];
int InputBufferIndex = 0;




// input thread
void inputController(void const *argument){
    while(1)
    {
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
                        current_item = (current_item - 1) % MENU_SIZE;
                        break;
                    case DOWN:
                        current_item = (current_item + 1) % MENU_SIZE;
                        break;
                    case SELECT:
                        switch(current_item)
                        {
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
                        break;
                    default:
                    // do nothing
                        break;
                }   
                switch(current_item)
                {
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
            //not currently used:
            InputBuffer[InputBufferIndex] = input;
            InputBufferIndex = (InputBufferIndex + 1) % 100;
        #endif
    }
}