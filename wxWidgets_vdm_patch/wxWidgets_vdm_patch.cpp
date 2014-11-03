/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

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
    static const char* line1="#if defined(__INTEL_COMPILER) && 1 // VDM auto patch";
    static const char* line1_disabled="#if defined(__INTEL_COMPILER) && 0 // VDM auto patch";
    static const vector<const char*> line2={ "#   pragma ivdep", "#   pragma swp", "#   pragma unroll", "#endif" };

    static const char* line_prologue_token="/* token_VDM_prologue */";
    static const char* line_prologue="#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)\n#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)\n#elif !defined(VDM_MACRO_PRAGMA_IVDEP)\n#   define VDM_MACRO_PRAGMA_IVDEP\n#endif";

    static const char* inline_pragma="VDM_MACRO_PRAGMA_IVDEP \\";

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

    const vector<pair<string,string> > strip={ { "/*", "*/" }, { "'", "'" }, { "\"", "\""} };

    size_t do_cnt=0;
    size_t do_braces=0;

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
            while(len>0 && ('\r'==buf[len-1] || '\n'==buf[len-1])) {
                buf[len-1]='\0';
                --len;
            }
        }

        if(!do_patch && !strcmp(buf,inline_pragma)) {
            changed=true;
            continue;
        }

        ++ln;
        scrollback.push_back(buf);

        if(1==ln && strcmp(buf,line_prologue_token)==0) {
            size_t cnt=0;

            // seek for an empty line as ending of the prologue
            char buf2[length+16];
#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif
            do {
                scrollback.clear();
                scrollback.push_back(buf);

                buf2[0]='x';
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

        string line=buf;

        while(line.length()>0 && (' '==line[0] || '\t'==line[0] || '\r'==line[0] || '\n'==line[0])) {
            line.erase(0,1);
        }

        if(line.rfind("//")!=string::npos) {
            line.erase(line.rfind("//"));
        }

        if(line.rfind("\\\\")!=string::npos) {
            line.erase(line.rfind("\\\\"));
        }

#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif
        for(auto& i1 : strip) {
            string::size_type startpos;
            while((startpos=line.find(i1.first))!=string::npos) {
                const string::size_type endpos=line.find(i1.second);
                if(endpos==string::npos) {
                    // multi-line comment, delete until the end
                    line.erase(startpos);
                    break;
                }
                line.erase(startpos,endpos-startpos+2+1); // 2 is length of "*/"
            }
        }

        bool end_do=false;

        if(0!=do_braces) {
#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif
            for(size_t i1=0; i1<line.length(); ++i1) {
                if('{'==line[i1])
                    ++do_braces;
                if('}'==line[i1]) {
                    if(--do_braces==0)
                        end_do=true;
                }
            }
        }

#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif
        for(size_t i1=0; i1<line.length(); ++i1)

        {
            string line_new;
#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif
            for(size_t i1=0; i1<line.length(); ++i1) {
                if('('!=line[i1] && '{'!=line[i1] && ' '!=line[i1] && '\t'!=line[i1]) {
                    line_new.append(1,line[i1]);
                } else {
                    break;
                }
            }
            line=line_new;
        }

        bool reformat=false;

        // for
        if(!line.compare("for")) {
            reformat=true;
        }

        // for_each (C++0x)
        if(!line.compare("for_each")) {
            reformat=true;
        }

        // while
        if(!line.compare("while")) {
            if(do_braces)

            if(0==do_cnt || (do_cnt!=0 && do_cnt--==0))
                reformat=true;

#if 1 // ICC 14 bug workaround
            string line_orig=buf;
            char ch;
#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif
            do {
                ch=line_orig[line_orig.length()-1];
                if(' '==ch || '\t'==ch)
                    line_orig.erase(line_orig.length()-1);
            } while(' '==ch || '\t'==ch);
            if(';'==ch && !end_do)
                reformat=false;
#endif
        }

        // do
        if(!line.compare("do")) {
            ++do_cnt;
            reformat=true;
        }

        if(reformat && scrollback.size()>1+line2.size()) {
            auto i1=scrollback.crbegin();
            ++i1;
            bool got=true;
#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif
            for(int i2=line2.size()-1; i2>=0; --i2, ++i1) {
                if((*i1).compare(line2[i2])!=0) {
                    got=false;
                    break;
                }
            }
            if(got && ( (*i1).compare(line1)==0 || (*i1).compare(line1_disabled)==0 ) ) {
                reformat=!do_patch;
            } else {
                reformat=do_patch;
            }
        }

        if(reformat) {
            auto i1=scrollback.rbegin();
            if(i1!=scrollback.rend()) {
                ++i1;
                if(do_patch && !(*i1).compare(inline_pragma))
                    reformat=false;
            }
            int i3=0;
#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif
            for(int i2=0; i2<line2.size()+1; ++i2) {
                string ln=*i1;
                ++i1;
                while(ln.length()!=0 && ( ln[ln.length()-1]=='\r' || ln[ln.length()-1]=='\n') )
                    ln.erase(ln.length()-1,1);
                const char* cmp=line1;
#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif
                for(int i4=line2.size()-1; i4>=0; --i4) {
                    if(i2==i4) {
                        cmp=line2[i4];
                        break;
                    }
                }
                if(!ln.compare(cmp)) {
                    ++i3;
                } else if(2==i2 && !ln.compare(line1_disabled)) {
                    ++i3;
                } else {
                    break;
                }
            }
            if(line2.size()+1==i3) {
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
                        sprintf(tmp_buf,"%s\n",line1);
#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif
                        for(const auto& i4 : line2) {
                            strcat(tmp_buf,i4);
                            strcat(tmp_buf,"\n");
                        }
                        tmp_buf[strlen(tmp_buf)-1]='\0';
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
#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif
                        for(size_t i4=0; i4<line2.size(); ++i4)
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

#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#   pragma swp
#   pragma unroll
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

#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif
    for(;;) {
        string name=rc.name;

        bool sub_dir=false;

        if(!name.compare(".") || !name.compare("..")) {
            goto next_entry;
        } else {
            sub_dir=true;
        }

#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#   pragma swp
#   pragma unroll
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
#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#   pragma swp
#   pragma unroll
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

    ext_prologue=ext; // use prologue for ALL files

    TCHAR dir[MAX_PATH];
    if(!::GetCurrentDirectory(sizeof(dir) - 1, dir)) {
        return 1;
    }
    string d=dir;
    d.append("\\.\\*");
    unsigned cnt1=directory_recurse(".",d,d,do_patch);
    d=dir;
    d.append("\\..\\*");
    unsigned cnt2=directory_recurse(".",d,d,do_patch);

    if(!do_nowait) {
        printf("%u occurences processed. Press any key to exit.\n",cnt1+cnt2);
        getch();
    }

    return 0;
}

