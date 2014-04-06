#!/bin/bash

git branch -D merge 2>/dev/null ; git fetch wx_git && git checkout -b merge && git rebase merge wx_git/master ; while true ; do ./rebase-second.sh ; git add . ; ( git rebase --continue || git rebase --skip ) || true ; ( git branch -D merge 2>/dev/null ) ; done

#eof
