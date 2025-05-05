"""
Test some of the performance aspects of the search module.
"""

from gc import get_referents
import os
import sys
import time
import unittest

from src.multi_bible_search.bible_search_adapter import BibleSearch


def getsize(obj):
    """
    Sum the size of an object and its members.
    """
    seen_ids = set()
    size = 0
    objects = [obj]
    while objects:
        need_referents = []
        for this_obj in objects:
            if id(this_obj) not in seen_ids:
                seen_ids.add(id(this_obj))
                size += sys.getsizeof(this_obj)
                need_referents.append(this_obj)
        objects = get_referents(*need_referents)
    return size


def get_directory_size(directory):
    """
    Get the size of all files in a directory.
    :param directory: The directory to search.
    :return: The size of the directory's files in bytes.
    """
    total_size = 0
    for dirpath, _, filenames in os.walk(directory):
        for filename in filenames:
            file_path = os.path.join(dirpath, filename)
            if os.path.isfile(file_path):
                total_size += os.path.getsize(file_path)
    return total_size


class TestPerf(unittest.TestCase):
    """
    Test searching performance.
    """
    def setUp(self) -> None:
        """
        Create the search object.
        """
        self.bible_search = BibleSearch()

    def test_perf(self):
        """
        Test the average case performance.
        """
        count = 200_000
        self.bible_search.load('KJV')
        # Warmup
        for _ in range(200):
            self.bible_search.search("Jesus wept")
        start = time.perf_counter()
        for _ in range(count):
            # Do some searching
            self.bible_search.search("Jesus wept")
        end = time.perf_counter()
        avg_time = (end - start) / count
        print("Average case, no limit:")
        print(f"Total: {end - start:.4f}s\n"
              f"{avg_time:.8f}s per search average ({avg_time * 10 ** 6:.4f}μs)")
        print("-" * 20)
        start = time.perf_counter()
        for _ in range(count):
            # Do some searching
            self.bible_search.search("comest goest")
        end = time.perf_counter()
        avg_time = (end - start) / count
        print("Uncommon token pair:")
        print(f"Total: {end - start:.4f}s\n"
              f"{avg_time:.8f}s per search average ({avg_time * 10 ** 6:.4f}μs)")
        print("-" * 20)

    def test_perf_max_results(self):
        """
        Test the average case performance, but with limited results given to Python.
        """
        count = 200_000
        self.bible_search.load('KJV')
        start = time.perf_counter()
        for _ in range(count):
            # Do some searching
            self.bible_search.search("Jesus wept", max_results=100)
        end = time.perf_counter()
        avg_time = (end - start) / count
        print("Common case, limit of 100 results")
        print(f"Total: {end - start:.4f}s\n"
              f"{avg_time:.8f}s per search average ({avg_time * 10 ** 6:.4f}μs)")
        print("-" * 20)
        start = time.perf_counter()
        for _ in range(count):
            # Do some searching
            self.bible_search.search("comest goest", max_results=100)
        end = time.perf_counter()
        avg_time = (end - start) / count
        print("Uncommon token pair, limit of 100 results:")
        print(f"Total: {end - start:.4f}s\n"
              f"{avg_time:.8f}s per search average ({avg_time * 10 ** 6:.4f}μs)")
        print("-" * 20)

    def test_perf_long_query(self):
        """
        Test a couple of worst-case queries.
        """
        count = 50
        self.bible_search.load('KJV')
        start = time.perf_counter()
        for _ in range(count):
            # Do some searching
            self.bible_search.search(
                "Then were the king's scribes called at that time in the third month, that is, "
                "the month Sivan, on the three and twentieth day thereof; and it was written "
                "according to all that Mordecai commanded unto the Jews, and to the lieutenants, "
                "and the deputies and rulers of the provinces which are from India unto "
                "Ethiopia, an hundred twenty and seven provinces, unto every province according "
                "to the writing thereof, and unto every people after their language, and to the "
                "Jews according to their writing, and according to their language. Then were the "
                "king's scribes called at that time in the third month, that is, "
                "the month Sivan, on the three and twentieth day thereof; and it was written "
                "according to all that Mordecai commanded unto the Jews, and to the lieutenants, "
                "and the deputies and rulers of the provinces which are from India unto "
                "Ethiopia, an hundred twenty and seven provinces, unto every province according "
                "to the writing thereof, and unto every people after their language, and to the "
                "Jews according to their writing, and according to their language.",
                max_results=100)
        end = time.perf_counter()
        avg_time = (end - start) / count
        print(f"Excessively long query total: {end - start:.4f}s\n"
              f"{avg_time:.8f}s per search average")
        print("-" * 20)

        start = time.perf_counter()
        for _ in range(count):
            # Do some searching
            self.bible_search.search(
                "Then were the king's scribes called at that time in the third month, that is, "
                "the month Sivan, on the three and twentieth day thereof; and it was written "
                "according to all that Mordecai commanded unto the Jews, and to the lieutenants, "
                "and the deputies and rulers of the provinces which are from India unto "
                "Ethiopia, an hundred twenty and seven provinces, unto every province according "
                "to the writing thereof, and unto every people after their language, and to the "
                "Jews according to their writing, and according to their language.",
                max_results=100)
        end = time.perf_counter()
        avg_time = (end - start) / count
        print(f"Longest reasonable query total: {end - start:.4f}s\n"
              f"{avg_time:.8f}s per search average")
        print("-" * 20)

    def test_profile(self):
        """
        Test the memory usage of the search object with a single version and all versions loaded.
        """
        self.bible_search.load('KJV')
        new_kjv_size = \
            (getsize(self.bible_search) + self.bible_search.internal_index_size()) / (1024 ** 2)
        self.bible_search.load_all()
        new_all_size = \
            (self.bible_search.internal_index_size() + getsize(self.bible_search)) / (1024 ** 2)
        dir_size = get_directory_size("../src/multi_bible_search/data") / (1024 ** 2)
        print(f"KJV Only: {new_kjv_size:7.4f} MiB")
        print(f"All:      {new_all_size:7.4f} MiB")
        print(f"Disk:     {dir_size:7.4f} MiB")
        print(f"KJV reduction: {1106.2344970703125 - new_kjv_size:.4f} MiB")
        print(f"All reduction: {1106.2344970703125 - new_all_size:.4f} MiB")

    def test_average_keys(self):
        """
        Test the average retrieval speed of single keys.
        """
        count = 200
        with open("kjv_keys.txt", "r", encoding="utf-8") as key_file:
            keys = key_file.read().splitlines()
        self.bible_search.load('KJV')
        # warmup
        for _ in range(200):
            # Do some searching
            self.bible_search.search("Jesus wept")

        time_accumulator: float = 0.0

        for _ in range(count):
            for key in keys:
                start = time.perf_counter()
                result = self.bible_search.search(key)
                end = time.perf_counter()
                # Do something useful with the exhaustive search
                self.assertGreater(
                    len(result),
                    0,
                    msg=f"Couldn't find key {key} for some reason. "
                        f"Probably an issue in the C code."
                )
                time_accumulator += end - start
        avg_time = time_accumulator / count / len(keys)
        print("Average retrieval time for 1 key:")
        print(f"Total: {time_accumulator:.4f}s\n"
              f"{avg_time:.8f}s per search average ({avg_time * 10 ** 6:.4f}μs)")
        print("-" * 20)


if __name__ == '__main__':
    unittest.main()
