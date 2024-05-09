import bz2
from collections import Counter
import json
import os
from typing import List, Union
# Try to use the C version of rtranslate
try:
    from ctranslate.ctranslate import rtranslate
# Otherwise, use the pure Python implementation (~45% slower)
except ImportError:
    from .translate import rtranslate


class BibleSearch(object):
    def __init__(self, preload: Union[List[str], None] = None, debug=False):
        """
        :param preload: List of versions to preload.
        :param debug: Flag for indicating when the specified and common indices are loaded.
        """
        # Some attributes
        self.__search_index: dict = {}
        self.__both_niv: set = {"NIV 1984", "NIV 2011"}
        self.__kjv_like: set = {"AKJV", "KJV", "KJV 1611", "RNKJV", "UKJV"}
        self.__versions: set = {'ACV', 'AKJV', 'AMP', 'ASV', 'BBE', 'BSB', 'CSB', 'Darby', 'DRA', 'EBR', 'ESV', 'GNV',
                                'KJV', 'KJV 1611', 'LSV', 'MSG', 'NASB 1995', 'NET', 'NIV 1984', 'NIV 2011', 'NKJV',
                                'NLT', 'RNKJV', 'RSV', 'RWV', 'UKJV', 'WEB', 'YLT'}

        # Preload common indices
        self._load_version("All", preload=True)
        self._load_version("KJV-like", preload=True)

        self.__loaded: set = set()

        if preload:
            for version in preload:
                self.load(version)

        if debug:
            print("Search index loaded")

    @staticmethod
    def _tokenize(input_string: str) -> List[str]:
        """
        Tokenizes a string based on spaces.
        :param input_string: string to tokenize.
        :return: list of lowercase tokens (words) in the string.
        """
        cleaned = ''.join(x for x in input_string.lower() if x.isalpha() or x.isspace())
        return cleaned.split()

    @staticmethod
    def _ranked(refs: List[tuple], num_tokens: int) -> List[tuple]:
        """
        Ranks search results by the count of tokens.
        :param refs: List of tuple references to rank.
        :param num_tokens: The number of tokens in the query. Hoists results with this number of tokens to the top of
        results.
        :return: Ranked search results.
        """
        most_likely = []
        others = []
        for ref in refs:
            if ref[1] == num_tokens:
                most_likely.append(ref)
            else:
                others.append(ref)
        most_likely.extend(others)
        return most_likely

    def _load_version(self, version: str, preload: bool = False) -> None:
        """
        Preloads a given version's search index.
        It assumes that the version is valid.
        :param version: The version to preload.
        :return: None
        """
        if version in self.__both_niv:
            self._load_version("NIV", preload=True)
        base_path = os.path.dirname(os.path.abspath(__file__))
        with bz2.open(f"{base_path}/data/{version}.json.pbz2", "rt", encoding='utf-8') as data_file:
            self.__search_index[version] = json.load(data_file)
        if not preload:
            self.__loaded.add(version)

    def load(self, version: str) -> None:
        """
        Preloads a given version's search index.
        :param version: The version to preload.
        :return: None
        """
        # Quick check that the version is valid
        assert version in self.__versions
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
        :raises Exception: If the version is invalid, raises an exception.
        """
        try:
            del self.__search_index[version]
        except KeyError:
            raise Exception(f"Invalid version {version}")

    def search(self, query: str, version="KJV"):
        """
        Search for a passage in the Bible.
        :param query: Search query
        :param version: version to src
        :return: List of match references
        """
        # Load the version if it is not already loaded
        if version not in self.__loaded:
            self.load(version)
        query_tokens = self._tokenize(query)

        # Find most likely matches
        all_refs = []

        if version in self.__kjv_like:
            for token in query_tokens:
                all_refs.extend(self.__search_index[version].get(token, []))
                all_refs.extend(self.__search_index["All"].get(token, []))
                all_refs.extend(self.__search_index["KJV-like"].get(token, []))
        elif version in self.__both_niv:
            for token in query_tokens:
                all_refs.extend(self.__search_index[version].get(token, []))
                all_refs.extend(self.__search_index["NIV"].get(token, []))
                all_refs.extend(self.__search_index["All"].get(token, []))
        else:
            for token in query_tokens:
                all_refs.extend(self.__search_index[version].get(token, []))
                all_refs.extend(self.__search_index["All"].get(token, []))

        ref_counter = Counter(all_refs)
        ref_counter = sorted(ref_counter.items(), key=lambda x: x[1], reverse=True)
        ranked = self._ranked(ref_counter, len(query_tokens))
        return [rtranslate(ref[0]) for ref in ranked]

    @property
    def loaded(self):
        """
        A list of the versions currently loaded in the search objects. This does not include common indices.
        """
        return list(self.__loaded)

    @property
    def versions(self) -> List[str]:
        """
        A list of versions available in the search index.
        """
        return list(self.__versions)
