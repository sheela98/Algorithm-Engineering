set(FOO "outer")

# each new scope creates a local copy of all variables
# new scopes are in functions and files added with add_subdirectory(<dir>)
function(dummy_func)
  set(FOO "inner")
endfunction()

dummy_func() # call dummy_func
message("${FOO}") # prints outer

# but can change variables from parent scope with PARENT_SCOPE
function(dummy_func)
  set(FOO "inner" PARENT_SCOPE)
endfunction()

dummy_func() # call dummy_func
message("${FOO}") # prints inner

# Top level scope is the CACHE, can serve as global variables
# values are kept between runs
set(GFOO "42" CACHE STRING "need to provide an explanation")

# FORCE --> overwrite value that already exists in the CACHE
set(GFOO "42" CACHE STRING "need to provide an explanation" FORCE)
# like calling from command line $ cmake -DGFOO=42 ..
