/*
 *
 * LightGL - Utility Library
 * A small and simple software rasterization library with vertex and fragment shader support.
 * Copyright(c) 2010 by Christoph Schunk. All rights reserved.
 *
 */

#include <stddef.h>
#include <math.h>
#include <float.h>
#include <assert.h>
#include "lglu.h"

/*
 *  Matrix/Vector functions
 */

void lgluMatrixSetIdentity(LGLm4x4f* m) {
	assert(m != NULL);

	m->m11 = 1.0;
	m->m12 = 0.0;
	m->m13 = 0.0;
	m->m14 = 0.0;

	m->m21 = 0.0;
	m->m22 = 1.0;
	m->m23 = 0.0;
	m->m24 = 0.0;

	m->m31 = 0.0;
	m->m32 = 0.0;
	m->m33 = 1.0;
	m->m34 = 0.0;

	m->m41 = 0.0;
	m->m42 = 0.0;
	m->m43 = 0.0;
	m->m44 = 1.0;
}

void lgluMatrixSetTranslation(LGLm4x4f* m, LGLfloat x, LGLfloat y, LGLfloat z) {
	assert(m != NULL);

	m->m11 = 1.0f;
	m->m12 = 0.0f;
	m->m13 = 0.0f;
	m->m14 = x;

	m->m21 = 0.0f;
	m->m22 = 1.0f;
	m->m23 = 0.0f;
	m->m24 = y;

	m->m31 = 0.0f;
	m->m32 = 0.0f;
	m->m33 = 1.0f;
	m->m34 = z;

	m->m41 = 0.0f;
	m->m42 = 0.0f;
	m->m43 = 0.0f;
	m->m44 = 1.0f;
}

void lgluMatrixSetScaling(LGLm4x4f* m, LGLfloat x, LGLfloat y, LGLfloat z) {
	assert(m != NULL);

	m->m11 = x;
	m->m12 = 0.0f;
	m->m13 = 0.0f;
	m->m14 = 0.0f;

	m->m21 = 0.0f;
	m->m22 = y;
	m->m23 = 0.0f;
	m->m24 = 0.0f;

	m->m31 = 0.0f;
	m->m32 = 0.0f;
	m->m33 = z;
	m->m34 = 0.0f;

	m->m41 = 0.0f;
	m->m42 = 0.0f;
	m->m43 = 0.0f;
	m->m44 = 1.0f;
}

void lgluMatrixSetRotationX(LGLm4x4f* m, LGLfloat theta) {
	assert(m != NULL);

	const LGLfloat costheta = (LGLfloat) cos(theta);
	const LGLfloat sintheta = (LGLfloat) sin(theta);

	m->m11 = 1.0f;
	m->m12 = 0.0f;
	m->m13 = 0.0f;
	m->m14 = 0.0f;

	m->m21 = 0.0f;
	m->m22 = costheta;
	m->m23 = -sintheta;
	m->m24 = 0.0f;

	m->m31 = 0.0f;
	m->m32 = sintheta;
	m->m33 = costheta;
	m->m34 = 0.0f;

	m->m41 = 0.0f;
	m->m42 = 0.0f;
	m->m43 = 0.0f;
	m->m44 = 1.0f;
}

void lgluMatrixSetRotationY(LGLm4x4f* m, LGLfloat theta) {
	assert(m != NULL);

	const LGLfloat costheta = (LGLfloat) cos(theta);
	const LGLfloat sintheta = (LGLfloat) sin(theta);

	m->m11 = costheta;
	m->m12 = 0.0f;
	m->m13 = sintheta;
	m->m14 = 0.0f;

	m->m21 = 0.0f;
	m->m22 = 1.0f;
	m->m23 = 0.0f;
	m->m24 = 0.0f;

	m->m31 = -sintheta;
	m->m32 = 0.0f;
	m->m33 = costheta;
	m->m34 = 0.0;

	m->m41 = 0.0;
	m->m42 = 0.0;
	m->m43 = 0.0;
	m->m44 = 1.0;
}

