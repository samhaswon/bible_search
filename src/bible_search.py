from src.translate import rtranslate
import bz2
from collections import Counter
import json
from typing import List


class BibleSearch(object):
    def __init__(self):
        try:
            with bz2.open("src/bible_index.json.pbz2", "rt", encoding='utf-8')as data_file:
                self.__search_index = json.load(data_file)
        except FileNotFoundError:
            with bz2.open("../src/bible_index.json.pbz2", "rt", encoding='utf-8') as data_file:
                self.__search_index = json.load(data_file)
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

        ref_counter = Counter(all_refs)
        ref_counter = sorted(ref_counter.items(), key=lambda x: x[1], reverse=True)
        return [self._long_reference(ref[0]) for ref in ref_counter]

    @property
    def versions(self) -> List[str]:
        return list(self.__search_index.keys())
