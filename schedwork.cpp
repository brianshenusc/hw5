#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#endif

// add or remove necessary headers as you please
#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1; 


// Add prototypes for any helper functions here

// helper function that recursively fills out the schedule
bool schedule_helper(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched, size_t day, int curr_worker, vector<Worker_T>& worker_shifts, unsigned int shifts_filled);


// Add your implementation of schedule() and other helper functions here



bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below
    vector<Worker_T> worker_shifts;
		// sets up the schedule 
    for(unsigned int i = 0; i < avail.size(); i++) {
        worker_shifts.push_back(0);
        vector<Worker_T> sched_row;
        for(unsigned int j = 0; j < dailyNeed; j++) {
            sched_row.push_back(INVALID_ID);
        }
        sched.push_back(sched_row);
    }
		// call to recursive function
    if(schedule_helper(avail, dailyNeed, maxShifts, sched, 0, 0, worker_shifts, 0)) {
        return true;
    }
    return false;
}


// helper function that recursively fills out the schedule
bool schedule_helper(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched, size_t day, int curr_worker, vector<Worker_T>& worker_shifts, unsigned int shifts_filled) {
      // base case if its the last day, then it ends
    if(day == sched.size()) {
        return true;
    }
		// loops through the workers
    for(size_t i = shifts_filled; i < avail[0].size(); i++) {
		// if the current worker is valid on the current day and no duplicate workers on a day
	    if(worker_shifts[i] < maxShifts && avail[day][i] && find(sched[day].begin(), sched[day].end(), i) == sched[day].end()) {
				// adds the worker to the schedule and update information
				sched[day][shifts_filled] = i;
				worker_shifts[i]++;
				// if the recursive call is true, return true
				if(shifts_filled < sched[0].size()-1) {
					// if there is still a shift to be shift to be filled
					if(schedule_helper(avail, dailyNeed, maxShifts, sched, day, i, worker_shifts, shifts_filled+1)) {
						return true;
					}
				}
				else if(schedule_helper(avail, dailyNeed, maxShifts, sched, day+1, i, worker_shifts, 0)) {
					return true;
				}
				// otherwise remove the worker from the schedule
				sched[day][shifts_filled] = INVALID_ID;
				worker_shifts[i]--;
			}
		}
		return false;
}
