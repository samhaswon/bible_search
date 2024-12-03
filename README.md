# bible_search

A Python package for searching multiple versions of the Bible.

This is, in effect, an exhaustive concordance.

## Installation

```shell
pip install multi_bible_search
```

## Usage 

```python
from multi_bible_search import BibleSearch


if __name__ == '__main__':
    searcher = BibleSearch()
    query_results = searcher.search("Jesus wept")
    print(query_results[0:10])

```

The `search` method returns a list of references. For example, the above code prints the following:

```
['Luke 8:52', 'Matthew 1:16', 'Luke 5:31', '1 Corinthians 2:2', 'Colossians 1:1', 'Matthew 22:41', 'Genesis 37:35', 'Luke 2:21', 'John 8:59', 'Matthew 3:16']
```

The results of each query are ranked by the number of keyword occurrences.

If a query is made with no matches, say "notawordinthebible," the result of the query will be a list of length 0. 

The maximum number of results may be specified with the optional `max_results` parameter.

### Preloading an Index

Versions are automatically loaded as needed, but you may wish to preload a version for the sake of speed. 

An index of a particular version can be preloaded with the `load()` method. Simply pass the version identifier as a string to load it.

Remember that capitalization is important when using this method.

You may also preload all versions by simply calling the `load_all()` method. 

Finally, you can unload a version with the `unload_version()` method for a particular version. 

## Supported Versions

Supported versions can be listed with this:

```python
from multi_bible_search import BibleSearch


searcher = BibleSearch()

# All versions
searcher.versions

# English versions
searcher.english_versions

# Spanish versions
searcher.spanish_versions
```

Currently, these versions are supported:

- English

  - A Conservative Version (ACV)

  - American King James Version (AKJV)

  - Amplified Bible (AMP)

  - American Standard Version (ASV)

  - Bible in Basic English (BBE)

  - Berean Standard Bible (BSB)

  - Christian Standard Bible (CSB)

  - Darby Bible (Darby)

  - Douay-Rheims 1899 American Edition (DRA)

  - Rotherham's Emphasized Bible (EBR)

  - English Standard Version (ESV)

  - Geneva Bible (GNV)

  - King James Version 1769 (KJV)

  - King James Version 1611 (KJV 1611)

  - Literal Standard Version (LSV)

  - The Message (MSG)

  - New American Standard Bible (NASB 1995)

  - New English Translation (NET)

  - New International Version (NIV 1984)

  - New International Version (NIV 2011)

  - New King James Version (NKJV)

  - New Living Translation (NLT)

  - Restored Name King James Version (RNKJV)

  - Revised Standard Version (RSV)

  - Revised Webster Version 1833 (RWV)

  - Updated King James Version (UKJV)

  - World English Bible (WEB)

  - Young’s Literal Translation (YLT)

- Spanish

  - La Biblia Textual 3ra Edicion (BTX3)

  - Reina-Valera 1960 (RV1960)

  - Reina Valera Gómez 2004 (RV2004)

Versions can be specified with the `version` parameter in `search()`. 
The string passed should be the short form of the version name.

For example, you could use:
```python 
from multi_bible_search import BibleSearch


searcher = BibleSearch()
searcher.search("Jesus wept", version="ESV")
```
to search the ESV.
