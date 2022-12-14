/**
* @file   iniFileLoaderBase.h
* @brief  iniファイルを読み込み
* @author Mat Buckland 様
* @note ※書籍「実例で学ぶゲームAIプログラミング」のサンプルコードから引っ張ってきました
*/

#ifndef ___INIFILELOADERBASE_H
#define ___INIFILELOADERBASE_H


#include <fstream>
#include <string>
#include <cassert>

class iniFileLoaderBase
{

private:

	//the file the parameters are stored in
	std::ifstream file;

	std::string   CurrentLine;

	void        GetParameterValueAsString(std::string& line);

	std::string GetNextParameter();

	//this ignores any comments and finds the next delimited string 
	std::string GetNextToken();

	//this is set to true if the file specified by the user is valid
	bool        m_bGoodFile;

public:

	//helper methods. They convert the next parameter value found into the 
	//relevant type
	double      GetNextParameterDouble(){ if (m_bGoodFile) return atof(GetNextParameter().c_str()); throw std::runtime_error("bad file"); }
	float       GetNextParameterFloat(){ if (m_bGoodFile) return (float)atof(GetNextParameter().c_str()); throw std::runtime_error("bad file"); }
	int         GetNextParameterInt(){ if (m_bGoodFile) return atoi(GetNextParameter().c_str()); throw std::runtime_error("bad file"); }
//	bool        GetNextParameterBool(){ return (bool)(atoi(GetNextParameter().c_str())); throw std::runtime_error("bad file"); }

	double      GetNextTokenAsDouble(){ if (m_bGoodFile) return atof(GetNextToken().c_str()); throw std::runtime_error("bad file"); }
	float       GetNextTokenAsFloat(){ if (m_bGoodFile) return (float)atof(GetNextToken().c_str()); throw std::runtime_error("bad file"); }
	int         GetNextTokenAsInt(){ if (m_bGoodFile) return atoi(GetNextToken().c_str()); throw std::runtime_error("bad file"); }
	std::string GetNextTokenAsString(){ if (m_bGoodFile) return GetNextToken(); throw std::runtime_error("bad file"); }

	bool        eof()const{ if (m_bGoodFile) return file.eof(); throw std::runtime_error("bad file"); }
	bool        FileIsGood()const{ return m_bGoodFile; }

	iniFileLoaderBase(char* filename) :CurrentLine(""), m_bGoodFile(true)
	{
		file.open(filename);

		if (!file){ m_bGoodFile = false; }
	}

};

#endif