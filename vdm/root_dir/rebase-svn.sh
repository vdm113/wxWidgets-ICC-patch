#!/bin/bash

if [ "x$1" != "xInternal" ] ; then
  cp -af ./vdm/root_dir/*.sh ..
  ../rebase-git.sh Internal
  exit 0
fi

cd ../wxWidgets_vanilla_trunk-svn/
git svn fetch ; git svn rebase
cd -
git branch -D merge 2>/dev/null ; git checkout -b merge && git pull wx_svn ; for i in `grep -r '^<<<<<<< ' . | sort | uniq | awk 'BEGIN { FS=":"; } { print $1; }'` ; do echo "$i" ; awk 'BEGIN { ignore=0; } /^<<<<<<< / { ignore=1; next; } /^=======$/ { ignore=0; next; } /^>>>>>>> / { ignore=0; next; } { if(0==ignore) print $0; }' "$i" >"${i}.tmp"; mv -f "${i}.tmp" "$i"; done
git branch -D merge 2>/dev/null
cp -rf ../wxWidgets_vanilla_trunk-svn/* .
cd ../wxWidgets_vanilla_trunk-svn/
dirs=`find . -mindepth 1 -type d | grep -v '^\./\.git/'`
files=`find . -mindepth 1 -type f | grep -v '^\./\.git/'`
cd -
for i in ${dirs} ; do mkdir ${i} 2>/dev/null ; done
for i in ${files} ; do cp -f "../wxWidgets_vanilla_trunk-svn/${i}" "./${i}" ; done
cd wxWidgets_vdm_patch && ./X64/Release/wxWidgets_vdm_patch.exe -p --no-wait && cd ..
for i in `find build/msw/ -type f -maxdepth 1` ; do ( git rm -f "${i}" 2>/dev/null ) ; ( rm -f "${i}" 2>/dev/null ) ; done
git commit -a --signoff -m 'sync with upstream (SVN); patched for ICC; removed files we do not support'
cp -rf ./vdm/build .
git add build/msw/
git commit -a --signoff -m 'restore our project files'
mv -f README.md README_orig_wx.md
cp -f ./vdm/root_dir/* ./vdm/root_dir/.??* .
git add README.md
git commit -a --signoff -m 'restore our git and script files'
git checkout master
git merge merge
git branch -D merge

#eof
