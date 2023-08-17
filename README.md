# C_Compiler_ANTLR4
 A partial C language compiler, programmed in C++ with ANTLR4. The compiler only generates x86 ASM code but is ready to generate ARM and MSP430 ASM code and for optimization.

## Installation

To install ANTLR4 (needs to be done only once on each machine - in the local directory), run the following command:

```bash
./install-antlr.sh
```

## Usage
Compilation on Linux
Navigate to the compiler directory and run the following command:
```
cd ./compiler
make ou runmake_ubuntu.sh
```
Running the Test Script
To run the test script, navigate to the tests directory and execute the following command:
```
cd ./tests
python3 ifcc-test.py ./testfiles/
```
Running a Single Test
To run a single test, navigate to the tests directory and use the following command (replace _name_test.c with the actual test file name):
```
cd ./tests
python3 ifcc-test.py ./testfiles/_name_test.c
```
Viewing the AST
To visualize the Abstract Syntax Tree (AST), navigate to the compiler directory and use the following command (replace _name_test.c with the actual test file name):
```
cd ./compiler
make gui FILE=../tests/testfiles/_name_test.c
```
## Implemented Features
- Variables
- Integer and character constants with single quotes (int and char types)
- Variable declaration (anywhere)
- Simple assignment and assignment during variable declaration
- Static variable verification:
- Verify that a variable used in an expression has been declared
- Verify that a variable is not declared multiple times
- Verify that a declared variable is used at least once
- Arithmetic expressions:
- Arithmetic operations: +, -, *, /
- Bitwise logical operations: |, &, ^
- Comparison operations: ==, !=, <, >, <=, >=
- Unary operations: ! and -
- Block structure {}
- Control structures if, else, while
- Return expression (anywhere)
- Definition of functions without parameters with int, char, or void return type