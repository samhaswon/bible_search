from src.multi_bible_search import BibleSearch
import faulthandler


faulthandler.enable()
searcher = BibleSearch()
searcher.load_all()


with open("version_keys.txt", "r") as token_file:
    tokens = token_file.read().splitlines()

unique_tokens = []

for token in tokens:
    unique = 0
    for version in searcher.versions:
        result = searcher.search(token, version)
        if len(result):
            unique += 1
    if unique < 2:
        unique_tokens.append(token)

if len(unique_tokens) == 0:
    print("No unique tokens")
else:
    print("Unique tokens:\n", ''.join(f"{t}, " for t in unique_tokens))
