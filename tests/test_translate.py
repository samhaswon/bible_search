import unittest
from src.multi_bible_search.translate import translate, rtranslate
from bibles.kjv import KJV


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


if __name__ == '__main__':
    unittest.main()
