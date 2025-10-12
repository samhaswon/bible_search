"""
Adapter class for the C implementation of the search engine.
This mostly just makes it easier to work with the module
by automatically managing version indexes which the C class does not do.
"""
import bz2
import os
import sys
from typing import List, Union

# PyCharm and Pylint both can't figure this one out,
# but it works and is correct.
# pylint: disable=no-name-in-module
from .multi_bible_search import BibleSearch as cBibleSearch
from .invalid_version import InvalidVersion


class BibleSearch:
    """
    Search versions of the Bible
    """
    def __init__(self, preload: Union[List[str], None] = None):
        """
        :param preload: List of versions to preload.
        """
        # (C) Search object
        self.__c_search = cBibleSearch()

        try:
            if len(os.listdir(os.path.join(__file__[:-23], "data"))) < 40:
                from .bible_downloader import BibleDownloader

                downloader = BibleDownloader()
                downloader.download()
        except FileNotFoundError:
            from .bible_downloader import BibleDownloader

            downloader = BibleDownloader()
            downloader.download()

        # Common sets
        self.__dynamic: set = {"CSB", "NLT", "NET"}
        self.__kjv_like: set = {"AKJV", "GNV", "KJV", "KJV 1611", "RNKJV", "UKJV"}
        self.__literal: set = {"ACV", "AMP", "ASV", "ESV", "NASB 1995", "NKJV", "RSV", "RWV", "WEB"}
        self.__literal2: set = {"BSB", "LSV", "YLT"}
        self.__niv: set = {"NIV 1984", "NIV 2011"}
        self.__esrv: set = {'RV1960', 'RV2004'}
        self.__extra_english: set = {"Darby", "EBR"}

        # Language sets
        self.__english_versions: set = {'ACV', 'AKJV', 'AMP', 'ASV', 'BBE', 'BSB', 'CSB', 'Darby',
                                        'DRA', 'EBR', 'ESV', 'GNV', 'KJV', 'KJV 1611', 'LSV',
                                        'MSG', 'NASB 1995', 'NET', 'NIV 1984', 'NIV 2011', 'NKJV',
                                        'NLT', 'RNKJV', 'RSV', 'RWV', 'UKJV', 'WEB', 'YLT'}
        self.__spanish_versions: set = {'BTX3', 'RV1960', 'RV2004'}

        # Every supported version
        self.__versions: set = self.__english_versions | self.__spanish_versions

        # Preload common index
        self._load_version("AllEng", preload=True)

        # What is currently stored in C
        self.__loaded: set = set()
        self.__preloaded: set = set()

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
        with bz2.open(
                f"{base_path}/data/{version}.json.pbz2",
                "rt",
                encoding='utf-8'
        ) as data_file:
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
            raise InvalidVersion(version)

        # load Spanish common index if applicable
        if version in self.__spanish_versions and "AllEs" not in self.__preloaded:
            self._load_version("AllEs", preload=True)
            self.__preloaded.add("AllEs")

        # Load KJV-like common index if applicable
        if version in self.__kjv_like and "KJV-like" not in self.__preloaded:
            self._load_version("KJV-like", preload=True)
            self.__preloaded.add("KJV-like")

        # Load NIV common index if applicable
        elif version in self.__niv and "NIV" not in self.__preloaded:
            self._load_version("NIV", preload=True)
            self.__preloaded.add("NIV")

        # Load Literal common index if applicable
        elif version in self.__literal and "Literal" not in self.__preloaded:
            self._load_version("Literal", preload=True)
            self.__preloaded.add("Literal")

        # Load Literal2 common index if applicable
        elif version in self.__literal2 and "Literal2" not in self.__preloaded:
            self._load_version("Literal2", preload=True)
            self.__preloaded.add("Literal2")

        # Load Dynamic common index if applicable
        elif version in self.__dynamic and "Dynamic" not in self.__preloaded:
            self._load_version("Dynamic", preload=True)
            self.__preloaded.add("Dynamic")

        # Load the common index of some common Spanish versions
        elif version in self.__esrv and "EsRV" not in self.__preloaded:
            self._load_version("EsRV", preload=True)
            self.__preloaded.add("EsRV")

        # Load the index of some assorted English versions
        elif version in self.__extra_english and "ExtraEng" not in self.__preloaded:
            self._load_version("ExtraEng", preload=True)
            self.__preloaded.add("ExtraEng")

        # Load the version
        self._load_version(version)

    def load_all(self) -> None:
        """
        Preload all version indices.
        """
        for version in self.__versions:
            self.load(version)

    def unload_version(self, version: str) -> None:
        """
        Unload a version's index from memory.
        :param version: The version to remove.
        :return: None
        :raises InvalidVersion: If the version is invalid or not loaded, raises an exception.
        """
        if version in self.__versions and version in self.__loaded:
            self.__c_search.unload(version)
            self.__loaded.remove(version)
        else:
            raise InvalidVersion(version)

    def search(
            self,
            query: str,
            version: str = "KJV",
            max_results: int = sys.maxsize
    ) -> List[str]:
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
        A list of the versions currently loaded in the search object.
        This does not include common indices.
        """
        return list(self.__loaded)

    @property
    def versions(self) -> List[str]:
        """
        A list of versions available in the search index.
        """
        return list(self.__versions)

    @property
    def english_versions(self) -> List[str]:
        """
        A list of English versions available in the search index.
        """
        return list(self.__english_versions)

    @property
    def spanish_versions(self) -> List[str]:
        """
        A list of Spanish versions available in the search index.
        """
        return list(self.__spanish_versions)
