#include <Python.h>
#include <stdexcept>

#define BOOST_PYTHON_STATIC_LIB
#include <boost/python.hpp>
using namespace boost::python;

char const* greetmsgs(unsigned x)
{
	static char const* const msgs[] = { "hello", "Boost.Python", "world!" };

	if (x > 2) 
		throw std::range_error("greet: index out of range");

	return msgs[x];
}

#define BOOST_PYTHON
#ifndef BOOST_PYTHON
extern "C" // all Python interactions use 'C' linkage and calling convention
{
	// Wrapper to handle argument/result conversion and checking
	PyObject* greet_wrap(PyObject* self, PyObject* args)
	{
		int x;
		if (PyArg_ParseTuple(args, "i", &x))    // extract/check arguments
		{
			char const* result = greetmsgs(x);  // invoke wrapped function
			return PyString_FromString(result); // convert result to Python
		}
		return 0;                               // error occurred
	}

	// Table of wrapped functions to be exposed by the module
	static PyMethodDef methods[] = {
		{ "greet", greet_wrap, METH_VARARGS, "return one of 3 parts of a greeting" }
		, { NULL, NULL, 0, NULL } // sentinel
	};

	// module initialization function
	PyMODINIT_FUNC inithelloboost()
	{
		(void) Py_InitModule("helloboost", methods); // add the methods to the module
	}
}
#else
BOOST_PYTHON_MODULE(helloboost)
{
	def("greet", greetmsgs, "return one of 3 parts of a greeting");
}
#endif

