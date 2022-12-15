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

void get_position(struct tunnels_t *tunnels, signed long long *pos_x, signed long long *pos_y) {
    size_t num_sensors = tunnels->num_sensors;
    struct sensor_t **sensors = tunnels->sensors;

    for (signed long long x = 0; x <= 4000000; x++) {
        for (signed long long y = 0; y <= 4000000; y++) {
            for (size_t i = 0; i < num_sensors; i++) {
                if (get_manhattan_distance_between(sensors[i]->x, sensors[i]->y, x, y) <= sensors[i]->distance) {
                    *pos_x = x;
                    *pos_y = y;
                }
            }
        }
    }
}

int main(void) {
    signed long long row_to_check;

    if (scanf("%lld", &row_to_check) != 1) {
        exit(1);
    }

    struct bounds_t bounds = { .min_col = 0, .min_row = 0, .max_col = 0, .max_row = 0};
    struct tunnels_t tunnels = { .num_sensors = 0, .num_beacons = 0, .sensors = NULL, .beacons = NULL, .bounds = &bounds};

    parse_tunnels(&tunnels);
    signed long long x = 0, y = 0;
    get_position(&tunnels, &x, &y);

    printf("ther distress beacon's position is at x=%lld, y=%lld\n", x, y);

    free_sensors(tunnels.sensors, tunnels.num_sensors);
    free_beacons(tunnels.beacons, tunnels.num_beacons);
}