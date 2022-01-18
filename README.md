# GUIDANCE

A multiplayer turn-based terminal daily planning game. 

Play with your friends as GUIDEs, planning out your CANDIDATEs' schedules each day to be the first one whose CANDIDATE finishes the main quest!

Host it on a POSIX server and play it with friends! Also modify the game however you want in `main.menu`, `event.c`, and `gucron` to add new content!

## Overview
`gush` is the frontend interface for the game, which allows players to take certain actions in the day and are written to their data files.

`gucron` is the backend for the game, which is a cronjob that activates every day by default to process all actions taken and handle any conflicts among candidates.

## Build Prerequisites
- POSIX system (for `make`, `xxd`, and `sed`)
- ncurses library
- C compiler

Edit the variables at the top of the Makefile to suit your preferences or system before running `make`.

The binaries will be created in the current directory.
