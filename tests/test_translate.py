import time
import unittest
from src.multi_bible_search.translate import translate, rtranslate
from bibles.kjv import KJV
from ctranslate.ctranslate import rtranslate as crtranslate


class TestTranslate(unittest.TestCase):
    def test_translate(self):
        bible = KJV()
        for book in bible.books_of_the_bible.keys():
            for chapter in range(1, bible.books_of_the_bible[book] + 1):
                # Get the chapter
                verse_count = len(bible.get_passage(book, chapter)['verses'])
                # Go through the verses
                for verse in range(1, verse_count + 1):
                    self.assertEqual(f"{book} {chapter}:{verse}", rtranslate(translate(book, chapter, verse)))
                    self.assertEqual(f"{book} {chapter}:{verse}", crtranslate(translate(book, chapter, verse)))

    def test_perf(self):
        count = 2_000_000
        reference: int = translate("John", 11, 35)

        py_start = time.perf_counter()
        for _ in range(count):
            rtranslate(reference)
        py_end = time.perf_counter()

        c_start = time.perf_counter()
        for _ in range(count):
            crtranslate(reference)
        c_end = time.perf_counter()

        py_time = py_end - py_start
        c_time = c_end - c_start

        print(f"Python time: {py_time:.17f}s ({py_time / count * 10 ** 9:.4f}ns average)\n"
              f"C time:      {c_time:.17f}s ({c_time / count * 10 ** 9:.4f}ns average)\n"
              f"{(c_time - py_time) / py_time * 100}%")


if __name__ == '__main__':
    unittest.main()
