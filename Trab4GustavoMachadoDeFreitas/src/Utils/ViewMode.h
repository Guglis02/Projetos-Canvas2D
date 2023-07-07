#ifndef VIEW_MODE_H
#define VIEW_MODE_H

// Enum que representa os modos de visualização disponíveis no programa.
enum class viewMode
{
    _2d,
    _ortho,
    _perspective
};

static char* ToString(viewMode mode)
{
    switch (mode)
    {
    case viewMode::_2d:
        return "2D";
    case viewMode::_ortho:
        return "3D Ortografica";
    case viewMode::_perspective:
        return "3D Perspectiva";
    default:
        return "Erro";
    }
}

#endif // VIEW_MODE_H