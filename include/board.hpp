#include <vector>

class board {
    unsigned int x, y;
    std::vector<std::vector<int>> state;

  public:
    board(const unsigned int x, const unsigned int y)
    {
        /*
            Nothing really, figuring out how to initialize the board with the above parameters.
            We need to set the board size on runtime
            Turns out the public method to construct is just the name of the class :)
            */
        this->x = x;
        this->y = y;
        for (int i = 0; i != x; ++i)
        {
            state.push_back(std::vector<int>(y, 0));
        }
        /*
           Well, it's much more complex than that https://docs.pommerman.com/environment/ 
           So we should now work on visualizing the board as a CUI. Just showing basic indications of what's present at each place
           We should also totally work on a better flow for working. We don't have a clear idea on what to work for now
           */
        }
}
