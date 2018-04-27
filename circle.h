#ifndef CIRCLE_H
#define CIRCLE_H
#include <aw/math/angle.h>
#include <aw/math/vector2d.h>
#include <vector>

namespace aw::gl3 {
struct program;
}
namespace graphics {
struct circle {
	circle(float radius);

	circle(circle&& other);

	~circle()
	{
		cleanup();
	}

	circle& operator=(circle&& other);

	void update_radius(float r);
	void set_colour(float(&colours)[16]);

	aw::gl3::mat4 transform() const;
	void render(aw::gl3::program& prg, aw::gl3::mat4 transform);
	void cleanup();

	aw::math::vector2d<float> position{};
	aw::math::degrees<float>  rotation{0};
	float radius;
	unsigned vbo, vao;
};

extern std::vector<circle> circles;

struct orbiter {
	aw::gl3::mat4 apply(aw::gl3::mat4 mat);
	size_t center;
	float  angle;
};
} // namespace graphics

#endif // CIRCLE_H
