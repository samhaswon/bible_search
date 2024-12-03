"""
Find a token in version_keys.txt
"""

import faulthandler

from src.multi_bible_search import BibleSearch


faulthandler.enable()
searcher = BibleSearch()
searcher.load_all()


with open("version_keys.txt", "r", encoding="utf-8") as token_file:
    tokens = token_file.read().splitlines()

unique_tokens = []

for token in tokens:
    UNIQUE = 0
    for version in searcher.versions:
        result = searcher.search(token, version)
        if len(result):
            UNIQUE += 1
    if UNIQUE < 2:
        unique_tokens.append(token)

if len(unique_tokens) == 0:
    print("No unique tokens")
else:
    print("Unique tokens:\n", ''.join(f"{t}, " for t in unique_tokens))
