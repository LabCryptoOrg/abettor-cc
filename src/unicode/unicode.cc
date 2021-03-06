/*  The MIT License (MIT)
 *
 *  Copyright (c) 2015 LabCrypto Org.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */
 
#include <org/labcrypto/abettor++/unicode/unicode.h>


namespace org {
namespace labcrypto {
namespace abettor {
namespace unicode {
  std::string 
  UTF16to8(const wchar_t *in) {
    std::string out;
    unsigned int codepoint = 0;
    for (in; *in != 0; ++in) {
      if (*in >= 0xd800 && *in <= 0xdbff){
        codepoint = ((*in - 0xd800) << 10) + 0x10000;
      } else {
        if (*in >= 0xdc00 && *in <= 0xdfff) {
          codepoint |= *in - 0xdc00;
        } else {
          codepoint = *in;
        }
        if (codepoint <= 0x7f) {
          out.append(1, static_cast<char>(codepoint));
        } else if (codepoint <= 0x7ff) {
          out.append(1, static_cast<char>(0xc0 | ((codepoint >> 6) & 0x1f)));
          out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
        } else if (codepoint <= 0xffff) {
          out.append(1, static_cast<char>(0xe0 | ((codepoint >> 12) & 0x0f)));
          out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
          out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
        } else {
          out.append(1, static_cast<char>(0xf0 | ((codepoint >> 18) & 0x07)));
          out.append(1, static_cast<char>(0x80 | ((codepoint >> 12) & 0x3f)));
          out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
          out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
        }
        codepoint = 0;
      }
    }
    return out;
  }
  std::wstring 
  UTF8to16(const char *in) {
    std::wstring out;
    if (in == NULL) {
      return out;
    }
    unsigned int codepoint;
    while (*in != 0) {
      unsigned char ch = static_cast<unsigned char>(*in);
      if (ch <= 0x7f) {
        codepoint = ch;
      } else if (ch <= 0xbf) {
        codepoint = (codepoint << 6) | (ch & 0x3f);
      } else if (ch <= 0xdf) {
        codepoint = ch & 0x1f;
      } else if (ch <= 0xef) {
        codepoint = ch & 0x0f;
      } else {
        codepoint = ch & 0x07;
      }
      ++in;
      if (((*in & 0xc0) != 0x80) && (codepoint <= 0x10ffff)) {
        if (codepoint > 0xffff) {
          out.append(1, static_cast<wchar_t>(0xd800 + (codepoint >> 10)));
          out.append(1, static_cast<wchar_t>(0xdc00 + (codepoint & 0x03ff)));
        } else if (codepoint < 0xd800 || codepoint >= 0xe000) {
          out.append(1, static_cast<wchar_t>(codepoint));
        }
      }
    }
    return out;
  }
}
}
}
}