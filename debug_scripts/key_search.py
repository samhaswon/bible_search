from src.multi_bible_search import BibleSearch

token = "jesus wept"


searcher = BibleSearch()
searcher.load_all()

for version in searcher.versions:
    result = searcher.search(token, version)
    if len(result):
        print(f"Version: {version}; Results {result}")
