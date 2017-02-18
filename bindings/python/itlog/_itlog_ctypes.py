################################################################################
#  THIS FILE IS 100% GENERATED BY ZPROJECT; DO NOT EDIT EXCEPT EXPERIMENTALLY  #
#  Read the zproject/README.md for information about making permanent changes. #
################################################################################

from __future__ import print_function
import os, sys
from ctypes import *
from ctypes.util import find_library

# itlog
lib = None
try:
    # check to see if the shared object was embedded locally, attempt to load it
    # if not, try to load it using the default system paths...
    # we need to use os.chdir instead of trying to modify $LD_LIBRARY_PATH and reloading the interpreter
    t = os.getcwd()
    p = os.path.join(os.path.dirname(__file__), '..')  # find the path to our $project_ctypes.py
    os.chdir(p)  # change directories briefly

    from itlog import libitlog                      # attempt to import the shared lib if it exists
    lib = CDLL(libitlog.__file__)            # if it exists try to load the shared lib
    os.chdir(t)  # switch back to orig dir
except ImportError:
    pass

if not lib:
    try:
        # If LD_LIBRARY_PATH or your OSs equivalent is set, this is the only way to
        # load the library.  If we use find_library below, we get the wrong result.
        if os.name == 'posix':
            if sys.platform == 'darwin':
                lib = cdll.LoadLibrary('libitlog.0.dylib')
            else:
                lib = cdll.LoadLibrary("libitlog.so.0")
        elif os.name == 'nt':
            lib = cdll.LoadLibrary('libitlog.dll')
    except OSError:
        libpath = find_library("itlog")
        if not libpath:
            raise ImportError("Unable to find libitlog")
        lib = cdll.LoadLibrary(libpath)

class il_log_t(Structure):
    pass # Empty - only for type checking
il_log_p = POINTER(il_log_t)

class FILE(Structure):
    pass # Empty - only for type checking
FILE_p = POINTER(FILE)

def return_py_file(c_file):
    if not sys.version_info > (3,):
        PyFile_FromFile_close_cb = CFUNCTYPE(c_int, FILE_p)
        PyFile_FromFile = pythonapi.PyFile_FromFile
        PyFile_FromFile.restype = py_object
        PyFile_FromFile.argtypes = [FILE_p,
                                    c_char_p,
                                    c_char_p,
                                    PyFile_FromFile_close_cb]
        return PyFile_FromFile(c_file, "", "r+", PyFile_FromFile_close_cb())

    else:
        fileno = libc.fileno
        fileno.restype = c_int
        fileno.argtypes = [c_void_p]

        return os.fdopen(fileno(c_file), r'r+b')

def coerce_py_file(obj):
    if not sys.version_info > (3,):
        PyFile_AsFile = pythonapi.PyFile_AsFile
        PyFile_AsFile.restype = FILE_p
        PyFile_AsFile.argtypes = [py_object]

        if isinstance(obj, FILE_p):
            return obj
        else:
            return PyFile_AsFile(obj)

    # Python 3 does not provide a low level buffered I/O (FILE*) API. Had to
    # resort to direct Standard C library calls.
    #
    #   https://docs.python.org/3/c-api/file.html.
    #
    else:
        fdopen = libc.fdopen
        fdopen.restype = FILE_p
        fdopen.argtypes = [c_int, c_char_p]

        setbuf = libc.setbuf
        setbuf.restype = None
        setbuf.argtypes = [FILE_p, c_char_p]

        if isinstance(obj, FILE_p):
            return obj
        else:
            fd = obj.fileno()
            fp = fdopen(fd, obj.mode.encode())

            # Make sure the file is opened in unbuffered mode. The test case
            # "test_zmsg" of the CZMQ Python fails if this mode is not set.
            setbuf(fp, None)

            return fp


# il_log
lib.il_log_new.restype = il_log_p
lib.il_log_new.argtypes = []
lib.il_log_destroy.restype = None
lib.il_log_destroy.argtypes = [POINTER(il_log_p)]
lib.il_log_set_output_interval.restype = None
lib.il_log_set_output_interval.argtypes = [il_log_p, c_int]
lib.il_log_set_header_frequency.restype = None
lib.il_log_set_header_frequency.argtypes = [il_log_p, c_int]
lib.il_log_entry.restype = c_bool
lib.il_log_entry.argtypes = [il_log_p, c_int, c_char_p, c_char_p, c_char_p, c_double, c_int]
lib.il_log_set_print_level.restype = None
lib.il_log_set_print_level.argtypes = [il_log_p, c_int]
lib.il_log_output_line.restype = None
lib.il_log_output_line.argtypes = [il_log_p]
lib.il_log_add_file.restype = None
lib.il_log_add_file.argtypes = [il_log_p, FILE_p]
lib.il_log_remove_file.restype = None
lib.il_log_remove_file.argtypes = [il_log_p, FILE_p]
lib.il_log_test.restype = None
lib.il_log_test.argtypes = [c_bool]

