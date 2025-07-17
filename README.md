A Simulated Command-Line Operating System in C++

Overview
MiniSimOS is a lightweight, console based simulated operating system built entirely using C++. It replicates core OS functionalities such as user authentication, hierarchical file management, memory/process simulation, and an interactive terminal interface. Designed as an academic project, it prioritizes ease of use, modularity, and intuitive UI elements like color-coded outputs, loading animations, and smart prompts—all within a command-line environment.

Features
  * User Login System
  * Account creation & password-based login
  * User data stored securely in a simulated database (text files)
  * File Management System
  * Create, edit, rename, delete files & folders
  * Navigate file system with commands like cd, ls, mkdir, rm
  * Simulates directory hierarchy with breadcrumb-like path display
  * Terminal Interface
  * Color-coded UI with stylized ASCII art and animations
  * Command suggestions and help screen
  * Clear feedback on success/failure of commands
  * Memory & Process Simulation (Basic)
  * Shows memory status with visual bars
  * Mimics process creation and priority handling (static logic)
  * Persistent Storage
  * All files, users, and settings saved between sessions

Tech Stack

* Language: C++
* Environment: Console (Windows/Linux)
* Libraries: Standard C++ libraries (fstream, iostream, string, map, etc.)

Usage Instructions

1. Compile the source files using g++ or any C++ compiler
   Example: g++ main.cpp -o MiniSimOS
2. Run the executable: ./MiniSimOS
3. Register or login as a user
4. Use on-screen command options to interact with the system

Example Commands

* mkdir foldername → Create a folder
* touch filename → Create a file
* cd foldername → Navigate into folder
* ls → List contents of current directory
* rm filename → Delete a file

Skills Demonstrated

* Object-Oriented Programming
* File I/O and data persistence
* UI/UX design for CLI tools
* Data structures (maps, vectors, stacks)
* Simulation of OS concepts (files, users, memory)

Future Improvements

* Add command history and autocomplete
* Encrypt passwords and files
* Real-time multitasking simulation
* Admin-level user management
