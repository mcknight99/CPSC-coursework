#include "interface.h"

using namespace std;

// desired additions:

// fix the executable still running when closed

// we can track the "venn diagrams" i was talking about by keeping track of the hints for each player:
// if a player says yes to a hint, we can log all of the potential loot islands to a hint map
    // ex. h p A 1 yes would save all of the islands on the 1 radius around A to the hint map
// if a player says no to a hint, we can remove all of the islands on that radius from all hints in their hint map
    // ex. h p C 1 no would remove the islands that are overlapping with the h p a 1 yes clue and any other clues with overlapping islands
// if a hint only has one remaining island, we can mark that island as a guaranteed treasure

// debatable utility:
// enter valid loot island digs to remember them as already dug, perhaps a gray color when printing and exclude it from potential loot islands


// implement best guess recommendation based on known information and what we need to know to maximize knowledge

// program name ideas or keywords:
// ultra rich text editor
// plunder
// map
// archipelago
// PLAN. Plundering Logic for Archipelago Navigation 
// SCUTTLE. Smart Captains Use Tricky Tactics for Looting Effectively
// ARR. Archipelago Rum-soaked Reasoning
// ART. Archipelago Reasoning Toolkit
// MAP. Marauders' Archipelago Plotter
// PLANK. Plundering Logic for Archipelago Navigation and Knowledge 
// OUTSMARTED. Overcomplicated and Underpowered Tactic by Sam for Manipulating the Archipelago as a Rich Text EDitor


int main() {
    Interface game;
    game.start();
    return 0;
}
