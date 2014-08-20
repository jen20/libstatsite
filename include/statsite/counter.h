#ifndef COUNTER_H
#define COUNTER_H
#include <stdint.h>

struct counter {
	uint64_t count;				// Count of items
	double sum;					// Sum of the values
	double squared_sum;			// Sum of the squared values
	double min;					// Minimum value
	double max;					// Maximum value
};

/**
 * Initializes the counter struct
 * @arg counter The counter struct to initialize
 * @return 0 on success.
 */
int init_counter(struct counter *counter);

/**
 * Adds a new sample to the struct
 * @arg counter The counter to add to
 * @arg sample The new sample value
 * @return 0 on success.
 */
int counter_add_sample(struct counter *counter, double sample);

/**
 * Returns the number of samples in the counter
 * @arg counter The counter to query
 * @return The number of samples
 */
uint64_t counter_count(struct counter *counter);

/**
 * Returns the mean counter value
 * @arg counter The counter to query
 * @return The mean value
 */
double counter_mean(struct counter *counter);

/**
 * Returns the sample standard deviation counter value
 * @arg counter The counter to query
 * @return The sample standard deviation
 */
double counter_stddev(struct counter *counter);

/**
 * Returns the sum of the counter
 * @arg counter The counter to query
 * @return The sum of values
 */
double counter_sum(struct counter *counter);

/**
 * Returns the sum squared of the counter
 * @arg counter The counter to query
 * @return The sum squared of values
 */
double counter_squared_sum(struct counter *counter);

/**
 * Returns the minimum value of the counter
 * @arg counter The counter to query
 * @return The minimum value.
 */
double counter_min(struct counter *counter);

/**
 * Returns the maximum value of the counter
 * @arg counter The counter to query
 * @return The maximum value.
 */
double counter_max(struct counter *counter);

#endif
