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
    CCTB
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
    virtual void assign(const ColorBaseType<T>& src) = 0;
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
    
protected:
    void assign(const ColorBaseType<colorType::RGB>& src) override
    {
        const Color<colorType::RGB>* pSrc = static_cast<const Color<colorType::RGB>*>(&src);
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
protected:
    void assign(const ColorBaseType<colorType::HSV>& src) override
    {
        const Color<colorType::HSV>* pSrc = static_cast<const Color<colorType::HSV>*>(&src);
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

protected:
    void assign(const ColorBaseType<colorType::CCTB>& src) override
    {
        const Color<colorType::CCTB>* pSrc = static_cast<const Color<colorType::CCTB>*>(&src);
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

protected:
    void assign(const ColorBaseType<colorType::CW>& src) override
    {
        const Color<colorType::CW>* pSrc = static_cast<const Color<colorType::CW>*>(&src);
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
    

    uint16_t linearStep;
    uint16_t linearIntervalMs;
    uint16_t linearMs;
    uint8_t loopDirection;
    uint16_t loopMs;
    Color<T> target;
    

    colorEffectVal() : linearStep(0), linearIntervalMs(10), linearMs(1000) {}
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
    colorBaseMgr(): colorMode(colorType::RGB) {
        portMgr.initImpl(*this);
    }
    colorBaseMgr(colorType mode): colorMode(mode) {
        portMgr.initImpl(*this);
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

    

    /* color linear */
    void startColorLinear(const Color<colorType::RGB> &tar);
    void startColorLinear(const Color<colorType::HSV> &tar);
    void startColorLinear(const Color<colorType::CCTB> &tar);
    void startColorLinear(const Color<colorType::CW> &tar);

    template <colorType>
    static void colorLinearCallback(void *arg);


    /* color loop */
    template <colorType>
    static void colorLoopCallback(void *arg);
    template <colorType>
    void startColorLoop(uint16_t loopMs);
    
    
    inline std::map<int, int> *getGammaMap()
    {
        return &this->gammaTable;
    };

   
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
    inline void setColorCallback(hookCallback<colorType::RGB> cb)
    {
        this->rgb.cbImpl = cb;
    }
    inline void setColorCallback(hookCallback<colorType::HSV> cb)
    {
        this->hsv.cbImpl = cb;
    }
    inline void setColorCallback(hookCallback<colorType::CCTB> cb)
    {
        this->cctb.cbImpl = cb;
    }
    inline void setColorCallback(hookCallback<colorType::CW> cb)
    {
        this->cw.cbImpl = cb;
    }

}; /* class colorBaseMgr */







} /* colorBase */


