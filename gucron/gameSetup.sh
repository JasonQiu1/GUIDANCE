#!/bin/bash

GUIDANCEUSER=guidance
PLAYERUSERPREFIX=gup
GUSHPATH="$HOME"/gush

# Run as guidance user!
if [[ "$USER" != "$GUIDANCEUSER" ]]
then
    echo Please switch users to \'"$GUIDANCEUSER"\' before proceeding!
fi

# look through $HOME/games directory for the next game number
GAMENUMBER=1
for gameDir in "$HOME"/games
do
    GAMENUMBER=$(( $GAMENUMBER + 1))
done

read -p "Create game number $GAMENUMBER? (Y/n): " CONFIRM
if [[ $CONFIRM != "Y" ]]
then
    exit
fi

while [ ! "$NUMPLAYERS" ~= "[0-9]+" ]
do
    read -p "How many players?: " NUMPLAYERS
done

# Generate candidate names
declare -a names
while (( ${#names[@]} < $NUMPLAYERS ))
do
    echo Generating candidate ${#names[@]}\'s name.
    jsonStr=$( curl -s "https://www.behindthename.com/api/random.json?usage=ita&number=2&gender=both&randomsurname=yes&key=ja101386669" )

    names[${#names[@]}]="`echo $jsonStr | jq -r '.names[0]'` `echo $jsonStr | jq -r '.names[1]'` `echo $jsonStr | jq -r '.names[2]'`"
    sleep 1
done

# Create game 
GAMEDIR=game"$(GAMENUMBER)"
mkdir "$GAMEDIR"

# Create player home directories and candidate names in them
numUser=1
while (( $numUser <= $NUMPLAYERS ))
do
    if grep -v "^$PLAYERUSERPREFIX$numUser" /etc/passwd
    then
	userdel "$PLAYERNAMEPREFIX$numUser"
    fi
    useradd -s "$GUSHPATH" -b "$GAMEDIR" -m "$PLAYERUSERPREFIX$numUser"
done

