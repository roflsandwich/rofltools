#!/bin/bash
if [ -z "$1" ]
        then
                echo "Usage: ${0} subdomainFile.txt"
                exit
        fi
for URL in $(cat $1)
        do
                URL="https://"$URL
                STATUS=$(curl --connect-timeout 5 -k -s $URL | grep -o -i "<title>.*</title>" | sed "s/<[^>]*>/\n/g" | tr -d '\n' )
                if [ -z "$STATUS" ]
                then
                        continue
                else
                        echo "[ $URL ] - [ $STATUS ]"
                fi
done
