/*
*********************************************************************************
*                            In the name of Almighty                            *
*                                                                               *
*            Logger.cpp : Robocup 3D Soccer Simulation Team Zigorat             *
*                     (This team was previously named Gcyrus)                   *
*                                                                               *
*  Date: 03/20/2007                                                             *
*  Author: Mahdi Hamdarsi                                                       *
*  Comments: Class definition for Logger, which is used to log information      *
*                                                                               *
*********************************************************************************
* Modified by Zhao Yin 
* 2010-12-12
*********************************************************************************
*/

/*! \file Logger.cpp
<pre>
<b>File:</b>          Logger.cpp
<b>Project:</b>       Robocup Soccer Simulation Team: Zigorat
<b>Authors:</b>       Mahdi Hamdarsi
<b>Created:</b>       03/20/2007
<b>Last Revision:</b> $ID$
<b>Contents:</b>      Class definition for Logger, which is used to log information
<hr size=2>
<h2><b>Changes</b></h2>
<b>Date</b>             <b>Author</b>          <b>Comment</b>
03/20/2007       Mahdi           Initial version created
</pre>
*/

#include "Logger.h"
#include <iostream>
#include <fstream>

using namespace std;

// Logger variables to log output of agent
//Logger BasicLogger ( "./.logfiles/BasicActions.log",   false );	/*!< You can quickly en debugging here */
//Logger CommLogger  ( "./.logfiles/Communications.log", false );	/*!< You can quickly en debugging here */
//Logger logger      ( "./.logfiles/Actions.log",        false  );	/*!< You can quickly en debugging here */
//Logger WMLogger    ( "./.logfiles/WM.log",             false );	/*!< You can quickly en debugging here */
//Logger AgentLog  ( "agentLog.log",             false );			/*!< You can quickly en debugging here */
//Logger gAgentInfoLog( "log/agentInfo.log",					true );		/*!< You can quickly en debugging here */
//Logger gVisionInfoLog( false );
//Logger gSpeedLog( false );
//Logger gLog	( false );												/*!< You can quickly en debugging here */
//Logger gDribbleLog( false );
//Logger gHearSayLog( false );


/*****************************************************************************/
/************************   CLASS LOGGER   ***********************************/
/*****************************************************************************/
Logger::Logger( bool bEnabled  )
{
	m_Enabled = bEnabled;
}

// void Logger::Init( string teamName, int playerNo )
// {
// 	m_Created = true;
// 	char fn[5000];
// 
// 	sprintf( fn,"log/%s_%d.log", teamName.data(), playerNo );
// 
// 	m_OS = new ofstream( fn );
// }

// void Logger::Init( string teamName, int playerNo, string name )
// {
// 	m_Created = true;
// 	char fn[5000];
// 
// 	sprintf( fn,"log/%s_%s_%d.log", name.data(), teamName.data(), playerNo );
// 
// 	m_OS = new ofstream( fn );
// }
/*! This is the constructor for the Logger. A standard output stream canbe specified.
\param os Output Stream (standard output stream) to which information is printed (default cout)
\param bEnabled Sets wheather logger is enabled
*/
Logger::Logger( ostream &os, bool bEnabled )
{
	m_Created = false;
	m_Enabled = bEnabled;

	if( os )
		m_OS = &os;
}

/*! This is the constructor for the Logger. A filename forThe output stream can be specified.
\param strFileName File name to log the information to
\param bEnabled Sets wheather logger is enabled
*/
Logger::Logger( const char *strFileName, bool bEnabled )
{
	m_Created = true;
	m_Enabled = bEnabled;
	m_OS = new ofstream( strFileName );
}

/*! This is the destructor for the Logger. The output stream is freed if it was a file. */
Logger::~Logger( )
{
	if( m_Created )
		delete m_OS;
}

/*! This method is used to log information.
\param strLog string that is logged to output stream.
\return The Logger output to continue
*/
Logger & Logger::operator << ( const char * strLog )
{
	if( m_Enabled )
	{
		( *m_OS ) << strLog;
		// ( *m_OS ).flush();
	}

	return *this;
}

Logger & Logger::operator<<      ( const long int intLog                  )
{
	if( m_Enabled )
	{
		( *m_OS ) << intLog;
		// ( *m_OS ).flush();
	}

	return *this;
}

//Logger & Logger::operator << ( const Vector3f vector3fLog )
//{
//  if( m_Enabled )
//  {
//	m_OS->setf(ios_base::fixed, ios_base::floatfield);
//    ( *m_OS ) << "(" << vector3fLog.x << ", " << vector3fLog.y << ", " << vector3fLog.z << ")";
//	m_OS->setf(ios_base::fmtflags(0), ios_base::floatfield);
//   // ( *m_OS ).flush();
//  }
//
//  return *this;
//}

/*! This method is used to log information.
\param chrLog charactor that is logged to output stream.
\return The Logger output to continue
*/
Logger & Logger::operator << ( const char chrLog )
{
	if( m_Enabled )
	{
		( *m_OS ) << chrLog;
		//  ( *m_OS ).flush();
	}

	return *this;
}

/*! This method is used to log information.
\param strLog string that is logged to output stream.
\return The Logger output to continue
*/
Logger & Logger::operator << ( const string strLog )
{
	if( m_Enabled )
	{
		( *m_OS ) << strLog;
		//  ( *m_OS ).flush();
	}

	return *this;
}

/*! This method is used to log information.
\param intLog integer value that is logged to output stream.
\return The Logger output to continue
*/
Logger & Logger::operator << ( const int intLog )
{
	if( m_Enabled )
	{
		( *m_OS ) << intLog;
		//  ( *m_OS ).flush();
	}

	return *this;
}

Logger & Logger::operator << ( const unsigned int intLog )
{
	if( m_Enabled )
	{
		( *m_OS ) << (int) intLog;
		//  ( *m_OS ).flush();
	}

	return *this;
}

/*! This method is used to log information.
\param dblLog double value that is logged to output stream.
\return The Logger output to continue
*/
Logger & Logger::operator << ( const double dblLog )
{
	if( m_Enabled )
	{
		m_OS->setf(ios_base::fixed, ios_base::floatfield);
		( *m_OS ) << dblLog;
		m_OS->setf(ios_base::fmtflags(0), ios_base::floatfield);
		//   ( *m_OS ).flush();
	}

	return *this;
}

/*! This method sets wheather logger is enabled or not
\param bEnabled Sets wheather logger is enabled
\return bool Indicates update was successfull */
bool Logger::setEnabled( bool bEnabled )
{
	m_Enabled = bEnabled;

	return ( true );
}

/*! This method returns wheather logger is enabled or not
\return bool Indicating wheather logger is enabled */
bool Logger::getEnabled( void )
{
	return ( m_Enabled );
}

/*****************************************************************************/
/********************** TESTING PURPOSES *************************************/
/*****************************************************************************/

/*
int main( void )
{
Logger log("~/Desktop/Logs");
log << "Hello world!\n";

return 0;
}

int main( void )
{
Logger log( cout );
log << '\n';

return 0;
}
*/
