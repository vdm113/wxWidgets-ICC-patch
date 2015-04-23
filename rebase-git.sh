#!/bin/bash

if [ "x$1" != "xInternal" ] ; then
  cp -af ./vdm/root_dir/*.sh ..
  ../rebase-git.sh Internal
  exit 0
fi

cd ../wxWidgets_vanilla_trunk/
rm -rf * .??*
#git clone --mirror https://github.com/wxWidgets/wxWidgets . || exit 1
git pull --mirror https://github.com/wxWidgets/wxWidgets || exit 1
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
for i in `find build/msw/ -type f -maxdepth 1` ; do ( git rm -f "${i}" 2>/dev/null ) ; ( rm -f "${i}" 2>/dev/null ) ; done
git commit -a --signoff -m 'sync with upstream (GIT); patched for ICC; removed files we do not support'
cp -rf ./vdm/build .
git add build/msw/
git commit -a --signoff -m 'restore our project files'
mv -f README.md README_orig_wx.md
cp -f ./vdm/root_dir/* ./vdm/root_dir/.??* .
git add README.md
export DATE=`date`
git commit -a --signoff -m "restore our support files: ${DATE}"
detached_head=`git rev-parse HEAD`
git checkout master
git merge ${detached_head}
find . -name '*~HEAD*' -exec rm -f '{}' \; -exec git commit -a --signoff -m 'remove transient file' \;

#eof
