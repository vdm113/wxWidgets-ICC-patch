#!/bin/bash

cd ../wxWidgets_vanilla_trunk/
git pull https://github.com/wxWidgets/wxWidgets
cd -
git branch -D merge 2>/dev/null ; git fetch wx_git && git checkout -b merge && git rebase merge wx_git/master ; while true ; do ./rebase-second.sh ; git add . ; git rebase --continue || git rebase --skip ; test -d ".git/rebase-merge" -o -d ".git/rebase-apply" || break ; done
git branch -D merge 2>/dev/null
cp -rf ../wxWidgets_vanilla_trunk/* .
cd ../wxWidgets_vanilla_trunk/
dirs=`find . -mindepth 1 -type d | grep -v '^\./\.git/'`
files=`find . -mindepth 1 -type f | grep -v '^\./\.git/'`
cd -
for i in ${dirs} ; do mkdir ${i} 2>/dev/null ; done
for i in ${files} ; do cp -f "../wxWidgets_vanilla_trunk/${i}" "./${i}" ; done
cd wxWidgets_vdm_patch && ./X64/Release/wxWidgets_vdm_patch.exe -p --no-wait && cd .. && rm -f build/msw/*.bcc build/msw/*.dsp build/msw/*.dsw build/msw/*.gcc build/msw/wx_vc10* build/msw/wx_vc7* build/msw/wx_vc8* build/msw/wx_vc9* build/msw/wx_vc10* build/msw/wx_vc11* build/msw/wx_vc12* build/msw/*.vc build/msw/*.wat build/msw/*.props
git commit -a --signoff -m 'sync with upstream; patched for ICC; removed files we do not support'
cp -rf ../build .
git commit -a --signoff -m 'restore our project files'
detached_head=`git rev-parse HEAD`
git checkout master
git merge ${detached_head}

#eof
