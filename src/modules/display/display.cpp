#include "display.hpp"
#include <Timer.hpp>
#include <TimeKeeper.hpp>

#include <U8g2lib.h>


namespace Display
{
    #define OLED_RESET 4
    #define SCREEN_ADDRESS 0x3C
    U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);

    Timer invert_timer(1000);

    TimeKeeper tk;

    bool hasUpdate = false;
    bool m_invert = 0;


    void Init()
    {
        display.begin();
        display.clear();
        display.setFont(u8g2_font_logisoso16_tr);
        display.setContrast(0);

        display.firstPage();
        do {
            display.setCursor(12, 16);
            display.println("Bonjour!");
        } while (display.nextPage());
    }

    void Update(unsigned long time)
    {
        tk.tick(time);
        if (invert_timer.Elapsed(tk.getDeltaTime()))
            _invert();
    }

    void _invert()
    {
        if (m_invert)
        {
            display.sendF("c", 0x0a7);
        }
        else
        {
            display.sendF("c", 0x0a6);
        }

        m_invert = !m_invert;
    }


    void DrawLayer(const int layer)
    {
        display.firstPage();
        do {
            display.setCursor(12, 20);
            display.print("Layer: ");
            display.println(layer);
        } while (display.nextPage());
    }
}