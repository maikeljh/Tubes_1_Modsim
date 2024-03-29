/*
    Tugas 1 Pemodelan dan Simulasi
    Arleen Chrysantha Gunardi 13521059
    Michael Jonathan Halim 13521124
    Marcel Ryan Antony 13521127
*/

/* External definitions for car-rental model. */

#include "simlib.h" /* Required for use of simlib.c. */
#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define EVENT_ARRIVAL_PASSENGER_1   1      /* Event type for arrival of a person to the air terminal 1. */
#define EVENT_ARRIVAL_PASSENGER_2   2      /* Event type for arrival of a person to the air terminal 2. */
#define EVENT_ARRIVAL_PASSENGER_3   3      /* Event type for arrival of a person to the car rental */
#define EVENT_ARRIVAL_BUS_1         4      /* Event type for arrival of a bus to the air terminal 1. */
#define EVENT_ARRIVAL_BUS_2         5      /* Event type for arrival of a bus to the air terminal 2. */
#define EVENT_ARRIVAL_BUS_3         6      /* Event type for arrival of a bus to the car rental. */
#define EVENT_DEPARTURE_1           7      /* Event type for departure of a person from the bus in air terminal 1. */
#define EVENT_DEPARTURE_2           8      /* Event type for departure of a person from the bus in air terminal 2. */
#define EVENT_DEPARTURE_3           9      /* Event type for departure of a person from the bus in car rental. */
#define EVENT_END_SIMULATION        10     /* Event type for end of the simulation. */

#define STREAM_INTERARRIVAL_TERMINAL_1    1  /* Random-number stream for interarrivals in air terminal 1 */
#define STREAM_INTERARRIVAL_TERMINAL_2    2  /* Random-number stream for interarrivals in air terminal 2 */
#define STREAM_INTERARRIVAL_CAR_RENTAL    3  /* Random-number stream for interarrivals in car_rental */
#define STREAM_UNLOADING_TIMES            4  /* Random-number stream for unloading times. */
#define STREAM_LOADING_TIMES              5  /* Random-number stream for loading times. */
#define STREAM_DESTINATION                6  /* Random-number stream for determining destination. */ 
#define MAX_NUM_LOCATIONS                 3  /* Maximum number of locations. */

#define BUS        0    /* Index for bus */
#define TERMINAL_1 1    /* Index for terminal 1 */
#define TERMINAL_2 2    /* Index for terminal 2 */
#define CAR_RENTAL 3    /* Index for car rental */

#define DESTINATION_PROBABILITY_TERMINAL_1 0.583        /* Probability of destination to air terminal 1 */
#define DESTINATION_PROBABILITY_TERMINAL_2 0.417        /* Probability of destination to air terminal 2 */
#define TIME_TERMINAL_1_2                  (1/30.0)     /* Time to commute from air terminal 1 to air terminal 2 (hour) */
#define TIME_TERMINAL_2_3                  (4.5/30)     /* Time to commute from air terminal 2 to car rental (hour) */
#define TIME_TERMINAL_3_1                  (4.5/30)     /* Time to commute from car rental to air terminal 1 (hour) */
#define BUS_CAPACITY                       20           /* Bus Capacity */

#define INTERARRIVAL_TIME_RATE_1 (1/14.0)              /* Rate per hour */
#define INTERARRIVAL_TIME_RATE_2 (1/10.0)              /* Rate per hour */
#define INTERARRIVAL_TIME_RATE_3 (1/24.0)              /* Rate per hour */
#define UNLOAD_TIME_MIN          (16/3600.0)           /* Minimum time to unload one person (hour) */
#define UNLOAD_TIME_MAX          (24/3600.0)           /* Maximum time to unload one person (hour) */
#define LOAD_TIME_MIN            (15/3600.0)           /* Minimum time to load one person (hour) */
#define LOAD_TIME_MAX            (25/3600.0)           /* Maximum time to load one person (hour) */
#define STOP_TIME_MIN            (5/60.0)              /* Minimum time to spend at each location (hour) */
#define SIMULATION_TIME          80                    /* Simulation duration (hour) */

