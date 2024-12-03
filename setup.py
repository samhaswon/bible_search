"""
Set up the multi-bible-search extension.
"""
import os
from typing import List, Tuple
from urllib import request

# pylint: disable=import-error
from setuptools import setup, Extension
from setuptools.command.build_py import build_py


# pylint: disable=too-few-public-methods
class CustomBuild(build_py):
    """
    Custom build to download indices at build time from GitHub LFS
    """
    indexes = [
        "ACV.json.pbz2", "AKJV.json.pbz2", "AllEng.json.pbz2", "AllEs.json.pbz2", "AMP.json.pbz2",
        "ASV.json.pbz2", "BBE.json.pbz2", "BSB.json.pbz2", "BTX3.json.pbz2", "CSB.json.pbz2",
        "Darby.json.pbz2", "DRA.json.pbz2", "Dynamic.json.pbz2", "EBR.json.pbz2", "EsRV.json.pbz2",
        "ESV.json.pbz2", "ExtraEng.json.pbz2", "GNV.json.pbz2", "KJV%201611.json.pbz2",
        "KJV-like.json.pbz2", "KJV.json.pbz2", "Literal.json.pbz2", "Literal2.json.pbz2",
        "LSV.json.pbz2", "MSG.json.pbz2", "NASB%201995.json.pbz2", "NET.json.pbz2",
        "NIV%201984.json.pbz2", "NIV%202011.json.pbz2", "NIV.json.pbz2", "NKJV.json.pbz2",
        "NLT.json.pbz2", "RNKJV.json.pbz2", "RSV.json.pbz2", "RV1960.json.pbz2",
        "RV2004.json.pbz2", "RWV.json.pbz2", "UKJV.json.pbz2", "WEB.json.pbz2", "YLT.json.pbz2",
    ]
    __index_file_urls: List[Tuple[str, str]] = \
        [
            (f"https://github.com/samhaswon/bible_search/"
             f"raw/refs/heads/main/src/multi_bible_search/data/{x}",
             x
             )
            for x in indexes
        ]

    def run(self):
        """
        Build the module
        """
        # Perform the normal build
        super().run()

        # Custom logic: Download files
        download_folder = os.path.join(self.build_lib, "multi_bible_search", "data")
        os.makedirs(download_folder, exist_ok=True)
        # file_url = "https://example.com/somefile.txt"
        # destination = os.path.join(download_folder, "somefile.txt")

        for file_url, destination in self.__index_file_urls:
            print(f"Downloading {file_url} to {destination}")
            request.urlretrieve(file_url, destination)


if os.name == "nt":
    # MSVC flags
    flags = ["/favor:blend",  # Optimize for AMD and Intel
             "/GF",  # (Eliminate Duplicate Strings)
             "/Qpar",  # (Auto-parallelizer)
             "/arch:AVX2",  # For some reason, AVX2 is faster than AVX512 in my testing.
             "/fp:fast"  # Faster floating point ops, since precision is not needed for this
             ]
else:
    flags = ["-O3", "-mavx2"]

setup(
    name='multi_bible_search',
    ext_modules=[
        Extension('multi_bible_search.multi_bible_search',
                  ['src/multi_bible_search/multi_bible_search.c'],
                  include_dirs=['src/multi_bible_search/'],
                  extra_compile_args=flags,
                  )
    ],
    cmdclass={
            "build_py": CustomBuild,  # Override the build_py step
        },
)
