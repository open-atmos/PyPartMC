# pylint: disable=wrong-import-position
# https://bugs.python.org/issue37373
import sys

if sys.platform == "win32" and sys.version_info[:2] >= (3, 7):
    import asyncio

    asyncio.set_event_loop_policy(asyncio.WindowsSelectorEventLoopPolicy())

import os
import pathlib
import re

import nbformat
import pytest
from nbconvert.preprocessors import ExecutePreprocessor


# https://stackoverflow.com/questions/7012921/recursive-grep-using-python
def findfiles(path, regex):
    reg_obj = re.compile(regex)
    res = []
    for root, _, fnames in os.walk(path):
        for fname in fnames:
            if reg_obj.match(fname):
                res.append(os.path.join(root, fname))
    return res


@pytest.fixture(
    params=findfiles(
        (pathlib.Path(__file__).parent.parent / "examples").absolute(), r".*\.(ipynb)$"
    )
)
def notebook_filename(request):
    return request.param


# pylint: disable=redefined-outer-name
def test_run_notebooks(notebook_filename, tmp_path):
    with open(notebook_filename, encoding="utf8") as f:
        nb = nbformat.read(f, as_version=4)

        cells_to_be_deleted = []
        for idx, cell in enumerate(nb.cells):
            if cell.cell_type == 'code' and cell.source.startswith('!'):
                cells_to_be_deleted.append(idx)
        for idx in reversed(cells_to_be_deleted):
            del nb.cells[idx]

        ep = ExecutePreprocessor(timeout=15 * 60, kernel_name="python3")
        ep.preprocess(nb, {"metadata": {"path": tmp_path}})
