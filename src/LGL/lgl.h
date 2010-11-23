/*
 *
 * LightGL - Core Library
 * A small and simple software rasterization library with vertex and fragment shader support.
 * Copyright(c) 2010 by Christoph Schunk. All rights reserved.
 *
 */

#ifndef LGL_H_INCLUDED
#define LGL_H_INCLUDED

#define LGL_MAX_TEXTURES       8
#define LGL_MAX_UNIFORMS      16
#define LGL_MAX_ATTRIBUTES     8
#define LGL_MAX_VARYINGS       8

typedef unsigned int LGLuint;
typedef int LGLint;
typedef unsigned short LGLushort;
typedef short LGLshort;
typedef unsigned char LGLbyte;
typedef float LGLfloat;
typedef unsigned int LGLsize;
typedef void* LGLdata;
typedef unsigned int LGLtexel;

typedef struct LGLcolor_s {
	LGLfloat r, g, b, a;
} LGLcolor;

typedef struct LGLv2f_s {
	LGLfloat x, y;
} LGLv2f;

typedef struct LGLv3f_s {
	LGLfloat x, y, z;
} LGLv3f;

typedef struct LGLv4f_s {
	LGLfloat x, y, z, w;
} LGLv4f;

typedef struct LGLm4x4f_s {
	LGLfloat m11, m12, m13, m14;
	LGLfloat m21, m22, m23, m24;
	LGLfloat m31, m32, m33, m34;
	LGLfloat m41, m42, m43, m44;
} LGLm4x4f;

typedef struct LGLtexture1d_s {
	LGLtexel* data;
	LGLuint width;
} LGLtexture1d;

typedef struct LGLtexture2d_s {
	LGLtexel* data;
	LGLuint width, height;
} LGLtexture2d;

typedef struct LGLtexture3d_s {
	LGLtexel* data;
	LGLuint width, height, depth;
} LGLtexture3d;

typedef struct LGLtexturecube_s {
	LGLtexel* data[6];
	LGLuint width, height;
} LGLtexturecube;

typedef union LGLtexture_u {
	LGLtexture1d t1d;
	LGLtexture2d t2d;
	LGLtexture3d t3d;
	LGLtexturecube tcube;
} LGLtexture;

typedef union LGLuniform_u {
	LGLfloat f;
	LGLv2f v2;
	LGLv3f v3;
	LGLv4f v4;
	LGLm4x4f m4x4;
} LGLuniform;

typedef union LGLvarying_u {
	LGLfloat f;
	LGLv2f v2;
	LGLv3f v3;
	LGLv4f v4;
} LGLvarying;

typedef union LGLattribute_u {
	LGLfloat* f;
	LGLv2f* v2;
	LGLv3f* v3;
	LGLv4f* v4;
} LGLattribute;

typedef struct LGLvsin_s {
	LGLuint       index;
	LGLv3f*       vertex_stream;
	LGLsize       vertex_stream_elements;
	LGLuint*      index_stream;
	LGLsize       index_stream_elements;
	LGLtexture    textures[LGL_MAX_TEXTURES];
	LGLuniform    uniforms[LGL_MAX_UNIFORMS];
	LGLattribute  attributes[LGL_MAX_ATTRIBUTES];
	LGLsize       num_attributes[LGL_MAX_ATTRIBUTES];
} LGLvsin;

typedef struct LGLvsout_s {
	LGLv3f position;
	LGLvarying varyings[LGL_MAX_VARYINGS];
} LGLvsout;

typedef struct LGLfsin_s {
	LGLfloat a, b, c;
	LGLvarying varyings[3][LGL_MAX_VARYINGS];
	LGLtexture textures[LGL_MAX_TEXTURES];
	LGLuniform uniforms[LGL_MAX_UNIFORMS];
} LGLfsin;

typedef struct LGLfsout_s {
	LGLcolor color;
} LGLfsout;

typedef enum LGLclear_e {
	LGL_CLEAR_FRAMEBUFFER = 1, LGL_CLEAR_ZBUFFER = 2
} LGLclear;

typedef enum LGLsourceformat_e {
	LGL_SOURCE_FORMAT_V2F, LGL_SOURCE_FORMAT_V3F, LGL_SOURCE_FORMAT_V3UI
} LGLsourceformat;

typedef enum LGLdrawtype_e {
	LGL_DRAW_TYPE_TRIANGLE_LIST
} LGLdrawtype;

typedef struct LGLFramebufferinfo_s {
	void* framebuffer;
	void* zbuffer;
	LGLuint width, height;
	LGLbyte rmask, gmask, bmask;
	LGLbyte rshift, gshift, bshift;
} LGLFramebufferinfo;

typedef void (*LGLvertexshader)(LGLvsout* out, const LGLvsin* in);
typedef void (*LGLfragmentshader)(LGLfsout* out, const LGLfsin* in);

typedef struct LGLcontext_s LGLcontext;

/* Context functions */

LGLcontext* lglCreateContext(const LGLFramebufferinfo* fbinfo);
LGLFramebufferinfo* lglGetFBInfo(LGLcontext* context);
void lglDestroyContext(LGLcontext* context);

/* Texture functions */

void lglSetTextureData2d(LGLcontext* context, LGLuint index, LGLtexel* data, LGLuint width, LGLuint height);
LGLtexel lglGetTex2d(const LGLcontext* context, LGLuint index, const LGLv2f* v);

/* Shader constants */

void lglSetUniformf(LGLcontext* context, LGLuint index, LGLfloat v);
void lglSetUniformv2f(LGLcontext* context, LGLuint index, const LGLv2f* v);
void lglSetUniformv3f(LGLcontext* context, LGLuint index, const LGLv3f* v);
void lglSetUniformv4f(LGLcontext* context, LGLuint index, const LGLv4f* v);
void lglSetUniformm4x4f(LGLcontext* context, LGLuint index, const LGLm4x4f* v);

/* Vertex/Index buffer functions */

void lglSetVertexStream(LGLcontext* context, LGLv3f vertices[], LGLsize elems);
void lglSetIndexStream(LGLcontext* context, LGLuint indices[], LGLsize elems);

/* Vertex attribute functions */

void lglSetVertexAttribsf(LGLcontext* context, LGLuint index, LGLfloat* v, LGLsize elems);
void lglSetVertexAttribsv2f(LGLcontext* context, LGLuint index, LGLv2f* v, LGLsize elems);
void lglSetVertexAttribsv3f(LGLcontext* context, LGLuint index, LGLv3f* v, LGLsize elems);
void lglSetVertexAttribsv4f(LGLcontext* context, LGLuint index, LGLv4f* v, LGLsize elems);

/* Shader function */

void lglSetVertexShader(LGLcontext* context, LGLvertexshader vsproc);
void lglSetFragmentShader(LGLcontext* context, LGLfragmentshader fsproc);

/* Draw functions */

void lglViewport(const LGLcontext* context, LGLint x, LGLint y, LGLsize width, LGLsize height);
void lglClear(const LGLcontext* context, LGLclear clear);
void lglDrawIndexed(const LGLcontext* context, LGLdrawtype type);

#endif
