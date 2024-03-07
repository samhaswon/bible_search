from src.multi_bible_search.bible_search import BibleSearch
import time
import unittest


class TestPerf(unittest.TestCase):
    def setUp(self) -> None:
        self.bible_search = BibleSearch()

    def test_perf(self):
        count = 2000
        start = time.perf_counter()
        for i in range(count):
            # Do some searching
            self.bible_search.search("Jesus wept")
        end = time.perf_counter()
        print(f"Total: {end - start:.4f}s\n"
              f"{(end - start) / count:.8f}s per search average")


if __name__ == '__main__':
    unittest.main()
