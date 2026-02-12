🖥️ Project Overview: The 20465 Assembler Engine
Project Overview
This project is a full implementation of a Two-Pass Assembler for a custom 15-bit Virtual Machine. It was developed as the final capstone project (Maman 14) for the System Programming Lab course (20465) at The Open University of Israel.


The software translates symbolic assembly language into binary machine code, formatted in octal for the target environment.


🛠️ Core Engineering Features

Smart Pre-Assembler (Macro Expansion): A dedicated preprocessing stage that identifies, parses, and expands macros into raw assembly instructions.


Robust Two-Pass Architecture: * Pass 1: Scans the code to build a dynamic Symbol Table, calculating memory offsets for instructions and data (IC/DC).



Pass 2: Resolves labels, handles forward references, and generates the final machine code in octal format.



Custom Virtual Machine Specs: Supports 8 general-purpose registers (r0-r7), a 15-bit word size, and a 4096-word memory space.



Advanced Data Management: Implements low-level data structures like linked lists for efficient symbol and macro tracking.


Comprehensive Error Suite: A built-in validation engine that detects syntax errors, illegal addressing modes, and label collisions, providing detailed feedback with line numbers.


📂 Technical Deliverables
The engine generates a complete suite of files for the simulated environment:


.am: The expanded source code after macro processing.



.ob: The binary object file, mapped to memory starting from address 100.



.ent & .ext: Symbol tables for internal and external linkages, enabling modular programming.

🎓 Academic Context
This project was developed as the final capstone for the System Programming Lab (20465) at The Open University of Israel.


Developed by:

Shahar Shriki

Doron Affriat

🚀 Getting Started

Note: This project is written in ANSI C (C90) and requires gcc with strict flags (-Wall -ansi -pedantic) for compilation
