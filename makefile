build:
	./venv/Scripts/activate
	py -m build

install:
    pip install --force-reinstall ./dist/bible_search-0.0.1.tar.gz