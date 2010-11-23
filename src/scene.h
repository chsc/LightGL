/*
 *
 */

#ifndef SCENE_H_INCLUDED
#define SCENE_H_INCLUDED

int sceneInit(unsigned int* pixels, int w, int h, int rshift, int gshift, int bshift);
void sceneUpdate(float delta);
void sceneRender();
void sceneKeyboardEvent();
void sceneClose();

#endif
