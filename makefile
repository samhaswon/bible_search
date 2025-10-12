build:
	TUNE_NATIVE=1 python3 -m build

install: dist/multi_bible_search-2.1.5.tar.gz
	pip install --force-reinstall ./dist/multi_bible_search-2.1.5.tar.gz
	cp venv/lib/python3.12/site-packages/multi_bible_search/*.so src/multi_bible_search/

full: build install
