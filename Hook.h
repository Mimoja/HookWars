#include "Chain.h"
#include <vector>

class Hook : public Chain {
    private:
        GameObject* next = NULL;
        int player;

    public:
        Hook(int player, glm::vec3 origin, glm::vec3 velocity);
        void update();
        void pull();
};
