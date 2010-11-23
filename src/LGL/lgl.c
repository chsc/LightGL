/*
 *
 * LightGL - Core Library
 * A small and simple software rasterization library with vertex and fragment shader support.
 * Copyright(c) 2010 by Christoph Schunk. All rights reserved.
 *
 */

#include <stdlib.h> /* for malloc and free */
#include <string.h> /* for memset and memcpy */
#include <assert.h>
#include "lgl.h"

typedef struct LGLcontext_s {
	LGLFramebufferinfo fbinfo;

	LGLint vport_x, vport_y;
	LGLsize vport_width, vport_height;

	LGLvertexshader vertex_shader;
	LGLfragmentshader fragment_shader;

	LGLv3f* vertex_stream;
	LGLsize vertex_stream_elements;
	LGLuint* index_stream;
	LGLsize index_stream_elements;

	LGLtexture textures[LGL_MAX_TEXTURES];
	LGLuniform uniforms[LGL_MAX_UNIFORMS];
	LGLattribute attributes[LGL_MAX_ATTRIBUTES];
	LGLsize num_attributes[LGL_MAX_ATTRIBUTES];
} LGLcontext_t;

/*
 *  Context functions
 */

LGLcontext* lglCreateContext(const LGLFramebufferinfo* fbinfo) {
	LGLcontext* context;

	context = malloc(sizeof(LGLcontext));
	if (context == NULL) {
		return NULL;
	}

	context->vport_x = 0;
	context->vport_y = 0;
	context->vport_width = fbinfo->width;
	context->vport_height = fbinfo->height;

	context->fbinfo = *fbinfo;

	context->vertex_stream = NULL;
	context->vertex_stream_elements = 0;
	context->index_stream = NULL;
	context->index_stream_elements = 0;

	context->fragment_shader = NULL;
	context->vertex_shader = NULL;

	memset(context->textures, 0, sizeof(context->textures));
	memset(context->uniforms, 0, sizeof(context->uniforms));
	memset(context->attributes, 0, sizeof(context->attributes));
	memset(context->num_attributes, 0, sizeof(context->num_attributes));

	return context;
}

LGLFramebufferinfo* lglGetFBInfo(LGLcontext* context) {
	assert(context != NULL);
	return &context->fbinfo;
}

void lglDestroyContext(LGLcontext* context) {
	assert(context != NULL);
	free(context);
}

/*
 *  Texture functions
 */

void lglSetTextureData2d(LGLcontext* context, LGLuint index, LGLtexel* data, LGLuint width, LGLuint height) {
	assert(context != NULL);
	assert(index < LGL_MAX_TEXTURES);
	assert(data != NULL);
	context->textures[index].t2d.data = data;
	context->textures[index].t2d.width = width;
	context->textures[index].t2d.height = height;
}

LGLtexel lglGetTex2d(const LGLcontext* context, LGLuint index, const LGLv2f* v) {
	assert(context);
	assert(index < LGL_MAX_TEXTURES);
	assert(v != NULL);
	const LGLtexel* data = context->textures[index].t2d.data;
	const LGLuint width = context->textures[index].t2d.width;
	const LGLuint height = context->textures[index].t2d.height;
	const LGLfloat x = (v->x - (LGLint) v->x) * width;
	const LGLfloat y = (v->y - (LGLint) v->y) * height;
	assert(x < width);
	assert(y < height);
	return data[(LGLint) y * width + (LGLuint) x];
}

/* Shader constants */

void lglSetUniformf(LGLcontext* context, LGLuint index, LGLfloat v) {
	assert(context != NULL);
	assert(index < LGL_MAX_UNIFORMS);
	context->uniforms[index].f = v;
}

void lglSetUniformv2f(LGLcontext* context, LGLuint index, const LGLv2f* v) {
	assert(context != NULL);
	assert(index < LGL_MAX_UNIFORMS);
	context->uniforms[index].v2 = *v;
}

void lglSetUniformv3f(LGLcontext* context, LGLuint index, const LGLv3f* v) {
	assert(context != NULL);
	assert(index < LGL_MAX_UNIFORMS);
	context->uniforms[index].v3 = *v;
}

void lglSetUniformv4f(LGLcontext* context, LGLuint index, const LGLv4f* v) {
	assert(context != NULL);
	assert(index < LGL_MAX_UNIFORMS);
	context->uniforms[index].v4 = *v;
}

void lglSetUniformm4x4f(LGLcontext* context, LGLuint index, const LGLm4x4f* v) {
	assert(context != NULL);
	assert(index < LGL_MAX_UNIFORMS);
	context->uniforms[index].m4x4 = *v;
}

/* Vertex/index buffer functions */

void lglSetVertexStream(LGLcontext* context, LGLv3f vertices[], LGLsize elems) {
	assert(context != NULL);
	context->vertex_stream = vertices;
	context->vertex_stream_elements = elems;
}

void lglSetIndexStream(LGLcontext* context, LGLuint indices[], LGLsize elems) {
	assert(context != NULL);
	context->index_stream = indices;
	context->index_stream_elements = elems;
}

