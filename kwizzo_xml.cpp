#if defined( _MSC_VER )
	#if !defined( _CRT_SECURE_NO_WARNINGS )
		#define _CRT_SECURE_NO_WARNINGS		// This test file is not intended to be secure.
	#endif
#endif

#include "tinyxml2.h"
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include "kwizzo_xml.h"

#if defined( _MSC_VER ) || defined (WIN32)
	#include <crtdbg.h>
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	_CrtMemState startMemState;
	_CrtMemState endMemState;
#else
	#include <sys/stat.h>
	#include <sys/types.h>
#endif

using namespace tinyxml2;
using namespace std;
int gPass = 0;
int gFail = 0;


bool XMLTest(const char* testString, const char* expected, const char* found, bool echo = true, bool extraNL = false)
{
	bool pass;
	if (!expected && !found)
		pass = true;
	else if (!expected || !found)
		pass = false;
	else
		pass = !strcmp(expected, found);
	if (pass)
		printf("[pass]");
	else
		printf("[fail]");

	if (!echo) {
		printf(" %s\n", testString);
	}
	else {
		if (extraNL) {
			printf(" %s\n", testString);
			printf("%s\n", expected);
			printf("%s\n", found);
		}
		else {
			printf(" %s [%s][%s]\n", testString, expected, found);
		}
	}

	if (pass)
		++gPass;
	else
		++gFail;
	return pass;
}

bool XMLTest(const char* testString, XMLError expected, XMLError found, bool echo = true, bool extraNL = false)
{
	return XMLTest(testString, XMLDocument::ErrorIDToName(expected), XMLDocument::ErrorIDToName(found), echo, extraNL);
}

bool XMLTest(const char* testString, bool expected, bool found, bool echo = true, bool extraNL = false)
{
	return XMLTest(testString, expected ? "true" : "false", found ? "true" : "false", echo, extraNL);
}

template< class T > bool XMLTest(const char* testString, T expected, T found, bool echo = true)
{
	bool pass = (expected == found);
	if (pass)
		printf("[pass]");
	else
		printf("[fail]");

	if (!echo)
		printf(" %s\n", testString);
	else {
		char expectedAsString[64];
		XMLUtil::ToStr(expected, expectedAsString, sizeof(expectedAsString));

		char foundAsString[64];
		XMLUtil::ToStr(found, foundAsString, sizeof(foundAsString));

		printf(" %s [%s][%s]\n", testString, expectedAsString, foundAsString);
	}

	if (pass)
		++gPass;
	else
		++gFail;
	return pass;
}

void NullLineEndings( char* p )
{
	while( p && *p ) {
		if ( *p == '\n' || *p == '\r' ) {
			*p = 0;
			return;
		}
		++p;
	}
}


kwizzo_xml::kwizzo_xml()
{
#if defined(_MSC_VER) && defined(TINYXML2_DEBUG)
    _CrtMemCheckpoint(&startMemState);
    // Enable MS Visual C++ debug heap memory leaks dump on exit
    _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
    {
        int leaksOnStart = _CrtDumpMemoryLeaks();
        XMLTest("No leaks on start?", FALSE, leaksOnStart);
    }
#endif

    {
        TIXMLASSERT(true);
    }

    doc = new XMLDocument();
    doc->LoadFile("./resources/quiz_2020_04_25_a.xml");

    int errorID = doc->ErrorID();

    if (!errorID)
    {
        quizz_root = kwizzo_xml::doc->RootElement();
        quizz = quizz_root->FirstChildElement("quizz");
    }
}

kwizzo_xml::~kwizzo_xml()
{
    delete doc;
    doc = 0;
}

bool kwizzo_xml::kwizzo_xml_next_quiz()
{
    if(quizz == NULL)
        return false;

    XMLElement *element_test; 

    element_test = quizz->NextSiblingElement("quizz");
    
    if (element_test == NULL)
        return false;


    quizz = element_test;
    return true;
}

bool kwizzo_xml::kwizzo_xml_prev_quiz()
{
    if(quizz == NULL)
        return false;
    
    XMLElement *element_test;

    element_test = quizz->PreviousSiblingElement("quizz");
    
    if (element_test == NULL)
    {
        return false;
    }
    else
    {
        quizz = element_test;
    }
    

    return true;
}

bool kwizzo_xml::kwizzo_xml_question(char *buffer)
{
    XMLElement *question;

    question = quizz->FirstChildElement("question");
    const char *question_txt = question->GetText();

    strcpy(buffer, question_txt);

    return true;
}

bool kwizzo_xml::kwizzo_xml_answer(char *buffer)
{
    XMLElement *answer;
    
    answer = quizz->FirstChildElement("answer");
    const char *answer_txt = answer->GetText();

    strcpy(buffer, answer_txt);

    return true;
}

bool kwizzo_xml::kwizzo_xml_catagory(char *buffer)
{
    XMLElement *catagory;

    catagory = quizz->FirstChildElement("catagory");
    const char *catagory_txt = catagory->GetText();

    strcpy(buffer, catagory_txt);

    return true;
}

bool kwizzo_xml::kwizzo_xml_rating(char *buffer)
{
    XMLElement *rating = NULL;

    rating = quizz->FirstChildElement("rating");

	if(rating)
	{
    	const char *rating_txt = rating->GetText();

    	strcpy(buffer, rating_txt);

    	return true;
	}

	return false;
}

