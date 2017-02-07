#include "LaserSensing.h"

LaserSensing::LaserSensing(int max_valid, int min_valid) // Constructor
{
    this->max_valid = max_valid;
    this->min_valid = min_valid;
    this->reading_pointer = 0;
}

void LaserSensing::reading(int data) // Records a reading, and does necessary processing
{
    this->readings[this->reading_pointer] = data;
    this->reading_pointer++;
    // Roll over when we reach the size of the array
    if (this->reading_pointer >= LASER_SENSING_NUMBER_READINGS) {
        this->reading_pointer = 0;
    }
}

int LaserSensing::getValue() {
    unsigned char countValid = 0;
    long sumValid = 0;
    for (int i=0; i<LASER_SENSING_NUMBER_READINGS; i++) {
        if (this->isValid(this->readings[i])) {
            countValid++;
            sumValid += this->readings[i];
        }
    }
    if (0 == countValid) {
        return 0;
    }
    return (int)sumValid/countValid;
}

int LaserSensing::percentValid() {
    unsigned char countValid = 0;
    for (int i=0; i<LASER_SENSING_NUMBER_READINGS; i++) {
        if (this->isValid(this->readings[i])) {
            countValid++;
        }
    }
    return (int)100*countValid/LASER_SENSING_NUMBER_READINGS;
}

bool LaserSensing::isValid(int data) {
    if (data < this->min_valid || data > this->max_valid) {
        return false;
    }
    return true;
}
