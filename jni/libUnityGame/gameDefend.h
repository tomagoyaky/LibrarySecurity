#ifndef GAME_DEFEND_H
#define GAME_DEFEND_H

// start all game defend
void collocateDefend(void);

typedef void (*pre_exit_t)(void);
// register handle before game exit when cheated
extern void registerPreFunc(pre_exit_t func);

#endif //GAME_DEFEND_H
