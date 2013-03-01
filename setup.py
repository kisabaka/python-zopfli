
import glob
from distutils.core import setup, Extension

module = Extension('Zopfli', sources=['Zopfli.c'] + glob.glob('zopfli/*.c'),
                    include_dirs=['zopfli'])
setup(name="Zopfli",
    version='0.1',
    description='Python module for the Zopfli algorithm',
    ext_modules=[module])
