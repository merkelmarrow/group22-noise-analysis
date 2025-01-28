# group22-noise-analysis

# Weekly Schedule

| Day       | Time  | Nassau | Pearse | Front  |
|-----------|-------|--------|--------|--------|
| Wed 29th  | 8 AM  | Ane    | Leanne | Daniel |
|           | 1 PM  | Ane    | Leanne | Daniel |
|           | 7 PM  | Ane    | Leanne | Daniel |
| Thu 30th  | 8 AM  | Marco  | Aryan  | Daniel |
|           | 1 PM  | Marco  | Aryan  | Daniel |
|           | 7 PM  | Marco  | Aryan  | Daniel |
| Fri 31st  | 8 AM  | Aryan  | Ane    | Marco  |
|           | 1 PM  | Aryan  | Ane    | Marco  |
|           | 7 PM  | Aryan  | Ane    | Marco  |
| Sun 2nd   | 8 AM  | Aryan  | Leanne | Marco  |
|           | 1 PM  | Aryan  | Leanne | Marco  |
|           | 7 PM  | Aryan  | Leanne | Marco  |


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
