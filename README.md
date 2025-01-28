# group22-noise-analysis

## Schedule
Times each day:
   - 8 am
   - 1 pm
   - 7 pm

### Wednesday 28th
Nassau -> Ane

Pearse -> Leanne

Front -> Daniel

### Thursday 28th
Nassau -> Marco

Pearse -> Aryan

Front -> Daniel

### Friday 28th
Nassau -> Aryan

Pearse -> Ane

Front -> Marco

### Sunday 28th
Nassau -> Aryan

Pearse -> Leanne

Front -> Marco



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
