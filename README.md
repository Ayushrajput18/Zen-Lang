# 🚀 Zen-Lang (formerly MyLang)  
A **custom scripting language** with **easy-to-learn syntax**, **unique data types**, and **Python-like simplicity**.  
Built entirely in **C++** for speed, extensibility, and fun! 🎯  

![GitHub repo size](https://img.shields.io/github/repo-size/Ayushrajput18/Zen-Lang?color=brightgreen&style=flat-square)
![GitHub last commit](https://img.shields.io/github/last-commit/Ayushrajput18/Zen-Lang?color=blue&style=flat-square)
![GitHub issues](https://img.shields.io/github/issues/Ayushrajput18/Zen-Lang?color=yellow&style=flat-square)
![GitHub stars](https://img.shields.io/github/stars/Ayushrajput18/Zen-Lang?style=social)


```text
███████╗███████╗███╗   ██╗      
╚══███╔╝██╔════╝████╗  ██║      
  ███╔╝ █████╗  ██╔██╗ ██║      
 ███╔╝  ██╔══╝  ██║╚██╗██║      
███████╗███████╗██║ ╚████║      
╚══════╝╚══════╝╚═╝  ╚═══╝      

```
## ✨ Features
✅ Python-like syntax (clean and minimal)  
✅ C++-style headers (e.g., `#use <core>`)  
✅ Unique data types (like `pack` and `map`)  
✅ Fully interpreted (no compiler needed)  
✅ Control structures: `if/else`, `while`, `for`  
✅ Built-in `print()` and core library  
✅ Written in **C++17** (fast & lightweight)  

---

## 🔥 Example Syntax
```
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
🧾 Data Types
num → Integer (e.g., 5)
dec → Decimal/float (e.g., 3.14)
text → String (e.g., "Hello")
flag → Boolean (e.g., true/false)
pack → List/array (e.g., [1,2,3])
map → Dictionary/map (e.g., {"key": value})

``` plain text
📂 Project Structure
bash
Copy
Edit
├── main.cpp            # Entry point
├── lexer.h / lexer.cpp  # Tokenizer
├── parser.h / parser.cpp# AST builder
├── interpreter.h / interpreter.cpp # Executor
├── tokens.h            # Token definitions
├── example.mylang      # Sample Zen-Lang code
└── README.md           # Documentation

```
🚀 Getting Started
1️⃣ Clone the Repository
```
git clone https://github.com/Ayushrajput18/Zen-Lang.git
cd Zen-Lang
```
2️⃣ Build the Interpreter
```
g++ -std=c++17 *.cpp -o zen
```
3️⃣ Run a Script
```
./zen example.mylang
```
Or start the interactive REPL:
```
./zen
>>> print("Hello Zen!")
```

🛠 Roadmap (Planned Features)

 Functions & scoping
 Lists and dictionaries enhancements
 Import system for modules
 Standard library (math, file I/O, etc.)
 REPL with auto-completion


🤝 Contributing

Want to improve Zen-Lang? Contributions are welcome:
Fork the repo
Create a branch (feature-xyz)
Commit your changes
Open a Pull Reques

📜 License
Licensed under the MIT License.
Free to use, modify, and distribute.


⭐ If you like Zen-Lang, give it a star on GitHub!
```

✅ This keeps **your old syntax, unique data types, and headers**, but merges it with a **modern look (ASCII banner, badges, roadmap)**.

Would you also like me to generate **a matching REPL welcome message** (so when you start Zen-Lang in terminal, it prints your ASCII logo)?
```
