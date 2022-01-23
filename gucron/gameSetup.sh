#!/bin/bash

GUIDANCEUSER=guidance
PLAYERUSERPREFIX=gup
DEFAULTPASS=GUIDE
GUIDANCEHOME=$(eval echo ~$GUIDANCEUSER)
GUSHPATH="$GUIDANCEHOME"/gush

# look through $GUIDANCEHOME/games directory for the next game number
GAMENUMBER=0
for prevGame in "$GUIDANCEHOME"/games/*
do
    GAMENUMBER=$(( $GAMENUMBER + 1))
done

read -p "Create game number $GAMENUMBER? (Y/n): " CONFIRM
if [[ $CONFIRM != "Y" ]]
then
    exit 0
fi

while ! [[ "$NUMPLAYERS" =~ [0-9]+ ]]
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
GAMEDIR="$GUIDANCEHOME/games/game$GAMENUMBER"
runuser -l "$GUIDANCEUSER" -c "mkdir $GAMEDIR"

# Create player home directories and record their name with candidate name
runuser -l "$GUIDANCEUSER" -c "touch $GAMEDIR/players"
echo "[PlayerInfo]" >> "$GAMEDIR"/players

numUser=0
while (( $numUser < $NUMPLAYERS ))
do
    PLAYERUSERNAME="$PLAYERUSERPREFIX$numUser"

    
    if grep -q "^$PLAYERUSERNAME" /etc/passwd
    then
        userdel "$PLAYERUSERNAME"
    fi
    useradd -s "$GUSHPATH" -k "$GUIDANCEHOME/skel" -b "$GAMEDIR" -m -g "$GUIDANCEUSER" "$PLAYERUSERNAME"
    echo "$PLAYERUSERNAME:$DEFAULTPASS" | chpasswd
    passwd -e "$PLAYERUSERNAME"

    # Update player data with candidate info
    echo "$PLAYERUSERNAME = ${names[$numUser]}" >> "$GAMEDIR"/players

    PLAYERDATAFILE="$GAMEDIR/$PLAYERUSERNAME/.local/share/guidance/data"
    echo "[CAND]" >> "$PLAYERDATAFILE"
    echo "Name = ${names[$numUser]}" >> "$PLAYERDATAFILE"

    numUser=$(( $numUser + 1 ))
done

echo Done!
