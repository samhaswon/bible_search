import bz2
import os
import sys
from typing import List, Union
from .multi_bible_search import BibleSearch as cBibleSearch


class BibleSearch(object):
    """
    Search versions of the Bible
    """
    def __init__(self, preload: Union[List[str], None] = None):
        """
        :param preload: List of versions to preload.
        """
        # Some attributes
        self.__c_search = cBibleSearch()
        self.__kjv_like: set = {"AKJV", "KJV", "KJV 1611", "RNKJV", "UKJV"}
        self.__versions: set = {'ACV', 'AKJV', 'AMP', 'ASV', 'BBE', 'BSB', 'CSB', 'Darby', 'DRA', 'EBR', 'ESV', 'GNV',
                                'KJV', 'KJV 1611', 'LSV', 'MSG', 'NASB 1995', 'NET', 'NIV 1984', 'NIV 2011', 'NKJV',
                                'NLT', 'RNKJV', 'RSV', 'RWV', 'UKJV', 'WEB', 'YLT'}

        # Preload common indices
        self._load_version("All", preload=True)
        self._load_version("KJV-like", preload=True)
        self._load_version("NIV", preload=True)

        self.__loaded: set = set()

        if preload:
            for version in preload:
                self.load(version)

    def _load_version(self, version: str, preload: bool = False) -> None:
        """
        Preloads a given version's search index.
        It assumes that the version is valid.
        :param version: The version to preload.
        :return: None
        """
        base_path = os.path.dirname(os.path.abspath(__file__))
        with bz2.open(f"{base_path}/data/{version}.json.pbz2", "rt", encoding='utf-8') as data_file:
            self.__c_search.load(data_file.read(), version)
        if not preload:
            self.__loaded.add(version)

    def load(self, version: str) -> None:
        """
        Preloads a given version's search index.
        :param version: The version to preload.
        :return: None
        :raises KeyError: For invalid version strings.
        """
        # Quick check that the version is valid
        if version not in self.__versions:
            raise KeyError("Invalid version string")
        # Load the version
        self._load_version(version)

    def load_all(self) -> None:
        """
        Preload all version indices.
        """
        for version in self.__versions:
            if version not in self.__loaded:
                self._load_version(version)

    def unload_version(self, version: str) -> None:
        """
        Unload a version's index from memory.
        :param version: The version to remove.
        :return: None
        :raises Exception: If the version is invalid or not loaded, raises an exception.
        """
        if version in self.__versions and version in self.__loaded:
            self.__c_search.unload(version)
            self.__loaded.remove(version)
        else:
            raise Exception(f"Invalid version {version}")

    def search(self, query: str, version: str = "KJV", max_results: int = sys.maxsize) -> List[str]:
        """
        Search for a passage in the Bible.
        :param query: The search query string.
        :param version: The version to search.
        :param max_results: The maximum number of results to retrieve.
        :return: List of match references (e.g., `["John 11:35", "Matthew 1:7", ...]`).
        """
        # Load the version if it is not already loaded
        if version not in self.__loaded:
            self.load(version)
        return self.__c_search.search(query, version, max_results)

    def internal_index_size(self) -> int:
        """
        Gets the size of the index stored in C in bytes.
        :return: Index size in bytes.
        """
        return self.__c_search.index_size()

    @property
    def loaded(self) -> List[str]:
        """
        A list of the versions currently loaded in the search object. This does not include common indices.
        """
        return list(self.__loaded)

    @property
    def versions(self) -> List[str]:
        """
        A list of versions available in the search index.
        """
        return list(self.__versions)
