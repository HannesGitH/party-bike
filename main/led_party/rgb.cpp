#include "rgb.h"
#include "math.h"

irgb_t iRGB
(
    uint8_t R,
    uint8_t G,
    uint8_t B
){
    return (uint32_t)R<<16 | (uint32_t)G<<8 | (uint32_t)B;
}

irgb_t invert(irgb_t iRGB){
    return (iRGB ^ 0x00FFFFFF)+1;
}

irgb_t lighten_fast(irgb_t iRGB, int8_t amount){
    return (iRGB + (0x010101*amount))&0xFFFFFF; //fast but overflows wrongly
}

//this isnt really working since the int multiplication does crap id its not unsigned
irgb_t lighten_wrong(irgb_t iRGB, int8_t amount){
    return 
    ((iRGB + (0x010000*amount)) & 0xFF0000) |
    ((iRGB + (0x000100*amount)) & 0x00FF00) |
    ((iRGB + (0x000001*amount)) & 0x0000FF) 
    ; //it was at this moment he knew.. he fucked up (should've used a color struct instead of an int?)
}

struct Color{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

irgb_t irgb_from_color
(
    Color c
){
    return iRGB(c.r,c.g,c.b);
}


uint8_t dim_from_irgb(irgb_t irgb, RGB_dim dim){
    return (irgb>>(8*(2-dim)))&0xFF;
}

uint8_t clamp(float v) //define a function to bound and round the input float value to 0-255
{
    if (v < 0)
        return 0;
    if (v > 255)
        return 255;
    return (uint8_t)v;
}

//done? make saturation and value change possible as well ( see http://beesbuzz.biz/code/16-hsv-color-transforms )
Color change_hsv_c(
    const Color &in, 
    const float fHue,
    const float fSat,
    const float fVal
)
{
    Color out;
    const float cosA = fSat*cos(fHue*3.14159265f/180); //convert degrees to radians
    const float sinA = fSat*sin(fHue*3.14159265f/180); //convert degrees to radians

    //helpers for faster calc //first 2 could actually be precomputed
    const float aThird = 1.0f/3.0f;
    const float rootThird = sqrtf(aThird);
    const float oneMinusCosA = (1.0f - cosA);
    const float aThirdOfOneMinusCosA = aThird * oneMinusCosA;
    const float rootThirdTimesSinA =  rootThird * sinA;
    const float plus = aThirdOfOneMinusCosA +rootThirdTimesSinA;
    const float minus = aThirdOfOneMinusCosA -rootThirdTimesSinA;

    //calculate the rotation matrix, only depends on Hue
    float matrix[3][3] = {
        {   cosA + oneMinusCosA / 3.0f  , minus                         , plus                          },
        {   plus                        , cosA + aThirdOfOneMinusCosA   , minus                         },
        {   minus                       , plus                          , cosA + aThirdOfOneMinusCosA   }
    };
    //Use the rotation matrix to convert the RGB directly
    out.r = clamp((in.r*matrix[0][0] + in.g*matrix[0][1] + in.b*matrix[0][2])*fVal);
    out.g = clamp((in.r*matrix[1][0] + in.g*matrix[1][1] + in.b*matrix[1][2])*fVal);
    out.b = clamp((in.r*matrix[2][0] + in.g*matrix[2][1] + in.b*matrix[2][2])*fVal);
    return out;
}


irgb_t change_hsv(
    irgb_t in, 
    const float fHue,
    const float fSat,
    const float fVal
){
    return
        irgb_from_color(
            change_hsv_c(
                Color{
                    .r = dim_from_irgb(in,red),
                    .g = dim_from_irgb(in,green),
                    .b = dim_from_irgb(in,blue),
                },
                fHue,
                fSat,
                fVal
            )
        )
    ;
}

irgb_t change_hue(irgb_t in, const float fHue){
    return change_hsv(in,fHue,1,1);
}