#!/bin/bash

echo "===================================="
echo "      NewGit 2.0 Setup Installer     "
echo "===================================="

# Check if setup.out exists
if [ ! -f "./setup.out" ]; then
    echo "Error: setup.out not found!"
    echo "Please compile the installer first:"
    echo "   gcc setup.c -o setup.out -lpthread"
    exit 1
fi

# Run the setup.out installer
echo "Running setup.out..."
chmod +x setup.out
./setup.out

echo "------------------------------------"
echo " NewGit 2.0 Installation Completed! "
echo "------------------------------------"
