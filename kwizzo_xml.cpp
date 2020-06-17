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
#include <iostream>
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


kwizzo_xml::kwizzo_xml(std::string xml_file_name)
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
    doc->LoadFile(xml_file_name.c_str());

    int errorID = doc->ErrorID();

    if (!errorID)
    {
        quizz_root = kwizzo_xml::doc->RootElement();
		quizz = quizz_root->LastChildElement("quizz");
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


bool kwizzo_xml::kwizzo_xml_new_quiz()
{
	// Stick in a new child element of the root
	quizz = quizz_root->InsertNewChildElement("quizz");

	if(quizz == NULL)
	{
		std::cout << "Failed to insert new element\n";
	}
	else
	{
		std::cout << "New element added\n";

	}
	
	return true;
}

bool kwizzo_xml::kwizzo_xml_delete_quiz()
{
	// Need to get a node before deleting this one
    XMLElement *nextnode;

	nextnode = quizz->PreviousSiblingElement("quizz");
	if(nextnode == NULL)
	{
		nextnode = quizz->NextSiblingElement("quizz");

		if(nextnode == NULL)
		{
			nextnode = quizz_root->FirstChildElement("quizz");
		}
	}

	quizz_root->DeleteChild(quizz);

	quizz = nextnode;

	return true;
}


bool kwizzo_xml::kwizzo_xml_question(char *buffer)
{
    XMLElement *question;

    question = quizz->FirstChildElement("question");

	if(question != NULL)
	{
		const char *question_txt = question->GetText();

		if(question_txt != NULL)
	    	strcpy(buffer, question_txt);
		else
			buffer[0] = 0;
		
	}
	else
	{
		buffer[0] = 0;
	}
	
    return true;
}

bool kwizzo_xml::kwizzo_xml_answer(char *buffer)
{
    XMLElement *answer;
    
    answer = quizz->FirstChildElement("answer");

	if (answer != NULL)
	{
		const char *answer_txt = answer->GetText();

		if(answer_txt != NULL)
		{
			strcpy(buffer, answer_txt);
		}
		else
		{
					buffer[0] = 0;
		}
		
	}
	else
	{
		buffer[0] = 0;
	}

	return true;
}

bool kwizzo_xml::kwizzo_xml_catagory(uint8_t index, char *buffer)
{
    XMLElement *catagory;
	const char *catagory_txt;
	uint8_t count = 0;

	catagory = quizz->FirstChildElement("catagory");

	while (count <= index)
	{
		if (catagory == NULL)
			return false;

		if (count == index)
		{
			catagory_txt = catagory->GetText();
			strcpy(buffer, catagory_txt);
			return true;
		}
		else
		{
			catagory = catagory->NextSiblingElement("catagory");
		}

		count++;
	}

	return false;
}

bool kwizzo_xml::kwizzo_xml_rating(char *buffer)
{
	XMLElement *rating = NULL;
	const char *rating_txt = NULL;

	rating = quizz->FirstChildElement("rating");

	if (rating == NULL)
	{
		// There is no rating element so add it
		rating = quizz->InsertNewChildElement("rating");

		if (rating == NULL)
		{
			std::cout << __FILE__ << " Failed to create a element \"rating\"\n";
			return false;
		}

		rating->SetText("UNKNOW");

	}

	// Get the rating element text
	rating_txt = rating->GetText();

	// If rating element is present but no text then rtn UNKOWN 
	if (rating_txt == NULL)
		rating_txt = "UNKNOWN";

	// Copy in the result
	strcpy(buffer, rating_txt);

	// Debug
	std::cout << __FILE__ << " Reading rating element : " << buffer << "\n";

	return true;
}


bool kwizzo_xml::kwizzo_xml_update_question(char *buffer)
{
    XMLElement *question = NULL;

    question = quizz->FirstChildElement("question");

	if(question == NULL)
	{
		question = quizz->InsertNewChildElement("question");
	}

	question->SetText(buffer);

	return true;
}

bool kwizzo_xml::kwizzo_xml_update_answer(char *buffer)
{
    XMLElement *answer = NULL;

    answer = quizz->FirstChildElement("answer");

	if(answer == NULL)
	{
		answer = quizz->InsertNewChildElement("answer");
	}

	answer->SetText(buffer);

	return true;
}

bool kwizzo_xml::kwizzo_xml_delete_catagory()
{
    XMLElement *catagory = NULL;

    catagory = quizz->FirstChildElement("catagory");

	if(catagory == NULL)
	{
		// No catagory elements , job already done
		return true;
	}

	while(catagory)
	{
		std::cout << __FILE__ " Deleting catagory\n";
		quizz->DeleteChild(catagory);
		catagory = quizz->FirstChildElement("catagory");
	}

	return true;
}

bool kwizzo_xml::kwizzo_xml_update_catagory(const char *buffer)
{
    XMLElement *catagory = NULL;

    catagory = quizz->FirstChildElement("catagory");

	while(1)
	{
		if (catagory == NULL)
		{
			// There is no catagory element so add it
			catagory = quizz->InsertNewChildElement("catagory");

			if (catagory == NULL)
			{
				std::cout << __FILE__ << " Cant create element catagory\n";
				return false;
			}

			std::cout << __FILE__ << " New Catagory element text: " << buffer << "\n";
			catagory->SetText(buffer);

			return true;
		}
		else
		{
			std::cout << __FILE__ << " Catagory element text " << catagory->GetText() << "\n";
			catagory = catagory->NextSiblingElement("catagory");
		}
	}
}

bool kwizzo_xml::kwizzo_xml_update_rating(const char *buffer)
{
    XMLElement *rating = NULL;

	std::cout << __FILE__ << " update rating\n";


    rating = quizz->FirstChildElement("rating");

	if(rating == NULL)
	{
		// There is no rating element so add it
		rating = quizz->InsertNewChildElement("rating");

		std::cout << __FILE__ << "Created new rating element";

		if(rating == NULL)
			return false;

	}
	
	std::cout << __FILE__ << " Updated rating element " << buffer << "\n";
	rating->SetText(buffer);

	return true;
}


bool kwizzo_xml::kwizzo_xml_file_save(std::string filename)
{
	if(XML_SUCCESS == doc->SaveFile( filename.c_str(), false))
		return true;

	return false;
}