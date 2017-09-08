#ifndef __XML_TOKEN_HH__
#define __XML_TOKEN_HH__

enum class XML_TOKEN{
  NONE,
  NAME,
  OPEN_BRACKET,
  END_BRACKET,
  CLOSE_BRACKET,
  ONE_LINE_CLOSE,
  SYMBOL,
  STRING,
  WORD,
  FINAL,
};

#endif

