🗂️ Simple File Explorer (C - Windows)

A lightweight command-line based file explorer built in C for Windows systems. It allows you to navigate through directories, manage files, and perform common file operations — all from the terminal, with a bit of flair. 

📋 What It Can Do - 

- 📁 List all files and folders in the current directory
- 📂 Navigate between directories
- ✏️ Create, delete, or rename files and folders
- 🔍 Search for files using name patterns
- 📐 Check file properties like size and last modified date
- 📋 Copy files from one location to another
- 🎨 Color-coded terminal output for better visibility
- 💬 Fun startup quote to keep the mood light
- 📝 All actions get logged to `file_explorer_log.txt`

🛠️ What You’ll Need -  

- A Windows operating system
- A working C compiler (like GCC via MinGW or MSVC)

⚙️ How to Compile -
  
  If you're using GCC (MinGW), simply open your terminal and run:
    gcc fileexp.c -o fileexp.exe
