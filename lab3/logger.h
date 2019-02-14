/*basic logging functionality class*/

#include <fstream>
#include <cstdio>


class Logger{
	
  string fFile = "flagPerson.log";
  string tFile = "cars.log";

  inline void Logger1( string logMsg ){
    string filePath = "/somedir/log_"+getCurrentDateTime("date")+".txt";
    string now = getCurrentDateTime("now");
    ofstream ofs(filePath.c_str(), std::ios_base::out | std::ios_base::app );
    ofs << now << '\t' << logMsg << '\n';
    ofs.close();
  }
	
	
	inline void Logger1( string logMsg ){
    string filePath = "/somedir/log_"+getCurrentDateTime("date")+".txt";
    string now = getCurrentDateTime("now");
    ofstream ofs(filePath.c_str(), std::ios_base::out | std::ios_base::app );
    ofs << now << '\t' << logMsg << '\n';
    ofs.close();
	}
};