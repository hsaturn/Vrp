/* 
 * File:   ClipBoard.cpp
 * Author: hsaturn
 * 
 * Created on 11 juillet 2015, 09:09
 */

#include "ClipBoard.h"

std::string ClipBoard::get() {
    FILE* pipe = popen("./xclip -o", "r");
	if (!pipe) pipe = popen("xclip -o","r");
    if (!pipe) return "";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
    	if(fgets(buffer, 128, pipe) != NULL)
    		result += buffer;
    }
    pclose(pipe);
    return result;
}