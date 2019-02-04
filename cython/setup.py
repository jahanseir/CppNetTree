from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize

# '-stdlib=libc++' needs to be added to extra_compile_args on Mac

ext_modules = [Extension('nettree_adapter', 
                         sources=['nettree_adapter.pyx'], 
                         extra_compile_args=['-std=c++11'],#, '-stdlib=libc++'], 
                         language='c++')]

setup(ext_modules = cythonize(ext_modules))
