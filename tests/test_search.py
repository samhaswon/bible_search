import unittest
from src.multi_bible_search.bible_search import BibleSearch


class TestSearch(unittest.TestCase):
    def setUp(self) -> None:
        self.bible_search = BibleSearch()

    def test_query(self):
        query1 = self.bible_search.search("Jesus wept")
        self.assertIn("John 11:35", query1)
        self.assertIn("John 11:35", query1[:15])
        query2 = self.bible_search.search("hospitality, a lover of good men, sober, just, holy, temperate")
        self.assertIn("Titus 1:8", query2)
        query3 = self.bible_search.search("elect")
        self.assertIn("Titus 1:1", query3)
        self.assertIn("Romans 8:33", query3)
        query4 = self.bible_search.search("Caesarea Philippi")
        self.assertIn("Mark 8:27", query4)
        self.assertIn("Mark 8:27", query4[:5])
        query5 = self.bible_search.search("scorneth")
        self.assertIn("Proverbs 19:28", query5)
        query6 = self.bible_search.search("Therefore came I forth to meet thee")
        self.assertIn("Proverbs 7:15", query6)
        self.assertIn("Proverbs 7:15", query6[:5])
        query7 = self.bible_search.search("And Israel dwelt in the land of Egypt, in the country of Goshen; and they "
                                          "had possessions therein, and grew, and multiplied exceedingly.")
        self.assertIn("Genesis 47:27", query7[:3])
        query8 = self.bible_search.search("notawordinthebible")
        self.assertEqual(len(query8), 0)

    def test_versions(self):
        self.assertIn("KJV", self.bible_search.versions)
        self.assertIn("ESV", self.bible_search.versions)
        self.assertIn("ASV", self.bible_search.versions)


if __name__ == '__main__':
    unittest.main()
