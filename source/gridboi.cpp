/*
Using the Bellman class to solve the Grid-Boi Markov decision process.
*/

////////////////////////////////////////////////// DEPENDENCIES

#include "bellman.hpp"
using namespace bellman;

////////////////////////////////////////////////// CORE

class GridBoi : public Bellman {
    // Grid dimensions
    uint const nX;
    uint const nY;

    // State space
    struct State {
        struct Coord {
            int x;
            int y;
            bool operator==(Coord const& other) const {
                return (x == other.x) and (y == other.y);
            }
            bool operator!=(Coord const& other) const {
                return (x != other.x) or (y != other.y);
            }
            Coord up() const {
                Coord other(*this);
                other.y++;
                return other;
            }
            Coord down() const {
                Coord other(*this);
                other.y--;
                return other;
            }
            Coord left() const {
                Coord other(*this);
                other.x--;
                return other;
            }
            Coord right() const {
                Coord other(*this);
                other.x++;
                return other;
            }
        };
        Coord boi;
        Coord gob;
        Coord goo;
    };
    Vector<State> state_space;

    // Action space
    enum Action {WAIT, UP, DOWN, LEFT, RIGHT};

public:
    GridBoi(uint nX=5, uint nY=5) :
        //            nS      nA   g
        Bellman(pow(nX*nY, 3), 5, 0.9),
        nX(nX),
        nY(nY),
        state_space(nS) {
        // Enumerate state space
        for(uint i=0; i<nS; ++i) {
            Vector<uint> const coords = coords_from_index(i, {nX, nY, nX, nY, nX, nY});
            state_space[i].boi.x = coords[0];
            state_space[i].boi.y = coords[1];
            state_space[i].gob.x = coords[2];
            state_space[i].gob.y = coords[3];
            state_space[i].goo.x = coords[4];
            state_space[i].goo.y = coords[5];
        }
    }

    // Returns the probability of transitioning to state s1 given state s and action a
    Real dynamic(Index s_index, Index a, Index s1_index) const {
        State const s = state_space[s_index];
        State const s1 = state_space[s1_index];
        Real p = 1.0;
        // Evaluate validity of boi move
        if(a == Action::WAIT) {
            // Stand still
            if(s1.boi != s.boi) return 0.0;
        }
        else if(a == Action::UP) {
            // If top of grid
            if(s.boi.y == nY-1) {
                // Stand still
                if(s1.boi != s.boi) return 0.0;
            }
            // Move y+1
            else if(s1.boi != s.boi.up()) return 0.0;
        }
        else if(a == Action::DOWN) {
            // If bottom of grid
            if(s.boi.y == 0) {
                // Stand still
                if(s1.boi != s.boi) return 0.0;
            }
            // Move y-1
            else if(s1.boi != s.boi.down()) return 0.0;
        }
        else if(a == Action::LEFT) {
            // If leftmost of grid
            if(s.boi.x == 0) {
                // Stand still
                if(s1.boi != s.boi) return 0.0;
            }
            // Move x-1
            else if(s1.boi != s.boi.left()) return 0.0;
        }
        else if(a == Action::RIGHT) {
            // If rightmost of grid
            if(s.boi.x == nX-1) {
                // Stand still
                if(s1.boi != s.boi) return 0.0;
            }
            // Move x+1
            else if(s1.boi != s.boi.right()) return 0.0;
        }
        // Evaluate possible gob movements
        if((s1.gob == s.gob) or
           (s1.gob == s.gob.up()) or
           (s1.gob == s.gob.down()) or
           (s1.gob == s.gob.left()) or
           (s1.gob == s.gob.right())) {
            p *= (1.0/nA);
        }
        else {
            return 0.0;
        }
        // Evaluate possible goo movements
        if(s.boi == s.goo) {
            p *= (1.0/(nX*nY));
        }
        else {
            if(s1.goo != s.goo) return 0.0;
        }
        // Return compounded probability
        return p;
    }

    // Returns the (deterministic) reward for selecting action a in state s
    Real reward(Index s_index, Index a) const {
        State const s = state_space[s_index];
        // Get the goo!
        if(s.boi == s.goo) return 1.0;
        // Avoid the gob!
        if(s.boi == s.gob) return -1.0;
        return 0.0;
    }
};

////////////////////////////////////////////////// MAIN

// Solves the Grid-Boi problem and displays the results
int main(/*int argc, char** argv*/) {
    GridBoi mdp;

    // mdp.improve(5000, 1e-4);
    // mdp.print_solution();
    return 0;
}
