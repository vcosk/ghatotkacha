/*
    This file is part of Ghatotkacha.

    Ghatotkacha is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Ghatotkacha is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <cstdlib>
#include <iostream>
#include "windows.h"
#include <stdio.h>
#include <Winbase.h>

using namespace std;

int main(int argc, char *argv[])
{
     // Create timer object.
    HANDLE hTimer = CreateWaitableTimer (NULL, FALSE, "MyTimer");
    
    boolean status = false;
    LARGE_INTEGER liDueTime;
    liDueTime.QuadPart = -1000000LL;
    
    LARGE_INTEGER sleepDueTime;
    sleepDueTime.QuadPart = -1000000LL;
    
   	_SYSTEM_POWER_STATUS sps;
    
    boolean awake = false;
    
    boolean firstCheckOnBattery = true;
    
    while(true) {
                
        GetSystemPowerStatus(&sps);
        if((int)sps.ACLineStatus) { 
            if (!awake && NULL != hTimer) {
                //Set timer
                status = SetWaitableTimer(hTimer, &liDueTime, 0, NULL, NULL, true);
                
                if(!status) {
                            cout << "Error in setting timer!!";
                }
                
                if (WaitForSingleObject(hTimer, INFINITE) != WAIT_OBJECT_0) {
                        cout << "WaitForSingleObject failed :" << GetLastError();
                }
                else {
                     cout << "Timer was signaled" << endl;
                     awake = true;
                     firstCheckOnBattery = true;
                     sleepDueTime.QuadPart = -1000000LL;
                }
            }
        }
        else {
             status = SetWaitableTimer(hTimer, &sleepDueTime, 0, NULL, NULL, true);
              if (!WaitForSingleObject(hTimer, INFINITE) != WAIT_OBJECT_0) {
                  if(firstCheckOnBattery) {
                      cout << "On Battery First" << endl;
                      firstCheckOnBattery = false;
                      sleepDueTime.QuadPart = -(10000000LL) * (60LL) * (3LL);
                      system("start wizmo.exe standby");
                  }
                  else {
                       GetSystemPowerStatus(&sps);
                      if(!((int)sps.ACLineStatus)) { 
                         cout << "On Battery" << endl;
                         system("start wizmo.exe standby");
                      }
                  }
              }
             awake = false;
        }
    }
    
    return EXIT_SUCCESS;
}


