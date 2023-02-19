# pyximport automatically recompiles and reloads your .pyx modules
import pyximport
pyximport.install()

# import your extension type
from book import Cy_Book

# create an object of extension type Cy_Book
some_book = Cy_Book('Cython: A Guide for Python Programmers')
print(some_book.title)
