/*
 *
 * LightGL - Utility Library
 * A small and simple software rasterization library with vertex and fragment shader support.
 * Copyright(c) 2010 by Christoph Schunk. All rights reserved.
 *
 */

#ifndef PGLU_H_INCLUDED
#define PGLU_H_INCLUDED

#include "lgl.h"

/* Matrix/Vector utility functions */

void lgluMatrixSetIdentity(LGLm4x4f* m);
void lgluMatrixSetTranslation(LGLm4x4f* m, LGLfloat x, LGLfloat y, LGLfloat z);
void lgluMatrixSetScaling(LGLm4x4f* m, LGLfloat x, LGLfloat y, LGLfloat z);
void lgluMatrixSetRotationX(LGLm4x4f* m, LGLfloat theta);
void lgluMatrixSetRotationY(LGLm4x4f* m, LGLfloat theta);
void lgluMatrixSetRotationZ(LGLm4x4f* m, LGLfloat theta);
void lgluMatrixSetOrtho(LGLm4x4f* m, LGLfloat left, LGLfloat right, LGLfloat bottom, LGLfloat top, LGLfloat near,
		LGLfloat far);
void lgluMatrixSetLookAt(LGLm4x4f* m, const LGLv3f* eye, const LGLv3f* center, const LGLv3f* up);
void lgluMatrixMultiply(LGLm4x4f* d, const LGLm4x4f* m1, const LGLm4x4f* m2);
LGLint lgluMatrixInverse(LGLm4x4f* o, const LGLm4x4f* m);
void lgluMatrixTranspose(LGLm4x4f* o, const LGLm4x4f* m);
void lgluTransform(LGLv3f* d, const LGLm4x4f* m, const LGLv3f* v);
void lgluVectorNormalize(LGLv3f* v);
LGLfloat lgluVectorDot(LGLv3f* a, LGLv3f* b);
void lgluInterpolatev3f(LGLv3f* o, LGLfloat a, const LGLv3f* v1, LGLfloat b, const LGLv3f* v2, LGLfloat c, const LGLv3f* v3);

#endif
