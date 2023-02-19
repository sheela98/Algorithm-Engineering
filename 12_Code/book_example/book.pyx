# cython: language_level = 3

# cdef class statement tells cython to make an extension type
cdef class Cy_Book: # Cy_Book is an extension type
    # either "readonly" or "public"
    cdef readonly str title # data field

    def __init__(self, title):
        self.title = title
