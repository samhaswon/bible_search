import os
from typing import List, Tuple
from urllib import request


class BibleDownloader:
    indexes = [
        "ACV.json.pbz2", "AKJV.json.pbz2", "AllEng.json.pbz2", "AllEs.json.pbz2", "AMP.json.pbz2",
        "ASV.json.pbz2", "BBE.json.pbz2", "BSB.json.pbz2", "BTX3.json.pbz2", "CSB.json.pbz2",
        "Darby.json.pbz2", "DRA.json.pbz2", "Dynamic.json.pbz2", "EBR.json.pbz2", "EsRV.json.pbz2",
        "ESV.json.pbz2", "ExtraEng.json.pbz2", "GNV.json.pbz2", "KJV%201611.json.pbz2",
        "KJV-like.json.pbz2", "KJV.json.pbz2", "Literal.json.pbz2", "Literal2.json.pbz2",
        "LSV.json.pbz2", "MSG.json.pbz2", "NASB%201995.json.pbz2", "NET.json.pbz2",
        "NIV%201984.json.pbz2", "NIV%202011.json.pbz2", "NIV.json.pbz2", "NKJV.json.pbz2",
        "NLT.json.pbz2", "RNKJV.json.pbz2", "RSV.json.pbz2", "RV1960.json.pbz2",
        "RV2004.json.pbz2", "RWV.json.pbz2", "UKJV.json.pbz2", "WEB.json.pbz2", "YLT.json.pbz2",
    ]
    __index_file_urls: List[Tuple[str, str]] = \
        [
            (f"https://github.com/samhaswon/bible_search/"
             f"raw/refs/heads/main/src/multi_bible_search/data/{x}",
             x
             )
            for x in indexes
        ]

    def download(self):
        base_dir = os.path.join(__file__[:-19], "data")
        os.makedirs(base_dir, exist_ok=True)
        for file_url, destination in self.__index_file_urls:
            destination = os.path.join(base_dir, destination.replace("%20", " "))
            print(f"Downloading {file_url} to {destination}")
            request.urlretrieve(file_url, destination)
