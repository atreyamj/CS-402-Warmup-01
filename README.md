Documentation for Warmup Assignment 1
=====================================

+-------+
| BUILD |
+-------+

Comments: We use "make" command to build the source for this project.
          The usage of the command is described below:

          1. To build warmup1 :

              make
              or
              make warmup1

          2. To build listtest :

              make listtest


+------+
| SKIP |
+------+

The code was tested against all the cases provided in the grading guidelines on nunki.
The code passes all the test cases and error messages are generated whenever the transaction file do not meet the requirements.

The structure of error messages generated may differ,
however the intent of the messages remain the same. The messages convey the same meaning.

Hence, there no test cases to be skipped

+---------+
| GRADING |
+---------+

(A) Doubly-linked Circular List : 40 out of 40 pts

(B.1) Sort (file) : 30 out of 30 pts
(B.2) Sort (stdin) : 30 out of 30 pts

Missing required section(s) in README file : No
Cannot compile : Can compile.
Compiler warnings : No warnings found
"make clean" : All .o and executables removed
Segmentation faults : No Segmentation Faults detected
Separate compilation : Meets the criterion.
Malformed input : Handles correctly.
Too slow : No.
Bad commandline : Handles correctly.
Bad behavior for random input : Handles correctly.
Did not use My402List and My402ListElem to implement "sort" in (B) : Used List implementations.

+------+
| BUGS |
+------+

Comments: A signed integer for balance field. It might cause overflow if the values are too large.

+-------+
| OTHER |
+-------+

Comments on design decisions: The code is written in a way to make it as modular as possible
Comments on deviation from spec: The code was built on the ideas described in the guidelines and
                                  validated against all test cases in grading guidelines
