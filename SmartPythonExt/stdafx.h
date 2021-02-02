// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>



// TODO: reference additional headers your program requires here

#include <string>
#include <iostream>
#include <sstream>

#pragma push_macro("slots")
#undef slots
#include <Python.h>
#include <frameobject.h>

//#include <boost/python/module.hpp>
//#include <boost/python/def.hpp>
//#include <boost/python.hpp>
#pragma pop_macro("slots")

//using namespace boost::python;

#include <pybind11/pybind11.h>
#include <pybind11/eval.h>
#include <pybind11/iostream.h>