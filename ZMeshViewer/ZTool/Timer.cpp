#include "Timer.h"

/*****************************************************************************/
/********************** CLASS TIMING *****************************************/
/*****************************************************************************/

/*! This method returns the difference between two timevals in seconds.
    \param tv1 first timeval
    \param tv2 second timeval
    \return double representing the difference between t1 and t2 in seconds */
#ifdef WIN32
double Timing::getTimeDifference( DWORD tv1, DWORD tv2 )
{
  return  ((double)(tv1 - tv2) / 1000.0) ;
}
#else
double Timing::getTimeDifference( struct timeval tv1, struct timeval tv2 )
{

  return  ((double)tv1.tv_sec + (double)tv1.tv_usec/1000000 ) -
          ((double)tv2.tv_sec + (double)tv2.tv_usec/1000000 ) ;
}
#endif

/*! This method prints the time in seconds that elapsed since
    the timer was restarted. It is possible to multiply this time with a
    factor, such that it can be used for different quantity values. In the
    default case this value is 1000 with the effect that the resulting value
    resembles miliseconds.
    \param os output stream to which output should be written.
    \param str that should be printed
    \param iFactor with which the elapsed time is multiplied (default 1000) */
void Timing::printTimeDiffWithText( std::ostream &os, char *str, int iFactor )
{
  // set the with to 6 and fill remaining places with '0'.
  os << std::setw(6)<< std::setfill('0') << getElapsedTime()*iFactor << ":" << str << "\n";
}

/*! This method returns the time (in seconds) since the last time the timer
    was restarted.
    \return double seconds that have passed since last restart of timer */
double Timing::getElapsedTime( int iFactor )
{
#ifdef WIN32
  DWORD time2 = GetTickCount();
#else
  struct timeval time2;
  gettimeofday( &time2, NULL );
#endif
  return getTimeDifference( time2, time1 )*iFactor;
}

/*! This method restarts the timer by setting it to the current time */
void Timing::restartTime( )
{
#ifdef WIN32
  time1 = GetTickCount();
#else
  gettimeofday( &time1, NULL );
#endif
}