/* Vertex attribute functions */

void lglSetVertexAttribsf(LGLcontext* context, LGLuint index, LGLfloat* v, LGLsize elems) {
	assert(context != NULL);
	assert(index < LGL_MAX_ATTRIBUTES);
	context->attributes[index].f = v;
	context->num_attributes[index] = elems;
}

void lglSetVertexAttribsv2f(LGLcontext* context, LGLuint index, LGLv2f* v, LGLsize elems) {
	assert(context != NULL);
	assert(index < LGL_MAX_ATTRIBUTES);
	context->attributes[index].v2 = v;
	context->num_attributes[index] = elems;
}

void lglSetVertexAttribsv3f(LGLcontext* context, LGLuint index, LGLv3f* v, LGLsize elems) {
	assert(context != NULL);
	assert(index < LGL_MAX_ATTRIBUTES);
	context->attributes[index].v3 = v;
	context->num_attributes[index] = elems;
}

void lglSetVertexAttribsv4f(LGLcontext* context, LGLuint index, LGLv4f* v, LGLsize elems) {
	assert(context != NULL);
	assert(index < LGL_MAX_ATTRIBUTES);
	context->attributes[index].v4 = v;
	context->num_attributes[index] = elems;
}

/* Shader functions */

void lglSetVertexShader(LGLcontext* context, LGLvertexshader vshader) {
	assert(context != NULL);
	assert(vshader != NULL);
	context->vertex_shader = vshader;
}

void lglSetFragmentShader(LGLcontext* context, LGLfragmentshader fshader) {
	assert(context != NULL);
	assert(fshader != NULL);
	context->fragment_shader = fshader;
}

/*  Drawing functions  */

void lglViewport(const LGLcontext* context, LGLint x, LGLint y, LGLsize width, LGLsize height) {
	assert(context != NULL);

}

void lglClear(const LGLcontext* context, LGLclear clear/*, LGLpixel pixel*/) {
	int i;
	assert(context != NULL);
	if (clear & LGL_CLEAR_FRAMEBUFFER) {
		// TODO: clear color
		memset(context->fbinfo.framebuffer, 0, context->fbinfo.width * context->fbinfo.height * sizeof(unsigned int));
	}
	if (clear & LGL_CLEAR_ZBUFFER) {
		// TODO: clear depth
		memset(context->fbinfo.zbuffer, 0xff, context->fbinfo.width * context->fbinfo.height * sizeof(unsigned short));
	}
}

static void ilglSetPixel(const LGLcontext* context, LGLsize offset, const LGLcolor* color) {
	unsigned char r, g, b;
	unsigned int c;
	r = (unsigned int) (color->r * 255.0f) & 0xff;
	g = (unsigned int) (color->g * 255.0f) & 0xff;
	b = (unsigned int) (color->b * 255.0f) & 0xff;
	c = r << context->fbinfo.rshift | g << context->fbinfo.gshift | b << context->fbinfo.bshift;
	((unsigned int*) context->fbinfo.framebuffer)[offset] = c;
}

static LGLint ilglMax2(LGLint a, LGLint b) {
	if (a > b) {
		return a;
	}
	return b;
}

static LGLint ilglMin2(LGLint a, LGLint b) {
	if (a < b) {
		return a;
	}
	return b;
}

static LGLint ilglMax3(LGLint a, LGLint b, LGLint c) {
	if (a > b) {
		if (c > a) {
			return c;
		}
		return a;
	}
	if (c > b) {
		return c;
	}
	return b;
}

static LGLint ilglMin3(LGLint a, LGLint b, LGLint c) {
	if (a < b) {
		if (c < a) {
			return c;
		}
		return a;
	}
	if (c < b) {
		return c;
	}
	return b;
}

