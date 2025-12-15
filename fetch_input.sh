#!/bin/bash
DAY=$(date +%d)
YEAR=$(date +%Y)
AOC_SESSION_FILE="/home/ghassemiramtin/Projects/AdventOfCode/.aoc_session"
# Fetch input arguments --day and --year
while [[ $# -gt 0 ]]; do
	case $1 in
		-d|--day)
			DAY="$2"
			shift # past argument
			shift # past value
			;;
		-y|--year)
			YEAR="$2"
			shift #past argument
			shift #past value
			;;
		-*|--*)
			echo "Unknown parameter $1"
			exit 1
			;;
	esac
done


OUTPUT_FILE="input$DAY-$YEAR.txt"
INPUT_URL="https://adventofcode.com/${YEAR}/day/${DAY}/input"

if [ -f $AOC_SESSION_FILE ]; then
	AOC_SESSION=$(cat $AOC_SESSION_FILE)
else
	echo "Error: AOC SESSION cookie not set"
	exit 1
fi

echo "Attempting to fetch input for $DAY-$YEAR..."
echo "Saving to: $OUTPUT_FILE"

touch $OUTPUT_FILE

curl -s --cookie "session=$AOC_SESSION" $INPUT_URL -o $OUTPUT_FILE

if [ $? -eq 0 ] && [ -s $OUTPUT_FILE ]; then
	echo "Successfully downloaded input to $OTUPUT_FILE"
else
	echo "Error: Failed to download input. Check your session cookie and the date"
	rm -f $OUTPUT_FILE
fi

