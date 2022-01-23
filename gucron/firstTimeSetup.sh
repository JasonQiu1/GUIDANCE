#!/bin/bash

GUSHDIR=../gush
GUIDANCEUSER=guidance

if ! grep -q "^$GUIDANCEUSER" /etc/passwd
then
    if ! useradd -m guidance 
    then
        echo Failed to create \'"$GUIDANCEUSER"\' user. Try rerunning the script with higher privileges.
        exit 255
    fi
    chown guidance:guidance "$GUIDANCEHOME"
fi

GUIDANCEHOME=/home/"$GUIDANCEUSER"
if [ ! -d "$GUIDANCEHOME" ]
then
    echo Failed to find \'"$GUIDANCEUSER"\' home.
    exit 254
fi

if [ ! -d "$GUIDANCEHOME"/games ]
then
    runuser -l "$GUIDANCEUSER" -C "mkdir $GUIDANCEHOME/games"
fi

if ! cp "$GUSHDIR"/gush "$GUIDANCEHOME"
then
	echo Failed to copy gush to "$GUIDANCEHOME"! Try running with higher privileges!
	echo or 'gush' binary not found! Please run 'make' in the gush directory first!
	exit 253
fi

# Create skeleton directory for player users
mkdir -p "$GUIDANCEHOME/skel/.local/state/guidance"
mkdir -p "$GUIDANCEHOME/skel/.local/share/guidance"
touch "$GUIDANCEHOME/skel/.local/share/guidance/data"

echo Done!
