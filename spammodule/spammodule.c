#include <Python.h>

static PyObject* spam_system(PyObject *self, PyObject *args)
{
	int ret;
	const char *command;

	if (!PyArg_ParseTuple(args, "s", &command))
		return NULL;

	ret = system(command);
	return Py_BuildValue("i", ret);
}

static PyMethodDef SpamMethods[] = {
	{ "system", spam_system, METH_VARARGS, "Execute a shell command" },
	{ NULL, NULL, 0, NULL }
};

PyMODINIT_FUNC initspammodule(void)
{
	PyObject *module;

	module = Py_InitModule("spammodule", SpamMethods);
	if (NULL == module)
		return;
}