// CommandParser.h
//

#ifndef __COMMANDPARSER_H__
#define __COMMANDPARSER_H__

#include <Arduino.h>
#include "CommandStack.h"

#define MAX_FIELD_LEN	(16)


/////////////////////////////////////////////////////////////////////////////
// class CommandParser

class CommandParser
{
public:
	CommandParser(uint8_t src, Stream & strm, CommandStack & stack);

public:
	void			update();
	
private:
	uint32_t		toNum(const char * str);

private:
	//
	uint8_t			StrmSouce;
	Stream &		Strm;
	
	CommandStack & 	Stack;
	
	//
	int32_t			parseStep;
	
	char			fieldData[MAX_FIELD_LEN];
	uint8_t			fieldIndex;
	
	uint16_t		cmdCode;
};

#endif // __COMMANDPARSER_H__
