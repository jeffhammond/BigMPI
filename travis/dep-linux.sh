#! /bin/sh

# Exit on error
set -ev

# Update package list
sudo apt-get update -qq

# Install other deps
sudo apt-get install -qq -y cmake mpich
