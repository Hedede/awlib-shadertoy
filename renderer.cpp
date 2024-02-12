#include "renderer.h"
#include <aw/graphics/gl/api/gl_33.h>
#include <aw/graphics/gl/awgl/shader_func.h>
#include <aw/graphics/gl/utility/program_loader.h>
#include <aw/graphics/gl/utility/model/obj.h>
#include <aw/graphics/gl/camera.h>
#include <aw/graphics/gl/program.h>
#include <aw/utility/to_string/math/matrix.h>
#include <aw/math/orientation.h>
#include <aw/math/transform.h>
#include <aw/io/input_memory_stream.h>
#include <aw/math/matrix4.h>
#include <iostream>
#include <vector>
#include <string_view>
#include <chrono>
#include <QFile>
#include <QDebug>
#include <fstream>

void debug_matrix(int,aw::gl3::mat4 const& mat);
//void matrixreport(aw::math::matrix3<float> mat, int id);
namespace graphics {
using namespace std::string_view_literals;
namespace gl3 = aw::gl3;
static std::vector<gl3::model> models;
static std::vector<gl3::program> programs;
struct object {
	void render( aw::gl3::program& program ) const
	{
		auto& model = models[model_id];
		gl::bind_vertex_array(model.vao);
		program["transform"] = trafo;
		for (auto obj : model.objects)
			gl::draw_elements_base_vertex(GL_TRIANGLES, obj.num_elements, GL_UNSIGNED_INT, 0, obj.offset);
	}

