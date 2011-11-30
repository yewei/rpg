#include "../include/gb2312_to_Unicode.h"

int myUTF8_to_UNICODE(Uint16* unicode, unsigned char* utf8, int len)
{
    int length;
    unsigned char* t = utf8;

    length = 0;
    while (utf8 - t < len){
        //one byte.ASCII as a, b, c, 1, 2, 3 ect
        if ( *(unsigned char *) utf8 <= 0x7f ) {
            //expand with 0s.
            *unicode++ = *utf8++;
        }
        //2 byte.
        else if ( *(unsigned char *) utf8 <= 0xdf ) {
            *unicode++ = ((*(unsigned char *) utf8 & 0x1f) << 6) + ((*(unsigned char *) (utf8 + 1)) & 0x3f);
            utf8 += 2;
        }
        //3 byte.Chinese may use 3 byte.
        else {
            *unicode++ = ((int) (*(unsigned char *) utf8 & 0x0f) << 12) +
                ((*(unsigned char *) (utf8 + 1) & 0x3f) << 6) +
                (*(unsigned char *) (utf8 + 2) & 0x3f);
            utf8 += 3;
        }
        length++;
    }

    *unicode = 0;
    
    return (length);
}

std::vector<Uint16> getUnicode(const std::string& str)
{
    const int CHAR_SIZE = 256;
    //GB2312 src
    const unsigned char* src = (const unsigned char*)(str.c_str());
    size_t src_len = strlen((char*)src);
    //Unicode dst to get
    unsigned char dst[CHAR_SIZE] = {0};
    size_t dst_len = sizeof(dst);
    //iconv arg
    const unsigned char* in = src;
    unsigned char* out = dst;

    iconv_t cd;
    //GB2312 to UTF-8
    cd = iconv_open("UTF-8", "GB2312");
    if ((iconv_t)-1 == cd){
        exit (-1);
    }
    //conversion
    iconv(cd, (const char**)&in, &src_len, (char**)&out, &dst_len);

    //UTF-8 to Unicode
    int utf8Len = strlen((char*)dst);
    Uint16 unicodeData[CHAR_SIZE] = {0};
    int unicodeLen = myUTF8_to_UNICODE(unicodeData, dst, utf8Len);
    std::vector<Uint16> unicodeVectorArray;
    for (int i = 0; i < unicodeLen; i++) {
        unicodeVectorArray.push_back(unicodeData[i]);
    }
    
    iconv_close(cd); 
    return unicodeVectorArray;
}