static void ilglRasterTriangle(const LGLcontext* context, LGLfsin* fsin, LGLint p1x, LGLint p1y, LGLfloat p1z,
		LGLint p2x, LGLint p2y, LGLfloat p2z, LGLint p3x, LGLint p3y, LGLfloat p3z) {
	LGLfsout fsout;
	LGLint x, y;

	assert(context != NULL);
	assert(fsin != NULL);

	LGLint bminx = ilglMin3(p1x, p2x, p3x);
	LGLint bminy = ilglMin3(p1y, p2y, p3y);
	LGLint bmaxx = ilglMax3(p1x, p2x, p3x);
	LGLint bmaxy = ilglMax3(p1y, p2y, p3y);

	// clip non visible triangles
	if (bmaxx < context->vport_x)
		return;
	if (bminx >= context->vport_x + (LGLint) context->vport_width)
		return;
	if (bmaxy < context->vport_y)
		return;
	if (bminy >= context->vport_y + (LGLint) context->vport_height)
		return;

	bmaxx = ilglMin2(context->vport_x + context->vport_width - 1, bmaxx);
	bmaxy = ilglMin2(context->vport_y + context->vport_height - 1, bmaxy);
	bminx = ilglMax2(context->vport_x, bminx);
	bminy = ilglMax2(context->vport_y, bminy);

	const LGLint dx13 = p1x - p3x;
	const LGLint dy13 = p1y - p3y;
	const LGLint dx23 = p2x - p3x;
	const LGLint dy23 = p2y - p3y;

	const float idett = 1.0f / ((dx13 * dy23 - dx23 * dy13));

	// culling
	if (idett < 0.0) {
		//return;
	}

	for (y = bminy; y <= bmaxy; y++) {
		const LGLint dy2 = y - p3y;
		const LGLint dx02_dy2 = dx13 * dy2;
		const LGLint dx12_dy2 = dx23 * dy2;

		for (x = bminx; x <= bmaxx; x++) {
			const LGLint dx2 = x - p3x;

			const float l1 = (dy23 * dx2 - dx12_dy2) * idett;
			if (l1 > 1.0f || l1 < 0.0f)
				continue;

			const float l2 = (dx02_dy2 - dy13 * dx2) * idett;
			if (l2 > 1.0f || l2 < 0.0f)
				continue;

			const float l3 = 1.0f - l1 - l2;
			if (l3 > 1.0f || l3 < 0.0f)
				continue;

			const LGLfloat z = l1 * p1z + l2 * p2z + l3 * p3z;
			if(z < -1.0 || z > 1.0) /* z clipping */
				continue;

			const LGLint zdepth = (LGLint)(((z + 1.0f) * 0.5f) * 0xfffe);
			const LGLsize offset = context->fbinfo.width * y + x;

			if (zdepth < ((unsigned short*)context->fbinfo.zbuffer)[offset]) {
				((unsigned short*)context->fbinfo.zbuffer)[offset] = zdepth;
				fsin->a = l1;
				fsin->b = l2;
				fsin->c = l3;
				context->fragment_shader(&fsout, fsin);
				ilglSetPixel(context, offset, &fsout.color);
			}
		}
	}
}

void lglDrawIndexed(const LGLcontext* context, LGLdrawtype type) {
	LGLvsin vsin;
	LGLvsout vsout;
	LGLfsin fsin;
	LGLuint* face;
	LGLint p1x, p1y, p2x, p2y, p3x, p3y;
	LGLfloat p1z, p2z, p3z;

	assert(context != NULL);
	assert(type == LGL_DRAW_TYPE_TRIANGLE_LIST); /* the only type we support right now */
	assert(context->vertex_stream != NULL);
	assert(context->index_stream != NULL);
	assert(context->vertex_shader != NULL);
	assert(context->fragment_shader != NULL);

	vsin.vertex_stream = context->vertex_stream;
	vsin.vertex_stream_elements = context->vertex_stream_elements;
	vsin.index_stream = context->index_stream;
	vsin.index_stream_elements = context->index_stream_elements;
	memcpy(vsin.textures, context->textures, sizeof(context->textures));
	memcpy(vsin.uniforms, context->uniforms, sizeof(context->uniforms));
	memcpy(vsin.attributes, context->attributes, sizeof(context->attributes));
	memcpy(vsin.num_attributes, context->num_attributes, sizeof(context->num_attributes));
	memcpy(fsin.textures, context->textures, sizeof(context->textures));
	memcpy(fsin.uniforms, context->textures, sizeof(context->uniforms));

	for (face = context->index_stream; face != context->index_stream + context->index_stream_elements; face += 3) {
		assert(face[0] < context->vertex_stream_elements);
		assert(face[1] < context->vertex_stream_elements);
		assert(face[2] < context->vertex_stream_elements);

		vsin.index = face[0];
		context->vertex_shader(&vsout, &vsin);
		memcpy(&fsin.varyings[0], vsout.varyings, sizeof(vsout.varyings));
		p1x = (LGLint) ((vsout.position.x + 1.0) * ((float) context->vport_width / 2.0) + context->vport_x);
		p1y = (LGLint) ((vsout.position.y + 1.0) * ((float) context->vport_height / 2.0) + context->vport_y);
		p1z = vsout.position.z;

		vsin.index = face[1];
		context->vertex_shader(&vsout, &vsin);
		memcpy(&fsin.varyings[1], &vsout.varyings, sizeof(vsout.varyings));
		p2x = (LGLint) ((vsout.position.x + 1.0) * ((float) context->vport_width / 2.0) + context->vport_x);
		p2y = (LGLint) ((vsout.position.y + 1.0) * ((float) context->vport_height / 2.0) + context->vport_y);
		p2z = vsout.position.z;

		vsin.index = face[2];
		context->vertex_shader(&vsout, &vsin);
		memcpy(&fsin.varyings[2], &vsout.varyings, sizeof(vsout.varyings));
		p3x = (LGLint) ((vsout.position.x + 1.0) * ((float) context->vport_width / 2.0) + context->vport_x);
		p3y = (LGLint) ((vsout.position.y + 1.0) * ((float) context->vport_height / 2.0) + context->vport_y);
		p3z = vsout.position.z;

		ilglRasterTriangle(context, &fsin, p1x, p1y, p1z, p2x, p2y, p2z, p3x, p3y, p3z);
	}
}
