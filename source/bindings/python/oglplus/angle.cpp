/*
 *  .file oglplus/angle.cpp
 *
 *  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <oglplus/gl.hpp>
#include <oglplus/angle.hpp>

#include <boost/python.hpp>

#include "./angle.hpp"

void oglplus_py_Angle(void)
{
	namespace bpy = ::boost::python;
	using namespace oglplus;

	oglplus_py_export_Angle((Anglef*)0, "Anglef");

	bpy::def("Degrees", Degrees);
	bpy::def("Radians", Radians);
	bpy::def("FullCircles", FullCircles);
	bpy::def("FullCircle", FullCircle);
	bpy::def("RightAngles", RightAngles);
	bpy::def("RightAngle", RightAngle);
}

