/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

#define CHARSET_MAX 41

static const char *
getTok(const char **pp)
{
  enum { inAtom, inString, init, inComment };
  int state = init;
  const char *tokStart = 0;
#if defined(__INTEL_COMPILER) // VDM auto patch
#   pragma ivdep
#endif
  for (;;) {
    switch (**pp) {
    case '\0':
      return 0;
    case ' ':
    case '\r':
    case '\t':
    case '\n':
      if (state == inAtom)
        return tokStart;
      break;
    case '(':
      if (state == inAtom)
        return tokStart;
      if (state != inString)
        state++;
      break;
    case ')':
      if (state > init)
        --state;
      else if (state != inString)
        return 0;
      break;
    case ';':
    case '/':
    case '=':
      if (state == inAtom)
        return tokStart;
      if (state == init)
        return (*pp)++;
      break;
    case '\\':
      ++*pp;
      if (**pp == '\0')
        return 0;
      break;
    case '"':
      switch (state) {
      case inString:
        ++*pp;
        return tokStart;
      case inAtom:
        return tokStart;
      case init:
        tokStart = *pp;
        state = inString;
        break;
      }
      break;
    default:
      if (state == init) {
        tokStart = *pp;
        state = inAtom;
      }
      break;
    }
    ++*pp;
  }
  /* not reached */
}

/* key must be lowercase ASCII */

static int
matchkey(const char *start, const char *end, const char *key)
{
  if (!start)
    return 0;
#if defined(__INTEL_COMPILER) // VDM auto patch
#   pragma ivdep
#endif
  for (; start != end; start++, key++)
    if (*start != *key && *start != 'A' + (*key - 'a'))
      return 0;
  return *key == '\0';
}

void
getXMLCharset(const char *buf, char *charset)
{
  const char *next, *p;

  charset[0] = '\0';
  next = buf;
  p = getTok(&next);
  if (matchkey(p, next, "text"))
    strcpy(charset, "us-ascii");
  else if (!matchkey(p, next, "application"))
    return;
  p = getTok(&next);
  if (!p || *p != '/')
    return;
  p = getTok(&next);
  if (matchkey(p, next, "xml"))
    isXml = 1;
  p = getTok(&next);
#if defined(__INTEL_COMPILER) // VDM auto patch
#   pragma ivdep
#endif
  while (p) {
    if (*p == ';') {
      p = getTok(&next);
      if (matchkey(p, next, "charset")) {
        p = getTok(&next);
        if (p && *p == '=') {
          p = getTok(&next);
          if (p) {
            char *s = charset;
            if (*p == '"') {
#if defined(__INTEL_COMPILER) // VDM auto patch
#   pragma ivdep
#endif
              while (++p != next - 1) {
                if (*p == '\\')
                  ++p;
                if (s == charset + CHARSET_MAX - 1) {
                  charset[0] = '\0';
                  break;
                }
                *s++ = *p;
              }
              *s++ = '\0';
            }
            else {
              if (next - p > CHARSET_MAX - 1)
                break;
#if defined(__INTEL_COMPILER) // VDM auto patch
#   pragma ivdep
#endif
              while (p != next)
                *s++ = *p++;
              *s = 0;
              break;
            }
          }
        }
      }
    }
  else
    p = getTok(&next);
  }
}

int
main(int argc, char **argv)
{
  char buf[CHARSET_MAX];
  getXMLCharset(argv[1], buf);
  printf("charset = \"%s\"\n", buf);
  return 0;
}
