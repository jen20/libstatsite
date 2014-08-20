#ifndef METRICS_H
#define METRICS_H
#include <stdint.h>
#include "config.h"
#include "radix.h"
#include "counter.h"
#include "timer.h"
#include "hashmap.h"
#include "set.h"

enum metric_type {
	METRIC_TYPE_UNKNOWN,
	METRIC_TYPE_KEY_VAL,
	METRIC_TYPE_GAUGE,
	METRIC_TYPE_COUNTER,
	METRIC_TYPE_TIMER,
	METRIC_TYPE_SET,
	METRIC_TYPE_GAUGE_DELTA
} metric_type;

struct key_val {
	char *name;
	double val;
	struct key_val *next;
};

struct timer_hist {
	timer tm;

	// Support for histograms
	histogram_config *conf;
	unsigned int *counts;
};

struct gauge {
	double value;
	double prev_value;
	uint64_t user;
};

struct metrics {
	hashmap *counters;           // Hashmap of name -> counter structs
	hashmap *timers;             // Map of name -> timer_hist structs
	hashmap *sets;               // Map of name -> set_t structs
	hashmap *gauges;             // Map of name -> guage struct
	struct key_val *kv_vals;     // Linked list of key_val structs
	double timer_eps;            // The error for timers
	double *quantiles;           // Array of quantiles
	uint32_t num_quants;         // Size of quantiles array
	radix_tree *histograms;      // Radix tree with histogram configs
	unsigned char set_precision; // The precision for sets
};

typedef int (*metric_callback) (void *data, enum metric_type type, char *name, void *val);

/**
 * Initializes the metrics struct.
 * @arg eps The maximum error for the quantiles
 * @arg quantiles A sorted array of double quantile values, must be on (0, 1)
 * @arg num_quants The number of entries in the quantiles array
 * @arg histograms A radix tree with histogram settings. This is not owned
 * by the metrics object. It is assumed to exist for the life of the metrics.
 * @arg set_precision The precision to use for sets
 * @return 0 on success.
 */
int init_metrics(double timer_eps, double *quantiles, uint32_t num_quants, radix_tree * histograms,
	unsigned char set_precision, struct metrics * m);

/**
 * Initializes the metrics struct, with preset configurations.
 * This defaults to a epsilon of 0.01 (1% error), and quantiles at
 * 0.5, 0.95, and 0.99.
 * @return 0 on success.
 */
int init_metrics_defaults(struct metrics * m);

/**
 * Destroys the metrics
 * @return 0 on success.
 */
int destroy_metrics(struct metrics * m);

/**
 * Adds a new sampled value
 * @arg type The type of the metrics
 * @arg name The name of the metric
 * @arg val The sample to add
 * @return 0 on success.
 */
int metrics_add_sample(struct metrics * m, enum metric_type type, char *name, double val);

/**
 * Adds a new gauge value
 * @arg name The name of the metric
 * @arg val The sample to add
 * @arg delta is this a GAUGE or GAUGE_DELTA
 * @arg meta User-specified metadata
 * @return 0 on success.
 */
int metrics_set_gauge(struct metrics * m, char *name, double val, bool delta, uint64_t user);

/**
 * Adds a value to a named set.
 * @arg name The name of the set
 * @arg value The value to add
 * @return 0 on success
 */
int metrics_set_update(struct metrics * m, char *name, char *value);

/**
 * Iterates through all the metrics
 * @arg m The metrics to iterate through
 * @arg data Opaque handle passed to the callback
 * @arg cb A callback function to invoke. Called with a type, name
 * and value. If the type is KEY_VAL, it is a pointer to a double,
 * for a counter, it is a pointer to a counter, and for a timer it is
 * a pointer to a timer. Return non-zero to stop iteration.
 * @return 0 on success.
 */
int metrics_iter(struct metrics * m, void *data, metric_callback cb);

#endif
