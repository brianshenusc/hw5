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
bool schedule_helper(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched, size_t day, int curr_worker, vector<Worker_T>& worker_shifts, unsigned int shifts_filled);



// bool valid_worker(
// 	size_t row, DailySchedule& sched, 
// 	const size_t maxShifts, vector<Worker_T>& worker_shifts, 
// 	const AvailabilityMatrix& avail, int curr_worker);



void printSchedule(DailySchedule& sched);


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

    for(unsigned int i = 0; i < avail.size(); i++) {
        worker_shifts.push_back(0);
        vector<Worker_T> sched_row;
        for(unsigned int j = 0; j < dailyNeed; j++) {
            sched_row.push_back(INVALID_ID);
        }
        sched.push_back(sched_row);
    }

    if(schedule_helper(avail, dailyNeed, maxShifts, sched, 0, 0, worker_shifts, 0)) {
        return true;
    }
    return false;
}

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

            // cout << "SCHEDULE AFTER: " << day << " " << shifts_filled << endl;
            // printSchedule(sched);
            // cout << endl; 
            
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




// bool valid_worker(
// 	size_t row, DailySchedule& sched, 
// 	const size_t maxShifts, vector<Worker_T>& worker_shifts, 
// 	const AvailabilityMatrix& avail, int curr_worker) {

// 		if(curr_worker == 1) {
// 			cout << "1" << endl;
// 		}
// 		for(size_t i = 0; i < row; i++) {
// 			for(size_t j = 0; j < sched[i].size(); j++) {
// 				// return false if: the worker is unavailable OR has worked too many shifts
// 				if(worker_shifts[curr_worker] >= maxShifts || !avail[i][curr_worker]) {
// 					return false;
// 				}
// 			}
// 		}
// 		return true;
// }





// void printSchedule(DailySchedule& sched) {
// 	for(unsigned int i = 0; i < sched.size(); i++) {
// 		for(unsigned int j = 0; j < sched[i].size(); j++) {
// 			cout << sched[i][j] << " ";
// 		}
// 		cout << endl;
// 	}
// }
