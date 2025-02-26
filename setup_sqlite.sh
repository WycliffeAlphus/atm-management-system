#!/bin/bash
set -e  # Stop script on error

SQLITE_VERSION="3490100"
SQLITE_URL="https://www.sqlite.org/2025/sqlite-amalgamation-3490100.zip"
SQLITE_DIR="sqlite-amalgamation-${SQLITE_VERSION}"

echo "Downloading SQLite..."
wget -q $SQLITE_URL -O sqlite.zip || { echo "Failed to download SQLite."; exit 1; }

echo "Verifying downloaded file..."
if [ ! -f sqlite.zip ]; then
    echo "Download failed or file not found."
    exit 1
fi

echo "Unzipping SQLite..."
unzip -q sqlite.zip || { echo "Failed to unzip."; exit 1; }

echo "Moving files..."
mv $SQLITE_DIR/sqlite3.c src/ || { echo "Failed to move sqlite3.c"; exit 1; }
mv $SQLITE_DIR/sqlite3.h src/ || { echo "Failed to move sqlite3.h"; exit 1; }

echo "Cleaning up..."
rm -rf sqlite.zip $SQLITE_DIR

echo "SQLite setup complete!"
