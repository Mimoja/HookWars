#ifndef PLAYER_H
#define PLAYER_H

class Player : gameObject{
    public:
    	Player(int p){
    	    playerID = p;
    	}
    	int playerID;
};

#endif
