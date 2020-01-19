#include <iostream>
#include <png++/png.hpp>
/**
 * 
 */
class Simulation {
public:
    Simulation(
        int save_interval,
        int max_duration
    ) : _save_interval(save_interval), _max_duration(max_duration)
    {}

    /**
     * Runs computation for and generates next image of the simulation.
     * Every frame must be generated because n-th frame computationaly depends
     * on n-1- th frame. Also because we may show every 5th frame but save every
     * 100 th.
     */
    void nextFrame() {
        // std::cout << "racunam\n";
    }
private:
    /**
     * Interval which defines how many frames will not be saved on disk.
     * After that many frames we save a picture on disk which can be used
     * for forward and backward, pause and play and resuming computation from
     * previously computed image
     */
    int _save_interval;
    /**
     * Defines after how many years simulation stops.
     */
    int _max_duration;

    /**
     * One frame represents 100 years. This must not be changed significatntly
     * because rules that simulation utilize are largely dependent on this duration.
     */
    static const int frame_duration = 100;

    png::image< png::rgba_pixel > lastFrame;


};