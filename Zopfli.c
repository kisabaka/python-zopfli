#include <Python.h>
#include "zopfli/util.h"
#include "zopfli/zopfli.h"

static PyObject *zopfli_compress_file(PyObject *self, PyObject *args) {
    const char *infilename;
    const char *outfilename;
    if (!PyArg_ParseTuple(args, "ss", &infilename, &outfilename)) {
        return NULL;
    }
    Options options;
    InitOptions(&options);
    OutputType output_type = OUTPUT_GZIP;
    options.numiterations = 5;
    CompressFile(&options, output_type, infilename, outfilename);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyMethodDef method_list[] = {
    {"compress_file", zopfli_compress_file, METH_VARARGS, "Compress a file"},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initZopfli(void) {
    PyObject *m;
    m = Py_InitModule("Zopfli", method_list);
    if (m == NULL)
        return;
}

int main(int argc, char *argv[]) {
    Py_SetProgramName(argv[0]);
    Py_Initialize();
    initZopfli();
}
