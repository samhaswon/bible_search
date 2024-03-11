from .translate import rtranslate
import bz2
from collections import Counter
import json
import os
from typing import List


class BibleSearch(object):
    def __init__(self, debug=False):
        base_path = os.path.dirname(os.path.abspath(__file__))

        with bz2.open(f"{base_path}/bible_index.json.pbz2", "rt", encoding='utf-8')as data_file:
            self.__search_index = json.load(data_file)
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
    def _long_reference(short_ref: str) -> str:
        """
        Translate a shortened reference into a longer one by converting the book number into its name.
        :param short_ref: Shortened reference string.
        :return: Long form of the reference
        """
        book = short_ref[0:short_ref.find(" ")]
        return f"{rtranslate(int(book))}{short_ref[short_ref.find(' '):]}"

    @staticmethod
    def _ranked(refs: List[tuple], num_tokens: int) -> List[tuple]:
        most_likely = []
        others = []
        for ref in refs:
            if ref[1] == num_tokens:
                most_likely.append(ref)
            else:
                others.append(ref)
        most_likely.extend(others)
        return most_likely

    def search(self, query: str, version="KJV"):
        """
        Search for a passage in the Bible.
        :param query: Search query
        :param version: version to src
        :return: List of match references
        """
        query_tokens = self._tokenize(query)

        # Find most likely matches
        all_refs = []

        for token in query_tokens:
            all_refs.extend(self.__search_index[version].get(token, []))
            all_refs.extend(self.__search_index["All"].get(token, []))

        ref_counter = Counter(all_refs)
        ref_counter = sorted(ref_counter.items(), key=lambda x: x[1], reverse=True)
        ranked = self._ranked(ref_counter, len(query_tokens))
        return [self._long_reference(ref[0]) for ref in ranked]

    @property
    def versions(self) -> List[str]:
        return list(self.__search_index.keys())
