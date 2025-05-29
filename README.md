# University Assignment: Database Management System
**Author**: Theodor Vulpe  
**Course**: PCLP1 (Programming)  
**Assignment Link**: [Tema 3 - CBD (Database Operations)](https://ocw.cs.pub.ro/courses/programare/teme_2024/tema3_2024_cbd)  

## 📖 Overview  
This project implements a simple **database management system** in C, capable of handling student records, course enrollments, and grades. Key features include:  
- **CRUD operations** (Create, Read, Update, Delete) for students, courses, and enrollments.  
- **Querying** with conditional filters (e.g., list students with grades above a threshold).  
- **Data encryption** via XOR operations (Task 3).  

## 🛠️ Implementation Details  

### 📂 Task 1: Database Initialization  
- **1.1**: Reads input files line-by-line, parsing data into structured tables (students, courses, enrollments). General averages are updated after all enrollments are processed.  
- **1.2**: Adds students to the database and increments the student count.  
- **1.3**: Fixed memory management issues (e.g., setting pointers to `NULL` after freeing).  

### 🔍 Task 2: Query System  
- **2.1**: Processes query instructions by splitting input strings into tokens and routing to appropriate functions.  
  - **2.1.2 & 2.1.3**: Implements conditional checks (e.g., filters for grades, courses) using a vector of conditions.  
- **2.2**: Updates general averages dynamically when enrollments are modified.  

### 🔒 Task 3: XOR Encryption  
- Follows the pseudocode provided in the assignment, operating on `void*` and `char*` buffers to perform XOR encryption/decryption.  
