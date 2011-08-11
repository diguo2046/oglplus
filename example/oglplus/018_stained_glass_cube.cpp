/**
 *  @example oglplus/018_stained_glass_cube.cpp
 *  @brief Shows how to draw a semi-transparent textured stained glass cube
 *
 *  @image html 018_stained_glass_cube.png
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/shapes/cube.hpp>
#include <oglplus/images/load.hpp>
#include <oglplus/bound/texture.hpp>

#include <cmath>

#include "example.hpp"

namespace oglplus {

class CubeExample : public Example
{
private:
	// helper object building cube vertex attributes
	shapes::Cube make_cube;
	// helper object encapsulating cube drawing instructions
	shapes::DrawingInstructions cube_instr;
	// indices pointing to cube primitive elements
	shapes::Cube::IndexArray cube_indices;

	// wrapper around the current OpenGL context
	Context gl;

	// Vertex shader
	VertexShader vs;

	// Fragment shader
	FragmentShader fs;

	// Program
	Program prog;

	// A vertex array object for the rendered cube
	VertexArray cube;

	// VBOs for the cube's vertex attributes
	Buffer verts, normals, texcoords;

	// The stained glass texture
	Texture tex;
public:
	CubeExample(void)
	 : cube_instr(make_cube.Instructions())
	 , cube_indices(make_cube.Indices())
	{
		// Set the vertex shader source
		vs.Source(
			"#version 330\n"
			"uniform mat4 projectionMatrix, cameraMatrix, modelMatrix;"
			"in vec4 vertex;"
			"in vec3 normal;"
			"in vec2 texcoord;"
			"out vec3 fragNormal;"
			"out vec3 fragLight;"
			"out vec2 fragTex;"
			"uniform vec3 lightPos;"
			"void main(void)"
			"{"
			"	fragNormal = ("
			"		modelMatrix *"
			"		vec4(normal, 0.0)"
			"	).xyz;"
			"	fragLight = ("
			"		vec4(lightPos, 0.0)-"
			"		modelMatrix*vertex"
			"	).xyz;"
			"	fragTex = texcoord;"
			"	gl_Position = "
			"		projectionMatrix *"
			"		cameraMatrix *"
			"		modelMatrix *"
			"		vertex;"
			"}"
		);
		// compile it
		vs.Compile();

		// set the fragment shader source
		fs.Source(
			"#version 330\n"
			"uniform sampler2D tex;"
			"in vec3 fragNormal;"
			"in vec3 fragLight;"
			"in vec2 fragTex;"
			"out vec4 fragColor;"
			"void main(void)"
			"{"
			"	float l = length(fragLight);"
			"	float d = l!=0? dot(fragNormal, fragLight)/l:0.0;"
			"	float e = d < 0? -0.7*d: d;"
			"	float i = clamp(0.1 + e, 0.0, 1.0);"
			"	vec4 t  = texture2D(tex, fragTex);"
			"	fragColor = vec4(t.rgb*i, t.a);"
			"}"
		);
		// compile it
		fs.Compile();

		// attach the shaders to the program
		prog.AttachShader(vs);
		prog.AttachShader(fs);
		// link and use it
		prog.Link();
		prog.Use();

		// bind the VAO for the cube
		cube.Bind();

		verts.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_cube.Vertices(data);
			Buffer::Data(Buffer::Target::Array, data);
			VertexAttribArray attr(prog, "vertex");
			attr.Setup(n_per_vertex, DataType::Float);
			attr.Enable();
		}

		normals.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_cube.Normals(data);
			Buffer::Data(Buffer::Target::Array, data);
			VertexAttribArray attr(prog, "normal");
			attr.Setup(n_per_vertex, DataType::Float);
			attr.Enable();
		}

		texcoords.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_cube.TexCoordinates(data);
			Buffer::Data(Buffer::Target::Array, data);
			VertexAttribArray attr(prog, "texcoord");
			attr.Setup(n_per_vertex, DataType::Float);
			attr.Enable();
		}

		// setup the texture
		{
			auto bound_tex = Bind(tex, Texture::Target::_2D);
			bound_tex.Image2D(images::LoadTexture("flower_glass"));
			bound_tex.GenerateMipmap();
			bound_tex.MinFilter(TextureMinFilter::LinearMipmapLinear);
			bound_tex.MagFilter(TextureMagFilter::Linear);
			bound_tex.WrapS(TextureWrap::Repeat);
			bound_tex.WrapT(TextureWrap::Repeat);
		}
		//
		Uniform(prog, "tex").Set(0);
		Uniform(prog, "lightPos").Set(Vec3f(1.0f, 2.0f, 3.0f));
		//
		gl.ClearColor(0.1f, 0.1f, 0.1f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);
		gl.Enable(Capability::Blend);
		gl.BlendFunc(
			BlendFn::SrcAlpha,
			BlendFn::OneMinusSrcAlpha
		);

		gl.Enable(Capability::CullFace);
		gl.FrontFace(make_cube.FaceWinding());
	}

	void Reshape(size_t width, size_t height)
	{
		gl.Viewport(width, height);
		// set the projection matrix fov = 24 deg. aspect = width/height
		prog.Use();
		Uniform(prog, "projectionMatrix").SetMatrix(
			CamMatrixf::Perspective(
				Degrees(24),
				double(width)/height,
				1, 100
			)
		);
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer().DepthBuffer();
		//
		// set the matrix for camera orbiting the origin
		Uniform(prog, "cameraMatrix").SetMatrix(
			CamMatrixf::Orbiting(
				Vec3f(),
				3.5 - std::sin(time) * 2.5,
				FullCircles(time * 0.7),
				Degrees(std::sin(time * 0.2) * 90)
			)
		);

		// set the model matrix
		Uniform(prog, "modelMatrix").SetMatrix(
			ModelMatrixf::RotationX(FullCircles(time * 0.1))
		);

		cube.Bind();
		gl.CullFace(Face::Front);
		cube_instr.Draw(cube_indices);
		gl.CullFace(Face::Back);
		cube_instr.Draw(cube_indices);
	}

	bool Continue(double time)
	{
		return time < 30.0;
	}
};

std::unique_ptr<Example> makeExample(void)
{
	return std::unique_ptr<Example>(new CubeExample);
}

} // namespace oglplus
