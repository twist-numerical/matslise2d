import pyslise2d
import sys
import doctest

failures, test_count = doctest.testmod(pyslise2d, verbose=True)

assert test_count > 0, "No tests were found"

sys.exit(1 if failures else 0)
