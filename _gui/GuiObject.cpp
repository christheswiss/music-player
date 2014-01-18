#include "GuiObject.hpp"

int GuiObject::init(PyObject* args, PyObject* kwds) {
	DefaultSpace = Vec(8,8);
	OuterSpace = Vec(8,8);
	return 0;
}

static PyObject* returnObj(PyObject* obj) {
	if(!obj) obj = Py_None;
	Py_INCREF(obj);
	return obj;
}

#define _ReturnAttr(attr) { if(strcmp(key, #attr) == 0) return returnObj(attr); }

PyObject* Vec::asTuple() const {
	PyObject* t = PyTuple_New(2);
	if(!t) return NULL;
	PyTuple_SET_ITEM(t, 0, PyInt_FromLong(x));
	PyTuple_SET_ITEM(t, 1, PyInt_FromLong(y));
	return t;
}

#define _ReturnAttrVec(attr) { if(strcmp(key, #attr) == 0) return attr.asTuple(); }

PyObject* GuiObject::getattr(const char* key) {
	_ReturnAttr(root);
	_ReturnAttr(parent);
	_ReturnAttr(attr);
	_ReturnAttr(nativeGuiObject);
	_ReturnAttr(subjectObject);
	_ReturnAttrVec(DefaultSpace);
	_ReturnAttrVec(OuterSpace);
	
	if(strcmp(key, "autoresize") == 0
	|| strcmp(key, "pos") == 0
	|| strcmp(key, "size") == 0
	|| strcmp(key, "addChild") == 0) {
		PyErr_Format(PyExc_AttributeError, "GuiObject attribute '%.400s' must be specified in subclass", key);
		return NULL;
	}
	
	PyErr_Format(PyExc_AttributeError, "GuiObject has no attribute '%.400s'", key);
	return NULL;
	
returnNone:
	Py_INCREF(Py_None);
	return Py_None;
}

int GuiObject::setattr(const char* key, PyObject* value) {
	PyObject* s = PyString_FromString(key);
	if(!s) return -1;
	// While we have no own __dict__, this will fail. But that is what we want.
	int ret = PyObject_GenericSetAttr((PyObject*) this, s, value);
	Py_XDECREF(s);
	return ret;
}