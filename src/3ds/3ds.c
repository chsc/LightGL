/*
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "3ds.h"

#define A3DS_OUT "3ds: "

#define MAIN3DS                         0x4D4D
#define EDIT3DS                         0x3D3D
#define EDIT3DS_OBJECT                  0x4000
#define EDIT3DS_OBJECT_TRIMESH          0x4100
#define	EDIT3DS_OBJECT_TRIMESH_VERTEXL	0x4110
#define	EDIT3DS_OBJECT_TRIMESH_FACEL    0x4120
#define	EDIT3DS_OBJECT_TRIMESH_MATRIX   0x4160

typedef struct A3DSchk_s {
	unsigned short id;
	unsigned int size;
} A3DSchk;

static int a3dsReadShort(FILE* f, unsigned short* us) {
	return fread(us, 2, 1, f) == 1;
}

static int a3dsReadInt(FILE* f, unsigned int* l) {
	return fread(l, 4, 1, f) == 1;
}

static int a3dsReadFloat(FILE* f, float* fl) {
	return fread(fl, 4, 1, f) == 1;
}

static int a3dsReadChar(FILE* f, char* c) {
	return fread(c, 1, 1, f) == 1;
}

static int a3dsReadString(FILE* f, char* str, size_t maxlen) {
	char ch;
	int i = 0;
	do {
		if (!a3dsReadChar(f, &ch)) {
			return 0;
		}
		if (i < maxlen) {
			str[i++] = ch;
		}
	} while (ch != '\0');
#ifndef NDEBUG
	printf(A3DS_OUT " Reading string: '%s' (len: %d)\n", str, i - 1);
#endif
	return 1;
}

static int a3dsReadChunk(FILE* f, A3DSchk* chunk) {
	if (!a3dsReadShort(f, &chunk->id)) {
		return 0;
	}
	if (!a3dsReadInt(f, &chunk->size)) {
		return 0;
	}
#ifndef NDEBUG
	printf(A3DS_OUT " Reading chunk: 0x%x size: %d\n", chunk->id, chunk->size);
#endif
	return 1;
}

static int a3dsSkipChunk(FILE* f, unsigned int skip) {
#ifndef NDEBUG
	printf(A3DS_OUT " Skipping chunk ...\n");
#endif
	return fseek(f, skip - (sizeof(unsigned short) + sizeof(unsigned int)), SEEK_CUR);
}

static void a3dsCalcNormals(A3DS *a3ds, unsigned int i) {
	unsigned int j;
	unsigned int a, b, c;
	float ax, ay, az;
	float bx, by, bz;
	float cx, cy, cz;
	float e1x, e1y, e1z;
	float e2x, e2y, e2z;
	float nx, ny, nz;
	float ilen;

	a3ds->normals[i] = malloc(a3ds->num_vertices[i] * 3 * sizeof(float));

	for (j = 0; j < a3ds->num_vertices[i]; j++) {
		a3ds->normals[i][j * 3 + 0] = 0.0f;
		a3ds->normals[i][j * 3 + 1] = 0.0f;
		a3ds->normals[i][j * 3 + 2] = 0.0f;
	}

	for (j = 0; j < a3ds->num_indices[i]; j += 3) {
		a = a3ds->indices[i][j + 0];
		b = a3ds->indices[i][j + 1];
		c = a3ds->indices[i][j + 2];

		/* get triangle vertices */
		ax = a3ds->vertices[i][a * 3 + 0];
		ay = a3ds->vertices[i][a * 3 + 1];
		az = a3ds->vertices[i][a * 3 + 2];
		bx = a3ds->vertices[i][b * 3 + 0];
		by = a3ds->vertices[i][b * 3 + 1];
		bz = a3ds->vertices[i][b * 3 + 2];
		cx = a3ds->vertices[i][c * 3 + 0];
		cy = a3ds->vertices[i][c * 3 + 1];
		cz = a3ds->vertices[i][c * 3 + 2];

		/* edge 1 */
		e1x = bx - ax;
		e1y = by - ay;
		e1z = bz - az;

		/* edge 2 */
		e2x = cx - ax;
		e2y = cy - ay;
		e2z = cz - az;

		/* calculate face normal */
		nx = e1y * e2z - e1z * e2y;
		ny = e1z * e2x - e1x * e2z;
		nz = e1x * e2y - e1y * e2x;

		ilen = 1.0f / sqrt(nx * nx + ny * ny + nz * nz);
		nx *= ilen;
		ny *= ilen;
		nz *= ilen;

		/* accumulate normals */
		a3ds->normals[i][a * 3 + 0] += nx;
		a3ds->normals[i][a * 3 + 1] += ny;
		a3ds->normals[i][a * 3 + 2] += nz;
		a3ds->normals[i][b * 3 + 0] += nx;
		a3ds->normals[i][b * 3 + 1] += ny;
		a3ds->normals[i][b * 3 + 2] += nz;
		a3ds->normals[i][c * 3 + 0] += nx;
		a3ds->normals[i][c * 3 + 1] += ny;
		a3ds->normals[i][c * 3 + 2] += nz;
	}

	/* normalize */
	for (j = 0; j < a3ds->num_vertices[i]; j++) {
		nx = a3ds->normals[i][j * 3 + 0];
		ny = a3ds->normals[i][j * 3 + 1];
		nz = a3ds->normals[i][j * 3 + 2];

		ilen = 1.0f / sqrt(nx * nx + ny * ny + nz * nz);

		a3ds->normals[i][j * 3 + 0] *= -ilen;
		a3ds->normals[i][j * 3 + 1] *= -ilen;
		a3ds->normals[i][j * 3 + 2] *= -ilen;
	}
}

