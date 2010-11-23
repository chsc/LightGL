/*
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "LGL/lgl.h"
#include "LGL/lglu.h"
#include "3ds/3ds.h"
#include "tga/tga.h"
#include "scene.h"

/* Uniforms */
#define UNI_MVP_MATRIX      0 /* model * view * projection */
#define UNI_M_MATRIX        1 /* model */
#define UNI_MIT_MATRIX      2 /* model inverse transpose */
#define UNI_LIGHT_POSITION  3 /* point light position */

/* Attributes */
#define ATR_POSITION  0 /* normal vector */
#define ATR_NORMAL    1 /* normal vector */
#define ATR_TEXCOORD  2 /* texcoord vector */

/* Textures */
#define TEX_DIFFUSE 0

LGLcontext* context;
TGA* tex_stone;
TGA* tex_wood;
A3DS* msh_monkey;
LGLfloat global_time = 0.0f;

LGLv3f vertices[6];
LGLuint indices[6];

LGLv3f light_pos;

void vsTransform(LGLvsout* out, const LGLvsin* in) {
	lgluTransform(&out->position, &in->uniforms[UNI_MVP_MATRIX].m4x4, &in->vertex_stream[in->index]);
	//out->varyings[ATR_NORMAL].v3 = in->attributes[ATR_NORMAL].v3[in->index];
	lgluTransform(&out->varyings[ATR_POSITION].v3, &in->uniforms[UNI_M_MATRIX].m4x4, &in->vertex_stream[in->index]);
	lgluTransform(&out->varyings[ATR_NORMAL].v3, &in->uniforms[UNI_MIT_MATRIX].m4x4,
			&in->attributes[ATR_NORMAL].v3[in->index]);
}

void fsBaryToColor(LGLfsout* out, const LGLfsin* in) {
	out->color.r = in->a;
	out->color.g = in->b;
	out->color.b = in->c;
}

void fsDiffuse(LGLfsout* out, const LGLfsin* in) {
	LGLv3f n;
	LGLv3f l;
	LGLv3f d;

	lgluInterpolatev3f(&n, in->a, &in->varyings[0][ATR_NORMAL].v3, in->b, &in->varyings[1][ATR_NORMAL].v3, in->c,
			&in->varyings[2][ATR_NORMAL].v3);
	lgluVectorNormalize(&n);

	l.x = -10;
	l.y = 10;
	l.z = 10;

	d.x = l.x - in->varyings[0][ATR_POSITION].v3.x;
	d.y = l.y - in->varyings[1][ATR_POSITION].v3.y;
	d.z = l.z - in->varyings[2][ATR_POSITION].v3.z;
	lgluVectorNormalize(&d);

	LGLfloat di = lgluVectorDot(&n, &d);
	if (di <= 0)
		di = 0.0f;

	out->color.r = di; //n.x * 0.5 + 0.5;
	out->color.g = di; //n.y * 0.5 + 0.5;
	out->color.b = di; //n.z * 0.5 + 0.5;
}

int sceneInit(unsigned int* pixels, int w, int h, int rshift, int gshift, int bshift) {
	LGLFramebufferinfo fbinfo;

	fbinfo.framebuffer = pixels;
	fbinfo.zbuffer = malloc(sizeof(unsigned short) * w * h); /* always 16 bit! */
	fbinfo.width = w;
	fbinfo.height = h;
	fbinfo.rshift = rshift;
	fbinfo.gshift = gshift;
	fbinfo.bshift = bshift;

	context = lglCreateContext(&fbinfo);
	if (context == NULL) {
		fprintf(stderr, "Could not create rendering context.\n");
		return -1;
	}

	tex_stone = tgaLoad("data/stone.tga");
	tex_wood = tgaLoad("data/wood.tga");

	if (tex_stone == NULL || tex_wood == NULL) {
		return -1;
	}

	msh_monkey = a3dsLoad("data/monkey.3ds");

	if (msh_monkey == NULL) {
		return -1;
	}

	vertices[0].x = 1.0f;
	vertices[0].y = 1.0f;
	vertices[0].z = 1.0f;

	vertices[1].x = -1.0f;
	vertices[1].y = 1.0f;
	vertices[1].z = -1.0f;

	vertices[2].x = 0.0f;
	vertices[2].y = -1.0f;
	vertices[2].z = -1.0f;

	vertices[3].x = -1.0f;
	vertices[3].y = 1.0f;
	vertices[3].z = 1.0f;

	vertices[4].x = 0.0f;
	vertices[4].y = -1.0f;
	vertices[4].z = -1.0f;

	vertices[5].x = 1.0f;
	vertices[5].y = 1.0f;
	vertices[5].z = -1.0f;

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	indices[3] = 3;
	indices[4] = 4;
	indices[5] = 5;

	return 1;
}

void sceneUpdate(float dtime) {
	LGLm4x4f mvp, mit, m, v, p, s, r, t;

	global_time += dtime * 0.5f;

	lgluMatrixSetScaling(&s, 0.25, -0.25, 0.25);
	lgluMatrixSetRotationY(&r, global_time);
	lgluMatrixSetTranslation(&t, 0.15, 0.0f, 0.0f);

	//lgluMatrixSetFrustum(&p, -1.0f, 1.0f, -1.0f, 1.0f, 0.5f, 20.0f);
	//lgluMatrixSetOrtho(&p, -1.0f, 1.0f, -1.0f, 1.0f, -10.0f, 10.0f);
	lgluMatrixSetIdentity(&p);

	lgluMatrixMultiply(&m, &r, &t);
	lgluMatrixMultiply(&m, &m, &s);

	lgluMatrixInverse(&mit, &m);
	lgluMatrixTranspose(&mit, &mit);

	lgluMatrixMultiply(&mvp, &p, &m);

	lglSetUniformm4x4f(context, UNI_MVP_MATRIX, &mvp);
	lglSetUniformm4x4f(context, UNI_M_MATRIX, &m);
	lglSetUniformm4x4f(context, UNI_MIT_MATRIX, &m);
	lglSetUniformv3f(context, UNI_LIGHT_POSITION, &light_pos);
}

void sceneRender() {
	lglClear(context, LGL_CLEAR_FRAMEBUFFER | LGL_CLEAR_ZBUFFER);

	lglSetFragmentShader(context, fsDiffuse);
	lglSetVertexShader(context, vsTransform);

	lglSetVertexStream(context, vertices, 6);
	lglSetIndexStream(context, indices, 6);

	lglSetVertexStream(context, (LGLv3f*) msh_monkey->vertices[0], msh_monkey->num_vertices[0]);
	lglSetVertexAttribsv3f(context, ATR_NORMAL, (LGLv3f*) msh_monkey->normals[0], msh_monkey->num_vertices[0]);
	//	lglSetVertexAttribsv2f(context, ATR_TEXCOORD, (LGLv2f*) (msh_monkey->texcoords[0]), msh_monkey->num_vertices[0]);
	lglSetIndexStream(context, msh_monkey->indices[0], msh_monkey->num_indices[0]);

	//lglSetTextureData2d(context, TEX_DIFFUSE, tex_stone->pixels, tex_stone->width, tex_stone->height);

	lglDrawIndexed(context, LGL_DRAW_TYPE_TRIANGLE_LIST);
}

void sceneClose() {
	tgaUnload(tex_stone);
	tgaUnload(tex_wood);
	a3dsUnload(msh_monkey);
	free(lglGetFBInfo(context)->zbuffer);
	lglDestroyContext(context);
}
