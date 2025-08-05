# 🚀 Zen-Lang (formerly MyLang)  
A **custom scripting language** with **easy-to-learn syntax**, **unique data types**, and **Python-like simplicity**.  
Built entirely in **C++** for speed, extensibility, and fun! 🎯  

![GitHub repo size](https://img.shields.io/github/repo-size/Ayushrajput18/Zen-Lang?color=brightgreen&style=flat-square)
![GitHub last commit](https://img.shields.io/github/last-commit/Ayushrajput18/Zen-Lang?color=blue&style=flat-square)
![GitHub issues](https://img.shields.io/github/issues/Ayushrajput18/Zen-Lang?color=yellow&style=flat-square)
![GitHub stars](https://img.shields.io/github/stars/Ayushrajput18/Zen-Lang?style=social)


```text
```text
███████╗███████╗███╗   ██╗      ██╗      █████╗ ███╗   ██╗  ████████╗ 
╚══███╔╝██╔════╝████╗  ██║      ██║     ██╔══██╗████╗  ██║ ██╔══
  ███╔╝ █████╗  ██╔██╗ ██║      ██║     ███████║██╔██╗ ██║ ██║   ████║
 ███╔╝  ██╔══╝  ██║╚██╗██║      ██║     ██╔══██║██║╚██╗██║ ██║     ██║
███████╗███████╗██║ ╚████║      ███████╗██║  ██║██║ ╚████║ ╚████████╔╝
╚══════╝╚══════╝╚═╝  ╚═══╝      ╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝  ╚═════╝ 
     

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

#use <core>
num a = 5
dec pi = 3.14
text name = "Ayush"
flag isOn = true
pack nums = [1,2,3]
map ages = {"Ayush": 21}
print(a, pi, name, isOn, nums, ages)

```
## 🧾 Data Types

Zen-Lang introduces **simple, readable data types** that are easy to learn:

| 🔑 Type  | 📝 Description          | 🖥️ Example             |
|----------|------------------------|-----------------------|
| 🔢 **`num`**  | Integer values         | `num a = 42`         |
| 🔣 **`dec`**  | Decimal/float values   | `dec pi = 3.14`      |
| 📝 **`text`** | String values          | `text name = "Zen"`  |
| ✅ **`flag`** | Boolean (true/false)   | `flag isOn = true`   |
| 📦 **`pack`** | List/array collection  | `pack nums = [1,2,3]`|
| 🗺️ **`map`**  | Key-value dictionary   | `map ages = {"Ayush": 21}` |

---

### 💻 **Quick Example**
```
num a = 5
dec pi = 3.14
text name = "Ayush"
flag isOn = false
pack nums = [10, 20, 30]
map scores = {"Math": 95, "CS": 98}


``` 
📂 Project Structure
```
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
## 🛠 Roadmap (Planned Features)

Zen-Lang is evolving quickly! Here's our progress:

### ✅ Core Language Features ![Progress](https://img.shields.io/badge/Progress-60%25-brightgreen?style=for-the-badge)
▓▓▓▓▓▓▓▓░░░░░░ (6/10)
- [x] Variables and assignments  
- [x] Loops (`for`, `while`)  
- [x] Conditionals (`if/else`)  
- [x] Built-in `print()` function  
- [ ] Functions & Scoping  
- [ ] Error handling system  

---

### 📦 Data Structures ![Progress](https://img.shields.io/badge/Progress-50%25-yellow?style=for-the-badge)
▓▓▓▓▓░░░░░░░░░ (2/4)
- [x] `pack` (lists/arrays)  
- [x] `map` (dictionaries)  
- [ ] Nested collections support  
- [ ] Iterators & advanced looping  

---

### 📚 Standard Library ![Progress](https://img.shields.io/badge/Progress-20%25-orange?style=for-the-badge)
▓░░░░░░░░░░░░░ (1/5)
- [ ] Math module (`sin`, `sqrt`, etc.)  
- [ ] File I/O (`read`, `write`)  
- [ ] String utilities (`split`, `join`)  
- [ ] Date/Time functions  
- [ ] Basic CLI tools  

---

### 🖥 Development Tools ![Progress](https://img.shields.io/badge/Progress-30%25-yellowgreen?style=for-the-badge)
▓▓░░░░░░░░░░░░ (1/4)
- [x] Basic REPL  
- [ ] REPL auto-completion/history  
- [ ] Syntax highlighting for IDEs  
- [ ] Module import system  

---

### 🌐 Community & Ecosystem ![Progress](https://img.shields.io/badge/Progress-10%25-red?style=for-the-badge)
░░░░░░░░░░░░░░ (0/4)
- [ ] Contributor's guide & documentation  
- [ ] Example projects & demos  
- [ ] CI/CD testing suite  
- [ ] Package manager support  
 

---

<h3>🤝 Contributing to Zen-Lang  </h3>
We ❤️ contributions! Whether it's fixing a bug, improving docs, or adding a new feature, all help is welcome.  
---

<h3>🔧 How to Contribute  </h3>
Fork the repository:  
Click the "Fork" button at the top right of the repo page.

Clone your fork:
```
git clone https://github.com/your-username/Zen-Lang.git
cd Zen-Lang
```
Create a branch for your feature or fix:
```
git checkout -b feature/amazing-feature
```
Make your changes & commit:
```
git add .
git commit -m "Added amazing feature 🎉"
```
Push to your fork:
```
git push origin feature/amazing-feature
```
Open a Pull Request (PR) on GitHub.

<hr style="height: 3px; border: solid; background: #4CAF50;">

<h3>🏆 Contribution Guidelines</h3>

Write clear commit messages (feat:, fix:, docs:).
<br>
Keep code modular and well-documented.
<br>
Run tests (if available) before submitting PRs.
<br>
Follow Zen-Lang’s coding style (readable, minimal).
<br>

<h3>📢 Ways to Contribute</h3> 

✅ Fix bugs or improve performance
<br>
✅ Suggest or add new language features
<br>
✅ Write or improve documentation
<br>
✅ Build example scripts or tutorials
<br>
✅ Test Zen-Lang on different OS/platforms
<br>

<h3>🌟 Recognition</h3>

Your name will appear in the Contributors list.
<br>
Top contributors may be added as collaborators!
<br>
<h3>🖊 Contributor Badge</h3>
<br>
Add this badge to your profile if you've contributed:  

```
[![Contributor](https://img.shields.io/badge/Zen--Lang-Contributor-blue?style=for-the-badge)](https://github.com/Ayushrajput18/Zen-Lang)

```
---
<h3>📜 License</h3>
Licensed under the Apache-2.0 license
<br>
Free to use, modify, and distribute.
<br>

---
<h2>⭐ If you like Zen-Lang, give it a star on GitHub! </h2>

