# about

toy symbolic interpreter for python; for functions that work on integers,
i.e.

def example(x):
  if x > 5:
    return 0
  else:
    return 5

will print out the solutions for x that have branches based on comparison
operations.

NOTE: this is very hacked together, and for 99% of cases, will probably not
work.

# building

cd build
cmake ../src
make -j

there's a dependency on STP.

# future

this is completely "unfinished", but it's finished for the case I was
interested in twiddling with: stack machines that operate on symbolic
variables.
