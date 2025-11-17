# 🌀 NewGit 2.0

NewGit 2.0 is a **mini version-control system** inspired by Git, implemented in **C on Linux**.  
It supports configuring a user, initializing a repository, staging files, committing changes,  
and reverting to any previous snapshot using a commit ID.

This project is focused on **system-level concepts**:
- `fork()` and **pipes** for process creation and inter-process communication
- **POSIX threads (pthreads)** in the setup utility  
- **Modern buffered I/O** (`fopen`, `fread`, `fwrite`, etc.) for efficient file handling  
- Directory/file handling using Linux system calls

---
## 📌 Features

NewGit 2.0 provides a Git-like CLI:

```bash
newgit config --global user.name  <UserName>
newgit config --global user.email <UserEmail>
newgit init
newgit add .
newgit commit -m "<Message>"
newgit revert <ID>
```

- `config` – Global User Configuration
  - Stores user name and email in a configuration file.
  - These values are attached to every commit (similar to Git author info).

- `init` – Repository Initialization
  - Creates a hidden `.newgit/` directory in the current folder.
  - Prepares metadata files and folders required by NewGit 2.0.

- `add.` - Staging Area
  - Scans the current directory.
  - Copies all tracked files and folders into a staging area inside .newgit/.
  - Generates a unique ID for this staged snapshot.
> The internal logic of the staging flow is documented using the attached Staging flowchart image.

- `commit -m "<message>"` - Commit System
  - Generates a new commit ID.
  - Copies the staged snapshot into a permanent folder under `.newgit/StagingInfo/<ID>/`.
  - Appends an entry in idInfo.txt (or StageInfo.txt depending on your naming) with:
    - Commit ID
    - Commit message
    - User name & email from config
    - Date & time
    - Summary of changes
  - Counts : 
    - Added lines (+ in green)
    - Removed lines (- in red)

- `revert <ID>` – Restore Previous Snapshot
  - Restores all files and folders from `.newgit/StagingInfo/<ID>/`
     back into the current working directory.
  - Works for any type of file: source code, text, images, executables, etc.
  - Effectively brings your project back to the state it had at that commit ID.


---

## 🧬 Internal Architecture

When newgit init is executed, the following structure is created:

```text
.newgit/
│
├── idInfo.txt          # Initial repository information
├── StagingInfo/          # Stores a folder per commit ID (snapshot)
│   ├── <ID1>/
│   ├── <ID2>/
│   └── ...
└── ... (other metadata files used internally)

```

## 🧪 System Programming Concepts Used : 
- `fork()` and Pipes 
  - `fork()` is used to create child processes for separate tasks such as:
    - File/directory traversal
    - Copying data
    - Generating IDs or handling metadata
  - Pipes provide communication between parent and child processes:
    - For passing generated IDs
    - For sending status / error messages.
    - For coordinating multi-step operations. 
  This demonstrates process management and IPC (Inter-Process Communication) in Linux.


- Threads in `setup.c`: 
  - Thread 1
    - Creates the main `NewGit2.0`directory in the user’s home.
    - Inside it, prepares base files (`InitInfo.txt`, `configUser.txt`, etc.).
  - Thread 2
    - Ensures the newgit executable is built before use.

- Modern Buffered I/O: 
  - All file operations use standard buffered I/O: `fopen`, `fclose`, `fread`, `fwrite`, `fprintf`, etc.
  - A custom buffer (for example, BUFFER_SIZE like 4096 bytes) is used for:
     - Efficient file copying
    - Line-by-line comparison to calculate added / removed lines
 
---
## ⚙️ Installation & Setup
✅ Requirements
- `Linux`
- `gcc`

1. Give execute permission to setup script

```bash
chmod +x setup.sh
```
```bash
./setup.sh
```
The script will:
- Compile `setup.c`.
- Run `setup.out`, which:
   - Starts threads for directory creation and compilation.
   - Sets up the NewGit2.0 environment.
- Compile the main newgit binary.

(If PATH is updated in the script, newgit can be used directly from any directory.)

---

## 🚀 Basic Usage Example

```bash
# 1. Configure user (only once)
newgit config --global user.name  "Tirth Patel"
newgit config --global user.email "tirth@example.com"

# 2. Initialize a repository in your project folder
cd /path/to/your/project
newgit init

# 3. Stage files
newgit add .

# 4. Commit changes
newgit commit -m "Initial commit of my project"

# 5. Make more changes...
#    Then stage and commit again
newgit add .
newgit commit -m "Implemented feature X"

# 6. Revert to a previous snapshot
newgit revert <ID>   # Use any valid commit ID from idInfo.txt

```

---
## 📜 Commit & Staging Logic (High-Level Overview)

> The project includes flowchart images which visually explain the logic.
Below is a textual summary corresponding to those diagrams.

### Staging Logic (`newgit add .`)

![Staging Logic](/Logics/Staging%20.png)

### Commit Logic (`newgit commit -m <Message>`)

![Commit Logic](/Logics/Commit.png)
---
## ✅ Learning Outcomes / What This Project Demonstrates
This project is ideal for a system programming / OS lab because it clearly demonstrates:

- Process creation using `fork()`.
- Inter-process communication using pipes
- Multithreading with POSIX threads (pthread_create, pthread_join, etc.).
- Efficient file handling using modern buffered I/O
- Practical use of directory and file system APIs
- Designing a realistic tool (Git-like) instead of trivial examples

## 🔮 Possible Future Enhancements 
- `newgit log`– Show formatted commit history.
- `newgit status` – Show modified / untracked files.
- `newgit diff <ID1> <ID2>` – Compare two snapshots.
- Encryption of internal metadata for integrity.

---

## 👨‍💻 Author
[@Tirth9978](https://github.com/Tirth9978)  

---
> 📎 Note: All commands and paths assume a Linux environment.
Please refer to the source code and flowchart images for a deeper understanding of the internal logic of staging and commit operations.