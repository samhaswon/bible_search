"""
Exception raised when an invalid version is passed to the search object.
"""


class InvalidVersion(Exception):
    """
    Raised when an invalid version is passed to the search object.
    """
    def __init__(self, version_attempt: str):
        super().__init__(f"Invalid version {version_attempt}")
