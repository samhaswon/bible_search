build:
	./venv/Scripts/activate
	py -m build

install:
	pip install --force-reinstall ./dist/multi_bible_search-1.0.0.tar.gz
