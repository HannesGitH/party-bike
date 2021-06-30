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

Color change_hue_c(const Color &in, const float fHue)
{
    Color out;
    const float cosA = cos(fHue*3.14159265f/180); //convert degrees to radians
    const float sinA = sin(fHue*3.14159265f/180); //convert degrees to radians
    //calculate the rotation matrix, only depends on Hue
    float matrix[3][3] = {{cosA + (1.0f - cosA) / 3.0f, 1.0f/3.0f * (1.0f - cosA) - sqrtf(1.0f/3.0f) * sinA, 1.0f/3.0f * (1.0f - cosA) + sqrtf(1.0f/3.0f) * sinA},
        {1.0f/3.0f * (1.0f - cosA) + sqrtf(1.0f/3.0f) * sinA, cosA + 1.0f/3.0f*(1.0f - cosA), 1.0f/3.0f * (1.0f - cosA) - sqrtf(1.0f/3.0f) * sinA},
        {1.0f/3.0f * (1.0f - cosA) - sqrtf(1.0f/3.0f) * sinA, 1.0f/3.0f * (1.0f - cosA) + sqrtf(1.0f/3.0f) * sinA, cosA + 1.0f/3.0f * (1.0f - cosA)}};
    //Use the rotation matrix to convert the RGB directly
    out.r = clamp(in.r*matrix[0][0] + in.g*matrix[0][1] + in.b*matrix[0][2]);
    out.g = clamp(in.r*matrix[1][0] + in.g*matrix[1][1] + in.b*matrix[1][2]);
    out.b = clamp(in.r*matrix[2][0] + in.g*matrix[2][1] + in.b*matrix[2][2]);
    return out;
}

irgb_t change_hue(irgb_t in, const float fHue){
    return
        irgb_from_color(
            change_hue_c(
                Color{
                    .r = dim_from_irgb(in,red),
                    .g = dim_from_irgb(in,green),
                    .b = dim_from_irgb(in,blue),
                },
                fHue
            )
        )
    ;
}