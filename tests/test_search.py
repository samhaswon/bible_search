"""
Test the functionality of the search module.
"""
import unittest

# pylint: disable=import-error
from src.multi_bible_search.bible_search_adapter import BibleSearch
from src.multi_bible_search.invalid_version import InvalidVersion


class TestSearch(unittest.TestCase):
    """
    Test the functionality of the search module.
    """
    def setUp(self) -> None:
        """
        Make the Bible search object.
        :return: None.
        """
        self.bible_search = BibleSearch()

    def test_query_a(self):
        """
        Test some basic queries.
        :return: None.
        """
        query1 = self.bible_search.search("Jesus wept")
        self.assertIn("John 11:35", query1)
        self.assertIn("John 11:35", query1[:15])
        query2 = self.bible_search.search(
            "hospitality, a lover of good men, sober, just, holy, temperate"
        )
        self.assertIn("Titus 1:8", query2)
        self.assertIn("Titus 1:8", query2[:5], "Actual index: " + str(query2.index("Titus 1:8")))
        query3 = self.bible_search.search("elect")
        self.assertIn("Titus 1:1", query3)
        self.assertIn("Romans 8:33", query3)
        query4 = self.bible_search.search("Caesarea Philippi")
        self.assertIn("Mark 8:27", query4)
        self.assertIn("Mark 8:27", query4[:5])
        query5 = self.bible_search.search("scorneth")
        self.assertIn("Proverbs 19:28", query5)

    def test_query_b(self):
        """
        Test some more complicated queries.
        :return: None.
        """
        query6 = self.bible_search.search("Therefore came I forth to meet thee")
        self.assertIn("Proverbs 7:15", query6)
        # Sorting is backwards for partial matches in the C version
        self.assertIn("Proverbs 7:15", query6[:5])
        query7 = self.bible_search.search(
            "And Israel dwelt in the land of Egypt, in the country of Goshen; and they "
            "had possessions therein, and grew, and multiplied exceedingly.")
        self.assertIn(
            "Genesis 47:27", query7[:3],
            "Actual index: " + str(query7.index("Genesis 47:27"))
        )
        query8 = self.bible_search.search("notawordinthebible")
        self.assertEqual(len(query8), 0)
        query9 = self.bible_search.search("Jesus wept", "ESV")
        self.assertGreater(len(query9), 0)
        self.assertIn('John 11:35', query9[:5])
        query10 = self.bible_search.search(
            "And your feet shod with the preparation of the gospel of peace"
        )
        self.assertIn("Ephesians 6:15", query10)
        self.assertEqual("Ephesians 6:15", query10[0])
        query11 = self.bible_search.search(
            "But a lover of hospitality, a lover of good men, sober, just, holy, temperate;"
        )
        self.assertIn("Titus 1:8", query11)
        self.assertIn("Titus 1:8", query11[:5])

    def test_query_edge_cases(self):
        """
        Test query edge cases.
        :return: None.
        """
        # Test it with some unicode
        query11 = self.bible_search.search("something æ\u200B\u201Dב” else")
        self.assertGreater(len(query11), 0)
        query12 = self.bible_search.search("wh")
        self.assertEqual(len(query12), 0)
        query13 = self.bible_search.search("w")
        self.assertEqual(len(query13), 0)
        query14 = self.bible_search.search("a")
        self.assertGreater(len(query14), 0)
        query15 = self.bible_search.search("")
        self.assertEqual(len(query15), 0)

    def test_versions(self):
        """
        Make sure the versions are placed correctly.
        :return: None.
        """
        # Basic version getter
        self.assertIn("KJV", self.bible_search.versions)
        self.assertIn("ESV", self.bible_search.versions)
        self.assertIn("ASV", self.bible_search.versions)

        # Language version getter
        self.assertIn("KJV", self.bible_search.english_versions)
        self.assertIn("ESV", self.bible_search.english_versions)
        self.assertIn("ASV", self.bible_search.english_versions)

        self.assertIn("BTX3", self.bible_search.spanish_versions)
        self.assertIn("RV1960", self.bible_search.spanish_versions)
        self.assertIn("RV2004", self.bible_search.spanish_versions)

        self.assertNotIn("KJV", self.bible_search.spanish_versions)
        self.assertNotIn("BTX3", self.bible_search.english_versions)
        self.assertNotIn("RV1960", self.bible_search.english_versions)
        self.assertNotIn("RV2004", self.bible_search.english_versions)

    def test_load_all(self):
        """
        Test loading all versions.
        """
        self.bible_search.load_all()
        self.assertEqual(self.bible_search.loaded.sort(), self.bible_search.versions.sort())
        english_versions = self.bible_search.english_versions
        english_versions.sort()
        for version in english_versions:
            # print(version)
            result = self.bible_search.search("jesus wept", version)
            self.assertGreater(len(result), 0, f"Fail for version {version}")

        spanish_versions = self.bible_search.spanish_versions
        spanish_versions.sort()
        for version in spanish_versions:
            result = self.bible_search.search("Jesús lloró", version)
            self.assertGreater(
                len(result),
                0,
                f"Fail for version {version}.\n"
                f"Loaded: {self.bible_search.loaded}\n"
            )

    def test_unload_version(self):
        """
        Make sure that unloading a version index works correctly.
        :return:
        """
        self.bible_search.load("KJV")
        self.bible_search.unload_version("KJV")

        # Should raise an Exception if we try to unload it again.
        with self.assertRaises(InvalidVersion):
            self.bible_search.unload_version("KJV")
        self.bible_search.load("KJV")
        # This should be skipped and not cause an error
        self.bible_search.load("KJV")


if __name__ == '__main__':
    unittest.main()