class IlLog(object):
    """
    Iteration logger class.
    """

    USE_LAST = 1 # Log entry supplied last.
    USE_AVERAGE = 2 # Log average of entries since last log line.
    USE_MIN = 3 # Log minimum of entries since last log line.
    USE_MAX = 4 # Log maximum of entries since last log line.
    USE_SUM = 5 # Log sum of entries since last log line.
    UNIT_INTERVAL = 0x100 # Flag for special printing of values in [0,1].
    allow_destruct = False
    def __init__(self, *args):
        """
        Constructor.
        """
        if len(args) == 2 and type(args[0]) is c_void_p and isinstance(args[1], bool):
            self._as_parameter_ = cast(args[0], il_log_p) # Conversion from raw type to binding
            self.allow_destruct = args[1] # This is a 'fresh' value, owned by us
        elif len(args) == 2 and type(args[0]) is il_log_p and isinstance(args[1], bool):
            self._as_parameter_ = args[0] # Conversion from raw type to binding
            self.allow_destruct = args[1] # This is a 'fresh' value, owned by us
        else:
            assert(len(args) == 0)
            self._as_parameter_ = lib.il_log_new() # Creation of new raw type
            self.allow_destruct = True

    def __del__(self):
        """
        Destructor.
        """
        if self.allow_destruct:
            lib.il_log_destroy(byref(self._as_parameter_))

    def __eq__(self, other):
        if type(other) == type(self):
            return other.c_address() == self.c_address()
        elif type(other) == c_void_p:
            return other.value == self.c_address()

    def c_address(self):
        """
        Return the address of the object pointer in c.  Useful for comparison.
        """
        return addressof(self._as_parameter_.contents)

    def __bool__(self):
        "Determine whether the object is valid by converting to boolean" # Python 3
        return self._as_parameter_.__bool__()

    def __nonzero__(self):
        "Determine whether the object is valid by converting to boolean" # Python 2
        return self._as_parameter_.__nonzero__()

    def set_output_interval(self, msecs):
        """
        Set the output interval in microseconds. The default is 0.
        """
        return lib.il_log_set_output_interval(self._as_parameter_, msecs)

    def set_header_frequency(self, freq):
        """
        Set the frequency of header lines. The default is 10.
        """
        return lib.il_log_set_header_frequency(self._as_parameter_, freq)

    def entry(self, print_level, header_format, header_name, entry_format, value, mode):
        """
        Add or update an entry of the logger provided that the print level argument does
not exceed the print level of the logger. If entries of the same header name
have been used before, the method accumulates the data and returns false. If no
entry exists yet, the method creates it and returns true. Possible modes for
accumulation of data are

* IL_LOG_USE_LAST: Entry supplied last
* IL_LOG_USE_AVERAGE: Average of entries since last printing.
* IL_LOG_USE_MIN: Minimum of entries since last printing.
* IL_LOG_USE_MAX: Maximum of entries since last printing.
* IL_LOG_USE_SUM: Sum of entries since last printing.

An additional flag IL_LOG_UNIT_INTERVAL can be set using "|" for special
printing of values between 0 and 1. In this case, the entry format should be
shorter than the header format by three characters.
        """
        return lib.il_log_entry(self._as_parameter_, print_level, header_format, header_name, entry_format, value, mode)

    def set_print_level(self, print_level):
        """
        Set the print level of the logger. All columns exceeding the current print level
will not get printed.
        """
        return lib.il_log_set_print_level(self._as_parameter_, print_level)

    def output_line(self):
        """
        If the time specified by the output interval has passed since the last time this
method has printed a line, the accumulated data will be formatted and printed.
Every 10th line, a newline and a header will be printed (the number can be
changed by setting the header frequency). The accumulated data of all entries
will be reset. If not enough time has passed, nothing happens. The output will
be printed to all files registerd with `il_log_add_file`. Standard output is
registered by default.
        """
        return lib.il_log_output_line(self._as_parameter_)

    def add_file(self, fid):
        """
        Add another file descriptor to the list of output streams.
        """
        return lib.il_log_add_file(self._as_parameter_, coerce_py_file(fid))

    def remove_file(self, fid):
        """
        Remove file descriptor from the list of output streams.
        """
        return lib.il_log_remove_file(self._as_parameter_, coerce_py_file(fid))

    @staticmethod
    def test(verbose):
        """
        Usage example and self test.
        """
        return lib.il_log_test(verbose)

################################################################################
#  THIS FILE IS 100% GENERATED BY ZPROJECT; DO NOT EDIT EXCEPT EXPERIMENTALLY  #
#  Read the zproject/README.md for information about making permanent changes. #
################################################################################
