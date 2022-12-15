#include <stdlib.h>
#include <stdio.h>
#include <math.h>

struct sensor_t {
    signed long long x;
    signed long long y;
    signed long long distance;
};

struct beacon_t {
    signed long long x;
    signed long long y;
};

struct bounds_t {
    signed long long min_col;
    signed long long min_row;
    signed long long max_col;
    signed long long max_row;
};

struct tunnels_t {
    size_t num_sensors;
    size_t num_beacons;
    struct sensor_t **sensors;
    struct beacon_t **beacons;
    struct bounds_t *bounds;
};

void free_sensors(struct sensor_t **sensors, size_t num_sensors) {
    for (size_t i = 0; i < num_sensors; i++) {
        free(sensors[i]);
    }
    free(sensors);
}

void free_beacons(struct beacon_t **beacons, size_t num_beacons) {
    for (size_t i = 0; i < num_beacons; i++) {
        free(beacons[i]);
    }
    free(beacons);
}

void update_bounds(struct bounds_t *bounds, signed long long x, signed long long y) {
    if (x < bounds->min_col) {
        bounds->min_col = x;
    } else if (x > bounds->max_col) {
        bounds->max_col = x;
    }
    if (y < bounds->min_row) {
        bounds->min_row = y;
    } else if (y > bounds->max_row) {
        bounds->max_row = y;
    }
}

signed long long get_manhattan_distance_between(signed long long x0, signed long long y0, signed long long x1, signed long long y1) {
    return fabs(x0 - x1) + fabs(y0 - y1);
}

int beacon_present(struct beacon_t **beacons, size_t num_beacons, signed long long x, signed long long y) {
    for (size_t i = 0; i < num_beacons; i++) {
        if (beacons[i]->y == y && beacons[i]->x == x) {
            return 1;
        }
    }

    return 0;
}

void parse_tunnels(struct tunnels_t *tunnels) {
    signed long long x0, y0, x1, y1;
    struct sensor_t *sensor;
    struct beacon_t *beacon;

    while (scanf(" Sensor at x=%lld, y=%lld: closest beacon is at x=%lld, y=%lld", &x0, &y0, &x1, &y1) == 4) {
        tunnels->num_sensors++;
        tunnels->sensors = (struct sensor_t **) realloc(tunnels->sensors, tunnels->num_sensors * sizeof(struct sensor_t *));
        sensor = (struct sensor_t *) calloc(1, sizeof(struct sensor_t));
        sensor->x = x0;
        sensor->y = y0;
        sensor->distance = get_manhattan_distance_between(x0, y0, x1, y1);
        tunnels->sensors[tunnels->num_sensors - 1] = sensor;

        if (!beacon_present(tunnels->beacons, tunnels->num_beacons, x1, y1)) {
            tunnels->num_beacons++;
            tunnels->beacons = (struct beacon_t **) realloc(tunnels->beacons, tunnels->num_beacons * sizeof(struct beacon_t *));
            beacon = (struct beacon_t *) calloc(1, sizeof(struct beacon_t));
            beacon->x = x1;
            beacon->y = y1;
            tunnels->beacons[tunnels->num_beacons - 1] = beacon;
        }

        update_bounds(tunnels->bounds, x0 - sensor->distance, y0 - sensor->distance);
        update_bounds(tunnels->bounds, x0 + sensor->distance, y0 + sensor->distance);
        update_bounds(tunnels->bounds, x1, y1);
    }
}

unsigned long long empty_positions_at_row(struct tunnels_t *tunnels, signed long long row) {
    unsigned long long empty_positions = 0;
    size_t num_sensors = tunnels->num_sensors;
    size_t num_beacons = tunnels->num_beacons;
    struct sensor_t **sensors = tunnels->sensors;
    struct beacon_t **beacons = tunnels->beacons;
    struct bounds_t *bounds = tunnels->bounds;

    for (signed long long x = bounds->min_col; x <= bounds->max_col; x++) {
        for (size_t i = 0; i < num_sensors; i++) {
            if (get_manhattan_distance_between(sensors[i]->x, sensors[i]->y, x, row) <= sensors[i]->distance) {
                if (beacon_present(beacons, num_beacons, x, row)) {
                    break;
                }
                empty_positions++;
                break;
            }
        }
    }

    return empty_positions;
}

int main(void) {
    signed long long row_to_check;

    if (scanf("%lld", &row_to_check) != 1) {
        exit(1);
    }

    struct bounds_t bounds = { .min_col = 0, .min_row = 0, .max_col = 0, .max_row = 0};
    struct tunnels_t tunnels = { .num_sensors = 0, .num_beacons = 0, .sensors = NULL, .beacons = NULL, .bounds = &bounds};

    parse_tunnels(&tunnels);
    unsigned long long empty_positions = empty_positions_at_row(&tunnels, row_to_check);

    printf("there were %llu empty position at row %lld\n", empty_positions, row_to_check);

    free_sensors(tunnels.sensors, tunnels.num_sensors);
    free_beacons(tunnels.beacons, tunnels.num_beacons);
}