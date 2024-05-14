import bz2
import json

with bz2.open(f"../src/multi_bible_search/data/NIV 2011.json.pbz2", "rt", encoding='utf-8') as data_file:
    data = json.load(data_file)

with open("version_keys.txt", "w") as key_file:
    keys = list(data.keys())
    keys.sort()
    key_file.write(''.join(f"{k}\n" for k in keys))
