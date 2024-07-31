#pragma once

#include <stdint.h>
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
};
/* enum class colorType */

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

};
/* struct Color */


enum class colorEffectMode {
    NORMAL,
    COLORLOOP,
    BRIGHTNESSLOOP,
    RANDOM,
    LINEAR,
};

template <colorType T>
struct colorEffectVal {
    uint16_t linearStep;
    Color<T> target;
    colorEffectMode mode;
    colorEffectVal() : linearStep(0), mode(colorEffectMode::NORMAL) {}
};
/* struct colorEffectVal */

template <colorType T>
struct colorMgr {
    Color<T> color;
    colorEffectVal<T> effectVal;

};
/* struct colorMgr */

namespace effects {
void convert(const Color<colorType::RGB> &src, Color<colorType::HSV> &tar);
void convert(const Color<colorType::HSV> &src,  Color<colorType::RGB> &tar);
void convert(const Color<colorType::CCTB> &src, Color<colorType::CW> &tar);
void convert(const Color<colorType::CW> &src, Color<colorType::CCTB> &tar);

} /* effects */



class colorBaseMgr {

public:
    colorBaseMgr(): colorMode(colorType::RGB) {}
    colorBaseMgr(colorType mode): colorMode(mode) {}

    
    void setColor(Color<colorType::RGB> &src)
    {
        this->rgb.color = src;
    }
    void setColor(Color<colorType::HSV> &src)
    {
        this->hsv.color = src;
    }
    void setColor(Color<colorType::CCTB> &src)
    {
        this->cctb.color = src;
    }
    void setColor(Color<colorType::CW> &src)
    {
        this->cw.color = src;
    }

private:
    colorType colorMode;
    colorMgr<colorType::RGB> rgb;
    colorMgr<colorType::HSV> hsv;
    colorMgr<colorType::CCTB> cctb;
    colorMgr<colorType::CW> cw;

};
/* class colorBaseMgr */




} /* colorBase */


