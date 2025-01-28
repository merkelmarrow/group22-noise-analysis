# group22-noise-analysis

## Git workflow
1. Clone the repo:
   ```bash
   git clone https://github.com/merkelmarrow/group22-noise-analysis
   ```
2. Create feature branch for a given task:
   ```bash
   git checkout -b <branch-name>
   ```
3. Commit changes often:
   ```bash
   git add .
   git commit -m "Message describing changes"
   git push origin <branch name>
   ```
4. Merge changes into repo:
   - Push to main for small changes.
   - Create a PR before merging to main when a version is ready). 
   - Delete branch
   ```bash
   git branch -d <branch-name>
   git push origin --delete <branch-name>
   ```


Branches:
- main (commit raw data to main)
- scripts/<description>-<name> (commit changes to scripts here, push/PR to main when version is working)
- data/<description>-<name> (commit changes to group CSV here, push to main when done)
- report/<description>-<name> (commit changes to report, push/PR to main when finishing)