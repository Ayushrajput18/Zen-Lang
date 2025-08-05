# MyLang

A custom scripting language with easy-to-learn, unique data types and simple, Python-like syntax. Features C++-inspired headers (e.g., `#use <core>`) and is implemented in C++ for speed and extensibility.

## Example Syntax

```plaintext
#use <core>
num a = 5
dec pi = 3.14
text name = "Ayush"
flag isOn = true
pack nums = [1,2,3]
map ages = {"Ayush": 21}
print(a, pi, name, isOn, nums, ages)
```

## Data Types
- `num`: integer
- `dec`: decimal/float
- `text`: string
- `flag`: boolean
- `pack`: list/array
- `map`: dictionary/map

## Project Structure
- `main.cpp`: Entry point
- `lexer.h`/`lexer.cpp`: Tokenizer
- `parser.h`/`parser.cpp`: AST builder
- `interpreter.h`/`interpreter.cpp`: Executor
- `tokens.h`: Token definitions

## Getting Started
To be filled in as the project develops. 