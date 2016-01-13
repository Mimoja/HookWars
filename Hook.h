#include "GameObject.h"
#include <vector>

class Hook : GameObject {
    private:
        std::vector<glm::vec3> linkpositions;
        bool pulling;

    public:
        Hook(int player, glm::vec3 origin, glm::vec3 velocity);
        void update();
        void pull();
        glm::vec3 origin();
};
