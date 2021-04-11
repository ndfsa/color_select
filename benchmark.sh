#!/bin/dash

[ $# -eq 0 ] && return 1
hyperfine \
    --warmup 5 \
    --show-output \
    --parameter-list file $(find $HOME/Pictures/Wallpapers/* | sed -z 's/\n/,/g' | sed 's/,$//') \
    "$HOME/bin/color_select {file}" > $HOME/Documents/$1
