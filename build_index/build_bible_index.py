"""
Build the index of the supported Bible versions.
This takes a while.
"""
# pylint: disable=import-error,wildcard-import
import copy
import bz2
import multiprocessing
import re
import time
from typing import List

import numpy

from bibles import *
from src.multi_bible_search.translate import translate


def encode_json(in_dict: dict) -> str:
    """
    Encodes the given dictionary as the project's custom JSON format string.
    :param in_dict: The dictionary to encode.
    :return: The string form of the dictionary.
    """
    keys = list(in_dict.keys())
    keys.sort()

    result_string = "{"

    for this_key in keys:
        result_string += f"\"{this_key}\":["
        for i in range(len(in_dict[this_key])):
            result_string += \
                (f"{numpy.base_repr(in_dict[this_key][i], 36)}"
                 f"{',' if i < len(in_dict[this_key]) - 1 else ''}")
        if this_key != keys[-1]:
            result_string += "],"
        else:
            result_string += "]"

    result_string += "}"
    return result_string


def remove_punctuation(input_string: str) -> str:
    """
    Removes characters that are not alphabetic or a space
    :param input_string: String to process
    :return: cleaned string
    """
    return ''.join(
        x for x in re.sub(r"\s\s+", " ",
                          re.sub(
                              r"[\u0080-\uffef](s?(?=\W))|['\"]s?|[,.:;\-¿?¡!°]+|\u2014|\\ul\d",
                              " ", input_string.replace("\u014D", "o")))
        if (x.isalpha() or x.isspace()))


def tokenize(input_string: str) -> List[str]:
    """
    Tokenizes a string based on its spaces.
    :param input_string: The string to tokenize.
    :return: A list of lowercase tokens (words) in the string.
    """
    return remove_punctuation(input_string.replace("I-chabod", "Ichabod").lower()).split()


