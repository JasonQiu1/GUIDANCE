# GUIDANCE

A multiplayer turn-based terminal daily planning game. 

Play with your friends as GUIDEs, planning out your CANDIDATEs' schedules each day to be the first one whose CANDIDATE finishes the main quest!

Host it on a POSIX server and play it with friends! Also modify the game however you want in `main.menu`, `event.c`, and `gucron` to add new content!

## Overview
`gush` is the frontend interface for the game, which allows players to take certain actions in the day and are written to their data files.

`gucron` is the backend for the game, which is a cronjob that activates every day by default to process all actions taken and handle any conflicts among candidates.

## Gush Build Prerequisites
- POSIX system (for `make`, `xxd`, `sed`)
- ncurses library
- C compiler

Edit the variables at the top of the Makefile to suit your preferences or system before running `make`.

The binaries will be created in the current directory.

## Gucron Scripts Prerequisites
- Bash
- POSIX system (for `grep`, `jq`, `useradd`, `userdel`, etc.)

Run the one-time `firstTimeSetup.sh` script with elevated permissions to setup the `guidance` user.

To setup a new game (you can only have one running at a time for now), run `gameSetup.sh` with elevated permissions, which will create player users `gup0`, `gup1`, `gup2`, etc. with default password at the top of the script.

** TODO: Add the gucron script to the `guidance` user's crontab **

After that, just tell let your players choose which player slot they want to start with, and they will be prompted to change their passwords after, and the game begins!
