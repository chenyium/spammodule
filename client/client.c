#include <Python.h>
#include "..\spammodule\spammodule.h"

static PyObject* client_system(PyObject *self, PyObject *args)
{
	int ret;
	const char *command;

	if (!PyArg_ParseTuple(args, "s", &command))
		return NULL;

	ret = PySpam_System(command);
	return Py_BuildValue("i", ret);
}

static PyMethodDef ClientMethods[] = {
	{ "system", client_system, METH_VARARGS, "Execute a shell command" },
	{ NULL, NULL, 0, NULL }
};

PyMODINIT_FUNC initclient(void)
{
	PyObject *module;
	module = Py_InitModule("client", ClientMethods);
	if (NULL == module)
		return;

	if (import_spam() < 0)
		return;
}