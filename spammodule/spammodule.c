#include <Python.h>

static PyObject *SpamError;
static PyObject *SpamNotFound;

static PyObject* spam_system(PyObject *self, PyObject *args)
{
	int ret;
	const char *command;

	if (!PyArg_ParseTuple(args, "s", &command))
		return NULL;

	ret = system(command);
	if (0 != ret) {
		if (ret < 0)
			PyErr_SetString(SpamError, "system call failed");
		else if (ret > 0)
			PyErr_SetString(SpamNotFound, "command not found");
		return NULL;
	}

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

	SpamError = PyErr_NewException("spammodule.errorClass", NULL, NULL);
	SpamNotFound = PyErr_NewException("spammodule.notfoundClass", NULL, NULL);
	Py_INCREF(SpamError);
	Py_INCREF(SpamNotFound);
	PyModule_AddObject(module, "error", SpamError);
	PyModule_AddObject(module, "notfound", SpamNotFound);

}