#pragma once

#include <stdint.h>
#include <stdio.h>
#include <functional>
#define COLORBASE_DEBUG_PRINT 1
namespace colorBase {

/**
 * @param RGB red green blue
 * @param HSV hue saturation value(brightness)
 * @param CW cold warm
 * @param CCTB CorrelatedColorTemperature Brightness
 */
enum class colorType {
    RGB,
    HSV,
    CW,
    CCTB
}; /* enum class colorType */


template <colorType>
struct Color;

template <>
struct Color<colorType::RGB> {
    struct _RGB {
        /* 0-255 */
        uint8_t r;
        /* 0-255 */
        uint8_t g;
        /* 0-255 */
        uint8_t b;
    } __attribute__((aligned(sizeof(void *))));

    _RGB val;

    Color(uint8_t r, uint8_t g, uint8_t b) : val{r, g, b} {}
    Color() : val{0, 0, 0} {}

};

template <>
struct Color<colorType::HSV> {
    struct _HSV {
        /* 0-359 */
        uint16_t h;
        /* 0-100 */
        uint8_t s;
        /* 0-100 */
        uint8_t v;
    } __attribute__((aligned(sizeof(void *))));

    _HSV val;

    Color(uint16_t h, uint8_t s, uint8_t v) : val{h, s, v} {}
    Color() : val{0, 0, 0} {}

};

template <>
struct Color<colorType::CW> {
    struct _CW {
        /* 0-255 */
        uint16_t c;
        /* 0-255 */
        uint16_t w;
    } __attribute__((aligned(sizeof(void *))));

    _CW val;

    Color(uint16_t c, uint16_t w) : val{c, w} {}
    Color() : val{0, 0} {}

};

template <>
struct Color<colorType::CCTB> {
    struct _CCTB {
        /* 0-100 */
        uint16_t cct;
        /* 0-100 */
        uint16_t b;
    } __attribute__((aligned(sizeof(void *))));

    _CCTB val;

    Color(uint16_t cct, uint16_t b) : val{cct, b} {}
    Color() : val{0, 0} {}

}; /* struct Color */



enum class colorEffectMode {
    NORMAL,
    COLORLOOP,
    BRIGHTNESSLOOP,
    RANDOM,
    LINEAR,
}; /* enum class colorEffectMode */

template <colorType T>
struct colorEffectVal {
    

    uint16_t linearStep;
    uint8_t loopDirection;
    uint16_t loopMs;
    Color<T> target;
    

    colorEffectVal() : linearStep(0) {}
}; /* struct colorEffectVal */



template <colorType T>
using hookCallback = std::function<void(Color<T>&)>;

template <colorType T>
struct colorMgr {
    Color<T> color;
    /* implement your actual light device's color setting callback */
    hookCallback<T> cbImpl;
    colorEffectVal<T> effectVal;

    colorMgr() : cbImpl(NULL) {};
}; /* struct colorMgr */


namespace effects {
void convert(const Color<colorType::RGB> &src, Color<colorType::HSV> &tar);
void convert(const Color<colorType::HSV> &src,  Color<colorType::RGB> &tar);
void convert(const Color<colorType::CCTB> &src, Color<colorType::CW> &tar);
void convert(const Color<colorType::CW> &src, Color<colorType::CCTB> &tar);

} /* effects */


#include <TimerEvents/TimerEvents.h>
class colorBaseMgr {

public:
    colorBaseMgr(): colorMode(colorType::RGB) {}
    colorBaseMgr(colorType mode): colorMode(mode) {}

    
    
    void setColor(Color<colorType::RGB> &src)
    {
        this->colorMode = colorType::RGB;
        this->rgb.color = src;
#if COLORBASE_DEBUG_PRINT
        printf("set color: %d %d %d\n", this->rgb.color.val.r, this->rgb.color.val.g, this->rgb.color.val.b);
#endif
        if(this->rgb.cbImpl)
            this->rgb.cbImpl(src);
    }
    void setColor(Color<colorType::HSV> &src)
    {
        this->colorMode = colorType::HSV;
        this->hsv.color = src;
#if COLORBASE_DEBUG_PRINT
        printf("set color: %d %d %d\n", this->hsv.color.val.h, this->hsv.color.val.s, this->hsv.color.val.v);
#endif
        if(this->hsv.cbImpl)
            this->hsv.cbImpl(src);
    }
    void setColor(Color<colorType::CCTB> &src)
    {
        this->colorMode = colorType::CCTB;
        this->cctb.color = src;
#if COLORBASE_DEBUG_PRINT
        printf("set color: %d %d %d\n", this->cctb.color.val.cct, this->cctb.color.val.b);
#endif
        if(this->cctb.cbImpl)
            this->cctb.cbImpl(src);
    }
    void setColor(Color<colorType::CW> &src)
    {
        this->colorMode = colorType::CW;
        this->cw.color = src;
#if COLORBASE_DEBUG_PRINT
        printf("set color: %d %d %d\n", this->cw.color.val.c, this->cw.color.val.w);
#endif
        if(this->cw.cbImpl)
            this->cw.cbImpl(src);
    }

    void setColorCallback(hookCallback<colorType::RGB> cb)
    {
        this->rgb.cbImpl = cb;
    }
    void setColorCallback(hookCallback<colorType::HSV> cb)
    {
        this->hsv.cbImpl = cb;
    }
    void setColorCallback(hookCallback<colorType::CCTB> cb)
    {
        this->cctb.cbImpl = cb;
    }
    void setColorCallback(hookCallback<colorType::CW> cb)
    {
        this->cw.cbImpl = cb;
    }

    template <colorType>
    static void colorLoopCallback(void *arg);
    
    template <colorType>
    void startColorLoop(uint16_t loopMs);
    
private:
    colorType colorMode;
    colorEffectMode effectMode;
    colorMgr<colorType::RGB> rgb;
    colorMgr<colorType::HSV> hsv;
    colorMgr<colorType::CCTB> cctb;
    colorMgr<colorType::CW> cw;
    
    TimerEvents::TimerEvents timer;


}; /* class colorBaseMgr */





} /* colorBase */