#define TERMINAL_1_QUEUE     1             /* Index of list_file for element terminal 1 queue */
#define TERMINAL_2_QUEUE     2             /* Index of list_file for element terminal 2 queue */
#define CAR_RENTAL_QUEUE     3             /* Index of list_file for element car rental queue */
#define BUS_QUEUE_TERMINAL_1 4             /* Index of list_file for element bus queue from arrival at terminal 1 */
#define BUS_QUEUE_TERMINAL_2 5             /* Index of list_file for element bus queue from arrival at terminal 2 */
#define BUS_QUEUE_CAR_RENTAL 6             /* Index of list_file for element bus queue from arrival at car rental */

#define STATS_NUMBER_IN_TERMINAL_1_QUEUE 1  /* timest */
#define STATS_NUMBER_IN_TERMINAL_2_QUEUE 2  /* timest */
#define STATS_NUMBER_IN_TERMINAL_3_QUEUE 3  /* timest */
#define STATS_DELAY_IN_TERMINAL_1_QUEUE  4  /* sampst */
#define STATS_DELAY_IN_TERMINAL_2_QUEUE  5  /* sampst */
#define STATS_DELAY_IN_TERMINAL_3_QUEUE  6  /* sampst */
#define STATS_NUMBER_ON_THE_BUS          7  /* timest */
#define STATS_BUS_STOP_AT_TERMINAL_1     8  /* sampst */
#define STATS_BUS_STOP_AT_TERMINAL_2     9  /* sampst */
#define STATS_BUS_STOP_AT_TERMINAL_3     10 /* sampst */
#define STATS_TIME_LOOP                  11 /* sampst */
#define STATS_TIME_PERSON_TERMINAL_1     12 /* sampst */
#define STATS_TIME_PERSON_TERMINAL_2     13 /* sampst */
#define STATS_TIME_PERSON_TERMINAL_3     14 /* sampst */

double bus_arrival_1 = 0, bus_arrival_2 = 0, bus_arrival_3 = 0, arrival_time_1 = 0, arrival_time_2 = 0, arrival_time_3 = 0; /* Time bus arrived at terminal 1, 2, or car rental */
double loop_time = 0;  /* Time for the bus to take a loop */

FILE *outfile; /* Output Stats Result */

