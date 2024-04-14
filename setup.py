from setuptools import setup, Extension

setup(
    name='multi_bible_search',
    ext_modules=[
        Extension('ctranslate.ctranslate', ['src/ctranslate/ctranslate.c']),
    ],
)
