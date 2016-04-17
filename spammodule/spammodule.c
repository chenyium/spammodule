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

static PyObject* my_callback = NULL;
static PyObject* callback_set(PyObject *self, PyObject *args)
{
	PyObject *temp;
	if (!PyArg_ParseTuple(args, "O:set_callback", &temp))
		return NULL;

	if (!PyCallable_Check(temp)) {
		PyErr_SetString(PyExc_TypeError, "parameter must be callable");
		return NULL;
	}

	Py_XINCREF(temp);         /* Add a reference to new callback */
	Py_XDECREF(my_callback);  /* Dispose of previous callback */
	my_callback = temp;       /* Remember new callback */

	/* Boilerplate to return "None" */
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject* callback_call(PyObject *self, PyObject *args)
{
	PyObject *result;
	if (NULL == my_callback) {
		PyErr_SetString(SpamNotFound, "callback is null");
		return NULL;
	}

	/* Time to call the callback */
	result = PyObject_CallObject(my_callback, args);
	if (result == NULL)
		return NULL; /* Pass error back */

	return result;
}

static PyMethodDef SpamMethods[] = {
	{ "system", spam_system, METH_VARARGS, "Execute a shell command" },
	{ "callbackset", callback_set, METH_VARARGS, "Set a callback function" },
	{ "callbackcall", callback_call, METH_VARARGS, "Call a callback function" },
	{ NULL, NULL, 0, NULL }
};

PyMODINIT_FUNC initspammodule(void)
{
	PyObject *module;
	PyObject *version = PyString_FromString("1.0.0");
	PyObject *author  = PyString_FromString("chenyao");

	module = Py_InitModule("spammodule", SpamMethods);
	if (NULL == module)
		return;

	SpamError = PyErr_NewException("spammodule.errorClass", NULL, NULL);
	SpamNotFound = PyErr_NewException("spammodule.notfoundClass", NULL, NULL);
	Py_INCREF(SpamError);
	Py_INCREF(SpamNotFound);
	PyModule_AddObject(module, "error", SpamError);
	PyModule_AddObject(module, "notfound", SpamNotFound);

	PyModule_AddObject(module, "version", version);
	PyModule_AddObject(module, "author", author);
}