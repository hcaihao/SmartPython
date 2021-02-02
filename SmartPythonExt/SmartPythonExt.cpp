// SmartPythonExt.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

char const* greet()
{
	return "SmartPythonExt Test";
}

//SmartPythonExt.pyd
//BOOST_PYTHON_MODULE(SmartPythonExt)
//{
//	boost::python::def("greet", greet);
//}
//


PYBIND11_MODULE(SmartPythonExt, m) {
	m.doc() = "pybind11 test"; // optional module docstring
	m.def("greet", &greet, "A function which test pybind11");

	//m.def("read_from_my_stream", [&my_stream]() {
	//	std::string r;
	//	std::getline(my_stream, r);
	//	return r;
	//	);

}