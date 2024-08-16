#include <colorBase/colorBase.h>
#include <stdio.h>
#include <math.h>

#include <time.h>

#define COLORBASEPORT_PRINT 1
#if COLORBASEPORT_PRINT
#define CBD_PRINT(format, ...) \
            do { \
                printf("[%d][%s]: " format "\r\n", __LINE__, __func__, ##__VA_ARGS__); \
            } while(0)
#else
#define CBD_PRINT(format, ...)
#endif


namespace colorBase {

namespace colorBasePort {

std::map<int, int> *gammaTable;

} /* colorBasePort */


void makeGammaTable(std::map<int, int> &output_gamma_table, int table_size, float gamma_curve_coefficient, int target_bit_depth)
{
    float value_tmp = 0;
    for (int i = 0; i < table_size; i++) {
        value_tmp = (float)(i) / (table_size - 1);
        value_tmp = powf(value_tmp, 1.0f / gamma_curve_coefficient);
        value_tmp *= ((1 << target_bit_depth));
        output_gamma_table[i] = static_cast<int>(value_tmp);
    }
}



template <colorType T>
void colorCallbackImpl(const Color<T> &rgb);

template <>
void colorCallbackImpl<colorType::RGB>(const Color<colorType::RGB> &rgb)
{
    CBD_PRINT("set rgb: %d, %d, %d", (*colorBasePort::gammaTable)[rgb.val.r], (*colorBasePort::gammaTable)[rgb.val.g], (*colorBasePort::gammaTable)[rgb.val.b]);

}

template <>
void colorCallbackImpl<colorType::HSV>(const Color<colorType::HSV> &hsv)
{
    CBD_PRINT("set hsv: %d, %d, %d", hsv.val.h, hsv.val.s, hsv.val.v);

}

template <>
void colorCallbackImpl<colorType::CCTB>(const Color<colorType::CCTB> &cctb)
{
    CBD_PRINT("set cctb: %d, %d", cctb.val.cct, cctb.val.b);

}

template <>
void colorCallbackImpl<colorType::CW>(const Color<colorType::CW> &cw)
{
    CBD_PRINT("set cw: %d, %d", cw.val.c, cw.val.w);

}

void colorBasePortMgr::initImpl(colorBaseMgr &mgr)
{
    mgr.setColorCallback(colorCallbackImpl<colorType::RGB>);
    mgr.setColorCallback(colorCallbackImpl<colorType::HSV>);
    mgr.setColorCallback(colorCallbackImpl<colorType::CCTB>);
    mgr.setColorCallback(colorCallbackImpl<colorType::CW>);
    colorBasePort::gammaTable = mgr.getGammaMap();
    makeGammaTable(*colorBasePort::gammaTable, 256, 1.0, 8);
    (*colorBasePort::gammaTable)[2^8 - 1] -= 1;

#if COLORBASEPORT_PRINT
    for (auto it = colorBasePort::gammaTable->begin(); it != colorBasePort::gammaTable->end(); ++it) {
        CBD_PRINT("[gammaTable] key: %d, value: %d", it->first, it->second);
    }
#endif

}

void colorBasePortMgr::initImpl(colorBaseMgr &mgr, uint8_t bitDepth)
{
    mgr.setColorCallback(colorCallbackImpl<colorType::RGB>);
    mgr.setColorCallback(colorCallbackImpl<colorType::HSV>);
    mgr.setColorCallback(colorCallbackImpl<colorType::CCTB>);
    mgr.setColorCallback(colorCallbackImpl<colorType::CW>);
    colorBasePort::gammaTable = mgr.getGammaMap();
    makeGammaTable(*colorBasePort::gammaTable, 256, 1.0, bitDepth);
    (*colorBasePort::gammaTable)[2^bitDepth - 1] -= 1;
    
#if COLORBASEPORT_PRINT
    for (auto it = colorBasePort::gammaTable->begin(); it != colorBasePort::gammaTable->end(); ++it) {
        CBD_PRINT("[gammaTable] key: %d, value: %d", it->first, it->second);
    }
#endif

}

int colorBasePortMgr::getRandom(int min, int max)
{
    int tmp;
    int random = rand();

    if(min == max)
        return max;
    if(min > max){
        tmp = min;
        min = max;
        max = tmp;
    }
    return random % (max - min + 1) + min;
}

} /* colorBase */