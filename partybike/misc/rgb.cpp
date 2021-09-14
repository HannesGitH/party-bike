#include "rgb.h"

irgb_t iRGB
(
    uint8_t R,
    uint8_t G,
    uint8_t B
){
    return {
        .r = R,
        .g = G,
        .b = B,
        .i = 0
    };
}

irgb_t invert(irgb_t iRGB){
    return {
        .r = iRGB.r ^ 0xFF,
        .g = iRGB.g ^ 0xFF,
        .b = iRGB.b ^ 0xFF,
        .i = iRGB.i ^ 0xFF
    };
}

irgb_t lighten_fast(irgb_t iRGB, int8_t amount){
    return {
        .r = iRGB.r + amount,
        .g = iRGB.g + amount,
        .b = iRGB.b + amount,
        .i = iRGB.i
    };
}

uint8_t clamp(float v) //define a function to bound and round the input float value to 0-255
{
    if (v < 0)
        return 0;
    if (v > 255)
        return 255;
    return (uint8_t)v;
}

irgb_t change_hsv(
    const irgb_t in, 
    const float fHue,
    const float fSat,
    const float fVal
)
{
    irgb_t out;
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

irgb_t change_hue(irgb_t in, const float fHue){
    return change_hsv(in,fHue,1.02,1.0);
}