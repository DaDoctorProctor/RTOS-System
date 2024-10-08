#ifndef MODESELECTOR_H
#define MODESELECTOR_H

// MODE SELECTOR CONTROLLLER:
// M1 -> Manual control (1)
// M2 -> Fuzzy control (2)
// M3 -> Neural control (3)


class ModeSelector {
public:
    // Getter for the mode
    int getMode() const;

    // Setter for the mode
    void setMode(int newMode);

    // Singleton instance getter
    static ModeSelector& getInstance();

private:
    // Private constructor to enforce singleton pattern
    ModeSelector();

    // Private member variable to store the mode
    int mode;
};

#endif // MODESELECTOR_H