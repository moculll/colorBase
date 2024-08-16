#include <colorBase/colorBase.h>
#include <algorithm>

#define COLORBASE_PRINT 0
#if COLORBASE_PRINT
#define CB_PRINT(format, ...) \
            do { \
                printf("[%d][%s]: " format "\r\n", __LINE__, __func__, ##__VA_ARGS__); \
            } while(0)
#else
#define CB_PRINT(format, ...)
#endif

namespace colorBase {
namespace effects {

void convert(const Color<colorType::RGB> &src, Color<colorType::HSV> &tar)
{

    int _hue, _saturation, _value;
    int m_max = std::max(src.val.r, std::max(src.val.g, src.val.b));
    int m_min = std::min(src.val.r, std::min(src.val.g, src.val.b));
    int m_delta = m_max - m_min;

    _value = (m_max * 100) / 255;

    if (m_delta == 0) {
        _hue = 0;
        _saturation = 0;
    }
    else {
        _saturation = (m_delta * 100) / m_max;

        if (src.val.r == m_max) {
            _hue = ((src.val.g - src.val.b) * 60) / m_delta;
        }
        else if (src.val.g == m_max) {
            _hue = (2 * 60) + ((src.val.b - src.val.r) * 60) / m_delta;
        }
        else {
            _hue = (4 * 60) + ((src.val.r - src.val.g) * 60) / m_delta;
        }

        if (_hue < 0) {
            _hue = _hue + 360;
        }
    }

    tar.val.h = _hue;
    tar.val.s = _saturation;
    tar.val.v = _value;

}

void convert(const Color<colorType::HSV> &src, Color<colorType::RGB> &tar)
{

    uint16_t hue = src.val.h % 360;
    uint16_t hi = (hue / 60) % 6;
    uint16_t F = 100 * hue / 60 - 100 * hi;
    uint16_t P = src.val.v * (100 - src.val.s) / 100;
    uint16_t Q = src.val.v * (10000 - F * src.val.s) / 10000;
    uint16_t T = src.val.v * (10000 - src.val.s * (100 - F)) / 10000;
    switch (hi) {
        case 0:
            tar.val.r = src.val.v;
            tar.val.g = T;
            tar.val.b = P;
            break;
        case 1:
            tar.val.r = Q;
            tar.val.g = src.val.v;
            tar.val.b = P;
            break;
        case 2:
            tar.val.r = P;
            tar.val.g = src.val.v;
            tar.val.b = T;
            break;
        case 3:
            tar.val.r = P;
            tar.val.g = Q;
            tar.val.b = src.val.v;
            break;
        case 4:
            tar.val.r = T;
            tar.val.g = P;
            tar.val.b = src.val.v;
            break;
        case 5:
            tar.val.r = src.val.v;
            tar.val.g = P;
            tar.val.b = Q;
            break;
        default:
        break;
    }
    tar.val.r = tar.val.r * 255 / 100;
    tar.val.g = tar.val.g * 255 / 100;
    tar.val.b = tar.val.b * 255 / 100;
}

void convert(const Color<colorType::CCTB> &src, Color<colorType::CW> &tar)
{

    int max_val = 255.0 * src.val.b / 100.0;
    if(src.val.cct > 50) {
        tar.val.c = max_val;
        tar.val.w = 100 * tar.val.c / src.val.cct - tar.val.c;
    }
    else {
        tar.val.w = max_val;
        tar.val.c = tar.val.w * src.val.cct / (100 - src.val.cct);
    }


}

void convert(const Color<colorType::CW> &src, Color<colorType::CCTB> &tar)
{

    int total = src.val.c + src.val.w;

    tar.val.b = (uint8_t)(((src.val.c > src.val.w ? src.val.c : src.val.w) / 255.0) * 100);

    if (total == 0) {
        tar.val.cct = 50;
    }
    else {
        tar.val.cct = (uint8_t)(src.val.c * 100 / total);
    }

}

} /* effects */

void colorBaseMgr::debugPrintMgrAllInfos()
{
    CB_PRINT("----------------[Mgr Info Start]-----------------------------------------");
    CB_PRINT("colorMode: %d, effectMode: %d, onoff: %d", (int)this->colorMode, (int)this->effectMode, this->onoff ? 1 : 0);
    CB_PRINT("----------------[RGB]-----------------------------------------");
    CB_PRINT("[rgb] %d %d %d, [prevColor] %d %d %d, [effectVal] linearStep: %d linearIntervalMs: %d linearMs: %d colorLoopMs: %d brightnessLoopMs: %d loopDirection: %d \
    random: %d [target] %d %d %d", this->rgb.color.val.r, this->rgb.color.val.g, this->rgb.color.val.b, this->rgb.prevColor.val.r, this->rgb.prevColor.val.g, this->rgb.prevColor.val.b \
    , this->rgb.effectVal.linearStep, this->rgb.effectVal.linearIntervalMs, this->rgb.effectVal.linearMs, this->rgb.effectVal.colorLoopMs, this->rgb.effectVal.brightnessLoopMs, 
    this->rgb.effectVal.loopDirection, this->rgb.effectVal.random ? 1 : 0, this->rgb.effectVal.target.val.r, this->rgb.effectVal.target.val.g, this->rgb.effectVal.target.val.b);
    
    CB_PRINT("----------------[HSV]-----------------------------------------");
    CB_PRINT("[hsv] %d %d %d, [prevColor] %d %d %d, [effectVal] linearStep: %d linearIntervalMs: %d linearMs: %d colorLoopMs: %d brightnessLoopMs: %d loopDirection: %d \
    random: %d [target] %d %d %d", this->hsv.color.val.h, this->hsv.color.val.s, this->hsv.color.val.v, this->hsv.prevColor.val.h, this->hsv.prevColor.val.s, this->hsv.prevColor.val.v \
    , this->hsv.effectVal.linearStep, this->hsv.effectVal.linearIntervalMs, this->hsv.effectVal.linearMs, this->hsv.effectVal.colorLoopMs, this->hsv.effectVal.brightnessLoopMs, 
    this->hsv.effectVal.loopDirection, this->hsv.effectVal.random ? 1 : 0, this->hsv.effectVal.target.val.h, this->hsv.effectVal.target.val.s, this->hsv.effectVal.target.val.v);

    CB_PRINT("----------------[CCTB]-----------------------------------------");
    CB_PRINT("[cctb] %d %d, [prevColor] %d %d, [effectVal] linearStep: %d linearIntervalMs: %d linearMs: %d colorLoopMs: %d brightnessLoopMs: %d loopDirection: %d \
    random: %d [target] %d %d", this->cctb.color.val.cct, this->cctb.color.val.b, this->cctb.prevColor.val.cct, this->cctb.prevColor.val.b \
    , this->cctb.effectVal.linearStep, this->cctb.effectVal.linearIntervalMs, this->cctb.effectVal.linearMs, this->cctb.effectVal.colorLoopMs, this->cctb.effectVal.brightnessLoopMs, 
    this->cctb.effectVal.loopDirection, this->cctb.effectVal.random ? 1 : 0, this->cctb.effectVal.target.val.cct, this->cctb.effectVal.target.val.b);

    CB_PRINT("----------------[CW]-----------------------------------------");
    CB_PRINT("[cw] %d %d, [prevColor] %d %d, [effectVal] linearStep: %d linearIntervalMs: %d linearMs: %d colorLoopMs: %d brightnessLoopMs: %d loopDirection: %d \
    random: %d [target] %d %d", this->cw.color.val.c, this->cw.color.val.w, this->cw.prevColor.val.c, this->cw.prevColor.val.w \
    , this->cw.effectVal.linearStep, this->cw.effectVal.linearIntervalMs, this->cw.effectVal.linearMs, this->cw.effectVal.colorLoopMs, this->cw.effectVal.brightnessLoopMs, 
    this->cw.effectVal.loopDirection, this->cw.effectVal.random ? 1 : 0, this->cw.effectVal.target.val.c, this->cw.effectVal.target.val.w);

    CB_PRINT("----------------[Mgr Info End]-----------------------------------------");
}


template <>
void colorBaseMgr::colorLoopCallback<colorType::HSV>(void *arg)
{
    colorBaseMgr *mgr = (colorBaseMgr *)arg;
    if(mgr->hsv.effectVal.random) {
        mgr->hsv.color.val.h = colorBaseMgr::getRandom(0, 359);
        mgr->hsv.color.val.s = colorBaseMgr::getRandom(0, 100);
    }
    else {
        mgr->hsv.color.val.h = (mgr->hsv.effectVal.loopDirection ? \
            (mgr->hsv.color.val.h >= 360 ? 0 : mgr->hsv.color.val.h + 1) : (mgr->hsv.color.val.h <= 0 ? 360 : mgr->hsv.color.val.h - 1) 
        );
    }
    mgr->setColorInternal(mgr->hsv.color);
    mgr->portMgr.timer.execute();
}

template <>
void colorBaseMgr::setColorLoop<colorType::HSV>(uint32_t loopMs, bool random)
{
    using colorLoopCallbackType = void (*)(void *);

    portMgr.timer.stopExecute();
    this->colorMode = colorType::HSV;
    this->effectMode = colorEffectMode::COLORLOOP;
    this->hsv.color.val.v = 0;
    this->hsv.effectVal.loopDirection = 1;
    if(loopMs)
        this->hsv.effectVal.colorLoopMs = loopMs;
    this->hsv.effectVal.random = random;

    portMgr.timer.setExecuteTime((this->hsv.effectVal.colorLoopMs / 360) > 0 ? (this->hsv.effectVal.colorLoopMs / 360) : 1);

    portMgr.timer.setCallback(reinterpret_cast<colorLoopCallbackType>(colorBaseMgr::colorLoopCallback<colorType::HSV>));

    portMgr.timer.execute();
}

template <>
void colorBaseMgr::brightnessLoopCallback<colorType::HSV>(void *arg)
{
    colorBaseMgr *mgr = (colorBaseMgr *)arg;
    if(mgr->hsv.effectVal.random) {
        mgr->hsv.color.val.v = colorBaseMgr::getRandom(0, 100);
    }
    else {
        if(mgr->hsv.color.val.v >= 100) {
            mgr->hsv.effectVal.loopDirection = 1;
        }
        else if(mgr->hsv.color.val.v <= 0) {
            mgr->hsv.effectVal.loopDirection = 0;
        }
        mgr->hsv.color.val.v = (mgr->hsv.effectVal.loopDirection ? mgr->hsv.color.val.v - 1 : mgr->hsv.color.val.v + 1);
    }
    mgr->setColorInternal(mgr->hsv.color);
    mgr->portMgr.timer.execute();
}

template <>
void colorBaseMgr::setBrightnessLoop<colorType::HSV>(uint32_t loopMs, bool random)
{
    using brightnessLoopCallbackType = void (*)(void *);

    portMgr.timer.stopExecute();
    this->colorMode = colorType::HSV;
    this->effectMode = colorEffectMode::BRIGHTNESSLOOP;
    this->hsv.color.val.v = 0;
    if(loopMs)
        this->hsv.effectVal.brightnessLoopMs = loopMs;
    this->hsv.effectVal.random = random;
    portMgr.timer.setExecuteTime((this->hsv.effectVal.brightnessLoopMs / 100) > 0 ? (this->hsv.effectVal.brightnessLoopMs / 100) : 1);

    portMgr.timer.setCallback(reinterpret_cast<brightnessLoopCallbackType>(colorBaseMgr::brightnessLoopCallback<colorType::HSV>));

    portMgr.timer.execute();

}

template <>
void colorBaseMgr::brightnessLoopCallback<colorType::CCTB>(void *arg)
{
    colorBaseMgr *mgr = (colorBaseMgr *)arg;
    if(mgr->cctb.effectVal.random) {
        mgr->cctb.color.val.b = colorBaseMgr::getRandom(0, 100);
    }
    else {
        if(mgr->cctb.color.val.b >= 100) {
            mgr->cctb.effectVal.loopDirection = 1;
        }
        else if(mgr->cctb.color.val.b <= 0) {
            mgr->cctb.effectVal.loopDirection = 0;
        }
        mgr->cctb.color.val.b = (mgr->cctb.effectVal.loopDirection ? mgr->cctb.color.val.b - 1 : mgr->cctb.color.val.b + 1);
    }
    
    mgr->setColorInternal(mgr->cctb.color);
    mgr->portMgr.timer.execute();
}

template <>
void colorBaseMgr::setBrightnessLoop<colorType::CCTB>(uint32_t loopMs, bool random)
{
    using brightnessLoopCallbackType = void (*)(void *);

    portMgr.timer.stopExecute();
    this->colorMode = colorType::CCTB;
    this->effectMode = colorEffectMode::BRIGHTNESSLOOP;
    this->cctb.color.val.b = 0;
    if(loopMs)
        this->cctb.effectVal.brightnessLoopMs = loopMs;
    this->cctb.effectVal.random = random;

    portMgr.timer.setExecuteTime((this->cctb.effectVal.brightnessLoopMs / 100) > 0 ? (this->cctb.effectVal.brightnessLoopMs / 100) : 1);

    portMgr.timer.setCallback(reinterpret_cast<brightnessLoopCallbackType>(colorBaseMgr::brightnessLoopCallback<colorType::CCTB>));

    portMgr.timer.execute();

}

template <>
void colorBaseMgr::setColor(const Color<colorType::RGB> &tar)
{
    bool nextOnoff = tar.isOn() ? true : false;
    
    if(this->onoff == nextOnoff && !nextOnoff \
        && (this->effectMode != colorEffectMode::NORMAL && this->effectMode != colorEffectMode::LINEAR) \
        || (this->rgb.color == tar) \
    ) return;
    this->onoff = true;
    this->portMgr.timer.stopExecute();
    if(!nextOnoff)
        this->rgb.prevColor = this->rgb.color;
    this->colorMode = colorType::RGB;
    this->effectMode = colorEffectMode::NORMAL;
    this->rgb.color = tar;

    this->rgb.cbImpl(tar);
}

template <>
void colorBaseMgr::setColor(const Color<colorType::HSV> &tar)
{
    bool nextOnoff = tar.isOn() ? true : false;
    
    if(this->onoff == nextOnoff && !nextOnoff \
        && (this->effectMode != colorEffectMode::NORMAL && this->effectMode != colorEffectMode::LINEAR) \
        || (this->hsv.color == tar) \
    ) return;
    this->onoff = true;
    this->portMgr.timer.stopExecute();
    if(!nextOnoff)
        this->hsv.prevColor = this->hsv.color;

    this->colorMode = colorType::HSV;
    this->effectMode = colorEffectMode::NORMAL;
    this->hsv.color = tar;

    this->hsv.cbImpl(tar);
}

template <>
void colorBaseMgr::setColor(const Color<colorType::CCTB> &tar)
{
    bool nextOnoff = tar.isOn() ? true : false;
    
    if(this->onoff == nextOnoff && !nextOnoff \
        && (this->effectMode != colorEffectMode::NORMAL && this->effectMode != colorEffectMode::LINEAR) \
        || (this->cctb.color == tar) \
    ) return;
    this->onoff = true;
    this->portMgr.timer.stopExecute();
    if(!nextOnoff)
        this->cctb.prevColor = this->cctb.color;

    this->colorMode = colorType::CCTB;
    this->effectMode = colorEffectMode::NORMAL;
    this->cctb.color = tar;

    this->cctb.cbImpl(tar);
}

template <>
void colorBaseMgr::setColor(const Color<colorType::CW> &tar)
{
    bool nextOnoff = tar.isOn() ? true : false;
    
    if(this->onoff == nextOnoff && !nextOnoff \
        && (this->effectMode != colorEffectMode::NORMAL && this->effectMode != colorEffectMode::LINEAR) \
        || (this->cw.color == tar) \
    ) return;
    this->onoff = true;
    this->portMgr.timer.stopExecute();
    if(!nextOnoff)
        this->cw.prevColor = this->cw.color;

    this->colorMode = colorType::CW;
    this->effectMode = colorEffectMode::NORMAL;
    this->cw.color = tar;

    this->cw.cbImpl(tar);
}

template <>
void colorBaseMgr::colorLinearCallback<colorType::RGB>(void *arg)
{
    colorBaseMgr *mgr = (colorBaseMgr *)arg;

    if(!mgr->rgb.effectVal.linearStep || mgr->rgb.color == mgr->rgb.effectVal.target)
        return;
 
    mgr->rgb.color.val.r += ((int8_t)(mgr->rgb.effectVal.target.val.r) - (int8_t)(mgr->rgb.color.val.r)) / (int8_t)mgr->rgb.effectVal.linearStep;
    mgr->rgb.color.val.g += ((int8_t)(mgr->rgb.effectVal.target.val.g) - (int8_t)(mgr->rgb.color.val.g)) / (int8_t)mgr->rgb.effectVal.linearStep;
    mgr->rgb.color.val.b += ((int8_t)(mgr->rgb.effectVal.target.val.b) - (int8_t)(mgr->rgb.color.val.b)) / (int8_t)mgr->rgb.effectVal.linearStep;
    
    mgr->setColorInternal(mgr->rgb.color);
    
    if(!mgr->rgb.color.isOn()){
        mgr->onoff = false;
        CB_PRINT("set end onoff to %d", mgr->onoff ? 1 : 0);
    }
    if(--mgr->rgb.effectVal.linearStep && mgr->portMgr.timer.getRunningStatus())
        mgr->portMgr.timer.execute();
    
}

void colorBaseMgr::setColorLinear(const Color<colorType::RGB> &tar)
{
    using linearCallbackType = void (*)(void *);
    bool nextOnoff = tar.isOn() ? true : false;
    CB_PRINT("current onoff: %d", this->onoff ? 1 : 0);
    if(this->onoff == nextOnoff && !nextOnoff \
        && (this->effectMode != colorEffectMode::NORMAL && this->effectMode != colorEffectMode::LINEAR) \
        || (this->rgb.color == tar) \
    ) return;
    if(!nextOnoff){
        this->rgb.prevColor = this->rgb.color;
        CB_PRINT("set prev to new: %d, %d, %d", this->rgb.prevColor.val.r, this->rgb.prevColor.val.g, this->rgb.prevColor.val.b);
    }
    this->onoff = true;
    this->portMgr.timer.stopExecute();
    
    this->colorMode = colorType::RGB;
    this->effectMode = colorEffectMode::LINEAR;
    this->rgb.effectVal.target = tar;
    CB_PRINT("target: %d %d %d", this->rgb.effectVal.target.val.r, this->rgb.effectVal.target.val.g, this->rgb.effectVal.target.val.b);
    CB_PRINT("current: %d %d %d", this->rgb.color.val.r, this->rgb.color.val.g, this->rgb.color.val.b);
    this->rgb.effectVal.linearStep = this->rgb.effectVal.linearMs / this->rgb.effectVal.linearIntervalMs;
    CB_PRINT("linearStep: %d", this->rgb.effectVal.linearStep);

    this->portMgr.timer.setExecuteTime(this->rgb.effectVal.linearIntervalMs);
    
    this->portMgr.timer.setCallback(reinterpret_cast<linearCallbackType>(colorBaseMgr::colorLinearCallback<colorType::RGB>));

    this->portMgr.timer.execute(); 

}

template <>
void colorBaseMgr::colorLinearCallback<colorType::HSV>(void *arg)
{
    colorBaseMgr *mgr = (colorBaseMgr *)arg;
    if(!mgr->hsv.effectVal.linearStep)
        return;
    
    mgr->hsv.color.val.h += (int16_t)(mgr->hsv.effectVal.target.val.h - mgr->hsv.color.val.h) / (int16_t)mgr->hsv.effectVal.linearStep;
    mgr->hsv.color.val.s += (int8_t)(mgr->hsv.effectVal.target.val.s - mgr->hsv.color.val.s) / (int8_t)mgr->hsv.effectVal.linearStep;
    mgr->hsv.color.val.v += (int8_t)(mgr->hsv.effectVal.target.val.v - mgr->hsv.color.val.v) / (int8_t)mgr->hsv.effectVal.linearStep;
    
    mgr->setColorInternal(mgr->hsv.color);
    if(!mgr->hsv.color.isOn()){
        mgr->onoff = false;
        CB_PRINT("set end onoff to %d", mgr->onoff ? 1 : 0);
    }
    if(--mgr->hsv.effectVal.linearStep && mgr->portMgr.timer.getRunningStatus())
        mgr->portMgr.timer.execute();

    
}

void colorBaseMgr::setColorLinear(const Color<colorType::HSV> &tar)
{
    using linearCallbackType = void (*)(void *);
    bool nextOnoff = tar.isOn() ? true : false;
    
    if(this->onoff == nextOnoff && !nextOnoff \
        && (this->effectMode != colorEffectMode::NORMAL && this->effectMode != colorEffectMode::LINEAR) \
        || (this->hsv.color == tar) \
    ) return;

    if(!nextOnoff){
        this->hsv.prevColor = this->hsv.color;
        CB_PRINT("set prev to new: %d, %d, %d", this->hsv.prevColor.val.h, this->hsv.prevColor.val.s, this->hsv.prevColor.val.v);
    }
    this->onoff = true;
    this->portMgr.timer.stopExecute();

    CB_PRINT("current onoff: %d", this->onoff ? 1 : 0);
    this->colorMode = colorType::HSV;
    this->effectMode = colorEffectMode::LINEAR;
    this->hsv.effectVal.target = tar;
    CB_PRINT("target: %d %d %d", this->hsv.effectVal.target.val.h, this->hsv.effectVal.target.val.s, this->hsv.effectVal.target.val.v);
    CB_PRINT("current: %d %d %d", this->hsv.color.val.h, this->hsv.color.val.s, this->hsv.color.val.v);
    this->hsv.effectVal.linearStep = this->hsv.effectVal.linearMs / this->hsv.effectVal.linearIntervalMs;
    CB_PRINT("linearStep: %d", this->hsv.effectVal.linearStep);

    this->portMgr.timer.setExecuteTime(this->hsv.effectVal.linearIntervalMs);
    
    this->portMgr.timer.setCallback(reinterpret_cast<linearCallbackType>(colorBaseMgr::colorLinearCallback<colorType::HSV>));

    this->portMgr.timer.execute(); 
}


template <>
void colorBaseMgr::colorLinearCallback<colorType::CCTB>(void *arg)
{
    colorBaseMgr *mgr = (colorBaseMgr *)arg;
    if(!mgr->cctb.effectVal.linearStep)
        return;
    /* CB_PRINT("set cctb: %d, %d", mgr->cctb.color.val.cct, mgr->cctb.color.val.b); */
    /* mgr->debugPrintMgrAllInfos(); */
    mgr->cctb.color.val.cct += (int16_t)(mgr->cctb.effectVal.target.val.cct - mgr->cctb.color.val.cct) / (int16_t)mgr->cctb.effectVal.linearStep;
    mgr->cctb.color.val.b += (int16_t)(mgr->cctb.effectVal.target.val.b - mgr->cctb.color.val.b) / (int16_t)mgr->cctb.effectVal.linearStep;

    mgr->setColorInternal(mgr->cctb.color);
    if(!mgr->cctb.color.isOn()){
        mgr->onoff = false;
        CB_PRINT("set end onoff to %d", mgr->onoff ? 1 : 0);
    }
    if(--mgr->cctb.effectVal.linearStep && mgr->portMgr.timer.getRunningStatus())
        mgr->portMgr.timer.execute();

}

void colorBaseMgr::setColorLinear(const Color<colorType::CCTB> &tar)
{
    using linearCallbackType = void (*)(void *);

    bool nextOnoff = tar.isOn() ? true : false;
    if(this->onoff == nextOnoff && !nextOnoff \
        && (this->effectMode != colorEffectMode::NORMAL && this->effectMode != colorEffectMode::LINEAR) \
        || (this->cctb.color == tar) \
    ) return;

    if(!nextOnoff){
        this->cctb.prevColor = this->cctb.color;
        CB_PRINT("set prev to new: %d, %d", this->cctb.prevColor.val.cct, this->cctb.prevColor.val.b);
    }
    this->onoff = true;
    this->portMgr.timer.stopExecute();

    CB_PRINT("current onoff: %d", this->onoff ? 1 : 0);
    this->colorMode = colorType::CCTB;
    this->effectMode = colorEffectMode::LINEAR;
    this->cctb.effectVal.target = tar;
    CB_PRINT("target: %d %d", this->cctb.effectVal.target.val.cct, this->cctb.effectVal.target.val.b);
    CB_PRINT("current: %d %d", this->cctb.color.val.cct, this->cctb.color.val.b);
    this->cctb.effectVal.linearStep = this->cctb.effectVal.linearMs / this->cctb.effectVal.linearIntervalMs;
    CB_PRINT("linearStep: %d", this->cctb.effectVal.linearStep);

    this->portMgr.timer.setExecuteTime(this->cctb.effectVal.linearIntervalMs);
    
    this->portMgr.timer.setCallback(reinterpret_cast<linearCallbackType>(colorBaseMgr::colorLinearCallback<colorType::CCTB>));

    this->portMgr.timer.execute(); 
}

template <>
void colorBaseMgr::colorLinearCallback<colorType::CW>(void *arg)
{
    colorBaseMgr *mgr = (colorBaseMgr *)arg;
    if(!mgr->cw.effectVal.linearStep)
        return;
    mgr->cw.color.val.c += (int16_t)(mgr->cw.effectVal.target.val.c - mgr->cw.color.val.c) / (int16_t)mgr->cw.effectVal.linearStep;
    mgr->cw.color.val.w += (int16_t)(mgr->cw.effectVal.target.val.w - mgr->cw.color.val.w) / (int16_t)mgr->cw.effectVal.linearStep;

    mgr->setColorInternal(mgr->cw.color);
    if(!mgr->cw.color.isOn()){
        mgr->onoff = false;
        CB_PRINT("set end onoff to %d", mgr->onoff ? 1 : 0);
    }
    if(--mgr->cw.effectVal.linearStep && mgr->portMgr.timer.getRunningStatus())
        mgr->portMgr.timer.execute();

}

void colorBaseMgr::setColorLinear(const Color<colorType::CW> &tar)
{
    using linearCallbackType = void (*)(void *);
    bool nextOnoff = tar.isOn() ? true : false;
    if(this->onoff == nextOnoff && !nextOnoff \
        && (this->effectMode != colorEffectMode::NORMAL && this->effectMode != colorEffectMode::LINEAR) \
        || (this->cw.color == tar) \
    ) return;
    if(!nextOnoff){
        this->cw.prevColor = this->cw.color;
        CB_PRINT("set prev to new: %d, %d", this->cw.prevColor.val.c, this->cw.prevColor.val.w);
    }
    this->onoff = true;
    this->portMgr.timer.stopExecute();

    CB_PRINT("current onoff: %d", this->onoff ? 1 : 0);
    this->colorMode = colorType::CW;
    this->effectMode = colorEffectMode::LINEAR;
    this->cw.effectVal.target = tar;
    CB_PRINT("target: %d %d", this->cw.effectVal.target.val.c, this->cw.effectVal.target.val.w);
    CB_PRINT("current: %d %d", this->cw.color.val.c, this->cw.color.val.w);
    this->cw.effectVal.linearStep = this->cw.effectVal.linearMs / this->cw.effectVal.linearIntervalMs;
    CB_PRINT("linearStep: %d", this->cw.effectVal.linearStep);

    this->portMgr.timer.setExecuteTime(this->cw.effectVal.linearIntervalMs);
    
    this->portMgr.timer.setCallback(reinterpret_cast<linearCallbackType>(colorBaseMgr::colorLinearCallback<colorType::CW>));

    this->portMgr.timer.execute(); 
}

void colorBaseMgr::setOnoff(bool onoff)
{
    CB_PRINT("set onoff: %d", onoff ? 1 : 0);
    
    if(this->colorMode == colorType::RGB) {
        CB_PRINT("prev: %d %d %d", this->rgb.prevColor.val.r, this->rgb.prevColor.val.g, this->rgb.prevColor.val.b);

        this->setColor( onoff ? \
            ( this->rgb.prevColor.isOn() ? this->rgb.prevColor : this->rgb.prevColor.setMax() ) \
            : Color<colorBase::colorType::RGB>(0, 0, 0) );
    }
    else if(this->colorMode == colorType::HSV) {
        CB_PRINT("prev: %d %d %d", this->hsv.prevColor.val.h, this->hsv.prevColor.val.s, this->hsv.prevColor.val.v);
        this->setColor( onoff ? \
            ( this->hsv.prevColor.isOn() ? this->hsv.prevColor : this->hsv.prevColor.setMax() ) \
            : Color<colorBase::colorType::HSV>(0, 0, 0) );
    }
    else if(this->colorMode == colorType::CCTB) {
        CB_PRINT("prev: %d %d", this->cctb.prevColor.val.cct, this->cctb.prevColor.val.b);
        this->setColor( onoff ? \
            ( this->cctb.prevColor.isOn() ? this->cctb.prevColor : this->cctb.prevColor.setMax() ) \
            : Color<colorBase::colorType::CCTB>(0, 0) );
    }
    else if(this->colorMode == colorType::CW) {
        CB_PRINT("prev: %d %d", this->cw.prevColor.val.c, this->cw.prevColor.val.w);
        this->setColor( onoff ? \
            ( this->cw.prevColor.isOn() ? this->cw.prevColor : this->cw.prevColor.setMax() ) \
            : Color<colorBase::colorType::CW>(0, 0) );
    }

}

void colorBaseMgr::setOnoffLinear(bool onoff)
{
    CB_PRINT("set onoff linear: %d", onoff ? 1 : 0);
    if(this->colorMode == colorType::RGB) {
        CB_PRINT("prev: %d %d %d", this->rgb.prevColor.val.r, this->rgb.prevColor.val.g, this->rgb.prevColor.val.b);

        this->setColorLinear( onoff ? \
            ( this->rgb.prevColor.isOn() ? this->rgb.prevColor : this->rgb.prevColor.setMax() ) \
            : Color<colorBase::colorType::RGB>(0, 0, 0) );
    }
    else if(this->colorMode == colorType::HSV) {
        CB_PRINT("prev: %d %d %d", this->hsv.prevColor.val.h, this->hsv.prevColor.val.s, this->hsv.prevColor.val.v);
        this->setColorLinear( onoff ? \
            ( this->hsv.prevColor.isOn() ? this->hsv.prevColor : this->hsv.prevColor.setMax() ) \
            : Color<colorBase::colorType::HSV>(0, 0, 0) );
    }
    else if(this->colorMode == colorType::CCTB) {
        CB_PRINT("prev: %d %d", this->cctb.prevColor.val.cct, this->cctb.prevColor.val.b);
        this->setColorLinear( onoff ? \
            ( this->cctb.prevColor.isOn() ? this->cctb.prevColor : this->cctb.prevColor.setMax() ) \
            : Color<colorBase::colorType::CCTB>(0, 0) );
    }
    else if(this->colorMode == colorType::CW) {
        CB_PRINT("prev: %d %d", this->cw.prevColor.val.c, this->cw.prevColor.val.w);
        this->setColorLinear( onoff ? \
            ( this->cw.prevColor.isOn() ? this->cw.prevColor : this->cw.prevColor.setMax() ) \
            : Color<colorBase::colorType::CW>(0, 0) );
    }

}

void colorBaseMgr::setBrightness(uint8_t brightness)
{
    CB_PRINT("set brightness: %d", brightness);
    if(this->colorMode == colorType::RGB) {
        Color<colorBase::colorType::RGB> tar;
        effects::convert(this->rgb.color, this->hsv.color);
        this->hsv.color.val.v = brightness;
        effects::convert(this->hsv.color, tar);
        this->setColor(tar);
    }
    else if(this->colorMode == colorType::HSV) {
        Color<colorBase::colorType::HSV> tar = this->hsv.color;
        tar.val.v = brightness;
        this->setColor(tar);
    }
    else if(this->colorMode == colorType::CCTB) {
        Color<colorBase::colorType::CCTB> tar = this->cctb.color;
        tar.val.b = brightness;
        this->setColor(tar);
    }
    else if(this->colorMode == colorType::CW) {
        Color<colorBase::colorType::CW> tar;
        effects::convert(this->cw.color, this->cctb.color);
        this->cctb.color.val.b = brightness;
        effects::convert(this->cctb.color, tar);
        this->setColor(tar);
    }
    
}

void colorBaseMgr::setBrightnessLinear(uint8_t brightness)
{
    CB_PRINT("set brightness linear: %d", brightness);
    if(this->colorMode == colorType::RGB) {
        Color<colorBase::colorType::RGB> tar;
        effects::convert(this->rgb.color, this->hsv.color);
        this->hsv.color.val.v = brightness;
        effects::convert(this->hsv.color, tar);
        this->setColorLinear(tar);
    }
    else if(this->colorMode == colorType::HSV) {
        Color<colorBase::colorType::HSV> tar = this->hsv.color;
        tar.val.v = brightness;
        this->setColorLinear(tar);
    }
    else if(this->colorMode == colorType::CCTB) {
        Color<colorBase::colorType::CCTB> tar = this->cctb.color;
        tar.val.b = brightness;
        this->setColorLinear(tar);
    }
    else if(this->colorMode == colorType::CW) {
        Color<colorBase::colorType::CW> tar;
        effects::convert(this->cw.color, this->cctb.color);
        this->cctb.color.val.b = brightness;
        effects::convert(this->cctb.color, tar);
        this->setColorLinear(tar);
    }

}




template <>
void colorBaseMgr::getColor(Color<colorType::RGB> &dst)
{
    dst = this->rgb.color;
}

template <>
void colorBaseMgr::getColor(Color<colorType::HSV> &dst)
{
    dst = this->hsv.color;
}

template <>
void colorBaseMgr::getColor(Color<colorType::CCTB> &dst)
{
    dst = this->cctb.color;
}

template <>
void colorBaseMgr::getColor(Color<colorType::CW> &dst)
{
    dst = this->cw.color;
}

} /* colorBase */

