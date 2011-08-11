/**
 *  @file oglplus/images/load.hpp
 *  @brief Image loader which finds and loads an image from its name
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef OGLPLUS_IMAGES_LOAD_1107121519_HPP
#define OGLPLUS_IMAGES_LOAD_1107121519_HPP

#include <oglplus/application.hpp>
#include <oglplus/image.hpp>
#include <oglplus/texture.hpp>
#include <oglplus/images/png.hpp>

#include <string>
#include <fstream>
#include <stdexcept>

namespace oglplus {
namespace images {
namespace aux {

bool FindImageWithExts(
	std::ifstream& file,
	const std::string& path,
	const char** exts,
	size_t& e,
	size_t nexts
)
{
	for(e=0; e!= nexts; ++e)
	{
		file.open(path + exts[e]);
		if(file.good()) return true;
	}
	return false;
}

bool FindImage(
	std::ifstream& file,
	std::string dirname,
	std::string name,
	const char** exts,
	size_t& e,
	size_t nexts
)
{
	// TODO: switch this on platforms using other directory separators
	const std::string dirsep("/");
	const std::string pardir(".." + dirsep);
	const std::string path = dirname+dirsep+name;
	const std::string apppath = Application::RelativePath();
	std::string prefix;

	for(size_t i=0; i!=3; ++i)
	{
		if(FindImageWithExts(file, apppath+prefix+path, exts, e, nexts))
			return true;
		prefix = pardir + prefix;
	}
	if(FindImageWithExts(file, apppath+name, exts, e, nexts))
		return true;
	return false;
}

} // namespace aux

inline Image<GLubyte> LoadByName(std::string dirname, std::string name)
{
	std::ifstream file;
	const char* exts[] = {".png"};
	size_t nexts = sizeof(exts)/sizeof(exts[0]);
	size_t iext = nexts;
	aux::FindImage(file, dirname, name, exts, iext, nexts);

	if(!file.good())
		throw std::runtime_error("Unable to open image: "+name);
	// TODO switch on extension
	assert(iext == 0);
	return PNG(file);
}

/// Helper function for loading textures that come with OGLplus in the examples
inline Image<GLubyte> LoadTexture(std::string name)
{
	return LoadByName("textures", name);
}

} // images
} // oglplus

#endif // include guard
