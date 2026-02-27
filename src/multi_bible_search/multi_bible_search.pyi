"""
The C search engine implementation stub.
"""
from typing import Optional

__all__ = ["BibleSearch"]


class BibleSearch:
    """
    The C search engine for searching the Bible.
    """
    def __init__(self) -> None: ...
    def search(self, query: str, version: str, max_results: int = ...) -> Optional[list[str]]:
        """
        Search for a passage in the Bible.
        :param query: The search query string.
        :param version: The version to search.
        :param max_results: The maximum number of results to retrieve.
        :return: List of match references (e.g., `["John 11:35", "Matthew 1:7", ...]`).
        """
        ...
    def load(self, json: str, version: str) -> None:
        """
        Load an index of either a version or multiple versions' combined index.
        Ideally, this would take in a file name and just do the parsing *and* extraction work on
        the C side of things in the future.
        :param json: The version index string to preload.
        :param version: The name of the version being loaded.
        :returns: None.
        :raises RuntimeError: For invalid version strings.
        """
        ...
    def unload(self, version: str) -> None:
        """
        Unloads the specified version index from memory iff it's in memory.
        :param version: The name of the version to unload.
        :returns: None.
        :raises RuntimeError: For invalid version strings.
        """
        ...
    def index_size(self) -> int:
        """
        Calculates the size of each hashtable in memory (in bytes)
        and the size of the object itself.
        :return: The bytes of memory used by the object.
        """
        ...
