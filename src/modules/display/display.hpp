#ifndef __DISPLAY_H__
#define __DISPLAY_H__


namespace Display
{
    void Init();
    void Update(unsigned long time);
    void _invert();
    void DrawLayer(const int layer);
}

#endif // __DISPLAY_H__