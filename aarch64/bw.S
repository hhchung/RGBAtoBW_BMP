
//extern void rgbaToBw_neon(int32_t *data, int width, int height, long stride);
    .global rgbaToBw_neon
    .type   rgbaToBw_neon, @function


rgbaToBw_neon:
    mov w12, w2
    lsr w1, w1, #4 //width /= 16
    mov x15, x0
    movi   v16.16b, #77
    movi   v17.16b, #150
    movi   v18.16b, #29
160:
    //mov x0, x15
    mov w11, w1
16:
    ld4 {v0.16b-v3.16b}, [x0]
    //r: v0
    //g: v1
    //b: v2
    //a: v3
    umull   v4.8h,  v0.8b,   v16.8b 
    umull2  v5.8h,  v0.16b,  v16.16b
    umlal   v4.8h,  v1.8b,   v17.8b
    umlal2  v5.8h,  v1.16b,  v17.16b
    umlal   v4.8h,  v2.8b,   v18.8b
    umlal2  v5.8h,  v2.16b,  v18.16b
    
    shrn    v0.8b,  v4.8h, #8   //shift 256
    shrn2   v0.16b, v5.8h, #8
    mov     v1.16b, v0.16b
    mov     v2.16b, v0.16b
  
    subs    w11, w11, #1
    st4     {v0.16b-v3.16b}, [x0], #64

    bgt    16b
    //add   x15, x15, x3
    subs   w12, w12, #1
    bgt    160b
    ret