A3DS* a3dsLoad(const char* filename) {
	A3DS* a3ds;
	FILE* file;
	A3DSchk chunk;
	char str[256];
	int done = 0;
	int object_index = -1;
	unsigned short us, len;

	int i, j;
	float v;

	file = fopen(filename, "rb");
	if (file == NULL) {
		fprintf(stderr, A3DS_OUT "Unable to load 3DS file: '%s'\n", filename);
		return NULL;
	}

	a3ds = malloc(sizeof(A3DS));
	if (a3ds == NULL) {
		fclose(file);
		return NULL;
	}

	memset(a3ds, 0, sizeof(A3DS));

	printf(A3DS_OUT "Loading %s ...\n", filename);

	if (!a3dsReadChunk(file, &chunk)) {
		fprintf(stderr, A3DS_OUT "Unable to read 3DS file: '%s'\n", filename);
		free(a3ds);
		fclose(file);
		return NULL;
	}
	if (chunk.id != MAIN3DS) {
		fprintf(stderr, A3DS_OUT "Invalid 3DS file: '%s'\n", filename);
		free(a3ds);
		fclose(file);
		return NULL;
	}

	while (a3dsReadChunk(file, &chunk) && !done) {
		switch (chunk.id) {
		case EDIT3DS:
			break;
		case EDIT3DS_OBJECT:
			a3dsReadString(file, str, sizeof(str));
			break;
		case EDIT3DS_OBJECT_TRIMESH:
			object_index++;
			if (object_index == A3DS_MAX_OBJECTS) { /* max objects reached -> we are done */
				done = 1;
			}
			break;
		case EDIT3DS_OBJECT_TRIMESH_VERTEXL:
			a3dsReadShort(file, &len);
			printf(A3DS_OUT " vertices: %d\n", (int) len);
			a3ds->vertices[object_index] = malloc(sizeof(float) * 3 * len);
			if (a3ds->vertices[object_index] == NULL) {
				fprintf(stderr, A3DS_OUT "Unable to allocate memory");
				done = 1;
			}
			for (i = 0, j = 0; i < len; i++) {
				a3dsReadFloat(file, &v);
				a3ds->vertices[object_index][j++] = v;
				a3dsReadFloat(file, &v);
				a3ds->vertices[object_index][j++] = v;
				a3dsReadFloat(file, &v);
				a3ds->vertices[object_index][j++] = v;
			}
			a3ds->num_vertices[object_index] = len;
			break;
		case EDIT3DS_OBJECT_TRIMESH_FACEL:
			a3dsReadShort(file, &len);
			printf(A3DS_OUT " faces: %d\n", (int) len);
			a3ds->indices[object_index] = malloc(sizeof(unsigned int) * 3 * len);
			if (a3ds->indices[object_index] == NULL) {
				fprintf(stderr, A3DS_OUT "Unable to allocate memory");
				done = 1;
			}
			for (i = 0, j = 0; i < len; i++) {
				a3dsReadShort(file, &us);
				a3ds->indices[object_index][j++] = us;
				a3dsReadShort(file, &us);
				a3ds->indices[object_index][j++] = us;
				a3dsReadShort(file, &us);
				a3ds->indices[object_index][j++] = us;
				a3dsReadShort(file, &us);
			}
			a3ds->num_indices[object_index] = len * 3;
			/* bytes read = face data size + size of chunk (6 bytes) + number of faces field (2 bytes) */
			if (((len * 2 * 4) + (4 + 2) + 2) != chunk.size) {
				fprintf(stderr, A3DS_OUT "Extra Face attributes not supported: '%s'\n", filename);
				done = 1;
			}
			break;
		default:
			if (a3dsSkipChunk(file, chunk.size)) {
				done = 1;
			}
		}
	}

	a3ds->num_objects = object_index + 1;

	for (i = 0; i < a3ds->num_objects; i++) {
		a3dsCalcNormals(a3ds, i);
	}

	fclose(file);
	return a3ds;
}

void a3dsUnload(A3DS* a3ds) {
	int i;
	assert(a3ds != NULL);
	for (i = 0; i < a3ds->num_objects; i++) {
		free(a3ds->vertices[i]);
		free(a3ds->indices[i]);
		free(a3ds->normals[i]);
		free(a3ds->texcoords[i]);
	}
	free(a3ds);
}