void arrive(int arrival_type){
    if (arrival_type < 4){
        // Passenger arrival
        switch (arrival_type)
        {
        case 1:
            event_schedule(sim_time + expon(INTERARRIVAL_TIME_RATE_1, STREAM_INTERARRIVAL_TERMINAL_1), arrival_type);
            transfer[1] = sim_time;
            list_file(LAST, TERMINAL_1_QUEUE);
            timest((double) list_size[TERMINAL_1_QUEUE], STATS_NUMBER_IN_TERMINAL_1_QUEUE);
            break;
        case 2:
            event_schedule(sim_time + expon(INTERARRIVAL_TIME_RATE_2, STREAM_INTERARRIVAL_TERMINAL_2), arrival_type);
            transfer[1] = sim_time;
            list_file(LAST, TERMINAL_2_QUEUE);
            timest((double) list_size[TERMINAL_2_QUEUE], STATS_NUMBER_IN_TERMINAL_2_QUEUE);
            break;
        case 3:
            event_schedule(sim_time + expon(INTERARRIVAL_TIME_RATE_3, STREAM_INTERARRIVAL_CAR_RENTAL), arrival_type);
            transfer[1] = sim_time;
            list_file(LAST, CAR_RENTAL_QUEUE);
            timest((double) list_size[CAR_RENTAL_QUEUE], STATS_NUMBER_IN_TERMINAL_3_QUEUE);
            break;
        default:
            break;
        }
    } else {
        double load_time, unload_time, person_time, destination_prob;

        printf("Arrival %d: %.2f\n", arrival_type, sim_time);

        switch (arrival_type)
        {
        case 4:
            if (list_size[BUS_QUEUE_TERMINAL_1] > 0){
                // Unload
                list_remove(FIRST, BUS_QUEUE_TERMINAL_1);
                unload_time = uniform(UNLOAD_TIME_MIN, UNLOAD_TIME_MAX, STREAM_UNLOADING_TIMES);
                person_time = sim_time - transfer[1] + transfer[3] + unload_time;

                printf("Sim time: %.2f\nTransfer 1 time: %.2f\nTransfer 3 time: %.2f\n", sim_time, transfer[1], transfer[3]);
                printf("Person time: %.2f\n", person_time);
                sampst(person_time, STATS_TIME_PERSON_TERMINAL_3);
                
                arrival_time_1 += unload_time;

                event_schedule(sim_time + unload_time, arrival_type);

                timest((double) (list_size[BUS_QUEUE_TERMINAL_1] + list_size[BUS_QUEUE_TERMINAL_2] + list_size[BUS_QUEUE_CAR_RENTAL]), STATS_NUMBER_ON_THE_BUS);
                return;
            }

            // Load if bus size < capacity
            if (list_size[BUS_QUEUE_TERMINAL_1] + list_size[BUS_QUEUE_TERMINAL_2] + list_size[BUS_QUEUE_CAR_RENTAL] < BUS_CAPACITY && list_size[TERMINAL_1_QUEUE] > 0){
                list_remove(FIRST, TERMINAL_1_QUEUE);

                load_time = uniform(LOAD_TIME_MIN, LOAD_TIME_MAX, STREAM_LOADING_TIMES);
                sampst(sim_time + load_time - transfer[1], STATS_DELAY_IN_TERMINAL_1_QUEUE);
                transfer[3] = sim_time + load_time - transfer[1];
                transfer[1] = sim_time + load_time;
                transfer[2] = CAR_RENTAL;
                transfer[4] = TERMINAL_1;

                printf("Load time: %.2f\n", transfer[1]);
                arrival_time_1 += load_time;

                list_file(LAST, BUS_QUEUE_CAR_RENTAL);
                event_schedule(sim_time + load_time, arrival_type);

                timest((double) (list_size[BUS_QUEUE_TERMINAL_1] + list_size[BUS_QUEUE_TERMINAL_2] + list_size[BUS_QUEUE_CAR_RENTAL]), STATS_NUMBER_ON_THE_BUS);
                return;
            }

            // If no loading and unloading after 5 mins, bus leaving
            if (arrival_time_1 >= STOP_TIME_MIN) {
                event_schedule(sim_time, EVENT_DEPARTURE_1);
                break;
            } else {
                event_schedule(sim_time + STOP_TIME_MIN - arrival_time_1, EVENT_DEPARTURE_1);
                break;
            }
            break;
            
        case 5:
            if (list_size[BUS_QUEUE_TERMINAL_2] > 0){
                // Unload
                list_remove(FIRST, BUS_QUEUE_TERMINAL_2);
                unload_time = uniform(UNLOAD_TIME_MIN, UNLOAD_TIME_MAX, STREAM_UNLOADING_TIMES);
                person_time = sim_time - transfer[1] + transfer[3] + unload_time;
                printf("Sim time: %.2f\nTransfer 1 time: %.2f\nTransfer 3 time: %.2f\n", sim_time, transfer[1], transfer[3]);
                printf("Person time: %.2f\n", person_time);
                sampst(person_time, STATS_TIME_PERSON_TERMINAL_3);

                arrival_time_2 += unload_time;
                event_schedule(sim_time + unload_time, arrival_type);

                timest((double) (list_size[BUS_QUEUE_TERMINAL_1] + list_size[BUS_QUEUE_TERMINAL_2] + list_size[BUS_QUEUE_CAR_RENTAL]), STATS_NUMBER_ON_THE_BUS);
                return;
            }

            // Load if bus size < capacity
            if (list_size[BUS_QUEUE_TERMINAL_1] + list_size[BUS_QUEUE_TERMINAL_2] + list_size[BUS_QUEUE_CAR_RENTAL] < BUS_CAPACITY && list_size[TERMINAL_2_QUEUE] > 0){
                list_remove(FIRST, TERMINAL_2_QUEUE);

                load_time = uniform(LOAD_TIME_MIN, LOAD_TIME_MAX, STREAM_LOADING_TIMES);
                sampst(sim_time + load_time - transfer[1], STATS_DELAY_IN_TERMINAL_2_QUEUE);
                transfer[3] = sim_time + load_time - transfer[1];
                transfer[1] = sim_time + load_time;
                transfer[2] = CAR_RENTAL;
                transfer[4] = TERMINAL_2;

                arrival_time_2 += load_time;
                list_file(LAST, BUS_QUEUE_CAR_RENTAL);
                event_schedule(sim_time + load_time, arrival_type);

                timest((double) (list_size[BUS_QUEUE_TERMINAL_1] + list_size[BUS_QUEUE_TERMINAL_2] + list_size[BUS_QUEUE_CAR_RENTAL]), STATS_NUMBER_ON_THE_BUS);
                return;
            }

            // If no loading and unloading after 5 mins, bus leaving
            if (arrival_time_2 >= STOP_TIME_MIN) {
                event_schedule(sim_time, EVENT_DEPARTURE_2);
                break;
            } else {
                event_schedule(sim_time + STOP_TIME_MIN - arrival_time_2, EVENT_DEPARTURE_2);
                break;
            }
            break;
        
        case 6:
            if (list_size[BUS_QUEUE_CAR_RENTAL] > 0) {
                // Unload passengers
                list_remove(FIRST, BUS_QUEUE_CAR_RENTAL);
                unload_time = uniform(UNLOAD_TIME_MIN, UNLOAD_TIME_MAX, STREAM_UNLOADING_TIMES);
                person_time = sim_time - transfer[1] + transfer[3] + unload_time;
                printf("Sim time: %.2f\nTransfer 1 time: %.2f\nTransfer 3 time: %.2f\n", sim_time, transfer[1], transfer[3]);
                printf("Person time: %.2f\n", person_time);
                if (transfer[4] == 1) {
                    sampst(person_time, STATS_TIME_PERSON_TERMINAL_1);
                }
                else {
                    sampst(person_time, STATS_TIME_PERSON_TERMINAL_2);
                }

                arrival_time_3 += unload_time;
                event_schedule(sim_time + unload_time, arrival_type);

                timest((double) (list_size[BUS_QUEUE_TERMINAL_1] + list_size[BUS_QUEUE_TERMINAL_2] + list_size[BUS_QUEUE_CAR_RENTAL]), STATS_NUMBER_ON_THE_BUS);
                return;
            }

            // Load if bus size < capacity
            if (list_size[BUS_QUEUE_TERMINAL_1] + list_size[BUS_QUEUE_TERMINAL_2] + list_size[BUS_QUEUE_CAR_RENTAL] < BUS_CAPACITY && list_size[CAR_RENTAL_QUEUE] > 0){
                list_remove(FIRST, CAR_RENTAL_QUEUE);

                load_time = uniform(LOAD_TIME_MIN, LOAD_TIME_MAX, STREAM_LOADING_TIMES);
                sampst(sim_time + load_time - transfer[1], STATS_DELAY_IN_TERMINAL_3_QUEUE);
                transfer[3] = sim_time + load_time - transfer[1];
                transfer[1] = sim_time + load_time;
                transfer[4] = CAR_RENTAL;

                arrival_time_3 += load_time;
                
                destination_prob = uniform(0, 1, STREAM_DESTINATION);

                if (destination_prob <= DESTINATION_PROBABILITY_TERMINAL_1) {
                    transfer[2] = TERMINAL_1;
                    list_file(LAST, BUS_QUEUE_TERMINAL_1);
                } else {
                    transfer[2] = TERMINAL_2;
                    list_file(LAST, BUS_QUEUE_TERMINAL_2);
                }

                event_schedule(sim_time + load_time, arrival_type);

                timest((double) (list_size[BUS_QUEUE_TERMINAL_1] + list_size[BUS_QUEUE_TERMINAL_2] + list_size[BUS_QUEUE_CAR_RENTAL]), STATS_NUMBER_ON_THE_BUS);
                return;
            }

            // If no loading and unloading after 5 mins, bus leaving
            if (arrival_time_3 >= STOP_TIME_MIN) {
                event_schedule(sim_time, EVENT_DEPARTURE_3);
                break;
            } else {
                event_schedule(sim_time + STOP_TIME_MIN - - arrival_time_3, EVENT_DEPARTURE_3);
                break;
            }
            break;

        default:
            break;
        }
    }

};

