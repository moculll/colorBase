#include <colorBase/colorBase.h>
#include <stdio.h>
#include <TimerEvents/TimerEvents.h>

#ifdef __linux__

#include <unistd.h>
#define TEST_SLEEP(ms) sleep(ms / 1000 ? ms / 1000 : 1)

#else

#include <windows.h>
#define TEST_SLEEP(ms) Sleep(ms)
#endif
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
        Color<colorType::RGB> rgb(25, 100, 100);
        Color<colorType::HSV> hsv(120, 100, 100);
        Color<colorType::CW> cw(255, 123);
        Color<colorType::CCTB> cctb(100, 100);
/*         rgb_unit_test();
        cw_unit_test(); */

        testcl testclass;
        testclass.color.init();
        testclass.color.setColorLoop<colorType::HSV>(0);
        TEST_SLEEP(3000);
        testclass.color.setBrightnessLoop<colorType::HSV>(4800);
        TEST_SLEEP(5000);
        testclass.color.setBrightnessLoop<colorType::CCTB>(4000);
        TEST_SLEEP(5000);
        testclass.color.setOnoffLinear(false);
        testclass.color.setColorLinear(cw);
        testclass.color.setOnoffLinear(false);
        testclass.color.setColorLoop<colorType::HSV>(3600);
        testclass.color.setColorLinear(rgb);
        testclass.color.setBrightnessLinear(100);
        TEST_SLEEP(1000);
        /* effects::convert(rgb, hsv);
        printf("r: %d, g: %d, b: %d, h: %d, s: %d, v: %d\n", rgb.val.r, rgb.val.g, rgb.val.b, hsv.val.h, hsv.val.s, hsv.val.v); */
        testclass.color.setColorLinear(hsv);
        TEST_SLEEP(1000);
        testclass.color.setBrightnessLinear(100);
        TEST_SLEEP(1000);
        testclass.color.setColorLinear(cw);
        TEST_SLEEP(1000);
        testclass.color.setBrightnessLinear(50);
        TEST_SLEEP(3000);

        testclass.color.setColorLoop<colorType::HSV>(3600);
        testclass.color.setColorLinear(cw);

        testclass.color.getColor<colorType::RGB>(rgb);
        rgb = rgb;
        cw = {0, 0};

        testclass.color.setOnoff(false);
        TEST_SLEEP(1000);
        cw = {30, 50};

        testclass.color.setColorLinear(cw);

        TEST_SLEEP(2000);
 
        testclass.color.setOnoff(false);
        TEST_SLEEP(1000);
        cw = {55, 22};

        testclass.color.setOnoff(true);
        TEST_SLEEP(3000);
        testclass.color.setColorLinear(cw);
        TEST_SLEEP(1000);
        testclass.color.setOnoffLinear(false);
        TEST_SLEEP(2000);
        testclass.color.setOnoffLinear(true);

        testclass.color.setColorLinear(rgb);
        TEST_SLEEP(3000);
        rgb = {0, 0, 0};

        testclass.color.setOnoff(false);
        TEST_SLEEP(1000);
        rgb = {30, 50, 213};
        testclass.color.setColorLoop<colorType::HSV>(3600);

        testclass.color.setColorLinear(rgb);

        TEST_SLEEP(2000);

        testclass.color.setOnoff(false);
        TEST_SLEEP(1000);
        rgb = {55, 22, 123};

        testclass.color.setOnoff(true);
        TEST_SLEEP(3000);
        testclass.color.setColorLinear(rgb);
        TEST_SLEEP(1000);
        testclass.color.setOnoffLinear(false);
        TEST_SLEEP(2000);
        testclass.color.setOnoffLinear(true);
        testclass.color.setColorLoop<colorType::HSV>(3600);
        TEST_SLEEP(3000);
    }

}