# Simple Data System – C

A lightweight data management system written in pure C, designed to demonstrate and apply fundamental container-based programming concepts.  
This project serves as a practical summary of the second C course, which focused on general software development principles and core data structures.

---

## Project Overview

The Simple Data System implements a basic infrastructure for storing, managing, and manipulating data using classic container abstractions.  
It emphasizes clarity, correctness, and low-level memory handling, staying close to how such systems are traditionally built in C.

The goal is not to compete with full-scale databases, but to show a solid understanding of:
- Data organization
- Container design
- Memory management
- Modular programming
- Clean procedural architecture

---

## Key Concepts Demonstrated

- **Containers**
  - Dynamic arrays  
  - Linked lists  
  - Basic collection handling patterns  

- **Memory Management**
  - Manual allocation and deallocation (`malloc`, `calloc`, `free`)
  - Ownership and lifetime of objects
  - Avoidance of memory leaks and dangling pointers  

- **Abstraction in C**
  - Encapsulation through structs and functions
  - Separation between interface and implementation
  - Reusable container logic  

- **General Coding Principles**
  - Modular file structure  
  - Clear naming conventions  
  - Defensive programming  
  - Input validation  
  - Error handling  

---

## Course Context

This project summarizes the main themes of the second C course:
- Transition from basic syntax to system-level thinking  
- Writing maintainable and structured C programs  
- Designing small-scale “systems” instead of isolated functions  
- Understanding how containers form the backbone of real software systems  

It reflects the traditional C mindset:  
**explicit control, predictable behavior, and minimal abstraction overhead.**

---

## Features

- Basic data storage and retrieval  
- Container-based internal structure  
- Simple and deterministic behavior  
- Fully written in ANSI C  
- No external libraries  

---

## Compilation

```bash
gcc *.c -o simple_data_system