void lgluMatrixSetRotationZ(LGLm4x4f* m, LGLfloat theta) {
	assert(m != NULL);

	const LGLfloat costheta = (LGLfloat) cos(theta);
	const LGLfloat sintheta = (LGLfloat) sin(theta);

	m->m11 = costheta;
	m->m12 = -sintheta;
	m->m13 = 0.0f;
	m->m14 = 0.0f;

	m->m21 = sintheta;
	m->m22 = costheta;
	m->m23 = 0.0f;
	m->m24 = 0.0f;

	m->m31 = 0.0f;
	m->m32 = 0.0f;
	m->m33 = 1.0f;
	m->m34 = 0.0f;

	m->m41 = 0.0f;
	m->m42 = 0.0f;
	m->m43 = 0.0f;
	m->m44 = 1.0f;
}

void lgluMatrixSetOrtho(LGLm4x4f* m, LGLfloat left, LGLfloat right, LGLfloat bottom, LGLfloat top, LGLfloat near,
		LGLfloat far) {
	assert(m != NULL);

	const LGLfloat irl = 1.0f / (right - left);
	const LGLfloat itb = 1.0f / (top - bottom);
	const LGLfloat ifn = 1.0f / (far - near);
	const LGLfloat tx = (right + left) * irl;
	const LGLfloat ty = (top + bottom) * itb;
	const LGLfloat tz = (far + near) * ifn;

	m->m11 = 2.0f * irl;
	m->m12 = 0.0f;
	m->m13 = 0.0f;
	m->m14 = -tx;

	m->m21 = 0.0f;
	m->m22 = 2.0f * itb;
	m->m23 = 0.0f;
	m->m24 = -ty;

	m->m31 = 0.0f;
	m->m32 = 0.0f;
	m->m33 = -2.0f * ifn;
	m->m34 = -tz;

	m->m41 = 0.0f;
	m->m42 = 0.0f;
	m->m43 = 0.0f;
	m->m44 = 1.0f;
}

void lgluMatrixSetFrustum(LGLm4x4f* m, LGLfloat left, LGLfloat right, LGLfloat bottom, LGLfloat top, LGLfloat near,
		LGLfloat far) {
	assert(m != NULL);

	const LGLfloat irl = 1.0f / (right - left);
	const LGLfloat itb = 1.0f / (top - bottom);
	const LGLfloat ifn = 1.0f / (far - near);

	m->m11 = 2.0f * near * irl;
	m->m12 = 0.0f;
	m->m13 = (right + left) * irl;
	m->m14 = 0.0f;

	m->m21 = 0.0f;
	m->m22 = 2.0f * near * itb;
	m->m23 = (top + bottom) * itb;
	m->m24 = 0.0f;

	m->m31 = 0.0f;
	m->m32 = 0.0f;
	m->m33 = -(far + near) * ifn;
	m->m34 = (-2.0f * far * near) * ifn;

	m->m41 = 0.0f;
	m->m42 = 0.0f;
	m->m43 = -1.0f;
	m->m44 = 0.0f;
}

void lgluMatrixSetLookAt(LGLm4x4f* m, const LGLv3f* eye, const LGLv3f* center, const LGLv3f* up) {
	assert(!"not implemented");
}

void lgluMatrixMultiply(LGLm4x4f* o, const LGLm4x4f* m1, const LGLm4x4f* m2) {
	assert(o != NULL);
	assert(m1 != NULL);
	assert(m2 != NULL);

	LGLm4x4f a;

	a.m11 = m1->m11 * m2->m11 + m1->m12 * m2->m21 + m1->m13 * m2->m31 + m1->m14 * m2->m41;
	a.m12 = m1->m11 * m2->m12 + m1->m12 * m2->m22 + m1->m13 * m2->m32 + m1->m14 * m2->m42;
	a.m13 = m1->m11 * m2->m13 + m1->m12 * m2->m23 + m1->m13 * m2->m33 + m1->m14 * m2->m43;
	a.m14 = m1->m11 * m2->m14 + m1->m12 * m2->m24 + m1->m13 * m2->m34 + m1->m14 * m2->m44;

	a.m21 = m1->m21 * m2->m11 + m1->m22 * m2->m21 + m1->m23 * m2->m31 + m1->m24 * m2->m41;
	a.m22 = m1->m21 * m2->m12 + m1->m22 * m2->m22 + m1->m23 * m2->m32 + m1->m24 * m2->m42;
	a.m23 = m1->m21 * m2->m13 + m1->m22 * m2->m23 + m1->m23 * m2->m33 + m1->m24 * m2->m43;
	a.m24 = m1->m21 * m2->m14 + m1->m22 * m2->m24 + m1->m23 * m2->m34 + m1->m24 * m2->m44;

	a.m31 = m1->m31 * m2->m11 + m1->m32 * m2->m21 + m1->m33 * m2->m31 + m1->m34 * m2->m41;
	a.m32 = m1->m31 * m2->m12 + m1->m32 * m2->m22 + m1->m33 * m2->m32 + m1->m34 * m2->m42;
	a.m33 = m1->m31 * m2->m13 + m1->m32 * m2->m23 + m1->m33 * m2->m33 + m1->m34 * m2->m43;
	a.m34 = m1->m31 * m2->m14 + m1->m32 * m2->m24 + m1->m33 * m2->m34 + m1->m34 * m2->m44;

	a.m41 = m1->m41 * m2->m11 + m1->m42 * m2->m21 + m1->m43 * m2->m31 + m1->m44 * m2->m41;
	a.m42 = m1->m41 * m2->m12 + m1->m42 * m2->m22 + m1->m43 * m2->m32 + m1->m44 * m2->m42;
	a.m43 = m1->m41 * m2->m13 + m1->m42 * m2->m23 + m1->m43 * m2->m33 + m1->m44 * m2->m43;
	a.m44 = m1->m41 * m2->m14 + m1->m42 * m2->m24 + m1->m43 * m2->m34 + m1->m44 * m2->m44;

	*o = a;
}

