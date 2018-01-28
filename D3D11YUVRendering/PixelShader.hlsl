// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
//----------------------------------------------------------------------

Texture2D tx : register( t0 );
SamplerState samLinear : register( s0 );

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD;
};

float3 yuv_to_rgb(float3 YUV)
{

	/*Y'= 0.299*R' + 0.587*G' + 0.114*B'

		U'= -0.147*R' - 0.289*G' + 0.436*B' = 0.492*(B'- Y')

		V'= 0.615*R' - 0.515*G' - 0.100*B' = 0.877*(R'- Y')

		R' = Y' + 1.140*V'

		G' = Y' - 0.394*U' - 0.581*V'

		B' = Y' + 2.032*U'*/


	float u = YUV.y;
	float v = YUV.z;
	float r = YUV.x + 1.403f*v;
	float g = YUV.x - 0.344f*u - 1.403f*v;
	float b = YUV.x + 1.770f*u;

	/*if (r < 0.018 && g < 0.018 && b < 0.018)
	{*/
		/*r = (r < 0.018) ? 4.5*r : 1.099*pow(r, 0.45) - 0.099;

		g = (g < 0.018) ? 4.5*g : 1.099*pow(g, 0.45) - 0.099;

		b = (b < 0.018) ? 4.5*b : 1.099*pow(b, 0.45) - 0.099;*/
	/*}
	else
	{
		r = 1.099*pow(r, 0.45) - 0.099;

		g = 1.099*pow(g, 0.45) - 0.099;

		b = 1.099*pow(b, 0.45) - 0.099;
	}*/

	return float3(r, g, b);
}

float3 rgb_to_yuv(float3 RGB)
{
	float y = dot(RGB, float3(0.299, 0.587, 0.114));
	float u = (RGB.z - y) * 0.565;
	float v = (RGB.x - y) * 0.713;
	return float3(y, u, v);
}

float3 rgb_to_yuv1(float3 RGB)
{
	float y = RGB.x * 0.299 + RGB.y * 0.587 + RGB.z * 0.114;
	float u = -RGB.x * 0.169 - RGB.y * 0.331 + RGB.z * 0.500;
	float v = RGB.x * 0.500 - RGB.y * 0.419 - RGB.z * 0.081;
	return float3(y, u, v);
}

float3 yuv_to_rgb1(float3 YUV)
{
	float u = YUV.y;
	float v = YUV.z;
	float r = YUV.x + 1.400*v;
	float g = YUV.x - 0.343*u - 0.711*v;
	float b = YUV.x + 1.765*u;

	return float3(r, g, b);
}

float3 yuv_to_rgb2(float3 YUV)
{
	float u = YUV.y;
	float v = YUV.z;
	float r = YUV.x + 1.140*v;
	float g = YUV.x - 0.395*u - 0.581*v;
	float b = YUV.x + 2.032*u;

	return float3(r, g, b);
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
	float4 yuva = tx.Sample(samLinear, input.Tex);
	//return yuva; //For RGB
//YUV to RGB uncomment the below 2 lines
	yuva -= float4(0, .5f, .5f, 0);
	return float4(yuv_to_rgb2(yuva.xyz), yuva.w); // don't lose alpha
}