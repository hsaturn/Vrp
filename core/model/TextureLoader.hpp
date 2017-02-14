/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ImageLoader.hpp
 * Author: hsaturn
 *
 * Created on 13 février 2017, 23:04
 */

#ifndef TEXTURELOADER_HPP
#define TEXTURELOADER_HPP

#include <string>


class TextureLoader {
	public:
		// Load an image file and call glTexImage2D on it.
		// Texture must be created and binded before
		static bool genGlTexture(std::string file);
		
		static bool genGlCheckerBoard();
	private:

};

#endif /* IMAGELOADER_HPP */

