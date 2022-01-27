void InitSerial()
{
    Serial.begin(9600);
    Serial.println("Bonjour!");
}

void InitKeyboards()
{
    Consumer.begin();
    BootKeyboard.begin();
    delay(1000);
}

/*
 * FastLED
 */

// Instead of keeping various state data in the global namespace, we will use a 
//   data struct to initialize once and pass around in the classic C style!
struct FastLEDSetupData_t {
    uint8_t PinData,
    const uint8_t LedCount,
    CRGB Leds[Led_Count],
};

void InitFastLED(struct FastLEDSetupData_t setup_data)
{
  FastLED.addLeds<SK6812, setup_data.PinData, GRB>(setup_data.Leds, setup_data.LedCount);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
  FastLED.setBrightness(200);
  FastLED.clear();
}

////
// Firmware functions
////
void firmware_init()
{
    InitSerial();
    InitKeyboards();

    FastLEDSetupData_t fastLEDData = {
        PinData = PIN_LED_DATA,
        LedCount = LED_COUNT,
        Leds = leds
    };
    InitFastLED(fastLEDData);

    firmware_loop();
}

void firmware_loop()
{
}