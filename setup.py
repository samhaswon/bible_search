from setuptools import setup, Extension

setup(
    name='multi_bible_search',
    ext_modules=[
        Extension('multi_bible_search.multi_bible_search',
                  ['src/multi_bible_search/multi_bible_search.c'],
                  include_dirs=['src/multi_bible_search/'],
                  )
    ],
)
