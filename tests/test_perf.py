from src.multi_bible_search.bible_search_adapter import BibleSearch
import time
import unittest
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


class TestPerf(unittest.TestCase):
    def setUp(self) -> None:
        self.bible_search = BibleSearch()

    def test_perf(self):
        count = 200_000
        self.bible_search.load('KJV')
        start = time.perf_counter()
        for i in range(count):
            # Do some searching
            self.bible_search.search("Jesus wept")
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

    def test_profile(self):
        self.bible_search.load('KJV')
        new_kjv_size = (getsize(self.bible_search) + self.bible_search.internal_index_size()) / (1024 ** 2)
        self.bible_search.load_all()
        new_all_size = (self.bible_search.internal_index_size() + getsize(self.bible_search)) / (1024 ** 2)
        print(f"KJV Only: {new_kjv_size:.4f} MiB")
        print(f"All: {new_all_size:.4f} MiB")
        print(f"KJV reduction: {1106.2344970703125 - new_kjv_size:.4f} MiB")
        print(f"All reduction: {1106.2344970703125 - new_all_size:.4f} MiB")


if __name__ == '__main__':
    unittest.main()