void depart(int departure_type){
    printf("Departure %d: %.2f\n", departure_type, sim_time);
    switch (departure_type)
    {
    case 7:
        event_schedule(sim_time + TIME_TERMINAL_1_2, EVENT_ARRIVAL_BUS_2);
        sampst(sim_time - bus_arrival_1, STATS_BUS_STOP_AT_TERMINAL_1);
        bus_arrival_2 = sim_time + TIME_TERMINAL_1_2;
        arrival_time_1 = 0;
        break;
    case 8:
        event_schedule(sim_time + TIME_TERMINAL_2_3, EVENT_ARRIVAL_BUS_3);
        sampst(sim_time - bus_arrival_2, STATS_BUS_STOP_AT_TERMINAL_2);
        bus_arrival_3 = sim_time + TIME_TERMINAL_2_3;
        arrival_time_2 = 0;
        break;
    case 9:
        event_schedule(sim_time + TIME_TERMINAL_3_1, EVENT_ARRIVAL_BUS_1);
        sampst(sim_time - bus_arrival_3, STATS_BUS_STOP_AT_TERMINAL_3);
        bus_arrival_1 = sim_time + TIME_TERMINAL_3_1;
        sampst(sim_time - loop_time, STATS_TIME_LOOP);
        loop_time = sim_time;
        arrival_time_3 = 0;
        break;
    
    default:
        break;
    }
};


