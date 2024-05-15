build:
	py -m build

install: dist/multi_bible_search-2.0.0.tar.gz
	pip install --force-reinstall ./dist/multi_bible_search-2.0.0.tar.gz
	copy venv\\Lib\\site-packages\\multi_bible_search\\*.pyd src\\multi_bible_search\\

full: build install
