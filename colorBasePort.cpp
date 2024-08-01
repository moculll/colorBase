#include <colorBase/colorBase.h>
#include <stdio.h>

#define COLORBASEPORT_PRINT 1
#if COLORBASEPORT_PRINT
#define CBD_PRINT(format, ...) \
            do { \
                printf("[colorBasePort][%s]: " format "\r\n", __func__, ##__VA_ARGS__); \
            } while(0)
#else
#define CBD_PRINT(format, ...)
#endif


namespace colorBase {

template <colorType T>
void colorCallbackImpl(Color<T> &rgb);

template <>
void colorCallbackImpl<colorType::RGB>(Color<colorType::RGB> &rgb)
{
    CBD_PRINT("set rgb: %d, %d, %d", rgb.val.r, rgb.val.g, rgb.val.b);

}

template <>
void colorCallbackImpl<colorType::HSV>(Color<colorType::HSV> &hsv)
{
    CBD_PRINT("set hsv: %d, %d, %d", hsv.val.h, hsv.val.s, hsv.val.v);

}
template <>
void colorCallbackImpl<colorType::CCTB>(Color<colorType::CCTB> &cctb)
{
    CBD_PRINT("set cctb: %d, %d", cctb.val.cct, cctb.val.b);

}
template <>
void colorCallbackImpl<colorType::CW>(Color<colorType::CW> &cw)
{
    CBD_PRINT("set cw: %d, %d, %d", cw.val.c, cw.val.w);

}

void colorBasePortMgr::initImpl(colorBaseMgr &mgr)
{
    mgr.setColorCallback(colorCallbackImpl<colorType::RGB>);
    mgr.setColorCallback(colorCallbackImpl<colorType::HSV>);
    mgr.setColorCallback(colorCallbackImpl<colorType::CCTB>);
    mgr.setColorCallback(colorCallbackImpl<colorType::CW>);


}


}