# pylint: disable=too-many-branches,too-many-nested-blocks
def index_bible(bible, name: str, result) -> None:
    """
    Builds the (reverse) index of a given Bible version.
    :param bible: Bible object used for indexing.
    :param name: The Short name of the version being indexed.
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
                previous_verse = 1
                for this_passage in passage_result[heading]:
                    try:
                        verse = int(this_passage[0:this_passage.find(" ")])
                    except ValueError:
                        verse = previous_verse
                    else:
                        previous_verse = verse
                    tokens = list(set(tokenize(this_passage[this_passage.find(" "):])))
                    reference = translate(book, chapter, verse)
                    for token in tokens:
                        if len(token) == 1 and token != "a":
                            continue
                        # XML source issue
                        if token in {
                            "lxx", "syr", "vg", "heb", "etc", "isha", "ish", "aleph", "kol"
                        }:
                            continue
                        # ????
                        if token == 'ij':
                            token = "i"
                        # Typo in the source?
                        elif token == "ad":
                            token = "and"
                        # Make this the same
                        elif token == 'aramnaharaim':
                            if "aram" not in tmp_index:
                                tmp_index["aram"] = []
                            if "naharaim" not in tmp_index:
                                tmp_index["naharaim"] = []
                            tmp_index["aram"].append(reference)
                            tmp_index["naharaim"].append(reference)
                            continue
                        try:
                            token.encode("ascii")
                        except UnicodeError:
                            continue
                        if token not in tmp_index:
                            tmp_index[token] = []
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
    index_copy = copy.deepcopy(index)
    all_versions = {}
    for token in index[versions[0]]:
        for match in index[versions[0]][token]:
            exists_in_all = True
            for j in range(1, len(versions)):
                if (token not in index_copy[versions[j]] or
                        match not in index_copy[versions[j]][token]):
                    exists_in_all = False
                    break
            if exists_in_all:
                if token not in all_versions:
                    all_versions[token] = [match]
                else:
                    all_versions[token].append(match)
                for i, _ in enumerate(versions):
                    index_copy[versions[i]][token].remove(match)
                    if len(index_copy[versions[i]][token]) == 0:
                        del index_copy[versions[i]][token]
    index_copy[combine_to] = all_versions
    return index_copy


# pylint: disable=too-many-locals,consider-using-with
def make_index(bibles_in: dict) -> dict:
    """
    Build the (reverse) src index of the given Bibles.
    :param bibles_in: A dictionary of Bible objects where the name of the version is the key.
    :return: Dictionary of src index for each version.
    """

    # Initialization
    num_processes = multiprocessing.cpu_count() - 1
    manager = multiprocessing.Manager()
    built_index = manager.dict({})
    versions = list(bibles_in.keys())
    pool = multiprocessing.Pool(processes=num_processes)

    # Spread out work to the pool
    for version in versions:
        pool.apply_async(index_bible, args=(bibles_in[version], version, built_index,))

    # Start, do the work, and wait for results
    pool.close()
    pool.join()

    # Dereference the dictionary to a normal one per the documentation
    # pylint: disable=no-member,protected-access
    index = built_index._getvalue()

    # Separate some duplicates
    print("Built primary index. Removing some duplicates across all versions...")
    english_versions = ['ACV', 'AKJV', 'AMP', 'ASV', 'BBE', 'BSB', 'CSB', 'Darby', 'DRA', 'EBR',
                        'ESV', 'GNV', 'KJV', 'KJV 1611', 'LSV', 'MSG', 'NASB 1995', 'NET',
                        'NIV 1984', 'NIV 2011', 'NKJV', 'NLT', 'RNKJV', 'RSV', 'RWV', 'UKJV',
                        'WEB', 'YLT']
    index = separate_duplicates(index, english_versions, "AllEng")

    # Separate duplicates in KJV-Like Bibles
    print("Built secondary index. Removing some duplicates across KJV-like versions...")
    kjv_like = ["AKJV", "GNV", "KJV", "KJV 1611", "RNKJV", "UKJV"]
    index = separate_duplicates(index, kjv_like, "KJV-like")

    print("Built tertiary index. Removing some duplicates from the two NIV versions...")
    both_niv = ["NIV 1984", "NIV 2011"]
    index = separate_duplicates(index, both_niv, "NIV")

    print("Built quaternary index. Removing some duplicates in Literal translations...")
    literal = ["ACV", "AMP", "ASV", "ESV", "NASB 1995", "NKJV", "RSV", "RWV", "WEB"]
    index = separate_duplicates(index, literal, "Literal")

    print("Built quinary index. Removing some duplicates in Dynamic translations...")
    dynamic = ["CSB", "NLT", "NET"]
    index = separate_duplicates(index, dynamic, "Dynamic")

    print("Build senary index. Removing some duplicates in more Literal translations...")
    literal2 = ["BSB", "LSV", "YLT"]
    index = separate_duplicates(index, literal2, "Literal2")

    print("Built septenary index. Removing duplicates in all Spanish indexes...")
    spanish_versions = ["BTX3", "RV1960", "RV2004"]
    index = separate_duplicates(index, spanish_versions, "AllEs")

    print("Built octonary index. Removing duplicates in Reina Valera translations...")
    rv_versions = ["RV1960", "RV2004"]
    index = separate_duplicates(index, rv_versions, "EsRV")

    print("Built nonary index. Removing duplicates from some assorted English versions...")
    extra_eng = ["Darby", "EBR"]
    index = separate_duplicates(index, extra_eng, "ExtraEng")

    return index


def save(data: dict, index_name: str) -> None:
    """
    Saves a given index with name.
    :param data: The index to save.
    :param index_name: The name to save the index with.
    :return: None.
    """
    # Normal save
    try:
        with bz2.open(f"../src/multi_bible_search/data/{index_name}.json.pbz2", "wb") as data_file:
            data_file.write(encode_json(data).encode('utf-8'))
    # Testing save
    except FileNotFoundError:
        with bz2.open(f"./data/{index_name}.json.pbz2", "wb") as data_file:
            data_file.write(encode_json(data).encode('utf-8'))


if __name__ == '__main__':
    # pylint: disable=undefined-variable
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
        'YLT': YLT(),
        'BTX3': BTX3(),
        'RV1960': RV1960(),
        'RV2004': RV2004(),
    }
    # Timer start, because I like stats
    start = time.perf_counter()

    reference_index = make_index(bibles)

    key_list = []
    # Save each version's index as a separate file to be able to load them independently.
    for key in reference_index.keys():
        save(reference_index[key], key)
        key_list.extend(reference_index[key].keys())

    key_list = list(set(key_list))
    key_list.sort()
    with open("../keys.txt", "w", encoding="utf-8") as key_file:
        key_file.write(''.join(f"{k}\n" for k in key_list))
    key_lengths = [len(x) for x in key_list]
    print(f"Longest key: {max(key_lengths)}")
    # How long did this take? Because this takes a while to run.
    end = time.perf_counter()
    print(f"Index time: {end - start:.4f}")
