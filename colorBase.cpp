#include <colorBase/colorBase.h>
#include <algorithm>

#define COLORBASE_PRINT 1
#if COLORBASE_PRINT
#define CB_PRINT(format, ...) \
            do { \
                printf("[%s %d][%s]: " format "\r\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
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


template <>
void colorBaseMgr::colorLoopCallback<colorType::HSV>(void *arg)
{
    colorBaseMgr *mgr = (colorBaseMgr *)arg;
    
    mgr->hsv.color.val.h = (mgr->hsv.effectVal.loopDirection ? \
        (mgr->hsv.color.val.h >= 360 ? 0 : mgr->hsv.color.val.h + 1) : (mgr->hsv.color.val.h <= 0 ? 360 : mgr->hsv.color.val.h - 1) 
    );
    
    mgr->setColorInternal(mgr->hsv.color);
    mgr->portMgr.timer.execute();
}

template <>
void colorBaseMgr::startColorLoop<colorType::HSV>(uint16_t loopMs)
{
    using colorLoopCallbackType = void (*)(void *);
    this->onoff = true;
    portMgr.timer.stopExecute();
    this->colorMode = colorType::RGB;
    this->effectMode = colorEffectMode::COLORLOOP;
    this->hsv.effectVal.loopDirection = 1;
    this->hsv.effectVal.loopMs = loopMs;
    portMgr.timer.setType(0);
    portMgr.timer.setExecuteTime((this->hsv.effectVal.loopMs / 360) > 0 ? (this->hsv.effectVal.loopMs / 360) : 1);
    portMgr.timer.setObj((void *)this);
    portMgr.timer.setCallback(reinterpret_cast<colorLoopCallbackType>(colorBaseMgr::colorLoopCallback<colorType::HSV>));

    portMgr.timer.execute();
}





template <>
void colorBaseMgr::colorLinearCallback<colorType::RGB>(void *arg)
{
    colorBaseMgr *mgr = (colorBaseMgr *)arg;

    mgr->rgb.color.val.r += (mgr->rgb.effectVal.target.val.r - mgr->rgb.color.val.r) / mgr->rgb.effectVal.linearStep;
    mgr->rgb.color.val.g += (mgr->rgb.effectVal.target.val.g - mgr->rgb.color.val.g) / mgr->rgb.effectVal.linearStep;
    mgr->rgb.color.val.b += (mgr->rgb.effectVal.target.val.b - mgr->rgb.color.val.b) / mgr->rgb.effectVal.linearStep;
    
    mgr->setColorInternal(mgr->rgb.color);
    if(!mgr->rgb.color.val.r & !mgr->rgb.color.val.g & !mgr->rgb.color.val.b)
        mgr->onoff = false;
    if(--mgr->rgb.effectVal.linearStep)
        mgr->portMgr.timer.execute();
    
}

void colorBaseMgr::startColorLinear(Color<colorType::RGB> &tar)
{
    using linearCallbackType = void (*)(void *);
    this->onoff = true;
    this->portMgr.timer.stopExecute();
    this->colorMode = colorType::RGB;
    this->effectMode = colorEffectMode::LINEAR;
    this->rgb.effectVal.target = tar;
    CB_PRINT("target: %d %d %d", this->rgb.effectVal.target.val.r, this->rgb.effectVal.target.val.g, this->rgb.effectVal.target.val.b);
    CB_PRINT("current: %d %d %d", this->rgb.color.val.r, this->rgb.color.val.g, this->rgb.color.val.b);
    this->rgb.effectVal.linearStep = this->rgb.effectVal.linearMs / this->rgb.effectVal.linearIntervalMs;
    CB_PRINT("linearStep: %d", this->rgb.effectVal.linearStep);
    this->portMgr.timer.setType(0);
    this->portMgr.timer.setExecuteTime(this->rgb.effectVal.linearIntervalMs);
    this->portMgr.timer.setObj((void *)this);
    this->portMgr.timer.setCallback(reinterpret_cast<linearCallbackType>(colorBaseMgr::colorLinearCallback<colorType::RGB>));

    this->portMgr.timer.execute(); 

}

void colorBaseMgr::startColorLinear(Color<colorType::HSV> &tar)
{
    this->colorMode = colorType::HSV;
    this->effectMode = colorEffectMode::LINEAR;
}

void colorBaseMgr::startColorLinear(Color<colorType::CCTB> &tar)
{
    this->colorMode = colorType::CCTB;
    this->effectMode = colorEffectMode::LINEAR;
}

void colorBaseMgr::startColorLinear(Color<colorType::CW> &tar)
{
    this->colorMode = colorType::CW;
    this->effectMode = colorEffectMode::LINEAR;
}

void colorBaseMgr::setOnoff(bool onoff)
{
    if(this->onoff ^ onoff ^ 1)
        return;
    
    switch(this->colorMode){
        case colorType::RGB:
        {
            Color<colorType::RGB> color;
            if(!onoff)
                this->rgb.prevColor = this->rgb.color;
            else {
                color = this->rgb.prevColor;
            }
            this->onoff ^= 1;
            setColor(color);
        }
        break;

        case colorType::HSV:
        {
            Color<colorType::HSV> color;
            if(!onoff)
                this->hsv.prevColor = this->hsv.color;
            else {
                color = this->hsv.prevColor;
            }
            this->onoff ^= 1;
            setColor(color);
        }
        break;

        case colorType::CCTB:
        {
            Color<colorType::CCTB> color;
            if(!onoff)
                this->cctb.prevColor = this->cctb.color;
            else {
                color = this->cctb.prevColor;
            }
            this->onoff ^= 1;
            setColor(color);
        }
        break;

        case colorType::CW:
        {
            Color<colorType::CW> color;
            if(!onoff)
                this->cw.prevColor = this->cw.color;
            else {
                color = this->cw.prevColor;
            }
            this->onoff ^= 1;
            setColor(color);
        }
        break;

        default:
        break;
    }

}

void colorBaseMgr::setOnoffLinear(bool onoff)
{
    if(this->onoff ^ onoff ^ 1)
        return;
    
    switch(this->colorMode){
        case colorType::RGB:
        {
            Color<colorType::RGB> color;
            if(!onoff)
                this->rgb.prevColor = this->rgb.color;
            else {
                color = this->rgb.prevColor;
            }
            this->onoff ^= 1;
            this->startColorLinear(color);
        }
        break;

        case colorType::HSV:
        {
            Color<colorType::HSV> color;
            if(!onoff)
                this->hsv.prevColor = this->hsv.color;
            else {
                color = this->hsv.prevColor;
            }
            this->onoff ^= 1;
            this->startColorLinear(color);
        }
        break;

        case colorType::CCTB:
        {
            Color<colorType::CCTB> color;
            if(!onoff)
                this->cctb.prevColor = this->cctb.color;
            else {
                color = this->cctb.prevColor;
            }
            this->onoff ^= 1;
            this->startColorLinear(color);
        }
        break;

        case colorType::CW:
        {
            Color<colorType::CW> color;
            if(!onoff)
                this->cw.prevColor = this->cw.color;
            else {
                color = this->cw.prevColor;
            }
            this->onoff ^= 1;
            this->startColorLinear(color);
        }
        break;

        default:
        break;
    }
}
} /* colorBase */

