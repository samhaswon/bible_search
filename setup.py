"""
Set up the multi-bible-search extension.
"""
import os
import platform

# pylint: disable=import-error
from setuptools import setup, Extension


if os.name == "nt":
    # MSVC flags
    flags = ["/favor:blend",  # Optimize for AMD and Intel
             "/GF",  # (Eliminate Duplicate Strings)
             "/Qpar",  # (Auto-parallelizer)
             "/arch:AVX2",  # For some reason, AVX2 is faster than AVX512 in my testing.
             "/fp:fast"  # Faster floating point ops, since precision is not needed for this
             ]
elif "arm" not in platform.processor() and "Arm" not in platform.processor():
    flags = ["-O3", "-mavx2"]
else:
    flags = ["-O3"]

setup(
    name='multi_bible_search',
    ext_modules=[
        Extension('multi_bible_search.multi_bible_search',
                  ['src/multi_bible_search/multi_bible_search.c'],
                  include_dirs=['src/multi_bible_search/'],
                  extra_compile_args=flags,
                  )
    ],
)
