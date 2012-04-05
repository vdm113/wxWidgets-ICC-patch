// wxWidgets_vdm_patch.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// wxWidgets_vdm_patch.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <io.h>
#include <wchar.h>
#include <string>
#include <list>
#include <sstream>
#include <vector>
#include <cassert>
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

void reformat(const string& file)
{
    static const char* line1="#if defined(__INTEL_COMPILER)";
    static const char* line1_disabled="#if defined(__INTEL_COMPILER) && 0";
    static const char* line2="#   pragma ivdep";
    static const char* line3="#endif";

    static const char* line_prologue_token="/* token_VDM_prologue */";
    static const char* line_prologue="#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)\n#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)\n#elif !defined(MY_MACRO_PRAGMA_IVDEP)\n#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */\n#endif";

    static const char* inline_pragma="MY_MACRO_PRAGMA_IVDEP \\";

    vector<string> scrollback;

    FILE* in=fopen(file.c_str(),"r");
    if(!in) {
        check_return_value(errno);
        return;
    }
    string tmp_file=file;
    tmp_file.append("-tmp");
    FILE* out=fopen(tmp_file.c_str(),"w+");
    if(!out) {
        check_return_value(errno);
        fclose(in);
        return;
    }

    bool last_char_was_backslash=false;
    size_t ln=0;

#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
    while(in && !feof(in)) {
again:
        static const size_t length=4096; // ugly limitation to 4096!

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
            while(len>0 && '\r'==buf[len-1] || '\n'==buf[len-1]) {
                buf[len-1]='\0';
                len=strlen(buf);
            }
        }

        ++ln;
        if(1==ln && strcmp(buf,line_prologue_token)!=0) {
            fprintf(out,"%s\n%s\n\n",line_prologue_token,line_prologue);
        }

        scrollback.push_back(buf);

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

        bool reformat=false;

#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
        while(line.length()>0 && (line.length()-1 !=0) && ( line[line.length()-1]=='\r' || line[line.length()-1]=='\n' || line[line.length()-1]=='\0' || line[line.length()-1]==' ' ) ) {
            line.erase(line.length()-1,1);
        }

#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
        while(line.length()>0 && (' '==line[0] || '\t'==line[0] || '\r'==line[0] || '\n'==line[0])) {
            line.erase(line.length()-1,1);
        }

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

        if(scrollback.size()>=3) {
            vector<string>::iterator i1=scrollback.end();
            bool okay=true;
            if(
                (
                    (*(i1-1)).compare(line3)==0 &&
                    (*(i1-2)).compare(line2)==0 &&
                    ( (*(i1-3)).compare(line1)==0 || (*(i1-3)).compare(line1_disabled)==0 || (*i1).compare(line_prologue_token)==0)
                )
            ) {
                fclose(in);
                fclose(out);
                remove(tmp_file.c_str());
                return;
            }
            --i1;
        }

        if(reformat) {
            vector<string>::reverse_iterator i1=scrollback.rbegin();
            if(i1!=scrollback.rend()) {
                if(!(*i1).compare(inline_pragma))
                    reformat=false;
            }
            if(scrollback.size()>=4) {
                ++i1;
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
                    } else {
                        break;
                    }
                }
                if(3==i3) {
                    reformat=false;
                }
            }

            if(reformat) {
                if(last_char_was_backslash) {
                    fprintf(out,"%s\n",inline_pragma);
                } else {
                    fprintf(out,"%s\n%s\n%s\n",line1, line2, line3);
                }
            }
        }
        fprintf(out,"%s\n",buf);

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
    fclose(out);

    remove(file.c_str());
    rename(tmp_file.c_str(),file.c_str());
}

static list<string> ext;

void directory_recurse(const string& base, const string& directory, const string& path)
{
    string current=directory;

    _finddata_t rc;
    intptr_t handle=_findfirst(current.c_str(),&rc);
    if(!check_return_value(errno))
        return;

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
        for(list<string>::iterator i1=ext.begin(); i1!=ext.end(); ++i1) {
            string::size_type pos=name.rfind(*i1);
            if(pos!=string::npos && pos==name.length()-(*i1).length()) {
                string n=directory;
                if(n.rfind("*")==n.length()-1)
                    n.erase(n.length()-1,1);
                n+=name;
                reformat(n);
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
    ext.push_back(".h");
    ext.push_back(".hh");
    ext.push_back(".hpp");
    ext.push_back(".hxx");
    ext.push_back(".c");
    ext.push_back(".cc");
    ext.push_back(".cpp");
    ext.push_back(".cxx");

    TCHAR dir[MAX_PATH];
    if(!::GetCurrentDirectory(sizeof(dir) - 1, dir)) {
        return 1;
    }
    string d=dir;
    d.append("\\..\\*");
    directory_recurse(".",d,d);
}

