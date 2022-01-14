// ================================ Imports ================================ //

#include <vector>
#include <algorithm>
#include <functional>

// ========================== Autonomous Utilities ========================== //

namespace autonutils
{
    enum FieldOrigin
    {
        Left,
        Right,
        Both
    };

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
        RoutineManager() {}

        std::vector<int> find(int s)
        {
            std::vector<int> matches;
            for (Routine &r : routines)
            {
                if (r.side == s)
                    matches.push_back(r.id);
            }
            return matches;
        }

        void add(int i, int s, std::function<void()> a)
        {
            Routine r;
            r.id = i, r.side = s;
            r.action = a;
            routines.push_back(r);
        }

        void exec(int i)
        {
            for (Routine &r : routines)
            {
                if (r.id == i)
                    r.action();
            }
        }
    };
}