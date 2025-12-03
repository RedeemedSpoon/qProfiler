#!/bin/bash

set -e
PIPE_DIR=$(mktemp -d)
REPO_URL="https://github.com/RedeemedSpoon/qProfiler.git"

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m'

info() { echo -e "${YELLOW}[INFO]${NC} $1"; }
success() { echo -e "${GREEN}[SUCCESS]${NC} $1"; }
error() { echo -e "${RED}[ERROR]${NC} $1"; }

cleanup() {
    info "Cleaning up temporary files..."
    rm -rf "$PIPE_DIR"
}

trap cleanup EXIT

info "Checking dependencies..."
for cmd in git cmake make g++; do
    if ! command -v $cmd &> /dev/null; then
        error "$cmd could not be found. Please install it first."
        exit 1
    fi
done

info "Cloning qProfiler..."
git clone --depth 1 "$REPO_URL" "$PIPE_DIR"
cd "$PIPE_DIR"

info "Building project..."
mkdir -p build && cd build
cmake ..
make -j$(nproc) 

if [ -f "./bin/qprofiler" ]; then
    info "Installing binary to /usr/local/bin..."
    if sudo mv ./bin/qprofiler /usr/local/bin/qprofiler; then
        success "Binary moved successfully."
    else
        error "Failed to move binary. Do you have sudo privileges?"
        exit 1
    fi
else
    error "Build failed. Binary './bin/qprofiler' not found."
    exit 1
fi

info "Verifying installation..."
hash -r 

if command -v qprofiler &> /dev/null; then
    success "qProfiler has been installed successfully!"
    echo -e "You can now run it using: ${GREEN}qprofiler${NC}"
else
    error "Installation verification failed."
    exit 1
fi
