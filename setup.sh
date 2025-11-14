#!/bin/bash

echo "===================================="
echo "      NewGit 2.0 Setup Installer     "
echo "===================================="

# Step 1: Compile setup.c automatically
echo "Checking for setup.c..."
if [ ! -f "./setup.c" ]; then
    echo "❌ Error: setup.c not found!"
    echo "Please place setup.c in this directory."
    exit 1
fi

echo "Compiling setup.c ..."
gcc setup.c -o setup.out -lpthread

# Check compilation success
if [ $? -ne 0 ]; then
    echo "❌ Compilation failed!"
    exit 1
else
    echo "✔ Compilation successful!"
fi

# Step 2: Check if setup.out exists
if [ ! -f "./setup.out" ]; then
    echo "❌ Error: setup.out not created!"
    exit 1
fi

# Step 3: Run the setup.out installer
echo "Running setup.out..."
chmod +x setup.out
./setup.out

echo "------------------------------------"
echo " ⭐ NewGit 2.0 Installation Completed! ⭐"
echo "------------------------------------"
