#include <colorBase/colorBase.h>
#include <algorithm>


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
    
    mgr->setColor(mgr->hsv.color);
    mgr->portMgr.timer.execute();
}

template <>
void colorBaseMgr::startColorLoop<colorType::HSV>(uint16_t loopMs)
{
    using colorLoopCallbackType = void (*)(void *);
    this->colorMode = colorType::RGB;
    this->effectMode = colorEffectMode::COLORLOOP;
    this->hsv.effectVal.loopDirection = 1;
    this->hsv.effectVal.loopMs = loopMs;
    portMgr.timer.stopExecute();
    portMgr.timer.setType(0);
    portMgr.timer.setExecuteTime((this->hsv.effectVal.loopMs / 360) > 0 ? (this->hsv.effectVal.loopMs / 360) : 1);
    portMgr.timer.setObj((void *)this);
    portMgr.timer.setCallback(reinterpret_cast<colorLoopCallbackType>(colorBaseMgr::colorLoopCallback<colorType::HSV>));

    portMgr.timer.execute();
}

} /* colorBase */

