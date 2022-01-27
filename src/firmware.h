////
// Constants
////

#pragma region Constants

const uint8_t MAX_LAYERS = 10;
const uint8_t ROWS = 3;
const uint8_t COLS = 4;

const uint8_t PIN_ANALOG = A1;

const uint8_t PIN_LED_DATA = A2;
const uint8_t LED_COUNT = 13;

#pragma endregion


unsigned long g_time;
void updateGlobalTime(unsigned long milliseconds);





void onPhysicalKeyPress(KeypadEvent key);



/*
 * Init functions
 */

void InitSerial();
void InitFastLED();
void InitKeyboards();



void firmware_init();
void firmware_loop();