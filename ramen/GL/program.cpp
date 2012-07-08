// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/GL/program.hpp>

#include<ramen/assert.hpp>

#include<iostream>

namespace ramen
{
namespace gl
{

program_t::program_t( GLenum shader_type) : shader_type_( shader_type)
{
	shader_ = 0;
	program_ = 0;
}

program_t::~program_t()
{
	base::gl_delete_program( program_);
	base::gl_delete_shader( shader_);
}

void program_t::compile( const char *src0, const char *src1)
{
	if( !program_)
	{
		shader_ = base::gl_create_shader( shader_type_);
		program_ = base::gl_create_program();
	}
	else
		base::gl_detach_shader( program_, shader_);

    const GLchar *shader_src[2];
    shader_src[0] = (GLchar *) src0;
    shader_src[1] = (GLchar *) src1;

	/*
	#ifndef NDEBUG
		std::cout << "Shader souce =\n";
		std::cout << "------------\n\n";
		std::cout << src0 << "\n";
		if( src1)
			std::cout << src1 << "\n";
		std::cout << "\n";
	#endif
	*/

	base::gl_shader_source( shader_, src1 ? 2 : 1, shader_src, 0);
    base::gl_compile_shader( shader_);

    GLint result;
    glGetShaderiv( shader_, GL_COMPILE_STATUS, &result);
	base::check_gl_errors();

    if( result == GL_FALSE)
    {
        char buffer[1025];
        GLsizei size;
        glGetShaderInfoLog( shader_, 1024, &size, buffer);
		base::check_gl_errors();

        buffer[ size] = 0;
        std::cout << buffer << "\n";
    }

    base::gl_attach_shader( program_, shader_);
    base::gl_link_program( program_);

    glGetProgramiv( program_, GL_LINK_STATUS, &result);
	base::check_gl_errors();

    if( result == GL_FALSE)
    {
        char buffer[1025];
        GLsizei size;
        glGetProgramInfoLog( program_, 1024, &size, buffer);
		base::check_gl_errors();
        buffer[ size] = 0;
        std::cout << buffer << "\n";
    }
}

void program_t::bind()
{
	RAMEN_ASSERT( program_);
	base::gl_use_program( program_);
}

void program_t::unbind() { base::gl_use_program( 0);}

void program_t::uniform_1i( const char *param, int v)
{
	RAMEN_ASSERT( program_);
    base::gl_uniform1i( base::gl_get_uniform_location( program_, param), v);
	base::check_gl_errors();
}

void program_t::uniform_1f( const char *param, float v)
{
	RAMEN_ASSERT( program_);
    base::gl_uniform1f( base::gl_get_uniform_location( program_, param), v);
	base::check_gl_errors();
}

void program_t::uniform_4f( const char *param, float *v)
{
	RAMEN_ASSERT( program_);
    base::gl_uniform4f( base::gl_get_uniform_location( program_, param), v[0], v[1], v[2], v[3]);
	base::check_gl_errors();
}

} // namespace
} // namespace
