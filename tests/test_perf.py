from src.multi_bible_search.bible_search_adapter import BibleSearch
import time
import unittest
import os
import sys
from gc import get_referents


def getsize(obj):
    """sum size of an object & members."""
    seen_ids = set()
    size = 0
    objects = [obj]
    while objects:
        need_referents = []
        for obj in objects:
            if id(obj) not in seen_ids:
                seen_ids.add(id(obj))
                size += sys.getsizeof(obj)
                need_referents.append(obj)
        objects = get_referents(*need_referents)
    return size


def get_directory_size(directory):
    total_size = 0
    for dirpath, dirnames, filenames in os.walk(directory):
        for filename in filenames:
            file_path = os.path.join(dirpath, filename)
            if os.path.isfile(file_path):
                total_size += os.path.getsize(file_path)
    return total_size


class TestPerf(unittest.TestCase):
    def setUp(self) -> None:
        self.bible_search = BibleSearch()

    def test_perf(self):
        count = 200_000
        self.bible_search.load('KJV')
        # Warmup
        for _ in range(200):
            self.bible_search.search("Jesus wept")
        start = time.perf_counter()
        for i in range(count):
            # Do some searching
            self.bible_search.search("Jesus wept")
        end = time.perf_counter()
        avg_time = (end - start) / count
        print(f"Total: {end - start:.4f}s\n"
              f"{avg_time:.8f}s per search average ({avg_time * 10 ** 6:.4f}μs)")
        start = time.perf_counter()
        for i in range(count):
            # Do some searching
            self.bible_search.search("comest goest")
        end = time.perf_counter()
        avg_time = (end - start) / count
        print(f"Total: {end - start:.4f}s\n"
              f"{avg_time:.8f}s per search average ({avg_time * 10 ** 6:.4f}μs)")

    def test_perf_max_results(self):
        count = 200_000
        self.bible_search.load('KJV')
        start = time.perf_counter()
        for i in range(count):
            # Do some searching
            self.bible_search.search("Jesus wept", max_results=100)
        end = time.perf_counter()
        avg_time = (end - start) / count
        print(f"Total: {end - start:.4f}s\n"
              f"{avg_time:.8f}s per search average ({avg_time * 10 ** 6:.4f}μs)")
        start = time.perf_counter()
        for i in range(count):
            # Do some searching
            self.bible_search.search("comest goest", max_results=100)
        end = time.perf_counter()
        avg_time = (end - start) / count
        print(f"Total: {end - start:.4f}s\n"
              f"{avg_time:.8f}s per search average ({avg_time * 10 ** 6:.4f}μs)")

    def test_perf_long_query(self):
        count = 50
        self.bible_search.load('KJV')
        start = time.perf_counter()
        for i in range(count):
            # Do some searching
            self.bible_search.search("Then were the king's scribes called at that time in the third month, that is, "
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

        start = time.perf_counter()
        for i in range(count):
            # Do some searching
            self.bible_search.search("Then were the king's scribes called at that time in the third month, that is, "
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

    def test_profile(self):
        self.bible_search.load('KJV')
        new_kjv_size = (getsize(self.bible_search) + self.bible_search.internal_index_size()) / (1024 ** 2)
        self.bible_search.load_all()
        new_all_size = (self.bible_search.internal_index_size() + getsize(self.bible_search)) / (1024 ** 2)
        dir_size = get_directory_size("../src/multi_bible_search/data") / (1024 ** 2)
        print(f"KJV Only: {new_kjv_size:7.4f} MiB")
        print(f"All:      {new_all_size:7.4f} MiB")
        print(f"Disk:     {dir_size:7.4f} MiB")
        print(f"KJV reduction: {1106.2344970703125 - new_kjv_size:.4f} MiB")
        print(f"All reduction: {1106.2344970703125 - new_all_size:.4f} MiB")

    def test_average_keys(self):
        count = 200
        with open("kjv_keys.txt", "r") as key_file:
            keys = key_file.read().splitlines()
        self.bible_search.load('KJV')
        # warmup
        for i in range(200):
            # Do some searching
            self.bible_search.search("Jesus wept")

        time_accumulator: float = 0.0

        for _ in range(count):
            for key in keys:
                start = time.perf_counter()
                result = self.bible_search.search(key)
                end = time.perf_counter()
                # Do something useful with the exhaustive search
                self.assertGreater(len(result), 0,
                                   msg=f"Couldn't find key {key} for some reason. Probably an issue in the C code.")
                time_accumulator += end - start
        avg_time = time_accumulator / count / len(keys)
        print(f"Total: {time_accumulator:.4f}s\n"
              f"{avg_time:.8f}s per search average ({avg_time * 10 ** 6:.4f}μs)")


if __name__ == '__main__':
    unittest.main()
