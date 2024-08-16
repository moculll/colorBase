#pragma once

#include <stdint.h>
#include <stdio.h>
#include <functional>
#include <colorBase/colorBasePort.h>

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
    CCTB,
}; /* enum class colorType */

template <colorType T>
struct ColorBaseType {
    ColorBaseType<T> &operator=(const ColorBaseType<T> &src) {
        if(this != &src) {
            assign(src);
        }
        return *this;
    }
protected:
    virtual void assign(const ColorBaseType<T> &src) = 0;
}; /* struct ColorBaseType */

template <colorType>
struct Color;

template <>
struct Color<colorType::RGB> : public ColorBaseType<colorType::RGB> {
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
    
    bool operator==(const Color<colorType::RGB> &rgb) const
    {
        return this->val.r == rgb.val.r && this->val.g == rgb.val.g && this->val.b == rgb.val.g;
    }

    inline bool isOn() const
    {
        return (this->val.r || this->val.g || this->val.b);
    }

    inline Color<colorType::RGB> &setMax()
    {
        this->val.r = 255;
        this->val.g = 255;
        this->val.b = 255;
        return *this;
    }
protected:
    void assign(const ColorBaseType<colorType::RGB> &src) override
    {
        const Color<colorType::RGB>* pSrc = static_cast<const Color<colorType::RGB> *>(&src);
        val = pSrc->val;
    }
    
};

template <>
struct Color<colorType::HSV> : public ColorBaseType<colorType::HSV> {
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

    bool operator==(const Color<colorType::HSV> &hsv) const
    {
        return this->val.h == hsv.val.h && this->val.s == hsv.val.s && this->val.v == hsv.val.v;
    }

    inline bool isOn() const
    {
        return (this->val.s && this->val.v);
    }

    inline Color<colorType::HSV> &setMax()
    {
        this->val.h = 359;
        this->val.s = 100;
        this->val.v = 100;
        return *this;
    }
protected:
    inline void assign(const ColorBaseType<colorType::HSV> &src) override
    {
        const Color<colorType::HSV>* pSrc = static_cast<const Color<colorType::HSV> *>(&src);
        val = pSrc->val;
    }
    
};

template <>
struct Color<colorType::CCTB> : public ColorBaseType<colorType::CCTB> {
    struct _CCTB {
        /* 0-100 */
        uint16_t cct;
        /* 0-100 */
        uint16_t b;
    } __attribute__((aligned(sizeof(void *))));

    _CCTB val;

    Color(uint16_t cct, uint16_t b) : val{cct, b} {}
    Color() : val{0, 0} {}

    bool operator==(const Color<colorType::CCTB> &cctb) const
    {
        return this->val.cct == cctb.val.cct && this->val.b == cctb.val.b;
    }

    inline bool isOn() const
    {
        return (bool)this->val.b;
    }

    inline Color<colorType::CCTB> &setMax()
    {
        this->val.cct = 100;
        this->val.b = 100;
        return *this;
    }
protected:
    void assign(const ColorBaseType<colorType::CCTB>& src) override
    {
        const Color<colorType::CCTB>* pSrc = static_cast<const Color<colorType::CCTB> *>(&src);
        val = pSrc->val;
    }
    
}; 

template <>
struct Color<colorType::CW> : public ColorBaseType<colorType::CW> {
    struct _CW {
        /* 0-255 */
        uint16_t c;
        /* 0-255 */
        uint16_t w;
    } __attribute__((aligned(sizeof(void *))));

    _CW val;

    Color(uint16_t c, uint16_t w) : val{c, w} {}
    Color() : val{0, 0} {}

    bool operator==(const Color<colorType::CW> &cw) const 
    {
        return this->val.c == cw.val.c && this->val.w == cw.val.w;
    }
    
    inline bool isOn() const
    {
        return (this->val.c || this->val.w);
    }

    inline Color<colorType::CW> &setMax()
    {
        this->val.c = 255;
        this->val.w = 255;
        return *this;
    }
protected:
    void assign(const ColorBaseType<colorType::CW>& src) override
    {
        const Color<colorType::CW>* pSrc = static_cast<const Color<colorType::CW> *>(&src);
        val = pSrc->val;
    }
    
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
    uint32_t linearStep;
    uint32_t linearIntervalMs;
    uint32_t linearMs;
    