void report(void) {
    fprintf(outfile, "Car Rental Simulation Report (80 hours)\n");
    fprintf(outfile, "\n--------------------------------------------------------------------------------------------\n");

    out_sampst(outfile, 1, 14);
    out_timest(outfile, 1, 14);
}

int main() {
    /* Initialize simlib */
    init_simlib();

    /* Open output file */
    outfile = fopen("car-rental-sim.out", "w");

    /* Set maxatr = max(maximum number of attributes per record, 4) */
    maxatr = 4; /* NEVER SET maxatr TO BE SMALLER THAN 4. */

    /* Schedule the arrival of the first person in air terminal 1, air terminal 2, and car rental.PASSENGER_ */
    event_schedule(expon(INTERARRIVAL_TIME_RATE_1, STREAM_INTERARRIVAL_TERMINAL_1), EVENT_ARRIVAL_PASSENGER_1);
    event_schedule(expon(INTERARRIVAL_TIME_RATE_1, STREAM_INTERARRIVAL_TERMINAL_2), EVENT_ARRIVAL_PASSENGER_2);
    event_schedule(expon(INTERARRIVAL_TIME_RATE_3, STREAM_INTERARRIVAL_CAR_RENTAL), EVENT_ARRIVAL_PASSENGER_3);
    event_schedule(0, EVENT_DEPARTURE_3);

    /* Schedule the end of the simulation.  (This is needed for consistency of
       units.) */
    event_schedule(SIMULATION_TIME, EVENT_END_SIMULATION);

    /* Run the simulation until it terminates after an end-simulation event */
    do {

        /* Determine the next event. */

        timing();

        /* Invoke the appropriate event function. */

        switch (next_event_type)
        {
        case EVENT_ARRIVAL_PASSENGER_1:
            arrive(1);
            break;
        case EVENT_ARRIVAL_PASSENGER_2:
            arrive(2);
            break;
        case EVENT_ARRIVAL_PASSENGER_3:
            arrive(3);
            break;
        case EVENT_ARRIVAL_BUS_1:
            arrive(4);
            break;
        case EVENT_ARRIVAL_BUS_2:
            arrive(5);
            break;
        case EVENT_ARRIVAL_BUS_3:
            arrive(6);
            break;
        case EVENT_DEPARTURE_1:
            depart(7);
            break;
        case EVENT_DEPARTURE_2:
            depart(8);
            break;
        case EVENT_DEPARTURE_3:
            depart(9);
            break;
        case EVENT_END_SIMULATION:
            report();
            break;
        }
    } while (next_event_type != EVENT_END_SIMULATION);

    /* If the event just executed was not the end-simulation event (type
       EVENT_END_SIMULATION), continue simulating.  Otherwise, end the
       simulation. */

    fclose(outfile);

    return 0;
}