static LGLfloat ilgluDet3x3(LGLfloat a11, LGLfloat a12, LGLfloat a13, LGLfloat a21, LGLfloat a22, LGLfloat a23,
		LGLfloat a31, LGLfloat a32, LGLfloat a33) {
	return a11 * a22 * a33 + a12 * a23 * a31 + a13 * a21 * a32 - a13 * a22 * a31 - a12 * a12 * a33 - a11 * a23 * a32;
}

static void ilgluMatrixAdjugate(LGLm4x4f* o, const LGLm4x4f* m) {
	assert(o != NULL);
	assert(m != NULL);

	LGLm4x4f a;

	a.m11 = +ilgluDet3x3(m->m22, m->m23, m->m24, m->m32, m->m33, m->m34, m->m42, m->m43, m->m44);
	a.m21 = -ilgluDet3x3(m->m21, m->m23, m->m24, m->m31, m->m33, m->m34, m->m41, m->m43, m->m44);
	a.m31 = +ilgluDet3x3(m->m21, m->m22, m->m24, m->m31, m->m32, m->m34, m->m41, m->m42, m->m44);
	a.m41 = -ilgluDet3x3(m->m21, m->m22, m->m23, m->m31, m->m32, m->m33, m->m41, m->m42, m->m43);

	a.m12 = -ilgluDet3x3(m->m12, m->m13, m->m14, m->m32, m->m33, m->m34, m->m42, m->m43, m->m44);
	a.m22 = +ilgluDet3x3(m->m11, m->m13, m->m14, m->m31, m->m33, m->m34, m->m41, m->m43, m->m44);
	a.m32 = -ilgluDet3x3(m->m11, m->m12, m->m14, m->m31, m->m32, m->m34, m->m41, m->m42, m->m44);
	a.m42 = +ilgluDet3x3(m->m11, m->m12, m->m13, m->m31, m->m32, m->m33, m->m41, m->m42, m->m43);

	a.m13 = +ilgluDet3x3(m->m12, m->m13, m->m14, m->m22, m->m23, m->m24, m->m42, m->m43, m->m44);
	a.m23 = -ilgluDet3x3(m->m11, m->m13, m->m14, m->m21, m->m23, m->m24, m->m41, m->m43, m->m44);
	a.m33 = +ilgluDet3x3(m->m11, m->m12, m->m14, m->m21, m->m22, m->m24, m->m41, m->m42, m->m44);
	a.m43 = -ilgluDet3x3(m->m11, m->m12, m->m13, m->m21, m->m22, m->m23, m->m41, m->m42, m->m43);

	a.m14 = -ilgluDet3x3(m->m12, m->m13, m->m14, m->m22, m->m23, m->m24, m->m32, m->m33, m->m34);
	a.m24 = +ilgluDet3x3(m->m11, m->m13, m->m14, m->m21, m->m23, m->m24, m->m31, m->m33, m->m34);
	a.m34 = -ilgluDet3x3(m->m11, m->m12, m->m14, m->m21, m->m22, m->m24, m->m31, m->m32, m->m34);
	a.m44 = +ilgluDet3x3(m->m11, m->m12, m->m13, m->m21, m->m22, m->m23, m->m31, m->m32, m->m33);

	*o = a;
}

