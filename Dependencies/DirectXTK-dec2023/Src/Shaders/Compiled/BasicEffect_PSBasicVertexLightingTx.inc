#if 0
//
// Generated by Microsoft (R) D3D Shader Disassembler
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// COLOR                    0   xyzw        0     NONE   float   xyzw
// COLOR                    1   xyzw        1     NONE   float   xyzw
// TEXCOORD                 0   xy          2     NONE   float   xy  
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_Target                0   xyzw        0   TARGET   float   xyzw
//
//
// Constant buffer to DX9 shader constant mappings:
//
// Target Reg Buffer  Start Reg # of Regs        Data Conversion
// ---------- ------- --------- --------- ----------------------
// c0         cb0            13         1  ( FLT, FLT, FLT, FLT)
//
//
// Sampler/Resource to DX9 shader sampler mappings:
//
// Target Sampler Source Sampler  Source Resource
// -------------- --------------- ----------------
// s0             s0              t0               
//
//
// Level9 shader bytecode:
//
    ps_2_0
    dcl t0  // pin<0,1,2,3>
    dcl t1  // pin<4,5,6,7>
    dcl t2.xy  // pin<8,9>
    dcl_2d s0

#line 551 "C:\Users\Mitch\Downloads\DirectXTK-dec2023\DirectXTK-dec2023\Src\Shaders\BasicEffect.fx"
    texld r0, t2, s0
    mul r0, r0, t0  // ::color<0,1,2,3>

#line 23 "C:\Users\Mitch\Downloads\DirectXTK-dec2023\DirectXTK-dec2023\Src\Shaders\Common.fxh"
    mad r1.xyz, t1, r0.w, r0  // AddSpecular::color<0,1,2>

#line 17
    mad r2.xyz, c0, r0.w, -r1
    mad r0.xyz, t1.w, r2, r1  // ApplyFog::color<0,1,2>
    mov oC0, r0  // ::PSBasicVertexLightingTx<0,1,2,3>

// approximately 6 instruction slots used (1 texture, 5 arithmetic)
ps_4_0
dcl_constantbuffer CB0[14], immediateIndexed
dcl_sampler s0, mode_default
dcl_resource_texture2d (float,float,float,float) t0
dcl_input_ps linear v0.xyzw
dcl_input_ps linear v1.xyzw
dcl_input_ps linear v2.xy
dcl_output o0.xyzw
dcl_temps 2
sample r0.xyzw, v2.xyxx, t0.xyzw, s0
mul r0.xyzw, r0.xyzw, v0.xyzw
mad r0.xyz, v1.xyzx, r0.wwww, r0.xyzx
mad r1.xyz, cb0[13].xyzx, r0.wwww, -r0.xyzx
mov o0.w, r0.w
mad o0.xyz, v1.wwww, r1.xyzx, r0.xyzx
ret 
// Approximately 0 instruction slots used
#endif

