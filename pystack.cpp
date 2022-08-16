#include "Python.h"
#include "frameobject.h"

#define PyFrame_Check(op) ((op)->ob_type == &PyFrame_Type)

static PyObject* getStackSize(PyObject *self, PyObject *args)
{
	PyObject **stack_pointer;
	PyFrameObject* frame;

	if (!PyFrame_Check(args))
	{
		PyErr_SetString(PyExc_TypeError, "Argument to getStackSize must be a frame object.");
		return NULL;
	}
	frame = (PyFrameObject*) args;
	stack_pointer = frame->f_stacktop;
	int stacksize = ((int)(stack_pointer - frame->f_valuestack));
	return PyInt_FromLong(stacksize);	
}

static PyObject* getStackItem(PyObject *self, PyObject *args)
{
	PyFrameObject* frame;
	PyObject **stack_pointer;
	int itemindex;

    if (!PyArg_ParseTuple(args, "Oi", &frame, &itemindex))
	{
		PyErr_SetString(PyExc_TypeError, "Argument to getStackItem must be frame object and an integer index.");
        return NULL;
	}
	stack_pointer = frame->f_stacktop;
	int stacksize = ((int)(stack_pointer - frame->f_valuestack));

    if (itemindex < 0 || itemindex >= stacksize)
	{
		// Invalid index
		Py_RETURN_NONE;		
	}
	
	PyObject *ob = stack_pointer[-(itemindex+1)];
	Py_INCREF(ob);
	return ob;
}


static PyMethodDef pystackMethods[] = 
{    
    {"getStackSize", getStackSize, METH_O, "Gets the stack size."}, 
	{"getStackItem", getStackItem, METH_VARARGS, "Gets the stack item at an index."}, 
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC initpystack(void)
{
    PyObject *m;

    m = Py_InitModule("pystack", pystackMethods);
    if (!m)
	{
	    PyErr_SetString(PyExc_Exception, "Cannot import pystack!");
	}
}
