/////////////////////////////////////////////////////////////////////////////
// Name:        wxWidgets_vdm_patch/stdafx.cpp
// Purpose:     patcher for ICL compiler
// Author:      Marian 'VooDooMan' Meravy
// RCS-ID:      $Id$
// Copyright:   (c) Marian 'VooDooMan' Meravy
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// wxWidgets_vdm_patch.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <io.h>
#include <wchar.h>
#include <string>
#include <set>
#include <sstream>
#include <vector>
#include <cassert>
#include <conio.h>
#include <Windows.h>

using namespace std;

bool check_return_value(int rc)
{
    static int iter=0;
    ++iter;

    if(rc<0) {
        static const size_t max=2048;
        char err[max];
        memset(err,0,max);

        strerror_s(err,rc);
        printf("ITER #%i ERROR: %s\n",iter,err);

        return false;
    }

    return true;
}

unsigned reformat(const string& file, bool do_prologue, bool do_patch)
{
    static const char* line1="#if defined(__INTEL_COMPILER)";
    static const char* line1_disabled="#if defined(__INTEL_COMPILER) && 0";
    static const char* line2="#   pragma ivdep";
    static const char* line3="#endif";

    static const char* line_prologue_token="/* token_VDM_prologue */";
    static const char* line_prologue="#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)\n#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)\n#elif !defined(MY_MACRO_PRAGMA_IVDEP)\n#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */\n#endif";

    static const char* inline_pragma="MY_MACRO_PRAGMA_IVDEP \\";

    static const size_t length=4096; // ugly limitation to 4096!
    char tmp_buf[length+16];

    unsigned cnt=0;
    bool changed=false;

    vector<string> scrollback;

    FILE* in=fopen(file.c_str(),"r");
    if(!in) {
        check_return_value(errno);
        return cnt;
    }

    bool last_char_was_backslash=false;
    size_t ln=0;

#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
    while(in && !feof(in)) {
again:
        char buf[length+16];
        if(fgets(buf,length,in)==NULL) {
            if(feof(in)) {
                break;
            }
            goto again;
        }

        buf[length]='\0';
        {
            size_t len=strlen(buf);
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
            while(len>0 && ('\r'==buf[len-1] || '\n'==buf[len-1])) {
                buf[len-1]='\0';
                --len;
            }
        }

        ++ln;
        scrollback.push_back(buf);

        if(1==ln && strcmp(buf,line_prologue_token)==0) {
            size_t cnt=0;

            // seek for an empty line as ending of the prologue
            char buf2[length+16];
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
            do {
                scrollback.clear();
                scrollback.push_back(buf);

                buf2[0]='x';
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
                while('\r'!=buf2[0] && '\n'!=buf2[0]) {
                    buf2[0]='x';
                    fgets(buf2,length,in);
                }
                ++cnt;
            } while(strcmp(buf2,line_prologue_token)==0); // might be broken, i.e. twice or more prologue occurence

            if(1!=cnt) {
                changed=true;
            }

            if(!do_patch) {
                changed=true;
                scrollback.clear();
                continue;
            }
        }

        if(do_patch && do_prologue && 1==ln) {
            sprintf(tmp_buf,"%s\n%s\n",line_prologue_token,line_prologue);
            string save=scrollback.back();
            scrollback.clear();
            scrollback.push_back(tmp_buf);
            if(save.compare(line_prologue_token)) {
                scrollback.push_back(save);
                changed=true;
            }
        }

        if(!do_patch && do_prologue && 1==ln && strcmp(buf,line_prologue_token)==0) {
            scrollback.push_back(buf);
            changed=true;
        }

        string line;
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
        for(size_t i1=0; i1<strlen(buf); ++i1) {
            if('('!=buf[i1] && '{'!=buf[i1]) {
                if(' '!=buf[i1] && '\t'!=buf[i1])
                    line.append(1,buf[i1]);
            } else
                break;
        }

#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
        while(line.length()>0 && line.length()-1!=0 && line[line.length()-1]==' ') {
            line.erase(line.length()-1,1);
        }

        if(line.rfind("//")!=string::npos) {
            line.erase(line.rfind("//"));
        }

        {
            string::size_type startpos;
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
            while((startpos=line.find("/*"))!=string::npos) {
                const string::size_type endpos=line.find("*/");
                if(endpos==string::npos) {
                    // multi-line comment, delete until the end
                    line.erase(startpos);
                    break;
                }
                line.erase(startpos,endpos-startpos+2+1); // 2 is length of "*/"
            }
        }

#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
        while(line.length()>0 && (' '==line[0] || '\t'==line[0] || '\r'==line[0] || '\n'==line[0])) {
            line.erase(line.length()-1,1);
        }

        bool reformat=false;

        // for
        if(!line.compare("for")) {
            reformat=true;
        }

        // while
        if(!line.compare("while")) {
            reformat=true;
        }

        // do
        if(!line.compare("do")) {
            reformat=true;
        }

        if(reformat && scrollback.size()>3) {
            vector<string>::const_iterator i1=scrollback.end();
            --i1;
            if(
                (
                    (*(i1-1)).compare(line3)==0 &&
                    (*(i1-2)).compare(line2)==0 &&
                    ( (*(i1-3)).compare(line1)==0 || (*(i1-3)).compare(line1_disabled)==0 )
                )
            ) {
                reformat=!do_patch;
            } else {
                reformat=do_patch;
            }
        }

        if(reformat) {
            vector<string>::reverse_iterator i1=scrollback.rbegin();
            if(i1!=scrollback.rend()) {
                ++i1;
                if(do_patch && !(*i1).compare(inline_pragma))
                    reformat=false;
            }
            int i3=0;
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
            for(int i2=0; i2<3; ++i2) {
                string ln=*i1;
                ++i1;
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
                while(ln.length()!=0 && ( ln[ln.length()-1]=='\r' || ln[ln.length()-1]=='\n') )
                    ln.erase(ln.length()-1,1);
                const char* cmp;
                switch(i2) {
                    case 0:
                        cmp=line3;
                        break;
                    case 1:
                        cmp=line2;
                        break;
                    case 2:
                        cmp=line1;
                        break;
                }
                if(!ln.compare(cmp)) {
                    ++i3;
                } else if(2==i2 && !ln.compare(line1_disabled)) {
                    ++i3;
                } else {
                    break;
                }
            }
            if(3==i3) {
                reformat=!do_patch;
            }

            if(reformat) {
                string save=scrollback.back();
                if(do_patch) {
                    if(last_char_was_backslash) {
                        sprintf(tmp_buf,"%s",inline_pragma);
                        scrollback.pop_back();
                        scrollback.push_back(tmp_buf);
                        scrollback.push_back(save);
                    } else {
                        sprintf(tmp_buf,"%s\n%s\n%s",line1, line2, line3);
                        scrollback.pop_back();
                        scrollback.push_back(tmp_buf);
                        scrollback.push_back(save);
                    }
                } else {
                    if(last_char_was_backslash) {
                        scrollback.pop_back();
                        scrollback.pop_back();
                        scrollback.push_back(save);
                    } else {
                        scrollback.pop_back();
                        scrollback.pop_back();
                        scrollback.pop_back();
                        scrollback.pop_back();
                        scrollback.push_back(save);
                    }
                }
                if(!changed)
                    ++cnt;
            }
        }

        {
            size_t i1=strlen(buf);
            if(i1>0 && '\\'==buf[i1-1]) {
                last_char_was_backslash=true;
            } else {
                last_char_was_backslash=false;
            }
        }
    }

    fclose(in);

    if(changed || 0!=cnt) {
        FILE* in=fopen(file.c_str(),"w+b");
        if(!in) {
            check_return_value(errno);
            return cnt;
        }

#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
        for(vector<string>::const_iterator i1=scrollback.begin(); i1!=scrollback.end(); ++i1) {
            fprintf(in,"%s\n",(*i1).c_str());
        }

        fclose(in);
    }

    if(changed)
        ++cnt;

    if(cnt) {
        printf("Processed file: %s\n",file.c_str());
    }

    return cnt;
}