    uint32_t colorLoopMs;
    uint32_t brightnessLoopMs;
    uint8_t loopDirection;
    bool random;

    Color<T> target;
    
    colorEffectVal() : linearStep(0), linearIntervalMs(10), linearMs(1000), colorLoopMs(720), brightnessLoopMs(200), random(false) {}
}; /* struct colorEffectVal */



template <colorType T>
using hookCallback = std::function<void(const Color<T>&)>;

template <colorType T>
struct colorMgr {
    Color<T> color;
    Color<T> prevColor;
    /* implement your actual light device's color setting callback in colorBasePort.cpp */
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






class colorBaseMgr {
    friend class colorBasePortMgr;
public:
    colorBaseMgr(): colorMode(colorType::RGB) {}
    colorBaseMgr(colorType mode): colorMode(mode) {}

    void init()
    {
        portMgr.initImpl(*this);
        this->portMgr.timer.setObj((void *)this);
    }

    void init(uint8_t bitDepth)
    {
        portMgr.initImpl(*this, bitDepth);
        this->portMgr.timer.setObj((void *)this);
    }

    inline void setColorInternal(Color<colorType::RGB> &src)
    {
        this->rgb.color = src;
        this->rgb.cbImpl(src);
    }
    inline void setColorInternal(Color<colorType::HSV> &src)
    {
        this->hsv.color = src;
        this->hsv.cbImpl(src);
    }
    inline void setColorInternal(Color<colorType::CCTB> &src)
    {
        this->cctb.color = src;
        this->cctb.cbImpl(src);
    }
    inline void setColorInternal(Color<colorType::CW> &src)
    {
        this->cw.color = src;
        this->cw.cbImpl(src);
    }


    void setOnoff(bool onoff);
    void setOnoffLinear(bool onoff);

    template <colorType T>
    void setColor(const Color<T> &tar);

    template <colorType T>
    void getColor(Color<T> &dst);

    inline colorEffectMode getCurrentEffectMode() { return this->effectMode; }

    void setBrightness(uint8_t brightness);
    void setBrightnessLinear(uint8_t brightness);

    /* color linear */
    void setColorLinear(const Color<colorType::RGB> &tar);
    void setColorLinear(const Color<colorType::HSV> &tar);
    void setColorLinear(const Color<colorType::CCTB> &tar);
    void setColorLinear(const Color<colorType::CW> &tar);

    template <colorType>
    static void colorLinearCallback(void *arg);


    /* color loop */
    template <colorType>
    static void colorLoopCallback(void *arg);
    template <colorType>
    void setColorLoop(uint32_t loopMs, bool random);

    template <colorType>
    static void brightnessLoopCallback(void *arg);
    template <colorType>
    void setBrightnessLoop(uint32_t loopMs, bool random);
    
    inline std::map<int, int> *getGammaMap() { return &this->gammaTable; };

    static inline int getRandom(int min, int max) { return colorBase::colorBasePortMgr::getRandom(min, max); }
    
    void debugPrintMgrAllInfos();
private:
    
    colorType colorMode;
    bool onoff;
    colorEffectMode effectMode;
    colorMgr<colorType::RGB> rgb;
    colorMgr<colorType::HSV> hsv;
    colorMgr<colorType::CCTB> cctb;
    colorMgr<colorType::CW> cw;
    
    /* colorBasePort.cpp */
    colorBasePortMgr portMgr;
    std::map<int, int> gammaTable;
    
    /* internal setcolor callback impl for port */
    inline void setColorCallback(hookCallback<colorType::RGB> cb) { this->rgb.cbImpl = cb; }
    inline void setColorCallback(hookCallback<colorType::HSV> cb) { this->hsv.cbImpl = cb; }
    inline void setColorCallback(hookCallback<colorType::CCTB> cb) { this->cctb.cbImpl = cb; }
    inline void setColorCallback(hookCallback<colorType::CW> cb) { this->cw.cbImpl = cb; }

}; /* class colorBaseMgr */







} /* colorBase */


