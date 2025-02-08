# 3E3 Group 22 - Sound Level Analysis

## Weekly Schedule

| Day       | Time  | Nassau | Pearse | Front  |
|-----------|-------|--------|--------|--------|
| Wed 29th  | 8 AM  | Ane    | Leann  | Daniel |
|           | 1 PM  | Ane    | Leann  | Daniel |
|           | 7 PM  | Ane    | Leann  | Daniel |
| Thu 30th  | 8 AM  | Marco  | Aryan  | Daniel |
|           | 1 PM  | Marco  | Aryan  | Daniel |
|           | 7 PM  | Marco  | Aryan  | Daniel |
| Fri 31st  | 8 AM  | Aryan  | Ane    | Marco  |
|           | 1 PM  | Aryan  | Ane    | Marco  |
|           | 7 PM  | Aryan  | Ane    | Marco  |
| Sun 2nd   | 8 AM  | Aryan  | Leann  | Marco  |
|           | 1 PM  | Aryan  | Leann  | Marco  |
|           | 7 PM  | Aryan  | Leann  | Marco  |

## Data Preprocessor
An application to preprocess time/gain .csv data is located in uni_cpp_sound-data-preprocessor_2025

### Features

- Automatically processes all CSV files in the working directory
- Groups gain data into 5-second buckets, and calculates averages up to 13 minutes
- Supports relative timestamps and ISO 8601 date time formats.
- Converts absolute timestamps to relative from start of measurements.

### Installation

If you use 64-bit windows, an executable is already available in uni_cpp_sound-data-preprocessor_2025/executables. Use win64-data-preprocessor-2.1.exe for latest version.

Otherwise, you have to build the program with CMake.

### Build with CMake
You need:
- C++20 compiler
- CMake 3.8+ configured with compiler
- Git

```bash
git clone https://github.com/merkelmarrow/group22-noise-analysis/
cd group22-noise-analysis/uni_cpp_sound-data-preprocessor_2025
mkdir build && cd build
cmake ..
cmake --build .
```

### Usage

Place executable in directory with CSV files and run it. The program processes all CSVs, creating new files with "_preprocessed" suffix.

### Dependencies
- csv-parser v2.2.1 (auto-fetched by CMake)

## Git workflow
1. Clone the repo:
   ```bash
   git clone https://github.com/merkelmarrow/group22-noise-analysis
   ```
2. Create branch for a given task:
   ```bash
   git pull origin main
   git checkout -b <branch-name>
   ```
3. Commit changes often:
   ```bash
   git add .
   git commit -m "Message describing changes"
   git push origin <branch-name>
   ```
4. Merge changes into main:
   - merge to main for small or low risk changes.
   ```bash
   git checkout main
   git pull origin main
   git merge <branch-name>
   git push origin main
   ```
   - Or create a PR on GitHub before merging to main when a version is ready.
   - After merging delete branch locally and on remote.
   ```bash
   git branch -d <branch-name>
   git push origin --delete <branch-name>
   ```


Branches:
- main (commit raw data to main)
- scripts/\<description\>-\<name\> (commit changes to scripts here, push/PR to main when version is working)
- data/\<description\>-\<name\> (commit changes to group CSV here, push to main when done)
- report/\<description\>-\<name\> (commit changes to report, push/PR to main when finishing)
