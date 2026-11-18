/**
 * @file dateTime.h
 * @brief Defines date and time utilities for the text editor project.
 * 
 * Provides functions and structures to handle simple date and time formatting,
 * conversions, and utilities that can be used throughout the program.
 */

#pragma once

#include <stdbool.h>

/**
 * @brief Structure representing a date and time.
 *
 * This structure holds day, month, year, hour, minute, and second fields,
 * used for tracking document timestamps such as load and modification times.
 */
typedef struct {
    int day;
    int month;
    int year;
    int hour;
    int minute;
    int second;
} DateTime;

/**
 * @brief Prints the contents of a DateTime structure in DD/MM/YYYY HH:MM:SS format.
 * 
 * @param dt The DateTime structure to print.
 */
void dateTimePrint(DateTime dt);

/**
 * @brief Creates and returns a DateTime structure with the current local time.
 * 
 * Uses the system clock to populate a DateTime with the current date and time.
 * 
 * @return A DateTime structure with the current timestamp.
 */
DateTime dateTimeCreate();

/**
 * @brief Checks whether a DateTime structure is empty (i.e., uninitialized).
 * 
 * A DateTime is considered empty if all its fields are zero.
 * 
 * @param dt The DateTime structure to check.
 * @return true if all fields are zero, false otherwise.
 */
bool dateTimeIsEmpty(DateTime dt);