static LGLfloat ilgluMatrixDet(const LGLm4x4f* m) {
	assert(m != NULL);
	const LGLfloat a = ilgluDet3x3(m->m22, m->m23, m->m24, m->m32, m->m33, m->m34, m->m42, m->m43, m->m44);
	const LGLfloat b = ilgluDet3x3(m->m21, m->m23, m->m24, m->m31, m->m33, m->m34, m->m41, m->m43, m->m44);
	const LGLfloat c = ilgluDet3x3(m->m21, m->m22, m->m24, m->m31, m->m32, m->m34, m->m41, m->m42, m->m44);
	const LGLfloat d = ilgluDet3x3(m->m21, m->m22, m->m23, m->m31, m->m32, m->m33, m->m41, m->m42, m->m43);
	return m->m11 * a - m->m12 * b + m->m13 * c - m->m14 * d;
}

LGLint lgluMatrixInverse(LGLm4x4f* o, const LGLm4x4f* m) {
	assert(o != NULL);
	assert(m != NULL);

	const LGLfloat det = ilgluMatrixDet(m);
	if (det == 0.0f) {
		return -1;
	}
	const LGLfloat idet = 1.0f / det;

	ilgluMatrixAdjugate(o, m);

	o->m11 *= idet;
	o->m12 *= idet;
	o->m13 *= idet;
	o->m14 *= idet;

	o->m21 *= idet;
	o->m22 *= idet;
	o->m23 *= idet;
	o->m24 *= idet;

	o->m31 *= idet;
	o->m32 *= idet;
	o->m33 *= idet;
	o->m34 *= idet;

	o->m41 *= idet;
	o->m42 *= idet;
	o->m43 *= idet;
	o->m44 *= idet;

	return 1;
}

void lgluMatrixTranspose(LGLm4x4f* o, const LGLm4x4f* m) {
	assert(o != NULL);
	assert(m != NULL);

	LGLm4x4f a;

	a.m11 = m->m11;
	a.m12 = m->m21;
	a.m13 = m->m31;
	a.m14 = m->m41;

	a.m21 = m->m12;
	a.m22 = m->m22;
	a.m23 = m->m32;
	a.m24 = m->m42;

	a.m31 = m->m13;
	a.m32 = m->m23;
	a.m33 = m->m33;
	a.m34 = m->m43;

	a.m41 = m->m14;
	a.m42 = m->m24;
	a.m43 = m->m34;
	a.m44 = m->m44;

	*o = a;
}

void lgluTransform(LGLv3f* d, const LGLm4x4f* m, const LGLv3f* v) {
	assert(d != v);
	d->x = m->m11 * v->x + m->m12 * v->y + m->m13 * v->z + m->m14;
	d->y = m->m21 * v->x + m->m22 * v->y + m->m23 * v->z + m->m24;
	d->z = m->m31 * v->x + m->m32 * v->y + m->m33 * v->z + m->m34;
}

void lgluVectorNormalize(LGLv3f* v) {
	const LGLfloat f = 1.0f / sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
	v->x *= f;
	v->y *= f;
	v->z *= f;
}

LGLfloat lgluVectorDot(LGLv3f* a, LGLv3f* b) {
	return a->x * b->x + a->y * b->y + a->z * b->z;
}

void lgluInterpolatev3f(LGLv3f* o, LGLfloat a, const LGLv3f* v1, LGLfloat b, const LGLv3f* v2, LGLfloat c, const LGLv3f* v3) {
	o->x = a * v1->x + b * v2->x + c * v3->x;
	o->y = a * v1->y + b * v2->y + c * v3->y;
	o->z = a * v1->z + b * v2->z + c * v3->z;
}

/* Utility functions */

/*
#define LGLU_ENCODE_COLOR(R, G, B) ((LGLpixel)(255*(R)) << 16 | (LGLpixel)(255*(G)) << 8 | (LGLpixel)(255*b) << 0)
#define LGLU_INTERPOLATE_V2(OUT, A, V1, B, V2, C, V3) {\
	(OUT).x = a * v1.x + b * v2->x + (C) * v3->x; \
	(OUT).y = a * v1.y + b * v2->y + c * v3->y; }
*/