static set<string> ext;
static set<string> ext_prologue;

unsigned directory_recurse(const string& base, const string& directory, const string& path, bool do_patch)
{
    unsigned cnt=0;

    string current=directory;

    _finddata_t rc;
    intptr_t handle=_findfirst(current.c_str(),&rc);
    if(!check_return_value(errno))
        return cnt;

#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
    for(;;) {
        string name=rc.name;

        bool sub_dir=false;

        if(!name.compare(".") || !name.compare("..")) {
            goto next_entry;
        } else {
            sub_dir=true;
        }

#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
        for(set<string>::iterator i1=ext.begin(); i1!=ext.end(); ++i1) {
            string::size_type pos=name.rfind(*i1);
            if(pos!=string::npos && pos==name.length()-(*i1).length()) {
                string n=directory;
                if(n.rfind("*")==n.length()-1)
                    n.erase(n.length()-1,1);
                n+=name;
                cnt+=reformat(n,(ext_prologue.find(*i1)!=ext_prologue.end()),do_patch);

                break;
            }
        }

        if(sub_dir && (rc.attrib & _A_SUBDIR)) {
            string dir_name=directory;
            dir_name.erase(dir_name.length()-1,1);
            dir_name+=name;
            dir_name.append("\\*");
            cnt+=directory_recurse(base,dir_name,name,do_patch);
        }

next_entry:
        if(handle<0)
            break;
        if(_findnext(handle,&rc)!=0) {
            break;
        }
        if(!check_return_value(errno)) {
            break;
        }
    }

    _findclose(handle);

    return cnt;
}

int _tmain(int argc, _TCHAR* argv[])
{
    bool do_patch;
    bool do_nowait=false;

    {
        bool usage=false;

        if(argc<2) {
            usage=true;
        } else {
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
            for(size_t i1=1; i1<argc; ++i1) {
                if(strcmp(argv[i1],"-p")==0) {
                    do_patch=true;
                } else if(strcmp(argv[i1],"-u")==0) {
                    do_patch=false;
                } else if(strcmp(argv[i1],"--no-wait")==0) {
                    do_nowait=true;
                } else {
                    usage=true;
                    break;
                }
            }
        }

        if(usage) {
            fprintf(stderr,"Usage: switch '-p' patches, switch '-u' unpatches.\nPress any key to exit.\n");
            getch();
            return 0;
        }
    }

    ext.insert(".h");
    ext.insert(".hh");
    ext.insert(".hpp");
    ext.insert(".hxx");

    ext_prologue=ext;

    ext.insert(".c");
    ext.insert(".cc");
    ext.insert(".cpp");
    ext.insert(".cxx");
    ext.insert(".C");

    TCHAR dir[MAX_PATH];
    if(!::GetCurrentDirectory(sizeof(dir) - 1, dir)) {
        return 1;
    }
    string d=dir;
    d.append("\\..\\*");
    unsigned cnt=directory_recurse(".",d,d,do_patch);

    if(!do_nowait) {
        printf("%u occurences processed. Press any key to exit.\n",cnt);
        getch();
    }

    return 0;
}