const BYTE BasicEffect_PSBasicVertexLightingTx[] =
{
     68,  88,  66,  67, 138, 173, 
    182, 253, 112, 215, 205,  19, 
    239,  63, 162, 143,  65,  82, 
     90,  69,   1,   0,   0,   0, 
    168,   5,   0,   0,   4,   0, 
      0,   0,  48,   0,   0,   0, 
    204,   3,   0,   0,  12,   5, 
      0,   0, 116,   5,   0,   0, 
     65, 111, 110,  57, 148,   3, 
      0,   0, 148,   3,   0,   0, 
      0,   2, 255, 255,  96,   3, 
      0,   0,  52,   0,   0,   0, 
      1,   0,  40,   0,   0,   0, 
     52,   0,   0,   0,  52,   0, 
      1,   0,  36,   0,   0,   0, 
     52,   0,   0,   0,   0,   0, 
      0,   0,  13,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   2, 255, 255, 254, 255, 
    175,   0,  68,  66,  85,  71, 
     40,   0,   0,   0, 144,   2, 
      0,   0,   0,   0,   0,   0, 
      2,   0,   0,   0, 212,   0, 
      0,   0,  10,   0,   0,   0, 
    220,   0,   0,   0,   5,   0, 
      0,   0,  44,   2,   0,   0, 
     44,   1,   0,   0,  67,  58, 
     92,  85, 115, 101, 114, 115, 
     92,  77, 105, 116,  99, 104, 
     92,  68, 111, 119, 110, 108, 
    111,  97, 100, 115,  92,  68, 
    105, 114, 101,  99, 116,  88, 
     84,  75,  45, 100, 101,  99, 
     50,  48,  50,  51,  92,  68, 
    105, 114, 101,  99, 116,  88, 
     84,  75,  45, 100, 101,  99, 
     50,  48,  50,  51,  92,  83, 
    114,  99,  92,  83, 104,  97, 
    100, 101, 114, 115,  92,  66, 
     97, 115, 105,  99,  69, 102, 
    102, 101,  99, 116,  46, 102, 
    120,   0,  67,  58,  92,  85, 
    115, 101, 114, 115,  92,  77, 
    105, 116,  99, 104,  92,  68, 
    111, 119, 110, 108, 111,  97, 
    100, 115,  92,  68, 105, 114, 
    101,  99, 116,  88,  84,  75, 
     45, 100, 101,  99,  50,  48, 
     50,  51,  92,  68, 105, 114, 
    101,  99, 116,  88,  84,  75, 
     45, 100, 101,  99,  50,  48, 
     50,  51,  92,  83, 114,  99, 
     92,  83, 104,  97, 100, 101, 
    114, 115,  92,  67, 111, 109, 
    109, 111, 110,  46, 102, 120, 
    104,   0,  40,   0,   0,   0, 
    128,   0,   0,   0,   0,   0, 
    255, 255, 196,   2,   0,   0, 
      0,   0, 255, 255, 208,   2, 
      0,   0,   0,   0, 255, 255, 
    220,   2,   0,   0,   0,   0, 
    255, 255, 232,   2,   0,   0, 
     39,   2,   0,   0, 244,   2, 
      0,   0,  39,   2,   0,   0, 
      4,   3,   0,   0,  23,   0, 
      1,   0,  20,   3,   0,   0, 
     17,   0,   1,   0,  40,   3, 
      0,   0,  17,   0,   1,   0, 
     60,   3,   0,   0,  17,   0, 
      1,   0,  80,   3,   0,   0, 
     80,  83,  66,  97, 115, 105, 
     99,  86, 101, 114, 116, 101, 
    120,  76, 105, 103, 104, 116, 
    105, 110, 103,  84, 120,   0, 
      1,   0,   3,   0,   1,   0, 
      4,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   9,   0, 
      0,   0,   0,   0,   1,   0, 
      2,   0,   3,   0,  65, 112, 
    112, 108, 121,  70, 111, 103, 
      0,  99, 111, 108, 111, 114, 
      0, 171,   1,   0,   3,   0, 
      1,   0,   4,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      8,   0,   0,   0,   0,   0, 
      1,   0,   2,   0, 255, 255, 
     65, 100, 100,  83, 112, 101, 
     99, 117, 108,  97, 114,   0, 
      6,   0,   0,   0,   0,   0, 
      1,   0,   2,   0, 255, 255, 
      5,   0,   0,   0,   0,   0, 
      1,   0,   2,   0,   3,   0, 
    112, 105, 110,   0,  68, 105, 
    102, 102, 117, 115, 101,   0, 
     83, 112, 101,  99, 117, 108, 
     97, 114,   0,  84, 101, 120, 
     67, 111, 111, 114, 100,   0, 
    171, 171,   1,   0,   3,   0, 
      1,   0,   2,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
    180,   1,   0,   0, 112,   1, 
      0,   0, 188,   1,   0,   0, 
    112,   1,   0,   0, 197,   1, 
      0,   0, 208,   1,   0,   0, 
      5,   0,   0,   0,   1,   0, 
     10,   0,   1,   0,   3,   0, 
    224,   1,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      2,   0,   3,   0,   1,   0, 
      0,   0,   4,   0,   5,   0, 
      6,   0,   7,   0,   2,   0, 
      0,   0,   8,   0,   9,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,  44,   1,   0,   0, 
     68,   1,   0,   0,   1,   0, 
      0,   0,  84,   1,   0,   0, 
     96,   1,   0,   0, 105,   1, 
      0,   0, 112,   1,   0,   0, 
      1,   0,   0,   0, 128,   1, 
      0,   0, 140,   1,   0,   0, 
    105,   1,   0,   0, 112,   1, 
      0,   0,   1,   0,   0,   0, 
    152,   1,   0,   0,   0,   0, 
      0,   0, 105,   1,   0,   0, 
    112,   1,   0,   0,   1,   0, 
      0,   0, 164,   1,   0,   0, 
     44,   1,   0,   0, 176,   1, 
      0,   0, 248,   1,   0,   0, 
      3,   0,   0,   0,   8,   2, 
      0,   0,  77, 105,  99, 114, 
    111, 115, 111, 102, 116,  32, 
     40,  82,  41,  32,  72,  76, 
     83,  76,  32,  83, 104,  97, 
    100, 101, 114,  32,  67, 111, 
    109, 112, 105, 108, 101, 114, 
     32,  49,  48,  46,  49,   0, 
     31,   0,   0,   2,   0,   0, 
      0, 128,   0,   0,  15, 176, 
     31,   0,   0,   2,   0,   0, 
      0, 128,   1,   0,  15, 176, 
     31,   0,   0,   2,   0,   0, 
      0, 128,   2,   0,   3, 176, 
     31,   0,   0,   2,   0,   0, 
      0, 144,   0,   8,  15, 160, 
     66,   0,   0,   3,   0,   0, 
     15, 128,   2,   0, 228, 176, 
      0,   8, 228, 160,   5,   0, 
      0,   3,   0,   0,  15, 128, 
      0,   0, 228, 128,   0,   0, 
    228, 176,   4,   0,   0,   4, 
      1,   0,   7, 128,   1,   0, 
    228, 176,   0,   0, 255, 128, 
      0,   0, 228, 128,   4,   0, 
      0,   4,   2,   0,   7, 128, 
      0,   0, 228, 160,   0,   0, 
    255, 128,   1,   0, 228, 129, 
      4,   0,   0,   4,   0,   0, 
      7, 128,   1,   0, 255, 176, 
      2,   0, 228, 128,   1,   0, 
    228, 128,   1,   0,   0,   2, 
      0,   8,  15, 128,   0,   0, 
    228, 128, 255, 255,   0,   0, 
     83,  72,  68,  82,  56,   1, 
      0,   0,  64,   0,   0,   0, 
     78,   0,   0,   0,  89,   0, 
      0,   4,  70, 142,  32,   0, 
      0,   0,   0,   0,  14,   0, 
      0,   0,  90,   0,   0,   3, 
      0,  96,  16,   0,   0,   0, 
      0,   0,  88,  24,   0,   4, 
      0, 112,  16,   0,   0,   0, 
      0,   0,  85,  85,   0,   0, 
     98,  16,   0,   3, 242,  16, 
     16,   0,   0,   0,   0,   0, 
     98,  16,   0,   3, 242,  16, 
     16,   0,   1,   0,   0,   0, 
     98,  16,   0,   3,  50,  16, 
     16,   0,   2,   0,   0,   0, 
    101,   0,   0,   3, 242,  32, 
     16,   0,   0,   0,   0,   0, 
    104,   0,   0,   2,   2,   0, 
      0,   0,  69,   0,   0,   9, 
    242,   0,  16,   0,   0,   0, 
      0,   0,  70,  16,  16,   0, 
      2,   0,   0,   0,  70, 126, 
     16,   0,   0,   0,   0,   0, 
      0,  96,  16,   0,   0,   0, 
      0,   0,  56,   0,   0,   7, 
    242,   0,  16,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      0,   0,   0,   0,  70,  30, 
     16,   0,   0,   0,   0,   0, 
     50,   0,   0,   9, 114,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  18,  16,   0,   1,   0, 
      0,   0, 246,  15,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
     50,   0,   0,  11, 114,   0, 
     16,   0,   1,   0,   0,   0, 
     70, 130,  32,   0,   0,   0, 
      0,   0,  13,   0,   0,   0, 
    246,  15,  16,   0,   0,   0, 
      0,   0,  70,   2,  16, 128, 
     65,   0,   0,   0,   0,   0, 
      0,   0,  54,   0,   0,   5, 
    130,  32,  16,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  50,   0, 
      0,   9, 114,  32,  16,   0, 
      0,   0,   0,   0, 246,  31, 
     16,   0,   1,   0,   0,   0, 
     70,   2,  16,   0,   1,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,  62,   0, 
      0,   1,  73,  83,  71,  78, 
     96,   0,   0,   0,   3,   0, 
      0,   0,   8,   0,   0,   0, 
     80,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,  15,  15,   0,   0, 
     80,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   1,   0, 
      0,   0,  15,  15,   0,   0, 
     86,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   2,   0, 
      0,   0,   3,   3,   0,   0, 
     67,  79,  76,  79,  82,   0, 
     84,  69,  88,  67,  79,  79, 
     82,  68,   0, 171,  79,  83, 
     71,  78,  44,   0,   0,   0, 
      1,   0,   0,   0,   8,   0, 
      0,   0,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,  15,   0, 
      0,   0,  83,  86,  95,  84, 
     97, 114, 103, 101, 116,   0, 
    171, 171
};