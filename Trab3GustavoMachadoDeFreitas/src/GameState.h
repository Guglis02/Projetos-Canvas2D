#ifndef GAMESTATE_H_INCLUDED
#define GAMESTATE_H_INCLUDED

// Estados que o jogo pode estar
enum class GameState
{
    StartScreen,
    Endless,
    TrenchRun,
    GameOver,
    Victory
};

#endif // GAMESTATE_H_INCLUDED