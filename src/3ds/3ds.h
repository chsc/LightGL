/*
 *
 */

#ifndef A3DS_H_INCLUDED
#define A3DS_H_INCLUDED

#define A3DS_MAX_OBJECTS 16

typedef struct A3DS_s {
	unsigned int  num_objects;
	float*        vertices[A3DS_MAX_OBJECTS];
	float*        normals[A3DS_MAX_OBJECTS];
	float*        texcoords[A3DS_MAX_OBJECTS];
	unsigned int* indices[A3DS_MAX_OBJECTS];
	unsigned int  num_vertices[A3DS_MAX_OBJECTS];
	unsigned int  num_indices[A3DS_MAX_OBJECTS];
}A3DS;

A3DS* a3dsLoad(const char* file);
void  a3dsUnload(A3DS* rawdata);

#endif