	size_t model_id;
	size_t program_id;
	gl3::mat4 trafo;
	bool draw = false;
};
static std::vector<object> objects;
void change_shader(int id)
{
	for (auto i = 1u; i < objects.size(); ++i) {
		objects[i].program_id = id+1;
	}
}

void toggle_object(int model)
{
	std::cerr << "object toggle " << model << '\n';
	for (auto& obj : objects) {
		if (obj.model_id == model)
			obj.draw = !obj.draw;
	}
}
using namespace aw;
QString str(std::string_view sv)
{
	return QString::fromUtf8(sv.data(), sv.size());
}

std::string read_resource(string_view path)
{
	QFile file(str(path));
	if(!file.open(QIODevice::ReadOnly|QFile::Text)) {
		qDebug() << "error: " << file.errorString();
	}
	return file.readAll().toStdString();
}

void load_model( std::string_view filename )
{
	//aw::io::input_file_stream file{ filename };
	auto res = read_resource(filename);
	aw::io::input_memory_stream file( res.data(), res.data() + res.size() );
	auto data = aw::obj::mesh::parse( file );
	auto model = gl3::model_from_obj(data);
	models.emplace_back( std::move(model) );
}

namespace gl = ::gl;
void load()
{
	auto result = ::gl::sys::load_functions_3_3();
	std::cout << "GL loaded, missing: " << result.num_missing() << '\n';
}

using aw::gl3::vec3;
static aw::gl3::camera camera;
std::optional<gl3::shader> compile_shader( string_view path )
{
	std::cerr << "compiling shader: " << path << '\n';
	auto buf = read_resource(path);
	/*std::ofstream nlya([=] { std::string tmp; for (auto c:path) if (c=='/')tmp+='|';else tmp+=c; return tmp; }());
	nlya << std::string_view(buf.data(), buf.size()) ;*/
	aw::gl::shader_type type;
	if (path.find(".vsh") != path.npos)
		type = aw::gl::shader_type::vertex;
	else if (path.find(".fsh") != path.npos)
		type = aw::gl::shader_type::fragment;
	return gl3::compile_shader( type, std::string_view(buf.data(), buf.size()) );
}

std::optional<gl3::program> link_program( aw::array_view<string_view> names )
{
	std::vector<gl3::shader> shader_list;
	for (auto name : names)
		if (auto shd = compile_shader(name))
			shader_list.push_back(std::move(*shd));
	return gl3::link_program(shader_list);
}

void load_program( aw::array_view<string_view> names )
{
	if (auto prg = link_program(names))
		programs.push_back(std::move(*prg));
	else
		programs.push_back(aw::gl3::program{});
}

//----------------------------------------
void initialize()
{
	/*auto from = vec3{10,10,10};
	auto to = vec3{0,0,0};
	auto up = vec3{0,1,0};
	debug_matrix(0,aw::math::look_at_inverse(from, vec3{0,0,0}, up));
	debug_matrix(1,*inverse(aw::math::look_at(vec3{10,10,10}, to, vec3{0,1,0})));
	auto trs = expand_matrix( aw::math::look_at_inverse(to - from, up) );
	auto rur = aw::math::identity_matrix<float, 4>;
	rur[0][3] = -from[0];
	rur[1][3] = -from[1];
	rur[2][3] = -from[2];
	debug_matrix(2,rur*trs);
	debug_matrix(3,trs*rur);*/
	gl::enable(GL_DEPTH_TEST);
	gl::depth_mask(GL_TRUE);
	gl::depth_func(GL_LEQUAL);
	gl::depth_range(0.0, 1.0);
	gl::clear_depth( 1.0 );
	gl::clear_color(0.0f, 0.0f, 0.0f, 0.0f);
	gl::enable(GL_CULL_FACE);
	load_model( ":/models/arrow.obj" );
	load_model( ":/models/sphere5.obj" );
	load_model( ":/models/cylinder.obj" );
	load_model( ":/models/plane.obj" );
	std::cerr << std::hex << "load_model: " << gl::get_error() << '\n';

	load_program({":/shaders/test1.vsh"sv, ":/shaders/test1.fsh"sv} );
	load_program({":/shaders/ambient.vsh"sv, ":/shaders/gouraud.fsh"sv} );
	load_program({":/shaders/gouraud.vsh"sv, ":/shaders/gouraud.fsh"sv} );
	load_program({":/shaders/phong.vsh"sv, ":/shaders/lambert.fsh"sv} );
	load_program({":/shaders/phong.vsh"sv, ":/shaders/phong.fsh"sv} );
	load_program({":/shaders/phong.vsh"sv, ":/shaders/blinn.fsh"sv} );
	load_program({":/shaders/phong.vsh"sv, ":/shaders/gauss.fsh"sv} );
	load_program({":/shaders/phong.vsh"sv, ":/shaders/cook-torrance.fsh"sv} );
    load_program({":/shaders/cook-torr.vsh"sv, ":/shaders/cook-torr.fsh"sv} );
	auto trafo = aw::math::identity_matrix<float,4>;
	objects.emplace_back(object{1, 0, trafo, true});
	objects.emplace_back(object{2, 3, trafo, true});
	objects.emplace_back(object{3, 3, trafo, false});

	using namespace std::string_view_literals;
	camera.set_near_z(0.1f);
	camera.set_far_z(1000.0f);
	camera.set_fov( aw::math::degrees<float>{60} );
	aw::gl::use_program(aw::gl::no_program);
	std::cerr << "init! " << gl::get_error() << '\n';
}
void clear()
{
	gl::clear(GL_COLOR_BUFFER_BIT);
}

void update()
{
}

using aw::math::matrix3;
using aw::math::matrix4;
using aw::math::vector3d;
template<typename T>
matrix3<T> look_at_inverse(vector3d<T> forward, vector3d<T> up = {0,1,0})
{
	forward = -forward.normalize();
	auto right   = cross(up,forward).normalize();
	up           = cross(forward,right);

	matrix3<T> rot{ right, up, forward };
	return rot;
}
template<typename T>
matrix3<T> look_at(vector3d<T> forward, vector3d<T> up = {0,1,0})
{
	return transpose(graphics::look_at_inverse(forward,up));
}
template<typename T>
matrix4<T> look_at(vector3d<T> from, vector3d<T> to, vector3d<T> up)
{
	auto trs = expand_matrix( graphics::look_at(to - from, up) );
	trs[0][3] = from[0];
	trs[1][3] = from[1];
	trs[2][3] = from[2];
	return trs;
}

gl3::mat4 orbit(float rho, float phi, float theta)
{
	auto pos  = aw::math::spherical_to_cartesian( rho, degrees<float>(phi), degrees<float>(theta) );
	auto rot  = aw::math::yaw_matrix( degrees<float>(theta) ) * aw::math::pitch_matrix( degrees<float>(phi-90) );
	/*auto trafo = aw::math::identity_matrix<float,4>;
	trafo[0][3] = pos[0];
	trafo[1][3] = pos[1];
	trafo[2][3] = pos[2];
	trafo = rot;
	return trafo;*/
	return make_inverse_transform( pos, rot );
	//return make_transform( -pos, transpose(rot) );
}


decltype(obs)  obs = {9.0, 60, 0};
decltype(src ) src = {1.75, 55, 45};
decltype(light) light = {1,1,1, 1,1,1, 0.01,1.0,5.0};
void render()
{
	using namespace std::chrono;
	using namespace std::chrono_literals;
	static auto start1 = steady_clock::now();
	static auto start2 = steady_clock::now();
	auto cur = steady_clock::now();
	auto progress1 = (cur - start1) / 10.0s;
	auto progress2 = (cur - start2) / 2.0s;
	if (progress1 >= 1.0) start1 = cur;
	if (progress2 >= 1.0) start2 = cur;

	using aw::math::degrees;

	gl3::vec3 l      = spherical_to_cartesian( src.r, degrees<float>(src.p), degrees<float>(src.t) );
	gl3::vec4 lpos   = gl3::vec4{l.x(), l.y(), l.z(), 1};
	gl3::mat4 campos = orbit(obs.r, obs.p, obs.t);
	//campos = *inverse( campos );
	//std::cerr << to_string(campos) << '\n';

	objects[0].trafo.get(0,3) = lpos.x();
	objects[0].trafo.get(1,3) = lpos.y();
	objects[0].trafo.get(2,3) = lpos.z();
	lpos = campos * lpos;
	for (auto const& obj : objects) {
		if (!obj.draw) continue;
		gl3::program& program = programs[obj.program_id];
		aw::gl::use_program(aw::gl::program_handle(program));
		program["camtrafo"] = campos;
		program["light_pos"] = vec3{lpos.x(), lpos.y(), lpos.z()};
		program["light_color"] = vec3{light.r, light.g, light.b};
		//std::cerr << "lcol: " << light.r << ' ' << light.g << ' ' << light.b << ' ' << gl::get_error() << '\n';
		program["power"] = light.p;
		program["shininess"] = light.s;
		program["attenuation"] = light.a;
		program["view_matrix"] = camera.projection_matrix();
		//std::cerr << "lprop: " << light.p << ' ' << light.s << ' ' << light.a << ' ' << gl::get_error() << '\n';
		obj.render( program );
	}
	aw::gl::use_program(aw::gl::no_program);
}

void reshape(int w, int h)
{
	camera.set_aspect_ratio(float(w) / float(h));
}
} // namespace graphics
