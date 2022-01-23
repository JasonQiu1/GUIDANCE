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
fi

GUIDANCEHOME=/home/"$GUIDANCEUSER"
if [ ! -d "$GUIDANCEHOME" ]
then
    echo Failed to find \'"$GUIDANCEUSER"\' home.
    exit 254
fi

if [ ! -d "$GUIDANCEHOME"/games ]
then
    mkdir "$GUIDANCEHOME"/games
fi

make -C "$GUSHDIR"
if [[ -f "$GUSHDIR"/gush ]]
then
    if ! cp "$GUSHDIR"/gush "$GUIDANCEHOME"
    then
        echo Failed to copy gush to "$GUIDANCEHOME"! Try running with higher privileges!
        exit 253
    fi
else
    echo Failed to make gush!
    exit 252
fi

echo Done!
