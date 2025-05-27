# Stack-Based Interpreter

This is a simple stack-based interpreter written in C++. It supports basic arithmetic, stack manipulation, and logical comparison instructions through a Forth-like syntax. Input is parsed from a file, tokenized, interpreted into instructions, and executed on a dynamic stack.

## Features

- Stack operations: `DUP`, `DROP`, `SWAP`, `ROT`, `NIP`
- Arithmetic operations: `+`, `-`, `*`, `/`, `MOD`, `NEG`, `ABS`, `MIN`, `MAX`
- Logical operations: `=`, `<`, `>`
- Output instructions: `.`, `CR`
- Literal integer support
- Basic error handling for unknown instructions and empty stack

## Instruction Set

| Token       | Operation                        |
|-------------|----------------------------------|
| `.`         | Print top of stack               |
| `CR`        | New line                         |
| Integer     | Push to stack                    |
| `DUP`       | Duplicate top element            |
| `DROP`      | Remove top element               |
| `SWAP`      | Swap top two elements            |
| `ROT`       | Rotate top three elements        |
| `NIP`       | Remove second element from top   |
| `+`         | Addition                         |
| `-`         | Subtraction                      |
| `*`         | Multiplication                   |
| `/`         | Division                         |
| `MOD`       | Modulus                          |
| `NEG`       | Push negative of top             |
| `ABS`       | Absolute value of top            |
| `MIN`       | Minimum of top two               |
| `MAX`       | Maximum of top two               |
| `=`         | Equals comparison (true = -1)    |
| `<`         | Less than (true = -1)            |
| `>`         | Greater than (true = -1)         |
| `(comment)` | Inline comment                   |

## File Structure

- `main.cpp` (your provided code): Contains the complete implementation including:
  - Stack class using singly linked list
  - Lexer, parser, and interpreter
  - Instruction handlers

## Usage

1. **Prepare Input File**
   Create a `.fth` file with your stack-based code, e.g.:
   ```forth.fth
    5 10 + .
    CR
    3 DUP * .


2. **Compile the Code**
g++ main.cpp -o main && ./main forth.fth  -- Linux & Unix based systems

**For Windows based systems**
g++ main.cpp -o main.exe 
main.exe forth.fth   


