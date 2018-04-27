#ifndef RENDERER_H
#define RENDERER_H

namespace graphics {
void initialize();
void load();
void clear();
void render();
void reshape(int w, int h);
void update();
void change_shader(int i);
void toggle_object(int i);
struct point {
	float x,y,z;
};
struct scoord {
	float r, p, t;
};
extern scoord obs;
extern scoord src;

struct light_st{
	float x,y,z;
	float r,g,b;
	float a,p,s;
};
extern light_st light;
} // namespace graphics

#endif // RENDERER_H
