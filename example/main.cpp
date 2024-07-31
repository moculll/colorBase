#include <colorBase/colorBase.h>
#include <stdio.h>

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

class test {
public:
    test(){}
    colorBase::colorBaseMgr color;
private:
    

};
int main(int argc, char *argv[])
{
    {
        using namespace std;
        using namespace colorBase;
        Color<colorType::RGB> rgb;
        Color<colorType::HSV> hsv;
        Color<colorType::CW> cw;
        Color<colorType::CCTB> cctb;
        rgb_unit_test();
        cw_unit_test();
        test a;
        a.color.setColor(rgb);
        

    }

}