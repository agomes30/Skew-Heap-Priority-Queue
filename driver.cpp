// CMSC 341 - Spring 2020 - Project 4
// driver.cpp: sample driver for the JQueue class.

#include "jqueue.h"

//
// Priority functions compute an integer priority for a job.  Internal
// computations may be floating point, but must return an integer.
//
// Function definitions are below main()
//

int priorityFn1(const job_t &job);
int priorityFn2(const job_t &job);

int main() {

  // Create a job queue with capacity five and priority function
  // priorityFn1()

  JQueue jobQueue(priorityFn1);

  // Create some test jobs and insert them into the queue
  
  job_t testJob1("test1", 50, 3, 11,  16, 1024, 120);
  job_t testJob2("test2", 50, 7,  5, 128,  512, 300);
  job_t testJob3("test3", 20, 3,  0, 128, 1024, 1800);
  job_t testJob4("test1a", 50, 3, 11,  16, 1024, 120);

  jobQueue.insertJob(testJob3);
  jobQueue.insertJob(testJob1);
  jobQueue.insertJob(testJob2);
  jobQueue.insertJob(testJob4);

  // Print the queue, change priority function, print again, change
  // again, print again.  Changing the priority function should cause
  // the heap to be rebuilt.

  std::cout << std::endl;
  jobQueue.printJobQueue();
  std::cout << "Dump the queue:\n";
  jobQueue.dump();
  
  jobQueue.setPriorityFn(priorityFn2);

  std::cout << std::endl;
  jobQueue.printJobQueue();
  std::cout << "Dump the queue:\n";
  jobQueue.dump();
  
  jobQueue.setPriorityFn(priorityFn1);

  std::cout << std::endl;
  jobQueue.printJobQueue();
  std::cout << "Dump the queue:\n";
  jobQueue.dump();

  // Remove and print jobs in priority order

  std::cout << "\nFinal queue in priority order:\n";
  while (jobQueue.numJobs() > 0) {
    job_t job = jobQueue.getNextJob();
    std::cout << "[" << priorityFn1(job) << "] " << job << std::endl;
  }

  return 0;
}

int priorityFn1(const job_t &job) {
  float pri;

  // 100 (regular user) + 100 (regular group) + job priority
  pri = 200 + job._priority;

  // If special user (7 or 23) add 200 
  if (job._user == 7 || job._user == 23) pri += 200;

  // If special group (0 or 11) add 100
  if (job._group == 0 || job._group == 11) pri += 100;

  // Subtract weighted proc, mem, and time
  //   1.0 * proc, 0.01 * mem, (1/3600) * time
  pri = pri - job._proc - 0.01*job._mem - (1.0/3600.0)*job._time;

  return int(pri);
}

int priorityFn2(const job_t&job) {
  return int(job._time);
}