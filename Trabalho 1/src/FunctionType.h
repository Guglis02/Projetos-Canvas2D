#ifndef FUNCTIONTYPE_H_INCLUDED
#define FUNCTIONTYPE_H_INCLUDED

/** \brief
 * Enum de funções dos botões.
 */
enum FunctionType
{
    // Nenhuma função selecionada no momento
    None,
    // Ferramentas de desenho
    Rect,
    Circle,
    Triangle,
    Poly,
    // Preenche desenho selecionado
    Fill,
    // Funções que alteram a ordem de render dos desenhos
    BringTop,
    SendBack,
    // Salva conjunto atual desenhos em um arquivo externo
    Save,
    // Botões de cor
    Color,
    // Deleta desenho selecionado
    Delete,
    // Limpa canvas de desenhos
    Clear
};

#endif // FUNCTIONTYPE_H_INCLUDED
