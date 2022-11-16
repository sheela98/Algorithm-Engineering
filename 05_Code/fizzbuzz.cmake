function(fizzbuzz n)
    foreach (i RANGE 1 ${n})
        set(result "") # all variables in cmake are strings
        math(EXPR fizz "${i} % 3") # strings are converted to numbers
        math(EXPR buzz "${i} % 5")
        if (0 EQUAL fizz)
            set(result "fizz")
        endif ()
        if (0 EQUAL buzz)
            string(APPEND result "buzz")
        endif ()
        if (NOT result) # nothing in result
            set(result "${i}") # put number i in result
        endif ()
        message("${result}") # show result
    endforeach ()
endfunction()

fizzbuzz(15) # call function
