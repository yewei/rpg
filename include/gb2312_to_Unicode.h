//FileName: gb2312_to_Unicode.h
#ifndef GB2312_TO_UNICODE_H_
#define GB2312_TO_UNICODE_H_

#include <iostream>
#include <vector>
#include "GNU/iconv.h"
#include "SDL/SDL.h"

std::vector<Uint16> getUnicode(const std::string& str);

#endif
