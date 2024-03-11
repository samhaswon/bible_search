from bibles import *
from src.multi_bible_search.translate import translate
import bz2
import json
import multiprocessing
import time
from typing import List


def remove_punctuation(input_string: str) -> str:
    """
    Removes characters that are not alphabetic or a space
    :param input_string: String to process
    :return: cleaned string
    """
    return ''.join(x for x in input_string if x.isalpha() or x.isspace())


def tokenize(input_string: str) -> List[str]:
    """
    Tokenizes a string based on spaces
    :param input_string: string to tokenize
    :return: list of lowercase tokens (words) in the string
    """
    return remove_punctuation(input_string.lower()).split()


def index_bible(bible, name: str, result) -> None:
    """
    Builds the (reverse) index of a given version of the Bible.
    :param bible: Bible object used for indexing.
    :param name: Short name of the version being indexed.
    :param result: Resulting dictionary to add the version's index to.
    :return: None
    """
    tmp_index = {}

    # Iterate through the Bible
    for book in bible.books_of_the_bible.keys():
        for chapter in range(1, bible.books_of_the_bible[book] + 1):
            # Get the chapter
            passage_result = bible.get_passage(book, chapter)['verses']
            # Go through the verses
            for heading in passage_result.keys():
                # Tokenize each verse, adding its reference
                for passage in passage_result[heading]:
                    verse = passage[0:passage.find(" ")]
                    tokens = list(set(tokenize(passage[passage.find(""):])))
                    reference = f"{translate(book)} {chapter}:{verse}"
                    for token in tokens:
                        if token not in tmp_index:
                            tmp_index[token] = []
                        # Don't duplicate references
                        # if reference in tmp_index[token]:
                        #    continue
                        tmp_index[token].append(reference)
    result.update({name: tmp_index})


def separate_duplicates(index: dict, versions: list, combine_to: str) -> dict:
    """
    Separates duplicate references as "<combine_to>".
    :param combine_to: Name to combine references under.
    :param index: Full reverse index of passages.
    :param versions: Version list to work with.
    :return: Reverse index with duplicate references under "<combine_to>"
    """
    index_copy = index.copy()
    all_versions = {}
    for token in index[versions[0]]:
        for match in index[versions[0]][token]:
            exists_in_all = True
            for j in range(1, len(versions)):
                if token not in index[versions[j]] or match not in index[versions[j]][token]:
                    exists_in_all = False
            if exists_in_all:
                if token not in all_versions:
                    all_versions[token] = [match]
                else:
                    all_versions[token].append(match)
                for i in range(len(versions)):
                    index_copy[versions[i]][token].remove(match)
    index_copy[combine_to] = all_versions
    return index_copy


def make_index(bibles: dict) -> dict:
    """
    Build the (reverse) src index of the given Bibles.
    :param bibles: dictionary of Bible objects where the name of the version is the key.
    :return: Dictionary of src index for each version
    """

    # Initialization
    num_processes = multiprocessing.cpu_count() - 1
    manager = multiprocessing.Manager()
    built_index = manager.dict({})
    versions = list(bibles.keys())
    pool = multiprocessing.Pool(processes=num_processes)

    # Spread out work to the pool
    for version in versions:
        pool.apply_async(index_bible, args=(bibles[version], version, built_index,))

    # Start, do the work, and wait for results
    pool.close()
    pool.join()

    # Dereference the dictionary to a normal one per the documentation
    index = built_index._getvalue()

    # Separate some duplicates
    print("Built main index. Removing some duplicates across all versions...")
    index = separate_duplicates(index, versions, "All")

    # Separate duplicates in KJV-Like Bibles
    print("Built secondary index. Removing some duplicates across KJV-like versions...")
    kjv_like = ["AKJV", "GNV", "KJV", "KJV 1611", "RNKJV", "UKJV"]
    index = separate_duplicates(index, kjv_like, "KJV-like")
    return index


if __name__ == '__main__':
    # Bible object initialization
    bibles = {
        'ACV': ACV(),
        'AKJV': AKJV(),
        'AMP': AMP(),
        'ASV': ASV(),
        'BBE': BBE(),
        'BSB': BSB(),
        'CSB': CSB(),
        'Darby': Darby(),
        'DRA': DRA(),
        'EBR': EBR(),
        'ESV': ESV(),
        'GNV': GNV(),
        'KJV': KJV(),
        'KJV 1611': KJV1611(),
        'LSV': LSV(),
        'MSG': MSG(),
        'NASB 1995': NASB1995(),
        'NET': NET(),
        'NIV 1984': NIV1984(),
        'NIV 2011': NIV2011(),
        'NKJV': NKJV(),
        'NLT': NLT(),
        'RNKJV': RNKJV(),
        'RSV': RSV(),
        'RWV': RWV(),
        'UKJV': UKJV(),
        'WEB': WEB(),
        'YLT': YLT()
    }
    # Timer start, because I like stats
    start = time.perf_counter()

    reference_index = make_index(bibles)

    # Normal save
    try:
        with bz2.open("../src/multi_bible_search/bible_index.json.pbz2", "wb") as data_file:
            data_file.write(json.dumps(reference_index, separators=(',', ':')).encode('utf-8'))
    # Testing save
    except FileNotFoundError:
        with bz2.open("./bible_index.json.pbz2", "wb") as data_file:
            data_file.write(json.dumps(reference_index, separators=(',', ':')).encode('utf-8'))

    # How long did this take? Because this takes a while to run.
    end = time.perf_counter()
    print(f"Index time: {end - start:.4f}")
