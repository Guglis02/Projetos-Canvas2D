#ifndef FUNCTIONTYPE_H_INCLUDED
#define FUNCTIONTYPE_H_INCLUDED

/** \brief
 * Enum de fun��es dos bot�es.
 */
enum FunctionType
{
    // Nenhuma fun��o selecionada no momento
    None,
    // Ferramentas de desenho
    Rect,
    Circle,
    Triangle,
    Poly,
    // Preenche desenho selecionado
    Fill,
    // Fun��es que alteram a ordem de render dos desenhos
    BringTop,
    SendBack,
    // Salva conjunto atual desenhos em um arquivo externo
    Save,
    // Bot�es de cor
    Color,
    // Deleta desenho selecionado
    Delete,
    // Limpa canvas de desenhos
    Clear
};

#endif // FUNCTIONTYPE_H_INCLUDED
