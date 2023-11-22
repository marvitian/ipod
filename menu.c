
// #########################################################
// #########################################################
// #########################################################
#define MAIN_MENU(OPTION) do { \
    GLCD_Clear(White); \
    GLCD_SetBackColor(Blue); \
    GLCD_SetTextColor(Yellow); \
    GLCD_DisplayString(0, 0, __FI, "  media centre "); \
    
    GLCD_SetBackColor(White); \
    GLCD_SetTextColor(Black); \
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

// #########################################################
// #########################################################
// #########################################################
