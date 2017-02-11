#ifndef LaserSensing_h
#define LaserSensing_h

#define LASER_SENSING_NUMBER_READINGS 50

class LaserSensing
{
    public:
        LaserSensing(int max_valid, int min_valid); // Constructor
        void reading(int data); // Records a reading, and does necessary processing
        int getValue(); // Gets currently assumed valid value
        int percentValid(); // Gets ratio of invalid readings in the current data set
    private:
        int readings[LASER_SENSING_NUMBER_READINGS]; // Array with raw readings
        unsigned char reading_pointer; // Pointer to the next reading to be overwritten
        int max_valid; // Anything above this will be considered invalid
        int min_valid; // Anything below this will be considered invalid
        bool isValid(int data); // Checks if data point is valid
};

#endif
