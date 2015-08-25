/*
*********************************************************************************
*                            In the name of Almighty                            *
*                                                                               *
*             Logger.h : Robocup 3D Soccer Simulation Team Zigorat              *
*                     (This team was previously named Gcyrus)                   *
*                                                                               *
*  Date: 03/20/2007                                                             *
*  Author: Mahdi Hamdarsi                                                       *
*  Comments: Class decleration for Logger, which is used to log information     *
*  
*                                                                               *
*********************************************************************************
* Modified by Zhao Yin 
* 2010-12-12
*********************************************************************************
*/

/*! \file Logger.h
<pre>
<b>File:</b>          Logger.h
<b>Project:</b>       Robocup Soccer Simulation Team: Zigorat
<b>Authors:</b>       Mahdi Hamdarsi
<b>Created:</b>       03/20/2007
<b>Last Revision:</b> $ID$
<b>Contents:</b>      Class decleration for Logger, which is used to log information
<hr size=2>
<h2><b>Changes</b></h2>
<b>Date</b>             <b>Author</b>          <b>Comment</b>
03/20/2007       Mahdi           Initial version created
</pre>
*/

#ifndef LOGGER
#define LOGGER

#include <iostream>
#include <string>
/*#include "geometry.h"*/

using namespace std;
#define END "\n"
#define endl "\n"  /*!< This is to use endl with loggers */
//   .
//  /|\  <-- That is because of iostream handling.
//   |

/*****************************************************************************/
/************************** CLASS LOGGER *************************************/
/*****************************************************************************/

/*! The Logger Class is used in the RoboCup Soccer environment 
    to log the agents debugging information to the specified 
    output, so that a backtrace of agents actions could be 
    generated and tested. */
class Logger
{
  ostream *m_OS;         /*!< Output stream to log to */
  bool    m_Created;     /*!< Checks if the file is created */
  bool    m_Enabled;     /*!< Sets wheather logger is enabled */

public:
   Logger(bool bEnabled);
   //void Init( string teamName, int playerNo );
   //void Init( string teamName, int PlayerNum, string name );

   Logger                        ( ostream &os = cout, 
                                   bool bEnabled = true              );
   Logger                        ( const char *strFileName, 
                                   bool bEnabled = true              );
  ~Logger                        (                                   );

  Logger & operator <<           ( const char * strLog               );
  Logger & operator <<           ( const char chrLog                 );
  Logger & operator <<           ( const string strLog               );
  Logger & operator <<           ( const int intLog                  );
  Logger & operator <<           ( const long int intLog             );
  Logger & operator <<			 ( const unsigned int intLog		 );
  Logger & operator <<           ( const double dblLog               );

  bool setEnabled                ( bool bEnabled                     );
  bool getEnabled                ( void                              );
};

//extern Logger logger;       /*!< Logger to log normal information */
//extern Logger BasicLogger;  /*!< Logger to log BasicAgent information */
//extern Logger WMLogger;     /*!< Logger to log WorldModel information */
//extern Logger CommLogger;   /*!< Logger to log Connection messages */
//extern Logger gVisionInfoLog;
//extern Logger gSpeedLog;
//extern Logger gLog;	/*!< Logger to log Connection messages */
//extern Logger gDribbleLog;
//extern Logger gHearSayLog;
//		

#endif
