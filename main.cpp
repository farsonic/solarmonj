#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <sstream>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <time.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "jfyconnection.h"

using namespace std;

// Date format has requested by PVOutPut
const std::string currentDate() {
time_t now = time(0);
struct tm tstruct;
char buf[80];
tstruct = *localtime(&now);
strftime(buf, sizeof(buf), "%Y%m%d", &tstruct);

return buf;
}

// Time format has requested by PVOutPut
const std::string currentTime() {
time_t now = time(0);
struct tm tstruct;
char buf[80];
tstruct = *localtime(&now);
strftime(buf, sizeof(buf), "%R", &tstruct);

return buf;
}

int main( int argc, char** argv )
{

Jfy::Connection conn( "/dev/serial0" );
string APIKey = "API Key";
string systemID = "SYSTEM ID";

if ( !conn.init() ) {
cerr << "Cannot initialise the connection." << endl;
return 1;
}

// Get the data
Jfy::InverterData data;
conn.readNormalInfo( &data );

std::ostringstream curr_val_str;
curr_val_str << "curl -d \"d=" << currentDate() << 
"\" -d \"t=" << currentTime() <<
"\" -d \"v1=" << data.frequency <<
"\" -d \"v2=" << data.energyToday <<
"\" -d \"v5=" << data.temperature <<
"\" -d \"v6=" << data.voltageDc <<
"\" -H \"X-Pvoutput-Apikey: " << APIKey <<
"\" -H \"X-Pvoutput-SystemId: " << systemID << 
"\" http://pvoutput.org/service/r2/addstatus.jsp";

std::cout << curr_val_str.str().c_str();

// Some times the inverter returns no data by the 1st attempt, just a quick check and launch a 2nd attempt if it failed
if ((int)data.temperature != 0) 
{
system(curr_val_str.str().c_str());
// Close the connection to the inverter. 
conn.close();
} else {
Jfy::InverterData data;
conn.readNormalInfo( &data );

std::ostringstream curr_val_str;
curr_val_str << "curl -d \"d=" << currentDate() << 
"\" -d \"t=" << currentTime() <<
"\" -d \"v1=" << data.frequency <<
"\" -d \"v2=" << data.energyToday <<
"\" -d \"v5=" << data.temperature <<
"\" -d \"v6=" << data.voltageDc <<
"\" -H \"X-Pvoutput-Apikey: " << APIKey <<
"\" -H \"X-Pvoutput-SystemId: " << systemID << 
"\" http://pvoutput.org/service/r2/addstatus.jsp";

std::cout << curr_val_str.str().c_str();	
if ((int)data.temperature != 0) 
{
system(curr_val_str.str().c_str());
}	
// Close the connection to the inverter. 
conn.close();
}

return 0;
}
