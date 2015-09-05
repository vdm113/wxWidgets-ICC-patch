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
#include <functional>
#include <map>
#include <stack>
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

unsigned reformat(const string& file, bool do_prologue, bool do_patch, bool opt_uu)
{
#if 1 // new
    static const char* line1="#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */";
    static const char* line1_disabled="#if defined(__INTEL_COMPILER) && 0 /* VDM auto patch */";
    static const char* line_end="#endif /* VDM auto patch */";
#else // old
    static const char* line1="#if defined(__INTEL_COMPILER) && 1 // VDM auto patch";
    static const char* line1_disabled="#if defined(__INTEL_COMPILER) && 0 // VDM auto patch";
    static const char* line_end="#endif";
#endif

    static const vector<const char*> line2={ "#   pragma ivdep", "#   pragma swp", "#   pragma unroll", "#   if 0", "#       pragma simd", "#   endif", line_end };

    static const char* line_prologue_token="/* token_VDM_prologue */";
#if !ICC_BUG_02
    static const char* line_prologue="#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)\n#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)\n#elif !defined(VDM_MACRO_PRAGMA_IVDEP)\n#   define VDM_MACRO_PRAGMA_IVDEP\n#endif";
#else
    static const char* line_prologue="#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)\n#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep)\n#elif !defined(VDM_MACRO_PRAGMA_IVDEP)\n#   define VDM_MACRO_PRAGMA_IVDEP\n#endif";
#endif

    static const char* inline_pragma="VDM_MACRO_PRAGMA_IVDEP \\";

    static const size_t length=4096; // ugly limitation to 4096!
    char tmp_buf[length+16];

    unsigned cnt=0;
    bool changed=false;
    bool in_stipped_comment=false;

    vector<string> scrollback;
    vector<string> lines;

    FILE* in=fopen(file.c_str(),"r");
    if(!in) {
        check_return_value(errno);
        return cnt;
    }

    size_t ln=0;

    bool in_comment=false;

    int ctrl_braces=0;
    stack<int> ctrl_stack;
    int stage=-1;
    bool last_line_was_backslash=false;

    vector<string> contents;

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
    while(in && !feof(in)) {
        char buf[length+16];
        if(fgets(buf,length,in)==NULL) {
            if(feof(in)) {
                break;
            }
        }
        contents.push_back(buf);
    }
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
    for(size_t i=0; i<contents.size(); ++i) {
        char buf[length+16];
        sprintf(buf,"%s",contents[i].c_str());

        bool do_not_pragma=false;

        buf[length]='\0';
        {
            size_t len=strlen(buf);
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
            while(len>0 && ('\r'==buf[len-1] || '\n'==buf[len-1])) {
                buf[len-1]='\0';
                --len;
            }
        }

        if(!strcmp(buf,inline_pragma)) {
            continue;
        }

        ++ln;
        scrollback.push_back(buf);

        if(1==ln && strcmp(buf,line_prologue_token)==0) {
            size_t cnt=0;

            // seek for an empty line as ending of the prologue
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
            do {
                scrollback.clear();
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
                for(int i1=0; i1<6; ++i1) {
                    sprintf(buf,"%s",contents[i].c_str());
                    scrollback.push_back(buf);
                    ++i;
                }
                ++cnt;
            } while(strcmp(buf,line_prologue_token)==0); // might be broken, i.e. twice or more prologue occurence
            buf[0]='\0';
            changed=true;
            scrollback.clear();

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
            string save;
            if(scrollback.size()!=0)
                save=scrollback.back();
            scrollback.clear();
            scrollback.push_back(tmp_buf);
            if(save.compare(line_prologue_token) && save.length()!=0) {
                scrollback.push_back(save);
                ++cnt;
                changed=true;
            }
        }

        if(!do_patch && do_prologue && 1==ln && strcmp(buf,line_prologue_token)==0) {
            scrollback.push_back(buf);
            ++cnt;
            changed=true;
        }

        string line=buf;

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
        while(line.length()>0 && (' '==line[0] || '\t'==line[0] || '\r'==line[0] || '\n'==line[0])) {
            line.erase(0,1);
        }

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
        while(line.length()>0 && ('{'==line[line.length()-1] || ' '==line[line.length()-1] || '\t'==line[line.length()-1])) {
            line.erase(line.length()-1,1);
        }

        bool last_char_was_backslash=false;
        if(line.length()>0 && '\\'==line[line.length()-1]) {
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
            while(line.length()>1 && ('{'==line[line.length()-2] || ' '==line[line.length()-2] || '\t'==line[line.length()-2])) {
                line.erase(line.length()-2,1);
            }
            last_char_was_backslash=true;
            last_line_was_backslash=true;
        }

        if(line.find("//")!=string::npos) {
            line.erase(line.find("//"));
        }

        {
        again2:
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
            for(int i1=0; i1<line.length(); ++i1) {
                if(in_comment) {
                    if(line.find("*/")==0) {
                        in_comment=false;
                        line.erase(0,2);
                        goto again2;
                    }
                    line.erase(0,1);
                    goto again2;
                } else {
                    if(line.find("/*")==i1) {
                        if(line.find("*/")==string::npos) {
                            in_comment=true;
                            line.erase(i1);
                            break;
                        }
                        line.erase(i1,line.find("*/")-i1+2);
                        goto again2;
                    }
                }
            }
        }
        if(in_comment)
            continue;

        auto command_winlets=[&]() {
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
            for(int i1=0; i1<line.length(); ++i1)  {
                switch(stage) {
                    case 0:
                    case 1:
                    case 2:
                            {
                                //
                            }
                            break;
                    case -1:
                        break;
                }
            }
        };

        if(in_stipped_comment) {
            continue;
        }

        const auto check_allowed_pragma=[&]() {
            bool finish=false;
            size_t parent=0;
            bool check_semicolon=false;

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
            for(size_t i1=i; i1<contents.size(); ++i1) {
                string next=contents[i1];

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
                while(next.length()>0 && ('\r'==next[next.length()-1] || '\n'==next[next.length()-1]))
                    next.erase(next.length()-1,1);
                bool backslash=false;
                if(next.length()>0 && '\\'==next[next.length()-1]) {
                    next.erase(next.length()-1,1);
                    backslash=true;
                }
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
                while(next.length()>0 && (' '==next[next.length()-1] || '\t'==next[next.length()-1]))
                    next.erase(next.length()-1,1);

                if(next.find("for")==0)
                    next.erase(0,3);
                if(next.find("while")==0)
                    next.erase(0,5);
                if(next.find("do")==0)
                    next.erase(0,2);
                if(next.find("for_each")==0)
                    next.erase(0,8);

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
                for(size_t i2=0; i2<next.length(); ++i2) {
                    if(' '==next[i2] || '\t'==next[i2])
                        continue;
                    if(check_semicolon) {
                        if(';'==next[i2]) {
                            do_not_pragma=true;
                            finish=true;
                            break;
                        } else {
                            do_not_pragma=false;
                            finish=true;
                            break;
                        }
                    }
                    if('('==next[i2]) {
                        parent++;
                        continue;
                    }
                    if(')'==next[i2]) {
                        parent--;
                        if(0==parent)
                            check_semicolon=true;
                        continue;
                    }
                    if('{'==next[i2]) {
                        do_not_pragma=false;
                        finish=true;
                        break;
                    }
                    do_not_pragma=false;
                    if(finish)
                        break;
                }
                if(finish)
                    break;
            }
        };

        bool reformat=false;
        string seeking;
        int ctrl_cnt=0;
        stack<int> ctrl_stack;

        const auto reformat_ctrl=[&]() -> bool {
            if(reformat || line.find(seeking)!=0)
                return false;
            if(line.length()<seeking.length())
                return false;
            check_allowed_pragma();
            if(line.length()>0 && '\\'==line[line.length()-1]) {
                last_char_was_backslash=true;
                last_line_was_backslash=true;
                line.erase(line.length()-1,1);
            }
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
            while(line.length()>0 && (' '==line[0] || '\t'==line[0])) {
                line.erase(0,1);
            }
        again:
            if(line.length()>seeking.length()) {
                const char ch=line[seeking.length()];
                if(' '==ch) {
                    line.erase(seeking.length(),1);
                    goto again;
                }
                if('\t'==ch || '('==ch || '{'==ch)
                    return true;
                else
                    return false;
            }
            return true;
        };

        seeking="for";
        if(reformat_ctrl()) {
            lines.push_back(line);
            string ln2=line;
            ln2.erase(0,seeking.length());
            reformat=true;
        }

        seeking="for_each";
        if(reformat_ctrl()) {
            lines.push_back(line);
            string ln2=line;
            ln2.erase(0,seeking.length());
            reformat=true;
        }

        seeking="while";
        if(reformat_ctrl()) {
            lines.push_back(line);
            string ln2=line;
            ln2.erase(0,seeking.length());
            reformat=true;
        }

        bool end_do=false;

        {
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
            for(size_t i1=0; i1<line.length(); ++i1) {
                if('{'==line[i1])
                    ++ctrl_braces;
                if('}'==line[i1]) {
                    if(--ctrl_braces==0)
                        end_do=true;
                }
            }
        }

        seeking="do";
        if(reformat_ctrl()) {
            lines.push_back(line);
            string ln2=line;

            ln2.erase(0,strlen("do"));

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
            while(ln2.length()>0 && (' '==ln2[ln2.length()-1] || '{'==ln2[ln2.length()-1]))
                ln2.erase(ln2.length()-1,1);

            if(ln2.length()>0 && '{'==ln2[ln2.length()-1]) {
                ctrl_stack.push(ctrl_cnt);
                ctrl_cnt=0;
                reformat=true;
            } else if(ln2.length()>0 && '}'==ln2[ln2.length()-1]) {
                if(--ctrl_cnt==0) {
                    ctrl_cnt=ctrl_stack.top();
                    ctrl_stack.pop();
                }
            } else if(ln2.empty()) {
                reformat=true;
            }
        }

        if(!last_char_was_backslash) {
            if(reformat && scrollback.size()>1 && scrollback[scrollback.size()-1-1].compare(inline_pragma) && scrollback.size()>1+line2.size()) {
                bool got=false;
                size_t i3=0;
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
                for(auto i2=scrollback.rbegin(); i2!=scrollback.rend(); ++i2) {
                    if(i3++<=line2.size())
                        continue;
                    if(((*i2).compare(line1)==0 || (*i2).compare(line1_disabled)==0)) {
                        got=true;
                        break;
                    } else {
                        break;
                    }
                }
                if(got)
                    reformat=true;
            } else if(reformat && scrollback.size()>1 && !scrollback[scrollback.size()-1-1].compare(inline_pragma)) {
                reformat=!do_patch;
            }
        }

        if(reformat) {
            string save=scrollback.back();
            if(do_patch) {
                if(last_char_was_backslash || last_line_was_backslash) {
                    if(!do_not_pragma) {
                        sprintf(tmp_buf,"%s",inline_pragma);
                        scrollback.pop_back();
                        scrollback.push_back(tmp_buf);
                        scrollback.push_back(save);
                        ++cnt;
                        changed=true;
                    }
                } else if(!(scrollback.size()>1 && !scrollback[scrollback.size()-1-1].compare(line_end))) {
                    if(!do_not_pragma)
                        sprintf(tmp_buf,"%s\n",line1);
                    else
                        sprintf(tmp_buf,"%s\n",line1_disabled);
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
                    for(const auto& i4 : line2) {
                        strcat(tmp_buf,i4);
                        strcat(tmp_buf,"\n");
                    }
                    tmp_buf[strlen(tmp_buf)-1]='\0';
                    scrollback.pop_back();
                    scrollback.push_back(tmp_buf);
                    scrollback.push_back(save);
                    ++cnt;
                    changed=true;
                }
            } else if(scrollback.size()>1 && !scrollback[scrollback.size()-2].compare(line_end)) {
                if(last_char_was_backslash) {
                    scrollback.pop_back();
                    scrollback.push_back(save);
                    ++cnt;
                    changed=true;
                } else {
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
                    while((*scrollback.rbegin()).compare(line1) && (*scrollback.rbegin()).compare(line1_disabled)) {
                        scrollback.pop_back();
                    }
                    scrollback.pop_back();
                    scrollback.push_back(save);
                    ++cnt;
                    changed=true;
                }
            }
            if(!changed)
                lines.pop_back();
        }
        last_line_was_backslash=last_char_was_backslash;
        last_char_was_backslash=false;
    }

    fclose(in);

    if(opt_uu) {
        uu_again:
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
        for(vector<string>::iterator i1=scrollback.begin(); i1!=scrollback.end(); ++i1) {
            if(!(*i1).compare(line1)) {
                vector<string>::iterator i2=i1+1+line2.size();
                scrollback.erase(i1,i2);
                changed=true;
                ++cnt;
                goto uu_again;
            } else if(!(*i1).compare(inline_pragma)) {
                scrollback.erase(i1);
                changed=true;
                ++cnt;
                goto uu_again;
            }
        }
    }

    if(changed || 0!=cnt) {
        FILE* in=fopen(file.c_str(),"w+b");
        if(!in) {
            check_return_value(errno);
            return cnt;
        }

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
        for(vector<string>::const_iterator i1=scrollback.begin(); i1!=scrollback.end(); ++i1) {
            fprintf(in,"%s\n",(*i1).c_str());
        }

        fclose(in);
    }

    if(cnt) {
        printf("Processed file: %s\n",file.c_str());
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
        for(auto i : lines) {
            printf("    %s\n",i.c_str());
        }
    }

    return cnt;
}

