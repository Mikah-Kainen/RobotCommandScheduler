#include "TryCatchGroup.h"


/*
TryCatchGroup behavior simple:

Will run group:
if return true -> return true

if return false -> check each catch condition *In The Order They Were Added*
	if condition met -> return true and enable catch group for that condition
	if condition not met -> check next condition
	if no conditions met -> return false to re-run

*/

//Maybe add a TryCatchContinue that continues with the original group after catching and correcting an error
//TryCatchContinue is remarkably dangerous, because it requires the user to guarentee that the Catch ends exactly where the Try would want/that the Catch doesn't unnecessarily change the state of the robot and ruin the Continue
//Ok TryCatchContinue is also pretty useless sooooooooo... probably not gonna implement it

//TryCatch group should just change what the ID is mapping to:
/*
Initialize starts mapping to the main Scheduleable with a switch in the end mapping to other Scheduleables for the catch
Possible TryCatchContinue variation can just have the other Scheduleables map back to the original
*/