#ifndef _AUTONUTILS_
#define _AUTONUTILS_

// ================================ Imports ================================ //

#include <vector>
#include <algorithm>
#include <functional>

// ====================== Autonomous Manager (Headers) ====================== //

namespace autonutils
{
    /**
     * @brief Side the autonomous routine starts on
     */
    enum FieldOrigin
    {
        Left,
        Right,
        Both
    };

    /**
     * @brief Routine struct
     */
    struct Routine
    {
        int id;
        int side;
        std::function<void()> action;
    };

    class RoutineManager
    {
    private:
        std::vector<Routine> routines;

    public:
        RoutineManager(){};

        /**
         * @brief Find ids of routines that start from a specified side
         * @param s Side
         * @returns A vector of ids
         */
        std::vector<int> find(int s);

        /**
         * @brief Add a routine to storage
         * @param i Id of routine
         * @param s Side routine starts on
         * @param a Routine action
         */
        void add(int i, int s, std::function<void()> a);

        /**
         * @brief Run a routine with id
         * @param i Id of routine to run
         */
        void exec(int i);
    };
}

// ========================================================================= //

#endif // _AUTONUTILS_