#!/bin/bash

if [ "x$1" != "xInternal" ] ; then
  cp -a ./rebase-git.sh ..
  ../rebase-git.sh Internal
  exit 0
fi

cd ../wxWidgets_vanilla_trunk/
git pull https://github.com/wxWidgets/wxWidgets
cd -
git branch -D merge 2>/dev/null ; git fetch wx_git && git checkout -b merge && git rebase merge wx_git/master ; while true ; do ( for i in `grep -r '^<<<<<<< ' . | sort | uniq | awk 'BEGIN { FS=":"; } { print $1; }'` ; do echo "$i" ; awk 'BEGIN { ignore=0; } /^<<<<<<< / { ignore=1; next; } /^=======$/ { ignore=0; next; } /^>>>>>>> / { ignore=0; next; } { if(0==ignore) print $0; }' "$i" >"${i}.tmp"; mv -f "${i}.tmp" "$i"; done ) ; git add . ; git rebase --continue || git rebase --skip ; test -d ".git/rebase-merge" -o -d ".git/rebase-apply" || break ; done
git branch -D merge 2>/dev/null
cp -rf ../wxWidgets_vanilla_trunk/* .
cd ../wxWidgets_vanilla_trunk/
dirs=`find . -mindepth 1 -type d | grep -v '^\./\.git/'`
files=`find . -mindepth 1 -type f | grep -v '^\./\.git/'`
cd -
for i in ${dirs} ; do mkdir ${i} 2>/dev/null ; done
for i in ${files} ; do cp -f "../wxWidgets_vanilla_trunk/${i}" "./${i}" ; done
cd wxWidgets_vdm_patch && ./X64/Release/wxWidgets_vdm_patch.exe -p --no-wait && cd ..
for i in `ls build/msw/ | grep -v vdm` ; do ( git rm -f build/msw/${i} >/dev/null ) ; ( rm -f build/msw/${i} >/dev/null ) ; done
git commit -a --signoff -m 'sync with upstream  (GIT); patched for ICC; removed files we do not support'
cp -rf ../build .
git commit -a --signoff -m 'restore our project files'
mv -f README.md README_orig_wx.md
cp -f ../README.md .
git commit -a --signoff -m 'restore our README.md for Github'
detached_head=`git rev-parse HEAD`
git checkout master
git merge ${detached_head}

#eof
