#include <colorBase/colorBase.h>
#include <stdio.h>
#include <TimerEvents/TimerEvents.h>
#include <windows.h>
void rgb_unit_test()
{
    using namespace std;
    using namespace colorBase;
    Color<colorType::RGB> rgb(200, 200, 255);
    Color<colorType::HSV> hsv;
    printf("origin rgb: %d %d %d, hsv: %d %d %d\n", rgb.val.r, rgb.val.g, rgb.val.b, hsv.val.h, hsv.val.v, hsv.val.v);
    effects::convert(rgb, hsv);
    printf("converted rgb: %d %d %d, hsv: %d %d %d\n", rgb.val.r, rgb.val.g, rgb.val.b, hsv.val.h, hsv.val.v, hsv.val.v);

}
void cw_unit_test()
{
    using namespace colorBase;
    Color<colorType::CCTB> cctb(53, 26);
    Color<colorType::CW> cw;
    printf("origin cctb: %d %d, cw: %d %d\n", cctb.val.cct, cctb.val.b, cw.val.c, cw.val.w);
    effects::convert(cctb, cw);
    printf("converted cctb: %d %d, cw: %d %d\n", cctb.val.cct, cctb.val.b, cw.val.c, cw.val.w);
    effects::convert(cw, cctb);
    printf("converted cctb: %d %d, cw: %d %d\n", cctb.val.cct, cctb.val.b, cw.val.c, cw.val.w);

}

class testcl {
public:
    testcl(){}
    colorBase::colorBaseMgr color;
private:
    

};

void test(void *arg)
{
    printf("test\n");

}
int main(int argc, char *argv[])
{
    {
        using namespace std;
        using namespace colorBase;
        Color<colorType::RGB> rgb(123, 234, 122);
        Color<colorType::HSV> hsv;
        Color<colorType::CW> cw;
        Color<colorType::CCTB> cctb;
/*         rgb_unit_test();
        cw_unit_test(); */
        testcl testclass;
/*         testclass.color.startColorLoop<colorType::HSV>(3600); */
        testclass.color.startColorLinear(rgb);
        Sleep(100);
        testclass.color.setOnoffLinear(true);
        Sleep(100);
        testclass.color.setOnoffLinear(true);

        Sleep(1000);
        testclass.color.setOnoffLinear(false);
        Sleep(1000);
        testclass.color.setOnoffLinear(true);
        /* TimerEvents::TimerEvents timer(true, 1000, test, nullptr); // Fire every 1000ms (1 second)
        timer.Execute(); */
        
        Sleep(1000000);
    }

}