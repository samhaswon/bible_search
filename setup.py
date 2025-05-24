"""
Set up the multi-bible-search extension.
"""
import os

# pylint: disable=import-error
from setuptools import setup, Extension


if os.name == "nt":
    # MSVC flags
    flags = ["/favor:blend",  # Optimize for AMD and Intel
             "/GF",  # (Eliminate Duplicate Strings)
             "/Qpar",  # (Auto-parallelizer)
             "/arch:AVX2",  # At this point, this has a negligible impact
             "/fp:fast"  # Faster floating point ops, since precision is unnecessary for this
             ]
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