static set<string> ext;
static set<string> ext_prologue;
static map<string,bool> files;

void directory_recurse(const string& base, const string& directory, const string& path)
{
    string current=directory;

    _finddata_t rc;
    intptr_t handle=_findfirst(current.c_str(),&rc);
    if(!check_return_value(errno))
        return;

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
    for(;;) {
        string name=rc.name;

        bool sub_dir=false;

        if(!name.compare(".") || !name.compare("..")) {
            goto next_entry;
        } else {
            sub_dir=true;
        }

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
        for(set<string>::iterator i1=ext.begin(); i1!=ext.end(); ++i1) {
            string::size_type pos=name.rfind(*i1);
            if(pos!=string::npos && pos==name.length()-(*i1).length()) {
                string n=directory;
                if(n.rfind("*")==n.length()-1)
                    n.erase(n.length()-1,1);
                n+=name;
                bool b=ext_prologue.find(*i1)!=ext_prologue.end();

                files.insert(make_pair(n,b));

                break;
            }
        }

        if(sub_dir && (rc.attrib & _A_SUBDIR)) {
            string dir_name=directory;
            dir_name.erase(dir_name.length()-1,1);
            dir_name+=name;
            dir_name.append("\\*");
            directory_recurse(base,dir_name,name);
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
}

int _tmain(int argc, _TCHAR* argv[])
{
    bool do_patch;
    bool opt_uu=false;
    bool do_nowait=false;

    {
        bool usage=false;

        if(argc<2) {
            usage=true;
        } else {
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
            for(size_t i1=1; i1<argc; ++i1) {
                if(strcmp(argv[i1],"-p")==0) {
                    do_patch=true;
                } else if(strcmp(argv[i1],"-u")==0) {
                    do_patch=false;
                } else if(strcmp(argv[i1],"-uu")==0) {
                    do_patch=false;
                    opt_uu=true;
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
    ext.insert(".");

    ext_prologue=ext; // use prologue for ALL files

    TCHAR dir[MAX_PATH];
    if(!::GetCurrentDirectory(sizeof(dir) - 1, dir)) {
        return 1;
    }
    string d=dir;
    d.append("\\.\\*");
    directory_recurse(".",d,d);
    d=dir;
    d.append("\\..\\*");
    directory_recurse(".",d,d);

    vector<function<unsigned()> > funcs;
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
    for(auto i1=files.cbegin(); i1!=files.cend(); ++i1) {
        string n=(*i1).first;
        bool b=(*i1).second;
        funcs.push_back([n,b,do_patch,opt_uu]{ return reformat(n,b,do_patch,opt_uu); });
    }

    size_t cnt=0;
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
    for(auto i1=funcs.begin(); i1!=funcs.end(); ++i1) {
        cnt+=(*i1)();
    }

    if(!do_nowait) {
        printf("%u occurences processed. Press any key to exit.\n",cnt);
        getch();
    }

    return 0;
}

