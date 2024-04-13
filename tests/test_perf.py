from src.multi_bible_search.bible_search import BibleSearch
import time
import unittest
import sys
from gc import get_referents


def getsize(obj):
    """sum size of object & members."""
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
        count = 20000
        start = time.perf_counter()
        for i in range(count):
            # Do some searching
            self.bible_search.search("Jesus wept")
        end = time.perf_counter()
        print(f"Total: {end - start:.4f}s\n"
              f"{(end - start) / count:.8f}s per search average")

    def test_profile(self):
        new_size = getsize(self.bible_search) / (1024 ** 2)
        print(f"{new_size:.4f} MiB")
        print(f"Reduction: {1106.2344970703125 - new_size:.4f} MiB")


if __name__ == '__main__':
    unittest.main()
