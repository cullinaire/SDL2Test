#ifndef LOGGER_H_
#define LOGGER_H_

//logger.h - routines for logging system messages

#include <iostream>
#include <string>

/**
* Log an SDL error with some error message to the output stream of our choice
* @param os The output stream to write the message to
* @param msg The error message to write, format will be msg error: SDL_GetError()
*/
void logSDLError(std::ostream &os, const std::string &msg);

#endif